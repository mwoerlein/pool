__elf_file_start:
__elf_ehdr_start:                                       # Elf32_Ehdr
    .byte 0x7f; .byte 0x45; .byte 0x4C; .byte 0x46      #   e_indent
    .byte 1; .byte 1; .byte 1; .byte 0
    .long 0; .long 0
    .word 2                                             #   e_type
    .word 3                                             #   e_machine
    .long 1                                             #   e_version
    .long __elf_code_start                              #   e_entry
    .long (__elf_phdr_start - __elf_ehdr_start)         #   e_phoff
    .long 0                                             #   e_shoff
    .long 0                                             #   e_flags
    .long ((__elf_phdr_size << 16) + __elf_ehdr_size)   #   e_ehsize & e_phentsize
    .word 1                                             #   e_phnum
    .word 0                                             #   e_shentsize
    .word 0                                             #   e_shnum
    .word 0                                             #   e_shstrndx
__elf_ehdr_end:
__elf_ehdr_size := (__elf_ehdr_end - __elf_ehdr_start)
__elf_phdr_size := (__elf_phdr_end - __elf_phdr_start)
  
__elf_phdr_start:                                       # Elf32_Phdr
    .long 1                                             #   p_type
    .long 0                                             #   p_offset
    .long __elf_ehdr_start                              #   p_vaddr
    .long __elf_ehdr_start                              #   p_paddr
    .long (__elf_file_end - __elf_ehdr_start)           #   p_filesz
    .long (__elf_file_end - __elf_ehdr_start)           #   p_memsz
    .long 7                                             #   p_flags
    .long 0x1000                                        #   p_align
__elf_phdr_end:

__elf_code_start:
    popl %eax       # read argc
    movl %esp, %ebx # read argv
    subl 4, %esp            # ret
    pushl %ebx; pushl %eax  # args
    pushl 0; pushl _entry_class_desc; call _entry_global_func # call global func
    addl 16, %esp
# Exit via the kernel:
    popl %ebx       # process' exit code
    movl 1, %eax    # system call number (sys_exit)
    int 0x80        # call kernel - this interrupt won't return


// helper
_eaxToChar:
    andl 0xf, %eax
    addl 48, %eax
    cmpl 58, %eax
    jl _eaxToChar_ret
    addl 7, %eax
_eaxToChar_ret:
    ret
    
writeHex:
    pusha
    movl hexBuf, %ecx
    movl %eax, %ebx
    movl %ebx, %eax; .byte 0xc1; .byte 0xe8; .byte 28; #// shrl 28, %ax;
    call _eaxToChar; movb %al, 2(%ecx) 
    movl %ebx, %eax; .byte 0xc1; .byte 0xe8; .byte 24; #// shrl 24, %ax;
    call _eaxToChar; movb %al, 3(%ecx) 
    movl %ebx, %eax; .byte 0xc1; .byte 0xe8; .byte 20; #// shrl 20, %ax;
    call _eaxToChar; movb %al, 4(%ecx) 
    movl %ebx, %eax; .byte 0xc1; .byte 0xe8; .byte 16; #// shrl 16, %ax;
    call _eaxToChar; movb %al, 5(%ecx) 
    movl %ebx, %eax; .byte 0xc1; .byte 0xe8; .byte 12; #// shrl 12, %ax;
    call _eaxToChar; movb %al, 6(%ecx) 
    movl %ebx, %eax; .byte 0xc1; .byte 0xe8; .byte 8; #// shrl 8, %ax;
    call _eaxToChar; movb %al, 7(%ecx) 
    movl %ebx, %eax; .byte 0xc1; .byte 0xe8; .byte 4; #// shrl 4, %ax;
    call _eaxToChar; movb %al, 8(%ecx) 
    movl %ebx, %eax; .byte 0xc1; .byte 0xe8; .byte 0; #// shrl 0, %ax;
    call _eaxToChar; movb %al, 9(%ecx) 
    
    movl 11, %edx   # message length
    movl 1, %ebx    # file descriptor (stdout)
    movl 4, %eax    # system call number (sys_write)
    int 0x80        # call kernel
    popa
    ret
hexBuf: .asciz "0x00000000\n"

strLen:
    movl %ecx, %edx
strLen_check:    
    cmpb 0, (%edx)
    jz strLen_end
    addl 1, %edx
    jmp strLen_check
strLen_end:
    subl %ecx, %edx
    ret
