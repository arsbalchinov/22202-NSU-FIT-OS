msg:
        .ascii "Hello World!\n"
        len = . - msg

.text
.global main

main:
        movq $1, %rax
        movq $1, %rdi
        movq $msg, %rsi
        movq $len, %rdx
        syscall

        cmp $len, %rax
        jne write_failed

        xorq %rdi, %rdi
        jmp exit_program

write_failed:
        movq $-1, %rdi

exit_program:
        movq $60, %rax
        syscall