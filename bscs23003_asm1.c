#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

struct Message {
    int value;
};

int getMyID() { return getpid(); }
int getParentID() { return getppid(); }

void runChild(int parentToChildRead, int childToParentWrite) {
    struct Message msg;
    for (int i = 0; i < 1000000; i++) {
        if (read(parentToChildRead, &msg, sizeof(msg)) != sizeof(msg)) exit(1);
        msg.value++;
        if (write(childToParentWrite, &msg, sizeof(msg)) != sizeof(msg)) exit(1);
    }
    close(parentToChildRead);
    close(childToParentWrite);
    exit(0);
}

void runParent(int parentToChildWrite, int childToParentRead) {
    struct Message msg;
    msg.value = 0;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < 1000000; i++) {
        if (write(parentToChildWrite, &msg, sizeof(msg)) != sizeof(msg)) exit(1);
        if (read(childToParentRead, &msg, sizeof(msg)) != sizeof(msg)) exit(1);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    close(parentToChildWrite);
    close(childToParentRead);
    long long totalTime = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    printf("Parent PID: %d PPID: %d\n", getMyID(), getParentID());
    printf("Total round-trip time for 1000000 messages: %lld ns\n", totalTime);
    printf("Average round-trip latency: %.2f ns\n", (double) totalTime / 1000000);
    wait(NULL);
}

int main() {
    int parentToChild[2];
    int childToParent[2];
    if (pipe(parentToChild) == -1) exit(1);
    if (pipe(childToParent) == -1) exit(1);
    int pid = fork();
    if (pid < 0) exit(1);
    if (pid == 0) {
        close(parentToChild[1]);
        close(childToParent[0]);
        printf("Child PID: %d PPID: %d\n", getMyID(), getParentID());
        runChild(parentToChild[0], childToParent[1]);
    } else {
        close(parentToChild[0]);
        close(childToParent[1]);
        runParent(parentToChild[1], childToParent[0]);
    }
    return 0;
}
