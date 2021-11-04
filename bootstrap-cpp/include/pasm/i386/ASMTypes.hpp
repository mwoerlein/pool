#ifndef PASM_I386_TYPES_HPP_LOCK
#define PASM_I386_TYPES_HPP_LOCK

#define ModRM(mod,regO,regM) ((char) ((((mod)&0x3)<<6) + (((regO)&0x7)<<3) + (((regM)&0x7)))) 
#define SIB(ss,index,base) ((char) ((((ss)&0x3)<<6) + (((index)&0x7)<<3) + (((base)&0x7)))) 

enum OperandType { reg, indirect, number, identifier, formula };

enum BitWidth { bit_8 = 1, bit_16 = 2, bit_32 = 4, bit_auto = 0 };

enum RegisterName {
    reg_al, reg_ah, reg_bl, reg_bh, reg_cl, reg_ch, reg_dl, reg_dh, // 8bit
    reg_ax, reg_bx, reg_cx, reg_dx, reg_di, reg_si, reg_bp, reg_sp, // 16bit
    reg_eax, reg_ebx, reg_ecx, reg_edx, reg_edi, reg_esi, reg_ebp, reg_esp, // 32bit
    reg_cs, reg_ds, reg_es, reg_fs, reg_gs, reg_ss, // segment register
    reg_cr0, reg_cr1, reg_cr2, reg_cr3, reg_cr4, // control register
    reg_none
};

enum RegisterKind {
    reg_general, reg_segment, reg_control
};

enum InstructionCondition {
    cond_above,
    cond_above_or_equal,
    cond_below,
    cond_below_or_equal,
    cond_carry,
    cond_equal,
    cond_greater,
    cond_greater_or_equal,
    cond_lesser,
    cond_lesser_or_equal,
    cond_overflow,
    cond_parity,
    cond_sign,
    cond_zero,
    
    cond_not_above,
    cond_not_above_or_equal,
    cond_not_below,
    cond_not_below_or_equal,
    cond_not_carry,
    cond_not_equal,
    cond_not_greater,
    cond_not_greater_or_equal,
    cond_not_lesser,
    cond_not_lesser_or_equal,
    cond_not_overflow,
    cond_not_parity,
    cond_not_sign,
    cond_not_zero,
    
    cond_parity_even,
    cond_parity_odd,
    
    cond_reg_cx,
    cond_reg_ecx,
};

#endif //PASM_I386_TYPES_HPP_LOCK
