
struc Elf32_Ehdr
    .e_ident            RESB    16
    .e_type             RESW    1
    .e_machine          RESW    1
    .e_version          RESD    1
    .e_entry            RESD    1
    .e_phoff            RESD    1
    .e_shoff            RESD    1
    .e_flags            RESD    1
    .e_ehsize           RESW    1
    .e_phentsize        RESW    1
    .e_phnum            RESW    1
    .e_shentsize        RESW    1
    .e_shnum            RESW    1
    .e_shstrndx         RESW    1
endstruc

;e_ident class values
EI_CLASS                EQU     4
ELFCLASSNONE            EQU     0
ELFCLASS32              EQU     1
ELFCLASS64              EQU     2
ELFCLASSNUM             EQU     3

;e_type values
ET_NONE                 EQU     0
ET_REL                  EQU     1
ET_EXEC                 EQU     2
ET_DYN                  EQU     3
ET_CORE                 EQU     4
ET_NUM                  EQU     5
ET_LOOS                 EQU     0xFE00
ET_HIOS                 EQU     0xFEFF
ET_LOPROC               EQU     0xFF00
ET_HIPROC               EQU     0xFFFF

;e_machine values (partly)
EM_NONE                 EQU     0
EM_386                  EQU     3
EM_X86_64               EQU     62

struc Elf32_Shdr
    .sh_name            RESD    1
    .sh_type            RESD    1
    .sh_flags           RESD    1
    .sh_addr            RESD    1
    .sh_offset          RESD    1
    .sh_size            RESD    1
    .sh_link            RESD    1
    .sh_info            RESD    1
    .sh_addralign       RESD    1
    .sh_entsize         RESD    1
endstruc

struc Elf32_Phdr
    .p_type             RESD    1
    .p_offset           RESD    1
    .p_vaddr            RESD    1
    .p_paddr            RESD    1
    .p_filesz           RESD    1
    .p_memsz            RESD    1
    .p_flags            RESD    1
    .p_align            RESD    1
endstruc

;p_type values
PT_NULL                 EQU     0
PT_LOAD                 EQU     1
PT_DYNAMIC              EQU     2
PT_INTERP               EQU     3
PT_NOTE                 EQU     4
PT_SHLIB                EQU     5
PT_PHDR                 EQU     6
PT_TLS                  EQU     7
PT_NUM                  EQU     8
PT_LOOS                 EQU     0x60000000
PT_GNU_EH_FRAME         EQU     0x6474E550
PT_GNU_STACK            EQU     0x6474E551
PT_GNU_RELRO            EQU     0x6474E552
PT_HIOS                 EQU     0x6FFFFFFF
PT_LOPROC               EQU     0x70000000
PT_HIPROC               EQU     0x7FFFFFFF

;p_flags values
PF_X                    EQU     1
PF_W                    EQU     2
PF_R                    EQU     4
PF_MASKOS               EQU     0x0FF00000
PF_MASKPROC             EQU     0xF0000000
