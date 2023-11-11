/* ********* ABI **********
 * caller-saved: %eax, %ebx, %edx
 * callee-saved: %ecx, %esi, %edi
 * Stack-Frame:
 * | caller prepared                                                                            | call         | callee prepared
 * |Ret N|...|Ret 2|Ret 1|Param N|...|Param 2|Param 1|@Obj-Handle|VTab-Method Offset/@class-desc|@caller return|caller %ebp|spilled regs|Local 1|Local 2|...|tmp-data/further frames
 *                                                                                                                         ^callee %ebp                     ^callee %esp
 */

/* ****** Class-Desc ******
 * 31                     0
 * +----------------------+
 * | 0x15AC1A55
 * +----------------------+
 * | 0 / @Class-Ref
 * +----------------------+
 * | Class-classname-Offset
 * +----------------------+
 * | Classes-Tab-Offset
 * +----------------------+
 * | Virt-Method-Tabs-Offset
 * +----------------------+
 * | Method-Decl-Tab-Offset
 * +----------------------+
 * | instance-template-Offset
 * +----------------------+
 * | instance size
 * +----------------------+
 * | Object-Handle-Offset
 * +----------------------+
 * | Class-Handle-Offset
 * +----------------------+
 * | Class-End-Offset/size
 * +----------------------+
 */

/* ****** Classes Tab *****
 * 31                     0
 * +----------------------+
 * | 0 / @Class-Desc
 * +----------------------+
 * | Class-classname-Offset
 * +----------------------+
 * | Class-Virt-Method-Tab-Offset
 * +----------------------+
 * | Class-Handle-Offset
 * +----------------------+
 * | 0 / @Super1-Desc
 * +----------------------+
 * | Super1-classname-Offset
 * +----------------------+
 * | Super1-Virt-Method-Tab-Offset
 * +----------------------+
 * | Super1-Handle-Offset
 * +----------------------+
 * | ...
 * +----------------------+
 * | 0 / @SuperN-Desc
 * +----------------------+
 * | SuperN-classname-Offset
 * +----------------------+
 * | SuperN-Virt-Method-Tab-Offset
 * +----------------------+
 * | SuperN-Handle-Offset
 * +----------------------+
 * | 0 / @Glob-Desc
 * +----------------------+
 * | Glob-classname-Offset
 * +----------------------+
 * | 0
 * +----------------------+
 * | 0
 * +----------------------+
 * | 0xFFFFFFFF / Used class marker (needn't to be resolved)
 * +----------------------+
 * | Used-classname-Offset
 * +----------------------+
 * | 0
 * +----------------------+
 * | 0
 * +----------------------+
 * | 0 / terminal entry
 * +----------------------+
 * | 0
 * +----------------------+
 * | 0
 * +----------------------+
 * | 0
 * +----------------------+
 */
/* *** Virt Method Tabs ***
 * 31                     0
 * +----------------------+
 * | Meth 0 - Method Decl Tab Offset
 * +----------------------+
 * | Meth 0 - Class-Desc-Offset
 * +----------------------+
 * | Meth 1 - Method Decl Tab Offset
 * +----------------------+
 * | Meth 0 - Class-Desc-Offset
 * +----------------------+
 * | ...
 * +----------------------+
 */
/* **** Method Decl Tab ***
 * 31                     0
 * +----------------------+
 * | Meth 0 - Method Offset
 * +----------------------+
 * | Meth 1 - Method Offset
 * +----------------------+
 * | ...
 * +----------------------+
 */
/* ******* Instance *******
 * +----------------------+
 * | @Obj-Class-Desc
 * +----------------------+
 * | @Inst-MemInfo
 * +======================+ < Handle Class
 * | @Call-Entry
 * +----------------------+
 * | @Inst
 * +----------------------+
 * | Class-VTab-Offset
 * +----------------------+
 * | Object-Vars-Offset
 * +----------------------+
 * | SuperN-Vars-Offset
 * +----------------------+
 * | ..
 * +----------------------+
 * | Super1-Vars-Offset
 * +----------------------+
 * | Class-Vars-Offset
 * +======================+ < Handle Super1
 * | @Call-Entry
 * +----------------------+
 * | @Inst
 * +----------------------+
 * | Super1-VTab-Offset
 * +----------------------+
 * | Object-Vars-Offset
 * +----------------------+
 * | ...
 * +----------------------+
 * | Super1-Super1-Vars-Offset
 * +----------------------+
 * | Super1-Class-Vars-Offset
 * +======================+ < Handle ...
 * | ...
 * +======================+ < Handle Object
 * | ...
 * +======================+ < Vars Object
 * | ...
 * +----------------------+ < Vars ...
 * | ...
 * +----------------------+ < Vars Super1
 * | Super1-Var1
 * +----------------------+
 * | ...
 * +----------------------+ < Vars Class
 * | This-Var1
 * +----------------------+
 * | This-Var4
 * +----------------------+
 * | This-Var2 | This-Var3
 * +----------------------+
 * | ...
 * +----------------------+
 */

// debug helper
//*/
_string_compare: # %esi:string 1, %edi:string 2, return %al:<0, =0, >0
    subl 1, %edi
_string_compare_loop:   	
    addl 1, %edi
    lodsb
    subb (%edi), %al
    jnz _string_compare_return // differrent chars
    addb (%edi), %al
    jnz _string_compare_loop  // not end of string
_string_compare_return:
    ret
	
_call_entry_resolved_vtab:
	movl 8(%esp), %ebx	        # load object handle
	movl 4(%ebx), %eax          # get object
	movl 0(%eax), %eax          # get class-desc
	addl 8(%ebx), %eax          # get vtab
	addl 4(%esp), %eax	        # get vtab-entry by adding method-offset number
	movl 4(%ebx), %ebx	        # get object
	addl 4(%eax), %ebx          # compute method-@this
	movl %ebx, 8(%esp)          # store method-@this
	jmp (%eax)                  # goto method

_print: # %eax:column, %ebx:row, %cl:character
    pushl %ebx
    pushl %ecx
	.byte 0x69; .byte 0xdb; .long print_line_offset #// imull print_line_offset, %ebx
	movb 0xf, %ch
	movw %cx, print_cga_buffer(%ebx,%eax,2)
	popl %ecx
	popl %ebx
	ret
_print_digit: # %eax:column, %ebx:row, %cl:digit
    pushl %ecx
    andl 0xf, %ecx
    addl 48, %ecx
    cmpl 58, %ecx
    jl _pd_print
    addl 7, %ecx
_pd_print:    
    call _print
    popl %ecx
    ret
    
_print_hex: # %eax:column, %ebx:row, %ecx:value
    pushl %eax
    pushl %ecx
    movl 0x30, %ecx; call _print; addl 1, %eax // '0'
    movl 0x78, %ecx; call _print; addl 1, %eax // 'x'
    movl (%esp), %ecx; .byte 0xc1; .byte 0xe9; .byte 0x1c; call _print_digit; addl 1, %eax // '?' #// shrl 28, %ecx; 
    movl (%esp), %ecx; .byte 0xc1; .byte 0xe9; .byte 0x18; call _print_digit; addl 1, %eax // '?' #// shrl 24, %ecx; 
    movl (%esp), %ecx; .byte 0xc1; .byte 0xe9; .byte 0x14; call _print_digit; addl 1, %eax // '?' #// shrl 20, %ecx; 
    movl (%esp), %ecx; .byte 0xc1; .byte 0xe9; .byte 0x10; call _print_digit; addl 1, %eax // '?' #// shrl 16, %ecx; 
    movl (%esp), %ecx; .byte 0xc1; .byte 0xe9; .byte 0x0c; call _print_digit; addl 1, %eax // '?' #// shrl 12, %ecx; 
    movl (%esp), %ecx; .byte 0xc1; .byte 0xe9; .byte 0x08; call _print_digit; addl 1, %eax // '?' #// shrl  8, %ecx; 
    movl (%esp), %ecx; .byte 0xc1; .byte 0xe9; .byte 0x04; call _print_digit; addl 1, %eax // '?' #// shrl  4, %ecx; 
    movl (%esp), %ecx; .byte 0xc1; .byte 0xe9; .byte 0x00; call _print_digit; addl 1, %eax // '?' #// shrl  0, %ecx; 
    popl %ecx
    popl %eax
    ret
//*/
//*/
dump_col: .long 0
_dump_regs:
    pushad
    pushl %eax
    pushl %ebx
    pushl %ecx
    
    movl 4, %eax
    movl 4, %ebx
    movl 65, %ecx
    call _print
    
    movl test_row, %ebx
    
    movl (dump_col), %eax; addl 1, %ebx
    movl 0x61, %ecx; call _print; addl 1, %eax // 'a'
    movl 0x3a, %ecx; call _print; addl 1, %eax // ':'
    movl 0x20, %ecx; call _print; addl 1, %eax // ' '
    movl 8(%esp), %ecx; call _print_hex

    movl (dump_col), %eax; addl 1, %ebx
    movl 0x62, %ecx; call _print; addl 1, %eax // 'b'
    movl 0x3a, %ecx; call _print; addl 1, %eax // ':'
    movl 0x20, %ecx; call _print; addl 1, %eax // ' '
    movl 4(%esp), %ecx; call _print_hex

    movl (dump_col), %eax; addl 1, %ebx
    movl 0x63, %ecx; call _print; addl 1, %eax // 'c'
    movl 0x3a, %ecx; call _print; addl 1, %eax // ':'
    movl 0x20, %ecx; call _print; addl 1, %eax // ' '
    movl (%esp), %ecx; call _print_hex

    movl (dump_col), %eax; addl 1, %ebx
    movl 0x64, %ecx; call _print; addl 1, %eax // 'd'
    movl 0x3a, %ecx; call _print; addl 1, %eax // ':'
    movl 0x20, %ecx; call _print; addl 1, %eax // ' '
    movl %edx, %ecx; call _print_hex
    
    movl (dump_col), %eax; addl 1, %ebx
    movl 0x65, %ecx; call _print; addl 1, %eax // 'e'
    movl 0x3a, %ecx; call _print; addl 1, %eax // ':'
    movl 0x20, %ecx; call _print; addl 1, %eax // ' '
    movl %edi, %ecx; call _print_hex
    
    movl (dump_col), %eax; addl 1, %ebx
    movl 0x66, %ecx; call _print; addl 1, %eax // 'f'
    movl 0x3a, %ecx; call _print; addl 1, %eax // ':'
    movl 0x20, %ecx; call _print; addl 1, %eax // ' '
    movl %esi, %ecx; call _print_hex
    
    addl 12, %esp
    popad
    ret
//*/
