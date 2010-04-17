#ifndef _ARCH_ADDRESSLAYOUT_H
#define _ARCH_ADDRESSLAYOUT_H

//Some constants describing the kernel address space layout are
//defined here. Use them whenever you can!

#define     KERNEL_STACK_ADDRESS                0xFFC00000          //Uppermost address we can use
#define     KERNEL_STACK_SIZE                   0x10000             //64KByte

#define     KERNEL_THREAD_STACK_REGION_START    0x1000000
#define     KERNEL_THREAD_STACK_REGION_END      0x2000000

#endif
