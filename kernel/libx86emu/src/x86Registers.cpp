#include <x86emu/x86Registers.h>
#include <x86emu/x86EmulatorDebug.h>
#include <x86emu/x86EmulatorEnvironment.h>

using namespace X86Emu;

u32 X86Registers::GetRegister(X86Register reg)
{
    X86EMU_DEBUG_MSG("Getting register " << GetRegisterName(reg));
    switch(REGISTER_TYPE(reg))
    {
        case REGISTER_TYPE_LOW:
            return registers[REGISTER(reg)] & 0x000000FF;
        
        case REGISTER_TYPE_HIGH:
            return registers[REGISTER(reg)] & 0x0000FF00;
        
        case REGISTER_TYPE_WORD:
            if(env->Overrides()->GetOverride(PREFIX_DATA32))
                return registers[REGISTER(reg)];
            else
                return registers[REGISTER(reg)] & 0x0000FFFF;
        
        case REGISTER_TYPE_INDEX:
            if(env->Overrides()->GetOverride(PREFIX_DATA32))
                return registers[REGISTER(reg) | 0x4];
            else
                return registers[REGISTER(reg) | 0x4] & 0x0000FFFF;
        
        default:
            X86EMU_DEBUG_MSG("Wrong register selector supplied");
            env->HaltMachine();
            break;
    }
}

void X86Registers::SetRegister(X86Register reg, u32 NewValue)
{
    X86EMU_DEBUG_MSG("Setting register " << GetRegisterName(reg) << " to " << hex << NewValue);
    switch(REGISTER_TYPE(reg))
    {
        case REGISTER_TYPE_LOW:
            registers[REGISTER(reg)] |= (u8)(NewValue);
            break;
        
        case REGISTER_TYPE_HIGH:
            registers[REGISTER(reg)] &= 0xFFFF00FF;
            registers[REGISTER(reg)] |= (u16)((NewValue << 8) & 0x0000FF00);
            break;
        
        case REGISTER_TYPE_WORD:
            if(env->Overrides()->GetOverride(PREFIX_DATA32))
                registers[REGISTER(reg)] = NewValue;
            else
            {
                registers[REGISTER(reg)] &= 0xFFFF0000;
                registers[REGISTER(reg)] |= (u16)NewValue;
            }
            break;
        
        case REGISTER_TYPE_INDEX:
            if(env->Overrides()->GetOverride(PREFIX_DATA32))
                registers[REGISTER(reg) | 0x4] = NewValue;
            else
            {
                registers[REGISTER(reg) | 0x4] &= 0xFFFF0000;
                registers[REGISTER(reg) | 0x4] |= (u16)NewValue;
            }
            break;
        
        default:
            X86EMU_DEBUG_MSG("Wrong register selector supplied");
            env->HaltMachine();
            break;
    }
}

const char* X86Registers::GetRegisterName(X86Register reg)
{
    switch(reg)
    {
        case AL:
            return "AL";
        case AH:
            return "AH";
        case BL:
            return "BL";
        case BH:
            return "BH";
        case CL:
            return "CL";
        case CH:
            return "CH";
        case DL:
            return "DL";
        case DH:
            return "DH";
        case AX:
            if(env->Overrides()->GetOverride(PREFIX_DATA32)) return "EAX"; else return "AX";
        case BX:
            if(env->Overrides()->GetOverride(PREFIX_DATA32)) return "EBX"; else return "BX";
        case CX:
            if(env->Overrides()->GetOverride(PREFIX_DATA32)) return "ECX"; else return "CX";
        case DX:
            if(env->Overrides()->GetOverride(PREFIX_DATA32)) return "EDX"; else return "DX";
        case SP:
            if(env->Overrides()->GetOverride(PREFIX_DATA32)) return "ESP"; else return "SP";
        case BP:
            if(env->Overrides()->GetOverride(PREFIX_DATA32)) return "EBP"; else return "BP";
        case SI:
            if(env->Overrides()->GetOverride(PREFIX_DATA32)) return "ESI"; else return "SI";
        case DI:
            if(env->Overrides()->GetOverride(PREFIX_DATA32)) return "EDI"; else return "DI";
    }
}
