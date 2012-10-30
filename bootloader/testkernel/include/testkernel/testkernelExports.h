#ifndef _TESTKERNEL_EXPORTS_H_
#define _TESTKERNEL_EXPORTS_H_

#ifdef KERNELAPI_EXPORTS
#define KERNEL_API __attribute__ ((visibility ("default")))
#else
#define KERNEL_API
#endif

#endif
