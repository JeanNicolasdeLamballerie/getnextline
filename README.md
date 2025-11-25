*This project has been created as part of the 42 curriculum by jenicola.*

*E.G, Jean Nicolas de Lamballerie.*
# Description

This is a very simple re-implementation of the libc's get_next_line() function.

You can call GNL on any file descriptor and you'll either receive a string for the next line, or a null pointer if an error occurred or the next line is empty.

This specific implementation uses a stack-allocated array, mostly for speed and code simplicity. This does have a massive limitation : it means that a buffer size superior to the stack size (platform dependent, but usually counted in dozens of MBs) will *not* function. 

The way this works is basically by bouncing a reader struct around; the `reader` is static, and while it stays as heap-allocation free as possible, it does keep some references between calls, such as the leftover position in the buffer that is required to read again.

If we wanted to read bigger amounts we could re-implement this using malloc or calloc instead.

This implementation does not handle binary files because it relies to some degree on string terminators to detect the end of phrases; but it would not be that difficult to move over by only processing based on bytes_read and handling the current char* values as more generic values that we only work on through lengths instead of also using string terminators.

# Instructions

Call :

```c
get_next_line(fd)```

where fd is a [file descriptor](https://en.wikipedia.org/wiki/File_descriptor) for either some `stdin` or some file. Get back a heap allocated string, or a `NULL` pointer if an allocation error occurred or there are no more lines to read.


# Resources

No AI was used in this project in any shape or form.
Reading the GNL man, and some tests through [this](https://github.com/kodpe/gnl-station-tester) and [this](https://github.com/Tripouille/gnlTester) library are mostly what tested my code.
The implementation wasn't very inspired by anything external, hence the stack allocation.
