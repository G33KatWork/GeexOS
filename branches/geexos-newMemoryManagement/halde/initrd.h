#ifndef _INITRD_H
#define _INITRD_H
//****************************************************************************
//**
//**    initrd.h
//**    - Initial filesystem support
//**
//****************************************************************************
#include <stdint.h>
#include "fs.h"

typedef struct
{
   unsigned nfiles; // The number of files in the ramdisk.
} initrd_header_t;

typedef struct
{
   char magic;     // Magic number, for error checking.
   char name[64];  // Filename.
   unsigned offset;   // Offset in the initrd that the file starts.
   unsigned length;   // Length of the file.
} initrd_file_header_t;

// Initialises the initial ramdisk. It gets passed the address of the multiboot module,
// and returns a completed filesystem node.
fs_node_t *initialise_initrd(unsigned location);


//****************************************************************************
//**
//**    END initrd.h
//**
//****************************************************************************
#endif
