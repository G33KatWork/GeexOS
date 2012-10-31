/*
 * Convert SysV calling convention to EFI x86_64 calling convention
 *
 *  Copyright (C) 2007-2010 Intel Corp
 *	Bibo Mao <bibo.mao@intel.com>
 *	Chandramouli Narayanan<mouli@linux.intel.com>
 *	Huang Ying <ying.huang@intel.com>
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following
 *   disclaimer in the documentation and/or other materials
 *   provided with the distribution.
 * - Neither the name of Hewlett-Packard Co. nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 *  BE LIABLE FOR ANYDIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 *  OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 *  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 *  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 *  SUCH DAMAGE.
 */

#include "efi.h"
#include "efistdarg.h"

UINT64 efi_call0(void *func);
UINT64 efi_call1(void *func, UINT64 arg1);
UINT64 efi_call2(void *func, UINT64 arg1, UINT64 arg2);
UINT64 efi_call3(void *func, UINT64 arg1, UINT64 arg2, UINT64 arg3);
UINT64 efi_call4(void *func, UINT64 arg1, UINT64 arg2, UINT64 arg3,
		 UINT64 arg4);
UINT64 efi_call5(void *func, UINT64 arg1, UINT64 arg2, UINT64 arg3,
		 UINT64 arg4, UINT64 arg5);
UINT64 efi_call6(void *func, UINT64 arg1, UINT64 arg2, UINT64 arg3,
		 UINT64 arg4, UINT64 arg5, UINT64 arg6);
UINT64 efi_call7(void *func, UINT64 arg1, UINT64 arg2, UINT64 arg3,
		 UINT64 arg4, UINT64 arg5, UINT64 arg6, UINT64 arg7);
UINT64 efi_call8(void *func, UINT64 arg1, UINT64 arg2, UINT64 arg3,
		 UINT64 arg4, UINT64 arg5, UINT64 arg6, UINT64 arg7,
		 UINT64 arg8);
UINT64 efi_call9(void *func, UINT64 arg1, UINT64 arg2, UINT64 arg3,
		 UINT64 arg4, UINT64 arg5, UINT64 arg6, UINT64 arg7,
		 UINT64 arg8, UINT64 arg9);
UINT64 efi_call10(void *func, UINT64 arg1, UINT64 arg2, UINT64 arg3,
		 UINT64 arg4, UINT64 arg5, UINT64 arg6, UINT64 arg7,
		 UINT64 arg8, UINT64 arg9, UINT64 arg10);

#define EFI_ARG_NUM_MAX 10

EFI_STATUS uefi_call_wrapper(void *fp, unsigned long va_num, ...)
{
	va_list ap;
	int i;
	unsigned long args[EFI_ARG_NUM_MAX];

	if (va_num > EFI_ARG_NUM_MAX || va_num < 0) {
		return EFI_LOAD_ERROR;
	}
	va_start(ap, va_num);
	for (i = 0; i < va_num; i++) {
		args[i] = va_arg(ap, UINT64);
	}
	va_end(ap);
	/* As the number of args grows extend it appropriately */
	switch (va_num) {
	case 0:
		return efi_call0(fp);
	case 1:
		return efi_call1(fp, args[0]);
	case 2:
		return efi_call2(fp,
				 args[0], args[1]);
	case 3:
		return efi_call3(fp,
				 args[0], args[1], args[2]);
	case 4:
		return efi_call4(fp,
				 args[0], args[1], args[2], args[3]);
	case 5:
		return efi_call5(fp,
				 args[0], args[1], args[2], args[3],
				 args[4]);
	case 6:
		return efi_call6(fp,
				 args[0], args[1], args[2], args[3],
				 args[4], args[5]);
	case 7:
		return efi_call7(fp,
				 args[0], args[1], args[2], args[3],
				 args[4], args[5], args[6]);
	case 8:
		return efi_call8(fp,
				 args[0], args[1], args[2], args[3],
				 args[4], args[5], args[6], args[7]);
	case 9:
		return efi_call9(fp,
				 args[0], args[1], args[2], args[3],
				 args[4], args[5], args[6], args[7],
				 args[8]);
	case 10:
		return efi_call10(fp,
				  args[0], args[1], args[2], args[3],
				  args[4], args[5], args[6], args[7],
				  args[8], args[9]);
	default:
		return EFI_LOAD_ERROR;
	}
}
