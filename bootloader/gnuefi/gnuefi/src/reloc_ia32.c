/* reloc_ia32.c - position independent x86 ELF shared object relocator
   Copyright (C) 1999 Hewlett-Packard Co.
	Contributed by David Mosberger <davidm@hpl.hp.com>.

    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials
      provided with the distribution.
    * Neither the name of Hewlett-Packard Co. nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
    CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
    BE LIABLE FOR ANYDIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
    OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
    TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
    THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
*/

//#include <elf.h>
//#include <link.h>	/* get _DYNAMIC decl and ElfW and ELFW macros */


//The following stuff is crap, I know...
//stdint.h
typedef     unsigned char   uint8_t;
typedef     char            int8_t;

typedef     unsigned short  uint16_t;
typedef     short           int16_t;

typedef     unsigned int    uint32_t;
typedef     int             int32_t;

typedef     unsigned long long uint64_t;
typedef     long long       int64_t;

//link.h
#define __WORDSIZE     32
#define __ELF_NATIVE_CLASS __WORDSIZE

#define ElfW(type)	_ElfW (Elf, __ELF_NATIVE_CLASS, type)
#define _ElfW(e,w,t)	_ElfW_1 (e, w, _##t)
#define _ElfW_1(e,w,t)	e##w##t

//elf.h
#define DT_NULL		0		/* Marks end of dynamic section */
#define DT_REL		17		/* Address of Rel relocs */
#define DT_RELSZ	18		/* Total size of Rel relocs */
#define DT_RELENT	19		/* Size of one Rel reloc */
#define DT_RELA		7		/* Address of Rela relocs */

#define ELF32_R_TYPE(val)		((val) & 0xff)

#define R_386_NONE	   0		/* No reloc */
#define R_386_RELATIVE	   8		/* Adjust by program base */

typedef uint32_t Elf32_Word;
typedef	int32_t  Elf32_Sword;
typedef uint32_t Elf32_Addr;

typedef struct
{
  Elf32_Sword	d_tag;			/* Dynamic entry type */
  union
    {
      Elf32_Word d_val;			/* Integer value */
      Elf32_Addr d_ptr;			/* Address value */
    } d_un;
} Elf32_Dyn;

typedef struct
{
  Elf32_Addr	r_offset;		/* Address */
  Elf32_Word	r_info;			/* Relocation type and symbol index */
} Elf32_Rel;


#undef NULL
#define uint64_t	efi_uint64_t
#define int64_t		efi_int64_t
#define uint32_t	efi_uint32_t
#define int32_t		efi_int32_t
#define uint16_t	efi_uint16_t
#define int16_t		efi_int16_t
#define uint8_t		efi_uint8_t
#define int8_t		efi_int8_t

#undef NULL
#define uint64_t	efi_uint64_t
#define int64_t		efi_int64_t
#define uint32_t	efi_uint32_t
#define int32_t		efi_int32_t
#define uint16_t	efi_uint16_t
#define int16_t		efi_int16_t
#define uint8_t		efi_uint8_t
#define int8_t		efi_int8_t

#include <efi.h>
#include <efilib.h>

EFI_STATUS _relocate (long ldbase, ElfW(Dyn) *dyn, EFI_HANDLE image, EFI_SYSTEM_TABLE *systab)
{
	long relsz = 0, relent = 0;
	ElfW(Rel) *rel = 0;
	unsigned long *addr;
	int i;

	for (i = 0; dyn[i].d_tag != DT_NULL; ++i) {
		switch (dyn[i].d_tag) {
			case DT_REL:
				rel = (ElfW(Rel)*)
					((unsigned long)dyn[i].d_un.d_ptr
					 + ldbase);
				break;

			case DT_RELSZ:
				relsz = dyn[i].d_un.d_val;
				break;

			case DT_RELENT:
				relent = dyn[i].d_un.d_val;
				break;

			case DT_RELA:
				break;

			default:
				break;
		}
	}

        if (!rel && relent == 0)
                return EFI_SUCCESS;

	if (!rel || relent == 0)
		return EFI_LOAD_ERROR;

	while (relsz > 0) {
		/* apply the relocs */
		switch (ELF32_R_TYPE (rel->r_info)) {
			case R_386_NONE:
				break;
			
			case R_386_RELATIVE:
				addr = (unsigned long *)
					(ldbase + rel->r_offset);
				*addr += ldbase;
				break;

			default:
				break;
		}
		rel = (ElfW(Rel)*) ((char *) rel + relent);
		relsz -= relent;
	}
	return EFI_SUCCESS;
}
