
struc peHeader
    .machine            RESW    1
    .numSections        RESW    1
    .dateTimeStamp      RESD    1
    .symtblPtr          RESD    1
    .numSymbols         RESD    1
    .optHeaderSize      RESW    1
    .characteristics    RESW    1
endstruc

struc peOptHeader
    .magic              RESW    1
    .lnkVersionMajor    RESB    1
    .lnkVersionMinor    RESB    1
    .sizeCode           RESD    1
    .sizeInitData       RESD    1
    .sizeUninitData     RESD    1
    .entrypoint         RESD    1
    .codeBase           RESD    1
    .dataBase           RESD    1
    
    ;NT-Fields
    .imageBase          RESD    1
    .sectionAlignment   RESD    1
    .fileAlignment      RESD    1
    .majorOSVersion     RESW    1
    .minorOSVersion     RESW    1
    .majorImageVersion  RESW    1
    .minorImageVersion  RESW    1
    .majorSubsysVersion RESW    1
    .minorSubsysVersion RESW    1
    .reserved           RESD    1
    .imageSize          RESD    1
    .headerSize         RESD    1
    .checksum           RESD    1
    .subsystem          RESW    1
    .dllCharacteristics RESW    1
    .sizeStackReserve   RESD    1
    .sizeStackCommit    RESD    1
    .sizeHeapReserve    RESD    1
    .sizeHeapCommit     RESD    1
    .loaderFlags        RESD    1
    .sizeAndNumRVA      RESD    1
    ;data directories are here...
endstruc

struc peDataDirectory
    .virtualAddress     RESD    1
    .size               RESD    1
endstruc

struc peSectionHeader
    .name               RESB    8
    .physAddrVirtSize   RESD    1       ;normally a union
    .virtualAddress     RESD    1
    .sizeOfRawData      RESD    1
    .pointerToRawData   RESD    1
    .pointerToRelocs    RESD    1
    .pointerToLineNmbr  RESD    1
    .numRelocs          RESW    1
    .numLineNmbrs       RESW    1
    .characteristics    RESD    1
endstruc


; section characteristics
SEC_CODE                EQU     5
SEC_DATA_INITIALIZED    EQU     6
SEC_DATA_UNINITIALIZED  EQU     7
SEC_COMMENTS            EQU     9
SEC_LINKERINFO          EQU     11
SEC_DISCARDABLE         EQU     25
SEC_SHAREABLE           EQU     28
SEC_EXECUTABLE          EQU     29
SEC_READABLE            EQU     30
SEC_WRITEABLE           EQU     31
