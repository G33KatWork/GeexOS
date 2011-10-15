#ifndef _ELFFILE_H_
#define _ELFFILE_H_

#include <types.h>
#include <kernel/ELF/format/elf.h>

namespace ELF {
    class ELFFile
    {
    public:
        ELFFile(Address startAddress, size_t length);
        virtual ~ELFFile();
    
        void Relocate(Address toAddress);
        
        size_t GetProgramHeaderCount() { return programHeaderCount; }
        Elf32ProgramHeader* GetProgramHeader(size_t idx);   //FIXME: encapsulate Elf32ProgramHeader with a class
        Address GetProgramContents(Elf32ProgramHeader* program);
        
        size_t GetSectionHeaderCount() { return sectionHeaderCount; }
        Elf32SectionHeader* GetSectionHeader(size_t idx);   //FIXME: encapsulate Elf32SectionHeader with a class
        Address GetSectionContents(Elf32SectionHeader* section);
        
        
        Address GetEntrypoint();
    
    protected:    
        bool isELF();
        void parse();
    
    private:
        Address start;
        size_t len;
        
        size_t programHeaderCount;
        size_t sectionHeaderCount;
    };
}

#endif
