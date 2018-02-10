# CS35L-Lab 4: C Programming and Debugging

## Lab: Debugging a C program

### Reproduce the bug

1. Dowloaded the [coreutils-with-bug](https://web.cs.ucla.edu/classes/winter18/cs35L/assign/coreutils-with-bug.tar.gz) and unzipping it in SEASNET server according to the instruction.

1. Ran
    ```s
    ./configure --prefix=/u/eng/class/classtzh/CS35L/assign4/coreutils-w-bug
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

1. Run `make install` and succeeded.

1. Change directory into the installation location, `coreutils-w-bug`, and then change again into the binary folder, `bin`.

1. Attempt to reproduce the bug. First run
    ```bash
    tmp=$(mktemp -d)
    cd $tmp
    touch -d '1918-11-11 11:00 GMT' wwi-armistice
    touch now
    sleep 1
    touch now1
    ```

1. Run `~/CS35L/assign4/coreutils-w-bug/bin/ls -lt` in the newly created temporary folder, and it shows
    ```bash
    -rw-r--r-- 1 classtzh class 0 1918-11-11 03:00 wwi-armistice
    -rw-r--r-- 1 classtzh class 0 2018-02-07 18:09 now1
    -rw-r--r-- 1 classtzh class 0 2018-02-07 18:09 now
    ```
    The oldest file should appear at the bottom but appeared in the top. Bug successfully reproduced.

### Locate the cause of the bug

1. While remaining in the temporary directory, we run debugger `gdb ~/CS35L/assign4/coreutils-with-bug/src/ls` to locate the code that caused this bug.

1. As a trial, I executed `run -lt now now1 wwi-armistice` as a attempt to mimic what we did in bash. It produced the same output as running the program in shell, in addition to some gdb specific message.

1. Executed `layout src` to view the C source code of the buggy `ls`.

1. Set a breakpoint at main with `break main`.

1. Execute `run -lt now now1 wwi-armistice` and use `nexti` to step through functions, looking for things that can impact file ordering.

1. Found a function `sort_files` called during execution. Set a breakpoint at this function with `break sort_files` and rerun the program with the same arguments.

1. After stepping through some codes, the 2975th line of code the program ran onto, as seen below, raised my interest.
    ```C
    func = sort_reverse ? rev_cmp_mtime : compare_mtime;
    ```
    The bug is essentially the wrong sorting of time. So looking at the comparator function should help.

1. With above reasoning, I set another breakpoint at `compare_mtime` with `break compare_mtime` and then run `continue` to have the program continue running until `compare_mtime` is called.

1. After running for a very short period of time, the program stopped at line 2884 of `ls.c`, which is
    ```C
    static int compare_mtime (V a, V b) { return cmp_mtime (a, b, xstrcoll); }
    ```
    So the true function definition should be in the function `cmp_mtime`.

1. Then I use `step` trying to step into `cmp_mtime`. To my great confusion, I was led to a function in `timespec.h` called `timespec_cmp`. 

1. I did not understand what is going on because first I was not led to the definition of the function `cmp_mtime`, and second, `timespec_cmp` has only two arguments while `cmp_mtime` is called with three arguments.

1. Not knowing what to do, I continued to step through the program execution with `step`. To be even more frustrated, I find that then a few functions from `/lib64/libc.so.6` are called, whose source I cannot even see.

1. The rest of the execution is mostly just printing out the result with little insight to gain.