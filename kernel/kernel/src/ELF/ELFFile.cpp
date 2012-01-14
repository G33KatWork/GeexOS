#include <kernel/ELF/ELFFile.h>
#include <kernel/global.h>

using namespace ELF;

ELFFile::ELFFile(Address startAddress)
    : start(startAddress),
    programHeaderCount(0),
    sectionHeaderCount(0)
{
    this->parse();
}

ELFFile::~ELFFile()
{
    
}

void ELFFile::Relocate(Address toAddress)
{
    if((((Elf32Header*)this->start)->type & ELF_TYPE_RELOCATABLE) == 0)
        PANIC("ELF is not relocatable but Relocate() was called");
    
    
}

Address ELFFile::GetEntrypoint()
{
    Elf32Header* header = (Elf32Header*)this->start;
    return (Address)header->entry;
}

Elf32ProgramHeader* ELFFile::GetProgramHeader(size_t idx)
{
    ASSERT(idx < GetProgramHeaderCount(), "Index out of bounds");
    
    Elf32ProgramHeader* firstHeader = (Elf32ProgramHeader*)(((uint8_t*)this->start) + ((Elf32Header*)this->start)->phoff);
    return firstHeader + idx;
}

Address ELFFile::GetProgramContents(Elf32ProgramHeader* program)
{
    ASSERT(program, "Program to get contents for may not be NULL");
    
    return (Address)(this->start + program->offset);
}

Elf32SectionHeader* ELFFile::GetSectionHeader(size_t idx)
{
    ASSERT(idx < GetSectionHeaderCount(), "Index out of bounds");
    
    Elf32SectionHeader* firstHeader = (Elf32SectionHeader*)(((uint8_t*)this->start) + ((Elf32Header*)this->start)->shoff);
    return firstHeader + idx;
}

Address ELFFile::GetSectionContents(Elf32SectionHeader* section)
{
    ASSERT(section, "Section to get contents for may not be NULL");
    
    return (Address)(this->start + section->offset);
}

void ELFFile::parse()
{
    ASSERT(this->isELF(), "Provided memory address doesn't seem to contain an ELF32");
    
    Elf32Header* header = (Elf32Header*)this->start;
    programHeaderCount = header->phnum;
    sectionHeaderCount = header->shnum;
}

bool ELFFile::isELF()
{
    Elf32Header* header = (Elf32Header*)this->start;
    
    return 
        header->ident[ELF_IDENT_MAGIC_OFFSET] == ELF_IDENT_MAGIC_0 &&
        header->ident[ELF_IDENT_MAGIC_OFFSET+1] == ELF_IDENT_MAGIC_1 &&
        header->ident[ELF_IDENT_MAGIC_OFFSET+2] == ELF_IDENT_MAGIC_2 &&
        header->ident[ELF_IDENT_MAGIC_OFFSET+3] == ELF_IDENT_MAGIC_3 &&
        header->ident[ELF_IDENT_CLASS_OFFSET] == ELF_IDENT_CLASS_32;
}
