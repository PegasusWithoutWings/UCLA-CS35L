The biggest challenge is to idetify the chunck of the code that is embarrisingly
parallel. Though it looked straightforward now looking back after I finished
the program, it is not easy to identify at the first glance. Indeed, I 
mistakenly included the part that prints out result in the threads as well,
making my first attempt completely wrong.

As for this assignment, we are specifically told that the program is 
embarrisingly parallel, but in the future we might need to figure it out 
ourselves, which will be even harder.

The performance improvement achieved is actually quite good. My program managed
to reduce the overall run time to near 1/n, where n is the number of the threads.