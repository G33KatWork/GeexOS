#ifndef _GXLDR_ARCH_I386_CPUID_H
#define _GXLDR_ARCH_I386_CPUID_H

// ECX Flags
#define CPUID_FLAG_SSE3         0x1             //SSE3 support
#define CPUID_FLAG_PCLMUL       0x2             //PCLMULQDQ support
#define CPUID_FLAG_DTES64       0x4             //64-bit debug store
#define CPUID_FLAG_MONITOR      0x8             //MONITOR and MWAIT instructions
#define CPUID_FLAG_DS_CPL       0x10            //CPL qualified debug store
#define CPUID_FLAG_VMX          0x20            //Virtual Machine eXtensions
#define CPUID_FLAG_SMX          0x40            //Safer Mode Extensions
#define CPUID_FLAG_EST          0x80            //Enhanced SpeedStep
#define CPUID_FLAG_TM2          0x100           //Thermal Monitor 2
#define CPUID_FLAG_SSSE3        0x200           //Suplemental SSE3 instructions
#define CPUID_FLAG_CID          0x400           //Context ID
//reserved
#define CPUID_FLAG_FMA          0x1000          //Fused multiply-add
#define CPUID_FLAG_CX16         0x2000          //CMPXCHG16B instruction
#define CPUID_FLAG_XTPR         0x4000          //Can disable sending task priority messages
#define CPUID_FLAG_PDCM         0x8000          //Perfmon & debug capability
//reserved
#define CPUID_FLAG_PCID         0x20000         //Process context identifiers
#define CPUID_FLAG_DCA          0x40000         //Direct cache access for DMA writes
#define CPUID_FLAG_SSE4_1       0x80000         //SSE4.1 instructions
#define CPUID_FLAG_SSE4_2       0x100000        //SSE4.2 instructions
#define CPUID_FLAG_x2APIC       0x200000        //x2APIC support
#define CPUID_FLAG_MOVBE        0x400000        //MOVBE instruction (big-endian, Intel Atom only)
#define CPUID_FLAG_POPCNT       0x800000        //POPCNT instruction
#define CPUID_FLAG_TSCDEADLINE  0x1000000       //APIC supports one-shot operation using a TSC deadline value
#define CPUID_FLAG_AES          0x2000000       //AES instruction set
#define CPUID_FLAG_XSAVE        0x4000000       //XSAVE, XRESTOR, XSETBV, XGETBV
#define CPUID_FLAG_OSXSAVE      0x8000000       //XSAVE enabled by OS
#define CPUID_FLAG_AVX          0x10000000      //Advanced Vector Extensions
#define CPUID_FLAG_F16C         0x20000000      //CVT16 instruction set (half-precision) FP support
#define CPUID_FLAG_RDRAND       0x40000000      //RDRAND (on-chip random number generator) support
#define CPUID_FLAG_HYPERVISOR   0x80000000      //Running on a hypervisor (always 0 on a real CPU)

// EDX Flags
#define CPUID_FLAG_FPU          0x1             //Floating Point Unit
#define CPUID_FLAG_VME          0x2             //Virtual Mode Extensions
#define CPUID_FLAG_DE           0x4             //Debugging Extensions
#define CPUID_FLAG_PSE          0x8             //Page Size Extensions
#define CPUID_FLAG_TSC          0x10            //Time Stamp Counter
#define CPUID_FLAG_MSR          0x20            //Model-specific registers
#define CPUID_FLAG_PAE          0x40            //Physical Address Extensions
#define CPUID_FLAG_MCE          0x80            //Machine Check Exceptions
#define CPUID_FLAG_CXCHG8       0x100           //Compare and exchange 8-byte
#define CPUID_FLAG_APIC         0x200           //On-chip APIC
//reserved
#define CPUID_FLAG_SEP          0x800           //Fast System Calls
#define CPUID_FLAG_MTRR         0x1000          //Memory Type Range Registers
#define CPUID_FLAG_PGE          0x2000          //Page Global Enable
#define CPUID_FLAG_MCA          0x4000          //Machine Check Architecture
#define CPUID_FLAG_CMOV         0x8000          //Conditional move-instruction
#define CPUID_FLAG_PAT          0x10000         //Page Attribute Table
#define CPUID_FLAG_PSE36        0x20000         //36-bit Page Size Extensions
#define CPUID_FLAG_PSN          0x40000         //Processor Serial Number
#define CPUID_FLAG_CLFL         0x80000         //CLFLUSH instruction (SSE2)
//reserved 
#define CPUID_FLAG_DTES         0x200000        //Debug Trace and EMON Store MSRs
#define CPUID_FLAG_ACPI         0x400000        //Thermal Cotrol MSR
#define CPUID_FLAG_MMX          0x800000        //MMX instruction set
#define CPUID_FLAG_FXSR         0x1000000       //Fast floating point save/restore
#define CPUID_FLAG_SSE          0x2000000       //SSE (Streaming SIMD Extensions)
#define CPUID_FLAG_SSE2         0x4000000       //SSE2 (Streaming SIMD Extensions - #2)
#define CPUID_FLAG_SS           0x8000000       //Selfsnoop
#define CPUID_FLAG_HTT          0x10000000      //Hyper-Threading Technology
#define CPUID_FLAG_TM1          0x20000000      //Thermal Interrupts, Status MSRs
#define CPUID_FLAG_IA64         0x40000000      //IA-64 (Itanium!) CPU emulating x86
#define CPUID_FLAG_PBE          0x80000000      //Pending Break Event

// CPUID feature requests
#define CPUID_GETVENDORSTRING               0
#define CPUID_GETFEATURES                   1
#define CPUID_GETTLB                        2
#define CPUID_GETSERIAL                     3
#define CPUID_INTELEXTENDED                 0x80000000
#define CPUID_INTELFEATURES                 0x80000001
#define CPUID_INTELBRANDSTRING              0x80000002
#define CPUID_INTELBRANDSTRINGMORE          0x80000003
#define CPUID_INTELBRANDSTRINGEND           0x80000004
#define CPUID_L1TLBIDENTIFIERS              0x80000005
#define CPUID_EXTENDEDL2FEATURES            0x80000006
#define CPUID_APMINFORMATION                0x80000007
#define CPUID_VIRTUALPHYSICALADDRESSSIZES   0x80000008

static inline void cpuid_abcd(uint32_t in, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d)
{
    asm volatile("cpuid":"=a"(*a),"=b"(*b),"=c"(*c),"=d"(*d):"0"(in));
}

static inline void cpuid_cd(uint32_t in, uint32_t *c, uint32_t *d)
{
    asm volatile("cpuid":"=c"(*c),"=d"(*d):"0"(in):"eax","ebx");
}

static inline void cpuid_a(uint32_t in, uint32_t *a)
{
    asm volatile("cpuid":"=a"(*a):"0"(in):"ebx","ecx","edx");
}

static inline bool cpuid_hasfeature(uint32_t feature, bool ecxRegister)
{
    uint32_t c,d;
    cpuid_cd(CPUID_GETFEATURES, &c, &d);

    if(ecxRegister)
        return (c & feature) != 0;
    else
        return (d & feature) != 0;
}

static inline void cpuid_getVendorString(char* buffer)
{
    uint32_t a,b,c,d;
    cpuid_abcd(CPUID_GETVENDORSTRING, &a, &b, &c, &d);

    memcpy(buffer, &b, 4);
    buffer += 4;
    memcpy(buffer, &d, 4);
    buffer += 4;
    memcpy(buffer, &c, 4);
}

static inline void cpuid_getBrandString(char* buffer)
{
    const char* unsupported = "unsupported";
    uint32_t a,b,c,d;

    cpuid_a(CPUID_INTELEXTENDED, &a);
    if(a < 0x80000004)
    {
        memcpy(buffer, unsupported, sizeof(unsupported));
        return;
    }

    cpuid_abcd(CPUID_INTELBRANDSTRING, &a, &b, &c, &d);
    memcpy(buffer, &a, 4);
    buffer += 4;
    memcpy(buffer, &b, 4);
    buffer += 4;
    memcpy(buffer, &c, 4);
    buffer += 4;
    memcpy(buffer, &d, 4);
    buffer += 4;

    cpuid_abcd(CPUID_INTELBRANDSTRINGMORE, &a, &b, &c, &d);
    memcpy(buffer, &a, 4);
    buffer += 4;
    memcpy(buffer, &b, 4);
    buffer += 4;
    memcpy(buffer, &c, 4);
    buffer += 4;
    memcpy(buffer, &d, 4);
    buffer += 4;

    cpuid_abcd(CPUID_INTELBRANDSTRINGEND, &a, &b, &c, &d);
    memcpy(buffer, &a, 4);
    buffer += 4;
    memcpy(buffer, &b, 4);
    buffer += 4;
    memcpy(buffer, &c, 4);
    buffer += 4;
    memcpy(buffer, &d, 4);
    buffer += 4;
}

static inline void rdtsc(uint32_t* upper, uint32_t* lower)
{
    asm volatile("rdtsc" : "=a"(*lower), "=d"(*upper));
}

#endif
