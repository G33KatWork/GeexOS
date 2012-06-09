#ifndef _GEEXLDR_PE_H_
#define _GEEXLDR_PE_H_

#include <types.h>

typedef struct _IMAGE_DOS_HEADER {  // DOS .EXE header
    uint16_t    Magic;             //'MZ'
    uint16_t    Cblp;
    uint16_t    Cp;
    uint16_t    Crlc;
    uint16_t    Cparhdr;
    uint16_t    Minalloc;
    uint16_t    Maxalloc;
    uint16_t    SS;
    uint16_t    SP;
    uint16_t    Checksum;
    uint16_t    IP;
    uint16_t    CS;
    uint16_t    Lfarlc;
    uint16_t    Ovno;
    uint16_t    Reserved[4];
    uint16_t    Oemid;
    uint16_t    Oeminfo;
    uint16_t    Reserved2[10];
    uint32_t    PEHeader;
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

typedef struct _IMAGE_FILE_HEADER {
    uint16_t    Machine;
    uint16_t    NumberOfSections;
    uint32_t    TimeDateStamp;
    uint32_t    PointerToSymbolTable;
    uint32_t    NumberOfSymbols;
    uint16_t    SizeOfOptionalHeader;
    uint16_t    Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

typedef struct _IMAGE_DATA_DIRECTORY {
  uint32_t      VirtualAddress;
  uint32_t      Size;
} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES    16

typedef struct _IMAGE_OPTIONAL_HEADER {
    uint16_t    Magic;
    uint8_t     MajorLinkerVersion;
    uint8_t     MinorLinkerVersion;
    uint32_t    SizeOfCode;
    uint32_t    SizeOfInitializedData;
    uint32_t    SizeOfUninitializedData;
    uint32_t    AddressOfEntryPoint;
    uint32_t    BaseOfCode;
    uint32_t    BaseOfData;
    uint32_t    ImageBase;
    uint32_t    SectionAlignment;
    uint32_t    FileAlignment;
    uint16_t    MajorOperatingSystemVersion;
    uint16_t    MinorOperatingSystemVersion;
    uint16_t    MajorImageVersion;
    uint16_t    MinorImageVersion;
    uint16_t    MajorSubsystemVersion;
    uint16_t    MinorSubsystemVersion;
    uint32_t    Reserved1;
    uint32_t    SizeOfImage;
    uint32_t    SizeOfHeaders;
    uint32_t    CheckSum;
    uint16_t    Subsystem;
    uint16_t    DllCharacteristics;
    uint32_t    SizeOfStackReserve;
    uint32_t    SizeOfStackCommit;
    uint32_t    SizeOfHeapReserve;
    uint32_t    SizeOfHeapCommit;
    uint32_t    LoaderFlags;
    uint32_t    NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER, *PIMAGE_OPTIONAL_HEADER;

typedef struct _IMAGE_NT_HEADERS {
    uint32_t                Signature;
    IMAGE_FILE_HEADER       FileHeader;
    IMAGE_OPTIONAL_HEADER   OptionalHeader;
} IMAGE_NT_HEADERS, *PIMAGE_NT_HEADERS;

#define IMAGE_SIZEOF_SHORT_NAME         8

typedef struct _IMAGE_SECTION_HEADER {
    uint8_t     Name[IMAGE_SIZEOF_SHORT_NAME];
    union {
        uint32_t    PhysicalAddress;
        uint32_t    VirtualSize;
    } Misc;
    uint32_t    VirtualAddress;
    uint32_t    SizeOfRawData;
    uint32_t    PointerToRawData;
    uint32_t    PointerToRelocations;
    uint32_t    PointerToLinenumbers;
    uint16_t    NumberOfRelocations;
    uint16_t    NumberOfLinenumbers;
    uint32_t    Characteristics;
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;

//Header Magics
#define IMAGE_DOS_SIGNATURE                 0x05A4D         // MZ
#define IMAGE_NT_SIGNATURE                  0x000004550     // PE00

//IMAGE_FILE_HEADER Machine
enum {
    IMAGE_FILE_MACHINE_UNKNOWN      = 0x000,
    IMAGE_FILE_MACHINE_ALPHA        = 0x184, // Alpha AXP
    IMAGE_FILE_MACHINE_ALPHA64      = 0x284, // Alpha AXP 64Bit
    IMAGE_FILE_MACHINE_ARM          = 0x1C0,
    IMAGE_FILE_MACHINE_I386         = 0x14C, // Intel 386 or later
    IMAGE_FILE_MACHINE_IA64         = 0x200, // Intel IA64
    IMAGE_FILE_MACHINE_M68K         = 0x268, // Motorola 68000 series
    IMAGE_FILE_MACHINE_MIPS16       = 0x266,
    IMAGE_FILE_MACHINE_MIPSFPU      = 0x366, // MIPS with FPU
    IMAGE_FILE_MACHINE_MIPSFPU16    = 0x466, // MIPS16 with FPU
    IMAGE_FILE_MACHINE_POWERPC      = 0x1F0, // Power PC
    IMAGE_FILE_MACHINE_R3000        = 0x162,
    IMAGE_FILE_MACHINE_R4000        = 0x166, // MIPS little endian
    IMAGE_FILE_MACHINE_R10000       = 0x168,
    IMAGE_FILE_MACHINE_SH3          = 0x1A2, // Hitachi SH3
    IMAGE_FILE_MACHINE_SH4          = 0x1A6, // Hitachi SH4
    IMAGE_FILE_MACHINE_THUMB        = 0x1C2,
    IMAGE_FILE_MACHINE_AMD64        = 0x8664 // x64
};

//IMAGE_FILE_HEADER Characteristics
enum {
    IMAGE_FILE_RELOCS_STRIPPED          = 0x0001,
    IMAGE_FILE_EXECUTABLE_IMAGE         = 0x0002,
    IMAGE_FILE_LINE_NUMS_STRIPPED       = 0x0004,
    IMAGE_FILE_LOCAL_SYMS_STRIPPED      = 0x0008,
    IMAGE_FILE_AGGRESSIVE_WS_TRIM       = 0x0010,
    IMAGE_FILE_LARGE_ADDRESS_AWARE      = 0x0020,
    IMAGE_FILE_16BIT_MACHINE            = 0x0040,
    IMAGE_FILE_BYTES_REVERSED_LO        = 0x0080,
    IMAGE_FILE_32BIT_MACHINE            = 0x0100,
    IMAGE_FILE_DEBUG_STRIPPED           = 0x0200,
    IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP  = 0x0400,
    IMAGE_FILE_FILE_SYSTEM              = 0x1000,
    IMAGE_FILE_DLL                      = 0x2000,
    IMAGE_FILE_UP_SYSTEM_ONLY           = 0x4000,
    IMAGE_FILE_BYTES_REVERSED_HI        = 0x8000
};

//IMAGE_OPTIONAL_HEADER Magic values
enum {
    IMAGE_NT_OPTIONAL_HDR32_MAGIC       = 0x010b,
    IMAGE_NT_OPTIONAL_HDR64_MAGIC       = 0x020b,
    IMAGE_ROM_OPTIONAL_HDR_MAGIC        = 0x0107
};

//IMAGE_OPTIONAL_HEADER Subsystems
enum {
    IMAGE_SUBSYSTEM_UNKNOWN                 = 0,
    IMAGE_SUBSYSTEM_NATIVE                  = 1,
    IMAGE_SUBSYSTEM_WINDOWS_GUI             = 2,
    IMAGE_SUBSYSTEM_WINDOWS_CUI             = 3,
    IMAGE_SUBSYSTEM_OS2_CUI                 = 5,
    IMAGE_SUBSYSTEM_POSIX_CUI               = 7,
    IMAGE_SUBSYSTEM_NATIVE_WINDOWS          = 8,
    IMAGE_SUBSYSTEM_WINDOWS_CE_GUI          = 9,
    IMAGE_SUBSYSTEM_EFI_APPLICATION         = 10,
    IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER = 11,
    IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER      = 12,
    IMAGE_SUBSYSTEM_EFI_ROM                 = 13,
    IMAGE_SUBSYSTEM_XBOX                    = 14,
    IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION = 16
};

//IMAGE_SECTION_HEADER Characteristics
enum {
    IMAGE_SCN_TYPE_REG                  = 0x00000000,
    IMAGE_SCN_TYPE_DSECT                = 0x00000001,
    IMAGE_SCN_TYPE_NOLOAD               = 0x00000002,
    IMAGE_SCN_TYPE_GROUP                = 0x00000004,
    IMAGE_SCN_TYPE_NO_PAD               = 0x00000008,
    IMAGE_SCN_TYPE_COPY                 = 0x00000010,
    IMAGE_SCN_CNT_CODE                  = 0x00000020,
    IMAGE_SCN_CNT_INITIALIZED_DATA      = 0x00000040,
    IMAGE_SCN_CNT_UNINITIALIZED_DATA    = 0x00000080,
    IMAGE_SCN_LNK_OTHER                 = 0x00000100,
    IMAGE_SCN_LNK_INFO                  = 0x00000200,
    IMAGE_SCN_TYPE_OVER                 = 0x00000400,
    IMAGE_SCN_LNK_REMOVE                = 0x00000800,
    IMAGE_SCN_LNK_COMDAT                = 0x00001000,
    IMAGE_SCN_MEM_FARDATA               = 0x00008000,
    IMAGE_SCN_MEM_PURGEABLE             = 0x00010000,
    IMAGE_SCN_MEM_16BIT                 = 0x00020000,
    IMAGE_SCN_MEM_LOCKED                = 0x00040000,
    IMAGE_SCN_MEM_PRELOAD               = 0x00080000,
    IMAGE_SCN_ALIGN_1BYTES              = 0x00100000,
    IMAGE_SCN_ALIGN_2BYTES              = 0x00200000,
    IMAGE_SCN_ALIGN_4BYTES              = 0x00300000,
    IMAGE_SCN_ALIGN_8BYTES              = 0x00400000,
    IMAGE_SCN_ALIGN_16BYTES             = 0x00500000,
    IMAGE_SCN_ALIGN_32BYTES             = 0x00600000,
    IMAGE_SCN_ALIGN_64BYTES             = 0x00700000,
    IMAGE_SCN_ALIGN_128BYTES            = 0x00800000,
    IMAGE_SCN_ALIGN_256BYTES            = 0x00900000,
    IMAGE_SCN_ALIGN_512BYTES            = 0x00A00000,
    IMAGE_SCN_ALIGN_1024BYTES           = 0x00B00000,
    IMAGE_SCN_ALIGN_2048BYTES           = 0x00C00000,
    IMAGE_SCN_ALIGN_4096BYTES           = 0x00D00000,
    IMAGE_SCN_ALIGN_8192BYTES           = 0x00E00000,
    IMAGE_SCN_ALIGN_MASK                = 0x00F00000, // Mask
    IMAGE_SCN_LNK_DISCARDABLE           = 0x02000000,
    IMAGE_SCN_MEM_NOT_CACHED            = 0x04000000,
    IMAGE_SCN_MEM_NOT_PAGED             = 0x08000000,
    IMAGE_SCN_MEM_SHARED                = 0x10000000,
    IMAGE_SCN_MEM_EXECUTE               = 0x20000000,
    IMAGE_SCN_MEM_READ                  = 0x40000000,
    IMAGE_SCN_MEM_WRITE                 = -2147483647//0x80000000
};

enum {
    IMAGE_DLLCHARACTERISTICS_NO_BIND                = 0x0800,
    IMAGE_DLLCHARACTERISTICS_WDM_DRIVER             = 0x2000,
    IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE  = 0x8000
};

//Helper macros
//#define FIELD_OFFSET(t,f) ((Address)&(((t*) 0)->f))
#define FIELD_OFFSET(t,f) ((Address)__builtin_offsetof(t,f))
#define IMAGE_FIRST_SECTION(h) ((PIMAGE_SECTION_HEADER) ((Address)h+FIELD_OFFSET(IMAGE_NT_HEADERS,OptionalHeader)+((PIMAGE_NT_HEADERS)(h))->FileHeader.SizeOfOptionalHeader))

//Data directories
//Direcory types
enum {
    IMAGE_DIRECTORY_ENTRY_EXPORT        = 0,
    IMAGE_DIRECTORY_ENTRY_IMPORT        = 1,
    IMAGE_DIRECTORY_ENTRY_RESOURCE      = 2,
    IMAGE_DIRECTORY_ENTRY_EXCEPTION     = 3,
    IMAGE_DIRECTORY_ENTRY_SECURITY      = 4,
    IMAGE_DIRECTORY_ENTRY_BASERELOC     = 5,
    IMAGE_DIRECTORY_ENTRY_DEBUG         = 6,
    IMAGE_DIRECTORY_ENTRY_COPYRIGHT     = 7,
    IMAGE_DIRECTORY_ENTRY_GLOBALPTR     = 8,
    IMAGE_DIRECTORY_ENTRY_TLS           = 9,
    IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG   = 10,
    IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT  = 11,
    IMAGE_DIRECTORY_ENTRY_IAT           = 12,
    IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT  = 13,
    IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR= 14
};

//Export
typedef struct _IMAGE_EXPORT_DIRECTORY {
    uint32_t Characteristics;
    uint32_t TimeDateStamp;
    uint16_t MajorVersion;
    uint16_t MinorVersion;
    uint32_t Name;
    uint32_t Base;
    uint32_t NumberOfFunctions;
    uint32_t NumberOfNames;
    uint32_t AddressOfFunctions;
    uint32_t AddressOfNames;
    uint32_t AddressOfNameOrdinals;
} IMAGE_EXPORT_DIRECTORY, *PIMAGE_EXPORT_DIRECTORY;

//Import
typedef struct _IMAGE_IMPORT_DESCRIPTOR {
    __extension__ union {
        uint32_t Characteristics;
        uint32_t OriginalFirstThunk;
    } DUMMY;
    uint32_t TimeDateStamp;
    uint32_t ForwarderChain;
    uint32_t Name;
    uint32_t FirstThunk;
} IMAGE_IMPORT_DESCRIPTOR, *PIMAGE_IMPORT_DESCRIPTOR;

typedef struct _IMAGE_THUNK_DATA32 {
    union {
        uint32_t ForwarderString;
        uint32_t Function;
        uint32_t Ordinal;
        uint32_t AddressOfData;
    } u1;
} IMAGE_THUNK_DATA32, *PIMAGE_THUNK_DATA32;

typedef struct _IMAGE_THUNK_DATA64 {
    union {
        uint64_t ForwarderString;
        uint64_t Function;
        uint64_t Ordinal;
        uint64_t AddressOfData;
    } u1;
} __attribute__((packed)) IMAGE_THUNK_DATA64, *PIMAGE_THUNK_DATA64;

typedef struct _IMAGE_IMPORT_BY_NAME {
    uint16_t Hint;
    char Name[1];
} IMAGE_IMPORT_BY_NAME, *PIMAGE_IMPORT_BY_NAME;

//Binding stuff - not used
/*typedef struct _IMAGE_BOUND_IMPORT_DESCRIPTOR {
    uint32_t TimeDateStamp;
    uint16_t OffsetModuleName;
    uint16_t NumberOfModuleForwarderRefs;
} IMAGE_BOUND_IMPORT_DESCRIPTOR, *PIMAGE_BOUND_IMPORT_DESCRIPTOR;

typedef struct _IMAGE_BOUND_FORWARDER_REF {
    uint32_t TimeDateStamp;
    uint16_t OffsetModuleName;
    uint16_t Reserved;
} IMAGE_BOUND_FORWARDER_REF, *PIMAGE_BOUND_FORWARDER_REF;*/

//Resources - not used
/*typedef struct _IMAGE_RESOURCE_DIRECTORY {
    uint32_t Characteristics;
    uint32_t TimeDateStamp;
    uint16_t MajorVersion;
    uint16_t MinorVersion;
    uint16_t NumberOfNamedEntries;
    uint16_t NumberOfIdEntries;
} IMAGE_RESOURCE_DIRECTORY, *PIMAGE_RESOURCE_DIRECTORY;

typedef struct _IMAGE_RESOURCE_DIRECTORY_ENTRY {
    _ANONYMOUS_UNION union {
        _ANONYMOUS_STRUCT struct {
            uint32_t NameOffset:31;
            uint32_t NameIsString:1;
        } DUMMYSTRUCTNAME1;
        uint32_t Name;
        uint16_t Id;
    } DUMMYUNIONNAME1;
    _ANONYMOUS_UNION union {
        uint32_t OffsetToData;
        _ANONYMOUS_STRUCT struct {
            uint32_t OffsetToDirectory:31;
            uint32_t DataIsDirectory:1;
        } DUMMYSTRUCTNAME3;
    } DUMMYUNIONNAME2;
} IMAGE_RESOURCE_DIRECTORY_ENTRY, *PIMAGE_RESOURCE_DIRECTORY_ENTRY;

typedef struct _IMAGE_RESOURCE_DIRECTORY_STRING {
    uint16_t Length;
    char NameString[1];
} IMAGE_RESOURCE_DIRECTORY_STRING, *PIMAGE_RESOURCE_DIRECTORY_STRING;

typedef struct _IMAGE_RESOURCE_DIR_STRING_U {
    uint16_t Length;
    wchar NameString[1];
} IMAGE_RESOURCE_DIR_STRING_U, *PIMAGE_RESOURCE_DIR_STRING_U;

typedef struct _IMAGE_RESOURCE_DATA_ENTRY {
    uint32_t OffsetToData;
    uint32_t Size;
    uint32_t CodePage;
    uint32_t Reserved;
} IMAGE_RESOURCE_DATA_ENTRY, *PIMAGE_RESOURCE_DATA_ENTRY;*/

//Relocations
typedef struct _IMAGE_BASE_RELOCATION {
    uint32_t VirtualAddress;
    uint32_t SizeOfBlock;
} IMAGE_BASE_RELOCATION, *PIMAGE_BASE_RELOCATION;

//relocation types
enum {
    IMAGE_REL_BASED_ABSOLUTE        = 0,
    IMAGE_REL_BASED_HIGH            = 1,
    IMAGE_REL_BASED_LOW             = 2,
    IMAGE_REL_BASED_HIGHLOW         = 3,
    IMAGE_REL_BASED_HIGHADJ         = 4,
    IMAGE_REL_BASED_MIPS_JMPADDR    = 5,
    IMAGE_REL_BASED_MIPS_JMPADDR16  = 9,
    IMAGE_REL_BASED_IA64_IMM64      = 9,
    IMAGE_REL_BASED_DIR64           = 10
};

//Thread local storage - unused
typedef struct _IMAGE_TLS_DIRECTORY32 {
    uint32_t StartAddressOfRawData;
    uint32_t EndAddressOfRawData;
    uint32_t AddressOfIndex;
    uint32_t AddressOfCallBacks;
    uint32_t SizeOfZeroFill;
    uint32_t Characteristics;
} IMAGE_TLS_DIRECTORY32, *PIMAGE_TLS_DIRECTORY32;

typedef struct _IMAGE_TLS_DIRECTORY64 {
    uint64_t StartAddressOfRawData;
    uint64_t EndAddressOfRawData;
    uint64_t AddressOfIndex;
    uint64_t AddressOfCallBacks;
    uint32_t SizeOfZeroFill;
    uint32_t Characteristics;
} IMAGE_TLS_DIRECTORY64, *PIMAGE_TLS_DIRECTORY64;

#endif
