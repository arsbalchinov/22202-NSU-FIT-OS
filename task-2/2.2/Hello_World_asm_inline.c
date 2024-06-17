#include <stdio.h>

int main(void) {
        long write = 1;
        long fd = 1;
        const char* msg = "Hello World!\n";
        long len = 13;
asm (
        "movq %0, %%rax \n\t"
        "movq %1, %%rdi \n\t"
        "movq %2, %%rsi \n\t"
        "movq %3, %%rdx \n\t"
        "syscall        \n"

        "cmp %3, %%rax  \n\t"
        "jne write_failed       \n"

        "xor %%rdi, %%rdi       \n\t"
        "jmp exit_program       \n"

"write_failed:  \n\t"
        "movq $-1, %%rdi        \n"

"exit_program:  \n\t"
        "movq $60, %%rax        \n\t"
        "syscall                \n\t"

        :
        :"r"(write), "r"((long)fd), "r"(msg), "r"((long)len)
        : "rax", "rbx", "rdi", "rsi", "rdx"
);
}