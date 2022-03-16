# Disk-File-Reading-Speed
This is the final project for **Operating Systems** and it is by its very nature quite open ended.
The environment for this project is real Linux (I recommend a recent Ubuntu, but really any recent distribution should work). I have used C.

## Goal of this Project
The project is centered around performance.
We will try to get disk I/O as fast as possible and evaluate the effects of caches and the cost of system calls. In the end you should have a good understanding of what sits in the way between your process requesting data from disk and receiving it.

### Basics
- Write a program that can read and write a file from disk using the standard C/C++ library's `open`, `read`, `write`, and `close` functions.
    - Add parameter for the file name;
    - Add parameter for how big the file should be (for writing);
    - Add a parameter to specify how much to read with a single call (block size);
<br>Way to execute: `./run <filename> [-r|-w] <block_size> <block_count`</br>

### Measurement
When measuring things it helps if they run for "reasonable" time. It is hard to measure things that run too fast as you need high-precision clocks and a lot of other things can affect the measurement. It is also annoying to wait for a long time for an experiment, especially if you have to do many experiments. For this reason you should make sure that your experiments take "reasonable" time. I recommend something between 5 and 15 seconds.
- Write a program to find a file size which can be read in "reasonable" time.
    - Input: block size
    - Output: file size
<aside>
Way to execute: './run2 <filename> <block_size>'
💡 Hint: You can start with a small file size and use your program from (1) above to read it and measure the time it takes. You can keep doubling your file size until you get between 5 and 15 seconds.

</aside>

### Raw Performance
Because you would be looking at different file sizes when using different block sizes, it makes sense to use units for performance that are independent of the size. Use MiB/s (this is megabytes per second).
- Make your program output the performance it achieved in MiB/s
- vMake a graph that shows its performance as you change the block size.

### Caching
Once you have a file of "reasonable" size created, reboot your machine.
Call your program to read the file and note the performance.
Call your program to read the file again (immediately after) and note the performance.
Ideally you should observe the second read be much faster, assuming the file can fit in your physical memory. This is the effect of caching.
**NOTE: On Linux there is a way to clear the disk caches without rebooting your machine. E.g. sudo sh -c "/usr/bin/echo 3 > /proc/sys/vm/drop_caches"**
Make a graph that shows performance for cached and non-cached reads for various block sizes.

### System Calls
If you have very, very small block size (e.g. 1 byte), most of the time would be spent trapping into the kernel. We talked in class how system calls are expensive. Let's quantify it!
- Measure performance MiB/s when using block size of 1 byte
- Measure performance in B/s. This is how many system calls you can do per second.
- Try with other system calls that arguably do even less real work (e.g. lseek)

### Raw Performance
Try to optimize your program as much as you can to run as fast as it could.
- Find a good enough block size?
- Use multiple threads?
- Report both cached and non-cached performance numbers.
Way to execute: './fast <file_to_read>'


_**Example code for xor:**_
#include <stdio.h>

unsigned int xorbuf(unsigned int *buffer, int size) {
    unsigned int result = 0;
    for (int i = 0; i < size; i++) {
        result ^= buffer[i];
    }
    return result;
}

#define SIZE 100

int main()
{
    
    unsigned int buffer[SIZE] = {};
    
    // random initialization
    for (int i = 1; i < SIZE; i++) {
        buffer[i] = buffer[i - 1] * 31 + 17;
    }
    
    // compute xor one way...
    printf("%08x\n", xorbuf(buffer, SIZE));
    
    // compute xor another way... (as if 2 threads to half each)
    unsigned int xor1 = xorbuf(buffer, SIZE / 2);
    unsigned int xor2 = xorbuf(buffer + SIZE / 2, SIZE / 2);
    printf("%08x = %08x ^ %08x\n", xor1 ^ xor2, xor1, xor2);
    
    return 0;
}
