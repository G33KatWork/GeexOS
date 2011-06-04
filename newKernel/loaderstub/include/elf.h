#ifndef _ELF_H
#define _ELF_H

/**************** ELF Identification ****************/

#define     ELF_IDENT_LEN           16

/** ELF Magic **/
#define     ELF_IDENT_MAGIC_OFFSET  0
#define     ELF_IDENT_MAGIC_0       0x7f
#define     ELF_IDENT_MAGIC_1       'E'
#define     ELF_IDENT_MAGIC_2       'L'
#define     ELF_IDENT_MAGIC_3       'F'

/** ELF Class **/
#define     ELF_IDENT_CLASS_OFFSET  4
#define     ELF_IDENT_CLASS_NONE    0
#define     ELF_IDENT_CLASS_32      1
#define     ELF_IDENT_CLASS_64      2

/** ELF Data encoding **/
#define     ELF_IDENT_DATA_OFFSET   5
#define     ELF_IDENT_DATA_NONE     0
#define     ELF_IDENT_DATA_2LSB     1
#define     ELF_IDENT_DATA_2MSB     2

/** ELF Version **/
#define     ELF_IDENT_VERSION_OFFSET    6
#define     ELF_IDENT_VERSION_NONE		0
#define     ELF_IDENT_VERSION_CURRENT	1

/** ELF ABI **/
#define     ELF_IDENT_ABI_OFFSET	    7
#define     ELF_IDENT_ABI_NONE		    0
#define     ELF_IDENT_ABI_SYSV		    0
#define     ELF_IDENT_ABI_HPUX		    1
#define     ELF_IDENT_ABI_NETBSD		2
#define     ELF_IDENT_ABI_LINUX		    3
#define     ELF_IDENT_ABI_SOLARIS	    6
#define     ELF_IDENT_ABI_AIX		    7
#define     ELF_IDENT_ABI_IRIX		    8
#define     ELF_IDENT_ABI_FREEBSD	    9
#define     ELF_IDENT_ABI_TRU64		    10
#define     ELF_IDENT_ABI_MODESTO	    11
#define     ELF_IDENT_ABI_OPENBSD	    12
#define     ELF_IDENT_ABI_ARM		    97
#define     ELF_IDENT_ABI_STANDALONE	255

#define     ELF_IDENT_ABI_VERSION_OFFSET    8

/** Padding bytes **/
#define     ELF_IDENT_PADDING_OFFSET    9


/**************** ELF File type (Elf__Header->type) ****************/

#define     ELF_TYPE_NONE               0
#define     ELF_TYPE_RELOCATABLE        1
#define     ELF_TYPE_EXECUTABLE         2
#define     ELF_TYPE_DYNAMIC            3
#define     ELF_TYPE_CORE               4


/**************** ELF machine type (Elf__Header->machine) ****************/

#define     ELF_MACHINE_NONE		    0		/* No machine */
#define     ELF_MACHINE_M32		        1		/* AT&T WE 32100 */
#define     ELF_MACHINE_SPARC	        2		/* SUN SPARC */
#define     ELF_MACHINE_386		        3		/* Intel 80386 */
#define     ELF_MACHINE_68K		        4		/* Motorola m68k family */
#define     ELF_MACHINE_88K		        5		/* Motorola m88k family */
#define     ELF_MACHINE_860		        7		/* Intel 80860 */
#define     ELF_MACHINE_MIPS		    8		/* MIPS R3000 big-endian */
#define     ELF_MACHINE_S370		    9		/* IBM System/370 */
#define     ELF_MACHINE_MIPS_RS3_LE	    10		/* MIPS R3000 little-endian */

#define     ELF_MACHINE_PARISC	        15		/* HPPA */
#define     ELF_MACHINE_VPP500	        17		/* Fujitsu VPP500 */
#define     ELF_MACHINE_SPARC32PLUS	    18		/* Sun's "v8plus" */
#define     ELF_MACHINE_960		        19		/* Intel 80960 */
#define     ELF_MACHINE_PPC		        20		/* PowerPC */
#define     ELF_MACHINE_PPC64	        21		/* PowerPC 64-bit */
#define     ELF_MACHINE_S390		    22		/* IBM S390 */

#define     ELF_MACHINE_V800		    36		/* NEC V800 series */
#define     ELF_MACHINE_FR20		    37		/* Fujitsu FR20 */
#define     ELF_MACHINE_RH32		    38		/* TRW RH-32 */
#define     ELF_MACHINE_RCE		        39		/* Motorola RCE */
#define     ELF_MACHINE_ARM		        40		/* ARM */
#define     ELF_MACHINE_FAKE_ALPHA	    41		/* Digital Alpha */
#define     ELF_MACHINE_SH		        42		/* Hitachi SH */
#define     ELF_MACHINE_SPARCV9	        43		/* SPARC v9 64-bit */
#define     ELF_MACHINE_TRICORE	        44		/* Siemens Tricore */
#define     ELF_MACHINE_ARC		        45		/* Argonaut RISC Core */
#define     ELF_MACHINE_H8_300	        46		/* Hitachi H8/300 */
#define     ELF_MACHINE_H8_300H	        47		/* Hitachi H8/300H */
#define     ELF_MACHINE_H8S		        48		/* Hitachi H8S */
#define     ELF_MACHINE_H8_500	        49		/* Hitachi H8/500 */
#define     ELF_MACHINE_IA_64	        50		/* Intel Merced */
#define     ELF_MACHINE_MIPS_X	        51		/* Stanford MIPS-X */
#define     ELF_MACHINE_COLDFIRE	    52		/* Motorola Coldfire */
#define     ELF_MACHINE_68HC12	        53		/* Motorola M68HC12 */
#define     ELF_MACHINE_MMA		        54		/* Fujitsu MMA Multimedia Accelerator*/
#define     ELF_MACHINE_PCP		        55		/* Siemens PCP */
#define     ELF_MACHINE_NCPU		    56		/* Sony nCPU embedded RISC */
#define     ELF_MACHINE_NDR1		    57		/* Denso NDR1 microprocessor */
#define     ELF_MACHINE_STARCORE	    58		/* Motorola Start*Core processor */
#define     ELF_MACHINE_ME16		    59		/* Toyota ME16 processor */
#define     ELF_MACHINE_ST100	        60		/* STMicroelectronics ST100 processor */
#define     ELF_MACHINE_TINYJ	        61		/* Advanced Logic Corp. Tinyj emb.fam*/
#define     ELF_MACHINE_X86_64	        62		/* AMD x86-64 architecture */
#define     ELF_MACHINE_PDSP		    63		/* Sony DSP Processor */

#define     ELF_MACHINE_FX66		    66		/* Siemens FX66 microcontroller */
#define     ELF_MACHINE_ST9PLUS	        67		/* STMicroelectronics ST9+ 8/16 mc */
#define     ELF_MACHINE_ST7		        68		/* STMicroelectronics ST7 8 bit mc */
#define     ELF_MACHINE_68HC16	        69		/* Motorola MC68HC16 microcontroller */
#define     ELF_MACHINE_68HC11	        70		/* Motorola MC68HC11 microcontroller */
#define     ELF_MACHINE_68HC08  	    71		/* Motorola MC68HC08 microcontroller */
#define     ELF_MACHINE_68HC05	        72		/* Motorola MC68HC05 microcontroller */
#define     ELF_MACHINE_SVX		        73		/* Silicon Graphics SVx */
#define     ELF_MACHINE_AT19		    74		/* STMicroelectronics ST19 8 bit mc */
#define     ELF_MACHINE_VAX		        75		/* Digital VAX */
#define     ELF_MACHINE_CRIS    		76		/* Axis Communications 32-bit embedded processor */
#define     ELF_MACHINE_JAVELIN	        77		/* Infineon Technologies 32-bit embedded processor */
#define     ELF_MACHINE_FIREPATH	    78		/* Element 14 64-bit DSP Processor */
#define     ELF_MACHINE_ZSP		        79		/* LSI Logic 16-bit DSP Processor */
#define     ELF_MACHINE_MMIX		    80		/* Donald Knuth's educational 64-bit processor */
#define     ELF_MACHINE_HUANY	        81		/* Harvard University machine-independent object files */
#define     ELF_MACHINE_PRISM	        82		/* SiTera Prism */
#define     ELF_MACHINE_AVR		        83		/* Atmel AVR 8-bit microcontroller */
#define     ELF_MACHINE_FR30    		84		/* Fujitsu FR30 */
#define     ELF_MACHINE_D10V	    	85		/* Mitsubishi D10V */
#define     ELF_MACHINE_D30V		    86		/* Mitsubishi D30V */
#define     ELF_MACHINE_V850    		87		/* NEC v850 */
#define     ELF_MACHINE_M32R	    	88		/* Mitsubishi M32R */
#define     ELF_MACHINE_MN10300	        89		/* Matsushita MN10300 */
#define     ELF_MACHINE_MN10200	        90		/* Matsushita MN10200 */
#define     ELF_MACHINE_PJ		        91		/* picoJava */
#define     ELF_MACHINE_OPENRISC	    92		/* OpenRISC 32-bit embedded processor */
#define     ELF_MACHINE_ARC_A5	        93		/* ARC Cores Tangent-A5 */
#define     ELF_MACHINE_XTENSA	        94		/* Tensilica Xtensa Architecture */


/**************** ELF section header type (Elf__SectionHeader->type) ****************/

#define     ELF_SECTION_HEADER_TYPE_NULL            0
#define     ELF_SECTION_HEADER_TYPE_PROGBITS        1
#define     ELF_SECTION_HEADER_TYPE_SYMTAB          2
#define     ELF_SECTION_HEADER_TYPE_STRTAB          3
#define     ELF_SECTION_HEADER_TYPE_RELOCA          4
#define     ELF_SECTION_HEADER_TYPE_HASH            5
#define     ELF_SECTION_HEADER_TYPE_DYNAMIC         6
#define     ELF_SECTION_HEADER_TYPE_NOTE            7
#define     ELF_SECTION_HEADER_TYPE_NOBITS          8
#define     ELF_SECTION_HEADER_TYPE_RELOC           9
#define     ELF_SECTION_HEADER_TYPE_SHLIB           10
#define     ELF_SECTION_HEADER_TYPE_DYNSYM          11
#define     ELF_SECTION_HEADER_TYPE_INIT_ARR        14
#define     ELF_SECTION_HEADER_TYPE_FINI_ARR        15
#define     ELF_SECTION_HEADER_TYPE_PREINIT_ARR     16
#define     ELF_SECTION_HEADER_TYPE_GROUP           17
#define     ELF_SECTION_HEADER_TYPE_SYMTAB_SHNDX    18


/**************** ELF section header flags (Elf__SectionHeader->flags) ****************/

#define     ELF_SECTION_HEADER_FLAG_WRITE           (1 << 0)
#define     ELF_SECTION_HEADER_FLAG_ALLOC           (1 << 1)
#define     ELF_SECTION_HEADER_FLAG_EXEC            (1 << 2)
#define     ELF_SECTION_HEADER_FLAG_MERGE           (1 << 4)
#define     ELF_SECTION_HEADER_FLAG_STRINGS         (1 << 5)
#define     ELF_SECTION_HEADER_FLAG_INFO_LINK       (1 << 6)
#define     ELF_SECTION_HEADER_FLAG_LINK_ORDER      (1 << 7)
#define     ELF_SECTION_HEADER_FLAG_OS_NONCONFORMING (1 << 8)
#define     ELF_SECTION_HEADER_FLAG_GROUP           (1 << 9)
#define     ELF_SECTION_HEADER_FLAG_THREAD_LOCAL_DATA (1 << 10)


/**************** ELF symbol special section header index (Elf__Symbol->shndx) ****************/

#define     ELF_SECTION_HEADER_SPECIALINDEX_UNDEF   0
#define     ELF_SECTION_HEADER_SPECIALINDEX_ABS     0xfff1
#define     ELF_SECTION_HEADER_SPECIALINDEX_COMMON  0xfff2


/**************** ELF process header types (Elf__ProcoessHeader->type) ****************/

#define     ELF_PROGRAM_HEADER_TYPE_NULL            0
#define     ELF_PROGRAM_HEADER_TYPE_LOAD            1
#define     ELF_PROGRAM_HEADER_TYPE_DYNAMIC         2
#define     ELF_PROGRAM_HEADER_TYPE_INTERP          3
#define     ELF_PROGRAM_HEADER_TYPE_NOTE            4
#define     ELF_PROGRAM_HEADER_TYPE_SHLIB           5
#define     ELF_PROGRAM_HEADER_TYPE_PHDR            6
#define     ELF_PROGRAM_HEADER_TYPE_TLS             7

#define     ELF_PROGRAM_HEADER_TYPE_GNU_EH_FRAME    0x6474e550
#define     ELF_PROGRAM_HEADER_TYPE_GNU_STACK       0x6474e551


/**************** ELF process header flags (Elf__ProcoessHeader->flags) ****************/

#define     ELF_PROGRAM_HEADER_FLAG_EXEC            (1 << 0)
#define     ELF_PROGRAM_HEADER_FLAG_WRITE           (1 << 1)
#define     ELF_PROGRAM_HEADER_FLAG_READ            (1 << 2)


/**************** ELF dynamic tag types (Elf__Dyn->tag) ****************/

//TODO: Fill this in...

#include <elf32.h>
//#include <elf64.h>

#endif
