# CS35L-Lab 4: Debugging a C program

## Reproduce the bug

1. Dowloaded the [coreutils-with-bug](https://web.cs.ucla.edu/classes/winter18/cs35L/assign/coreutils-with-bug.tar.gz) and unzipping it in SEASNET server according to the instruction.

1. Ran
    ```s
    ./configure
    ```
    in the `coreutils-with-bug` directory.

1. Exectuted `make` as usual but failed. The error message shows that `utimens.h:2:5: error: conflicting types for 'futimens'`.

1. Downloaded the patch file [coreutils.diff](https://web.cs.ucla.edu/classes/winter18/cs35L/assign/coreutils.diff) from the assignment website to `coreutils-with-bug` folder. After inspection, I found that
    ```C
    -futimens (int fd ATTRIBUTE_UNUSED,
    +coreutils_futimens (int fd ATTRIBUTE_UNUSED,
    ```
    This will remove one definition of `futimes` and should fix the problem caused by multiple type definitions.

1. After understanding the mechanism of the patch file, I executed
    ```bash
    cat coreutils.diff | patch -p0
    ```
    in `coreutils-with-bug` directory. The patch was applied successfully.

1. Run `./configure` again with the same prefix.

1. Run `make` again and completed the build this time.

1. Attempt to reproduce the bug. First run
    ```bash
    tmp=$(mktemp -d)
    cd $tmp
    touch -d '1918-11-11 11:00 GMT' wwi-armistice
    touch now
    sleep 1
    touch now1
    ```

1. Run `~/CS35L/assign4/coreutils-with-bug/src/ls -lt` in the newly created temporary folder, and it shows
    ```bash
    -rw-r--r-- 1 classtzh class 0 1918-11-11 03:00 wwi-armistice
    -rw-r--r-- 1 classtzh class 0 2018-02-10 15:23 now1
    -rw-r--r-- 1 classtzh class 0 2018-02-10 15:23 now
    ```
    The oldest file should appear at the bottom but appeared in the top. Bug successfully reproduced.

## Locate the cause of the bug

1. While remaining in the temporary directory, we run debugger `gdb ~/CS35L/assign4/coreutils-with-bug/src/ls` to locate the code that caused this bug.

1. As a trial, I executed `run -lt` as a attempt to mimic what we did in bash. It produced the same output as running the program in shell, in addition to some gdb specific message.

1. Executed `layout src` to view the C source code of the buggy `ls`.

1. Set a breakpoint at main with `break main`.

1. Execute `run -lt` and use `nexti` to step through functions, looking for things that can impact file ordering.

1. Found a function `sort_files` called during execution. Set a breakpoint at this function with `break sort_files`. Clear the break point at `main` with `clear main` and run the program with the same arguments.

1. After stepping through some codes, the 2975th line of code the program ran onto, as seen below, raised my interest.
    ```C
    func = sort_reverse ? rev_cmp_mtime : compare_mtime;
    ```
    The order of sorting is determined by the comparator function. If the sorting order is wrong, that means the comparator is wrong. Therefore, I decided to look into the comparator function.

1. With above reasoning, I set another breakpoint at `compare_mtime` with `break compare_mtime` and then run `continue` to have the program continue running until `compare_mtime` is called.

1. After running for a very short period of time, the program stopped at line 2884 of `ls.c`, which is
    ```C
    static int compare_mtime (V a, V b) { return cmp_mtime (a, b, xstrcoll); }
    ```
    So the true function definition should be in the function `cmp_mtime`.

1. Set a break point at `cmp_mtime` with `break cmp_mtime` and then rerun the program.

1. I use `step` trying to step into `cmp_mtime`. It then called `timespec_cmp`.

1. I decided to rerun the program and this time focus specifically on the return values of  `cmp_mtime`. For this purpose, I only set `break cmp_mtime`.

1. I stepped through the in total 3 executions of `cmp_mtime` functions and printed out the return value with `print /d $rax` each time after the function call. The values are:
    ```C
    1162836302
    1162836301
    4294967295
    ```

1. Based on the name of the function, it is easy to infer that these numbers are simply the difference of the modified time, in the format of "seconds since epoch" of the three files. 

1. To provide further evidence, I printed out the modified time of the free files in the "seconds since epoch" format in shell with command
    ```bash
    find . -type f -exec stat --printf="%n %Y\n" {} \;`
    ```
    The result is:
    ```txt
    ./now1 1518304995
    ./now 1518304994
    ./wwi-armistice -1613826000
    ```

1. With a few attempts, we find that
    ```txt
    -1613826000 - 1518304995 = 1162836301
    -1613826000 - 1518304994 = 1162836302
    ```
    Clearly the mtime is stored with 32-bit integer type, and there are overflows when taking the difference in `cmp_mtime`.

1. The property of the comparator function is such that, if `cmp(a, b) > 0` then `a > b`, which in the context of sorting time means a is created after b. Because `cmp_mtime(-1613826000, 1518304994) > 0`, the sorting algorithm will treat `wwi-armistice` as if it is created after `now` and, similarly, after `now1`.

1. To avoid the integer overflow that caused this problem, we can avoid subtraction and simply return a positive number or negative number based on the result of comparison. A good implementation of `timespec_cmp` can be
    ```C
    timespec_cmp (struct timespec a, struct timespec b)
    {
        if (a.tv_sec < b.tv_sec) {
            return -1;
        }
        else if (a.tv_sec > b.tv_sec) {
            return 1;
        }
        else {
            return (a.tv_nsec - b.tv_nsec);
        }
    }
    ```

## Patching

1. We are now going to make a patch file to the `timespec_cmp` function specified in `\lib\timespec.h`. Here we are going to use the ability of `git diff` to compare changed file with last commit to generate the needed patch file with right file path.

1. First we add the `\lib\timespec.h` to the repository with
    ```git
    git add -f \lib\timespec.h
    git commit -m "Add timespec.h to the repository"
    ```

1. Then we open the file with editor, and make the specified change to it. After the change, to generate the patch file, we will execute
    ```git
    git diff > lab4.diff
    ```

1. Now after returning back to the directory that contains `coreutils-with-bug`, we will open the patch file and delete some of the unnecessary messages generated by git. The result will be
    ```diff
    --- coreutils-with-bug/lib/timespec.h
    +++ coreutils-with-bug/lib/timespec.h
    @@ -45,8 +45,12 @@ struct timespec
    static inline int
    timespec_cmp (struct timespec a, struct timespec b)
    {
    -  int diff = a.tv_sec - b.tv_sec;
    -  return diff ? diff : a.tv_nsec - b.tv_nsec;
    +  if (a.tv_sec < b.tv_sec):
    +      return -1;
    +  else if (a.tv_sec > b.tv_sec):
    +      return 1;
    +  else:
    +      return (a.tv_nsec - b.tv_nsec);
    }
    
    # if ! HAVE_DECL_NANOSLEEP
    ```

1. We tested applying the batch by first reverting the change with `git checkout lib\timespec.h` and then apply the patch with `cat lab4.diff | patch -p0`. The patch is successfully applied.

## Testing patched program