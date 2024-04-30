#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>

int write_sys(int fd, const char* buf, unsigned int count) {
        return syscall(SYS_write, fd, buf, count);
}

int main() {
        const char* str = "Hello World\n";
        int ret = write_sys(1, str, strlen(str));
        if (ret == -1) {
                return -1;
        }
        return 0;
}