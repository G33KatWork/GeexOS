#include <x86emu/x86EmulatorInstructionDecoder.h>
#include <x86emu/instructions/x86IllegalInstruction.h>
#include <x86emu/x86EmulatorDebug.h>

#include <x86emu/instructions/x86SegmentOverridePrefixes.h>
#include <x86emu/instructions/x86IncrementDecrementRegisters.h>
#include <x86emu/instructions/x86SmallInstructions.h>
#include <x86emu/instructions/x86MovInstructions.h>
#include <x86emu/instructions/x86StackInstructions.h>
#include <x86emu/instructions/x86JumpInstructions.h>
#include <x86emu/instructions/x86XchgInstructions.h>
#include <x86emu/instructions/x86CallRetInstructions.h>
#include <x86emu/instructions/x86AddInstructions.h>
#include <x86emu/instructions/x86OrInstructions.h>
#include <x86emu/instructions/x86SubInstructions.h>
#include <x86emu/instructions/x86AndInstructions.h>
#include <x86emu/instructions/x86XorInstructions.h>
#include <x86emu/instructions/x86AsciiAdjustInstructions.h>

using namespace X86Emu;

X86Instruction X86EmulatorInstructionDecoder::operations[256] = {
    X86IllegalInstruction,  /*0x0*/
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86AddByteALRegister,
    X86AddWordAXRegister,
    X86PushESInstruction,
    X86PopESInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86OrByteALRegister,
    X86OrWordAXRegister,
    X86PushCSInstruction,
    X86IllegalInstruction,
    
    X86IllegalInstruction,  /* 0x10 */
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86AddCarryByteALRegister,
    X86AddCarryWordAXRegister,
    X86PushSSInstruction,
    X86PopSSInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86SbbByteALRegister,
    X86SbbWordAXRegister,
    X86PushDSInstruction,
    X86PopDSInstruction,
    
    X86IllegalInstruction,  /* 0x20 */
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86AndByteALRegister,
    X86AndWordAXRegister,
    X86SetESSegmentOverride,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86SubByteALRegister,
    X86SubWordAXRegister,
    X86SetCSSegmentOverride,    /* 0x2e */
    X86IllegalInstruction,
    
    X86IllegalInstruction,  /* 0x30 */
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86XorByteALRegister,
    X86XorWordAXRegister,
    X86SetSSSegmentOverride,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86SetDSSegmentOverride,
    X86IllegalInstruction,
    
    X86IncrementRegister,         /* 0x40 */
    X86IncrementRegister,
    X86IncrementRegister,
    X86IncrementRegister,
    X86IncrementIndexRegister,
    X86IncrementIndexRegister,
    X86IncrementIndexRegister,
    X86IncrementIndexRegister,
    X86DecrementRegister,
    X86DecrementRegister,
    X86DecrementRegister,
    X86DecrementRegister,
    X86DecrementIndexRegister,
    X86DecrementIndexRegister,
    X86DecrementIndexRegister,
    X86DecrementIndexRegister,
    
    X86PushWordRegisterInstruction,  /* 0x50 */
    X86PushWordRegisterInstruction,
    X86PushWordRegisterInstruction,
    X86PushWordRegisterInstruction,
    X86PushWordRegisterInstruction,
    X86PushWordRegisterInstruction,
    X86PushWordRegisterInstruction,
    X86PushWordRegisterInstruction,
    X86PopWordRegisterInstruction,
    X86PopWordRegisterInstruction,
    X86PopWordRegisterInstruction,
    X86PopWordRegisterInstruction,
    X86PopWordRegisterInstruction,
    X86PopWordRegisterInstruction,
    X86PopWordRegisterInstruction,
    X86PopWordRegisterInstruction,
    
    X86IllegalInstruction,  /* 0x60 */
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86SetFSSegmentOverride,
    X86SetGSSegmentOverride,
    X86Set32BitDataPrefix,
    X86Set32BitAddressPrefix,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    
    X86JumpNearOverflowInstruction,  /* 0x70 */
    X86JumpNearNoOverflowInstruction,
    X86JumpNearCarryInstruction,
    X86JumpNearNoCarryInstruction,
    X86JumpNearZeroInstruction,
    X86JumpNearNoZeroInstruction,
    X86JumpNearCarryOrZeroInstruction,
    X86JumpNearNotCarryOrZeroInstruction,
    X86JumpNearSignInstruction,
    X86JumpNearNoSignInstruction,
    X86JumpNearParityInstruction,
    X86JumpNearNoParityInstruction,
    X86JumpNearLessInstruction,
    X86JumpNearNotLessInstruction,
    X86JumpNearLessOrEqualInstruction,
    X86JumpNearNotLessOrEqualInstruction,
    
    X86IllegalInstruction,  /* 0x80 */
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    
    X86NopInstruction,  /* 0x90 */
    X86XchgAXWordRegisterInstruction,
    X86XchgAXWordRegisterInstruction,
    X86XchgAXWordRegisterInstruction,
    X86XchgAXIndexRegisterInstruction,
    X86XchgAXIndexRegisterInstruction,
    X86XchgAXIndexRegisterInstruction,
    X86XchgAXIndexRegisterInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86CallFarImmediateInstruction,
    X86WaitInstruction,
    X86PushFlagsInstruction,
    X86PopFlagsInstruction,
    X86SetFlagsFromAHInstruction,
    X86SetAHFromFlagsInstruction,
    
    X86MovALMemoryInstruction,  /* 0xa0 */
    X86MovAXMemoryInstruction,
    X86MovMemoryALInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    
    X86MovByteRegisterImmediateInstruction,  /* 0xb0 */
    X86MovByteRegisterImmediateInstruction,
    X86MovByteRegisterImmediateInstruction,
    X86MovByteRegisterImmediateInstruction,
    X86MovByteRegisterImmediateInstruction,
    X86MovByteRegisterImmediateInstruction,
    X86MovByteRegisterImmediateInstruction,
    X86MovByteRegisterImmediateInstruction,
    X86MovWordRegisterImmediateInstruction,
    X86MovWordRegisterImmediateInstruction,
    X86MovWordRegisterImmediateInstruction,
    X86MovWordRegisterImmediateInstruction,
    X86MovWordRegisterImmediateInstruction,
    X86MovWordRegisterImmediateInstruction,
    X86MovWordRegisterImmediateInstruction,
    X86MovWordRegisterImmediateInstruction,
    
    X86IllegalInstruction,  /* 0xc0 */
    X86IllegalInstruction,
    X86RetNearImmediateInstruction,
    X86RetNearInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86EnterInstruction,
    X86LeaveInstruction,
    X86RetFarImmediateInstruction,
    X86RetFarInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    
    X86IllegalInstruction,  /* 0xd0 */
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86AamWordAXRegister,
    X86AadWordAXRegister,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    
    X86IllegalInstruction,  /* 0xe0 */
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86CallNearImmediateInstruction,
    X86JmpNearImmediateInstruction,
    X86JmpFarImmediateInstruction,
    X86JmpNearImmediateByteInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    
    X86LockInstruction,     /* 0xf0 */
    X86IllegalInstruction,
    X86SetRepnePrefix,
    X86SetRepePrefix,
    X86HltInstruction,
    X86CmcInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction,
    X86ClcInstruction,
    X86StcInstruction,
    X86CliInstruction,
    X86StiInstruction,
    X86CldInstruction,
    X86StdInstruction,
    X86IllegalInstruction,
    X86IllegalInstruction
};

void X86EmulatorInstructionDecoder::ExecuteNext(X86EmulatorEnvironment* env)
{
    //get next linear ip
    unsigned long ip = (env->Registers()->GetSegmentRegister(CS) << 4) + env->Registers()->GetInstructionPointer();
    X86EMU_DEBUG_MSG("Fetching next instruction from " << hex << ip);
    X86EMU_DEBUG_MSG("Real memory address of instruction is " << hex << (env->Memory()->MemoryBase() + ip));
    u8 opcode = *((u8*)(env->Memory()->MemoryBase() + ip));   //FIXME: use functions do read from memory! perhaps a whole class for memory?
    X86EMU_DEBUG_MSG("Executing opcode " << hex << (unsigned)opcode);
    env->Registers()->SetInstructionPointer(ip + 1);
    operations[opcode](env, this, opcode);
}
