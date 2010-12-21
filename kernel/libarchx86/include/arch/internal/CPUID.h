#ifndef _ARCHX86_CPUID_H
#define _ARCHX86_CPUID_H

// ECX Flags
#define CPUID_FLAG_SSE3         0x1             //SSE3 support.
#define CPUID_FLAG_PCLMUL       0x2
#define CPUID_FLAG_DTES64       0x4
#define CPUID_FLAG_MONITOR      0x8             //MWAIT/MONITOR support.
#define CPUID_FLAG_DS_CPL       0x10
#define CPUID_FLAG_VMX          0x20            // Virtualization Extension.
#define CPUID_FLAG_SMX          0x40
#define CPUID_FLAG_EST          0x80
#define CPUID_FLAG_TM2          0x100
#define CPUID_FLAG_SSSE3        0x200
#define CPUID_FLAG_CID          0x400
#define CPUID_FLAG_FMA          0x1000
#define CPUID_FLAG_CX16         0x2000
#define CPUID_FLAG_ETPRD        0x4000
#define CPUID_FLAG_PDCM         0x8000
#define CPUID_FLAG_DCA          0x40000
#define CPUID_FLAG_SSE4_1       0x80000
#define CPUID_FLAG_SSE4_2       0x100000
#define CPUID_FLAG_x2APIC       0x200000
#define CPUID_FLAG_MOVBE        0x400000
#define CPUID_FLAG_POPCNT       0x800000
#define CPUID_FLAG_XSAVE        0x4000000
#define CPUID_FLAG_OSXSAVE      0x8000000
#define CPUID_FLAG_AVX          0x10000000

// EDX Flags
#define CPUID_FLAG_FPU          0x1             // Floating Point Unit. 
#define CPUID_FLAG_VME          0x2             // Virtual Mode Extensions. 
#define CPUID_FLAG_DE           0x4             // Debugging Extensions. 
#define CPUID_FLAG_PSE          0x8             // Page Size Extensions. 
#define CPUID_FLAG_TSC          0x10            // Time Stamp Counter. 
#define CPUID_FLAG_MSR          0x20            // Model-specific registers. 
#define CPUID_FLAG_PAE          0x40            // Physical Address Extensions. 
#define CPUID_FLAG_MCE          0x80            // Machine Check Exceptions. 
#define CPUID_FLAG_CXCHG8       0x100           // Compare and exchange 8-byte. 
#define CPUID_FLAG_APIC         0x200           // On-chip APIC. 
#define CPUID_FLAG_SEP          0x800           // Fast System Calls. 
#define CPUID_FLAG_MTRR         0x1000          // Memory Type Range Registers. 
#define CPUID_FLAG_PGE          0x2000          // Page Global Enable. 
#define CPUID_FLAG_MCA          0x4000          // Machine Check Architecture. 
#define CPUID_FLAG_CMOV         0x8000          // Conditional move-instruction. 
#define CPUID_FLAG_PAT          0x10000         // Page Attribute Table. 
#define CPUID_FLAG_PSE36        0x20000         // 36-bit Page Size Extensions. 
#define CPUID_FLAG_PSN          0x40000         // Processor Serial Number. 
#define CPUID_FLAG_CLFL         0x80000         // CLFLUSH - fixme? 
#define CPUID_FLAG_DTES         0x200000        // Debug Trace and EMON Store MSRs. 
#define CPUID_FLAG_ACPI         0x400000        // Thermal Cotrol MSR. 
#define CPUID_FLAG_MMX          0x800000        // MMX instruction set. 
#define CPUID_FLAG_FXSR         0x1000000       // Fast floating point save/restore. 
#define CPUID_FLAG_SSE          0x2000000       // SSE (Streaming SIMD Extensions) 
#define CPUID_FLAG_SSE2         0x4000000       // SSE2 (Streaming SIMD Extensions - #2) 
#define CPUID_FLAG_SS           0x8000000       // Selfsnoop. 
#define CPUID_FLAG_HTT          0x10000000      // Hyper-Threading Technology. 
#define CPUID_FLAG_TM1          0x20000000      // Thermal Interrupts, Status MSRs. 
#define CPUID_FLAG_IA64         0x40000000      // IA-64 (64-bit Intel CPU) 
#define CPUID_FLAG_PBE          0x80000000      // Pending Break Event.    

namespace Arch
{
    enum cpuid_requests
    {
        CPUID_GETVENDORSTRING,
        CPUID_GETFEATURES,
        CPUID_GETTLB,
        CPUID_GETSERIAL,

        CPUID_INTELEXTENDED=0x80000000,
        CPUID_INTELFEATURES,
        CPUID_INTELBRANDSTRING,
        CPUID_INTELBRANDSTRINGMORE,
        CPUID_INTELBRANDSTRINGEND
    };

    static inline void cpuid(int in, uint32_t *a, uint32_t *d)
    {
        asm volatile("cpuid":"=a"(*a),"=d"(*d):"0"(in):"ecx","ebx");
    }

    static inline int cpuid_string(int in, uint32_t dest[4])
    {
        int h = 0;
        asm volatile("cpuid":"=a"(*(dest+4)),"=b"(*(dest)), "=c"(*(dest+2)),"=d"(*(dest+1)):"0"(in));
        return h;
    }
}

#endif
