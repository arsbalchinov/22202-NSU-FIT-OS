#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>

int main() {
        const char* str = "Hello World\n";
        int ret = write(1, str, strlen(str));
        if (ret == -1) {
                return -1;
        }
        return 0;
}