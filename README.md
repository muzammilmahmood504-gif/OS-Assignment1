# OS-Assignment1
Operating Systems Assignment 1
# Parent-Child Communication with Pipes in C

This program shows how a parent and child process communicate using pipes in C and measures the latency of sending and receiving messages one million times. The parent sends a message, the child increments it and sends it back, repeating in a loop. The total and average time are measured using `clock_gettime()`.

# Functions Used

* getpid() → returns the current process ID
* getppid() → returns the parent process ID
* fork() → creates the child process
* pipe() → sets up communication between processes
* read() and write() → used for sending/receiving messages
* clock_gettime() → measures execution time with nanosecond precision
