/*
 * Urboot bootloader help functions
 *
 *  - Save one word of flash by using out instead of sts (except for ATmega64/128)
 *  - Assume the address is already in Z (r31:30) as per global variable in urboot.c
 *  - Caller must manage RAMPZ for etended access herself
 *
 * published under GNU General Public License, version 3 (GPL-3.0)
 * author Stefan Rueger
 *
 * v 7.4
 * 14.04.2021 (first version published in July 2016)
 */

#ifndef URBOOT_R30_H
#define URBOOT_R30_H

#include <avr/boot.h>

// Some microprocessors need a special code pipeline after SPM/LPM
#if defined(__AVR_ATmega161__) || defined(__AVR_ATmega163__) || defined(__AVR_ATmega323__)
#define SPM_PIPELINE  \
   ".word 0xffff\n\t" \
   "nop\n\t"
#else
#define SPM_PIPELINE
#endif

// processors with SPM_REG outside the I/O space
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__) || defined(__AVR_ATmega64A__) || \
    defined(__AVR_ATmega128A__)
#define oust_spm "sts "
#define inld_spm "lds "
#define UB_SPM_REG  _SFR_MEM_ADDR(__SPM_REG)
#else
#define oust_spm "out "
#define inld_spm "in "
#define UB_SPM_REG  _SFR_IO_ADDR(__SPM_REG)
#endif

#define urboot_page_fill_r30(data)               \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
        "movw  r0, %2\n\t"                       \
        oust_smp " %0, %1\n\t"                   \
        "spm\n\t"                                \
        SPM_PIPELINE                             \
        "clr  r1\n\t"                            \
        :                                        \
        : "i" (UB_SPM_REG),                      \
          "r" ((uint8_t)__BOOT_PAGE_FILL),       \
          "r" ((uint16_t)data)                   \
        : "r0"                                   \
    );                                           \
}))

#define  urboot_page_erase_r30()                 \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
        oust_smp " %0, %1\n\t"                   \
        "spm\n\t"                                \
        SPM_PIPELINE                             \
        :                                        \
        : "i" (UB_SPM_REG),                      \
          "r" ((uint8_t)__BOOT_PAGE_ERASE)       \
    );                                           \
}))

#define urboot_page_write_r30()                  \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
        oust_smp " %0, %1\n\t"                   \
        "spm\n\t"                                \
        SPM_PIPELINE                             \
        :                                        \
        : "i" (UB_SPM_REG),                      \
          "r" ((uint8_t)__BOOT_PAGE_WRITE)       \
    );                                           \
}))

#define urboot_rww_enable_r30()                  \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
        oust_smp " %0, %1\n\t"                   \
        "spm\n\t"                                \
        SPM_PIPELINE                             \
        :                                        \
        : "i" (UB_SPM_REG),                      \
          "r" ((uint8_t)__BOOT_RWW_ENABLE)       \
    );                                           \
}))

#define urboot_lock_fuse_bits_get_r30()          \
(__extension__({                                 \
    uint8_t __result;                            \
    __asm__ __volatile__                         \
    (                                            \
        oust_smp " %1, %2\n\t"                   \
        "lpm %0, Z\n\t"                          \
        : "=r" (__result)                        \
        : "i" (UB_SPM_REG),                      \
          "r" ((uint8_t)(urboot_LOCK_BITS_SET))  \
    );                                           \
    __result;                                    \
}))


#endif // BOOT_R30_H
