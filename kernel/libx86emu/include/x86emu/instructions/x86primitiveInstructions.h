#ifndef _X86EMU_X86EMUPRIMITIVEINSTRUCTIONS_H
#define _X86EMU_X86EMUPRIMITIVEINSTRUCTIONS_H

#include <x86emu/X86EmulatorEnvironment.h>
#include <x86emu/X86EmulatorInstructionDecoder.h>
#include <x86emu/x86EmulatorDebug.h>

/*
 * This stuff is mostly taken from x86emu prim_ops.c
 */

namespace X86Emu
{
    static u32 x86emu_parity_tab[8] =
    {
    	0x96696996,
    	0x69969669,
    	0x69969669,
    	0x96696996,
    	0x69969669,
    	0x96696996,
    	0x96696996,
    	0x69969669,
    };

    #define PARITY(x)   (((x86emu_parity_tab[(x) / 32] >> ((x) % 32)) & 1) == 0)
    #define XOR2(x) 	(((x) ^ ((x)>>1)) & 0x1)
    
    
    u8 DecrementByte(u8 b, X86EmulatorEnvironment* env)
    {
        register u32 result;
        register u32 borrowChain;
        
        result = b - 1;
        env->Flags()->SetFlag(FLAG_ZERO, (result & 0xff) == 0);
        env->Flags()->SetFlag(FLAG_SIGN, result & 0x80);
        env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
        
        borrowChain = (result & (~b | 1)) | (~b & 1);
        env->Flags()->SetFlag(FLAG_OVERFLOW, XOR2(borrowChain >> 6));
        env->Flags()->SetFlag(FLAG_AUX, borrowChain & 0x8);
        
        return (u8)result;
    }
    
    u16 DecrementWord(u16 w, X86EmulatorEnvironment* env)
    {
        register u32 result;
        register u32 borrowChain;
        
        result = w - 1;
        env->Flags()->SetFlag(FLAG_ZERO, (result & 0xffff) == 0);
        env->Flags()->SetFlag(FLAG_SIGN, result & 0x8000);
        env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
        
        borrowChain = (result & (~w | 1)) | (~w & 1);
        env->Flags()->SetFlag(FLAG_OVERFLOW, XOR2(borrowChain >> 14));
        env->Flags()->SetFlag(FLAG_AUX, borrowChain & 0x8);
        
        return (u16)result;
    }
    
    u32 DecrementDword(u32 d, X86EmulatorEnvironment* env)
    {
        register u32 result;
        register u32 borrowChain;
        
        result = d - 1;
        env->Flags()->SetFlag(FLAG_ZERO, (result & 0xffffffff) == 0);
        env->Flags()->SetFlag(FLAG_SIGN, result & 0x80000000);
        env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
        
        borrowChain = (result & (~d | 1)) | (~d & 1);
        env->Flags()->SetFlag(FLAG_OVERFLOW, XOR2(borrowChain >> 30));
        env->Flags()->SetFlag(FLAG_AUX, borrowChain & 0x8);
        
        return result;
    }
    
    u8 IncrementByte(u8 b, X86EmulatorEnvironment* env)
    {
        register u32 result;
        register u32 carryChain;
        
        result = b + 1;
        env->Flags()->SetFlag(FLAG_ZERO, (result & 0xff) == 0);
        env->Flags()->SetFlag(FLAG_SIGN, result & 0x80);
        env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
        
        carryChain = ((1 & b) | (~result)) & (1 | b);
        env->Flags()->SetFlag(FLAG_OVERFLOW, XOR2(carryChain >> 6));
        env->Flags()->SetFlag(FLAG_AUX, carryChain & 0x8);
        
        return (u8)result;
    }
    
    u16 IncrementWord(u16 w, X86EmulatorEnvironment* env)
    {
        register u32 result;
        register u32 carryChain;
        
        result = w + 1;
        env->Flags()->SetFlag(FLAG_ZERO, (result & 0xffff) == 0);
        env->Flags()->SetFlag(FLAG_SIGN, result & 0x8000);
        env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
        
        carryChain = ((1 & w) | (~result)) & (1 | w);
        env->Flags()->SetFlag(FLAG_OVERFLOW, XOR2(carryChain >> 14));
        env->Flags()->SetFlag(FLAG_AUX, carryChain & 0x8);
        
        return (u16)result;
    }
    
    u32 IncrementDword(u32 d, X86EmulatorEnvironment* env)
    {
        register u32 result;
        register u32 carryChain;
        
        result = d + 1;
        env->Flags()->SetFlag(FLAG_ZERO, (result & 0xffffffff) == 0);
        env->Flags()->SetFlag(FLAG_SIGN, result & 0x80000000);
        env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
        
        carryChain = ((1 & d) | (~result)) & (1 | d);
        env->Flags()->SetFlag(FLAG_OVERFLOW, XOR2(carryChain >> 30));
        env->Flags()->SetFlag(FLAG_AUX, carryChain & 0x8);
        
        return result;
    }
    
    u8 AddByte(u8 d, u8 s, X86EmulatorEnvironment* env)
    {
        register u32 result;
    	register u32 carryChain;

    	result = d + s;
    	env->Flags()->SetFlag(FLAG_CARRY, result & 0x100);
    	env->Flags()->SetFlag(FLAG_ZERO, (result & 0xff) == 0);
    	env->Flags()->SetFlag(FLAG_SIGN, result & 0x80);
        env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
    	
        carryChain = ((s & d) | (~result)) & (s | d);
    	env->Flags()->SetFlag(FLAG_OVERFLOW, XOR2(carryChain >> 6));
        env->Flags()->SetFlag(FLAG_AUX, carryChain & 0x8);
        
    	return (u8)result;
    }
    
    u16 AddWord(u16 d, u16 s, X86EmulatorEnvironment* env)
    {
        register u32 result;
    	register u32 carryChain;

    	result = d + s;
    	env->Flags()->SetFlag(FLAG_CARRY, result & 0x10000);
    	env->Flags()->SetFlag(FLAG_ZERO, (result & 0xffff) == 0);
    	env->Flags()->SetFlag(FLAG_SIGN, result & 0x8000);
        env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
    	
        carryChain = ((s & d) | (~result)) & (s | d);
    	env->Flags()->SetFlag(FLAG_OVERFLOW, XOR2(carryChain >> 14));
        env->Flags()->SetFlag(FLAG_AUX, carryChain & 0x8);
        
    	return (u16)result;
    }
    
    u32 AddDword(u32 d, u32 s, X86EmulatorEnvironment* env)
    {
        register u32 result;
    	register u32 carryChain;
    	register u32 low;
    	register u32 high;

        low = (d & 0xFFFF) + (s & 0xFFFF);
    	result = d + s;
    	high = (low >> 16) + (d >> 16) + (s >> 16);
    	
    	env->Flags()->SetFlag(FLAG_CARRY, high & 0x10000);
    	env->Flags()->SetFlag(FLAG_ZERO, (result & 0xffffffff) == 0);
    	env->Flags()->SetFlag(FLAG_SIGN, result & 0x80000000);
        env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
    	
        carryChain = ((s & d) | (~result)) & (s | d);
    	env->Flags()->SetFlag(FLAG_OVERFLOW, XOR2(carryChain >> 30));
        env->Flags()->SetFlag(FLAG_AUX, carryChain & 0x8);
        
    	return result;
    }
    
    u8 AddCarryByte(u8 d, u8 s, X86EmulatorEnvironment* env)
    {
        register u32 result;
    	register u32 carryChain;

        if(env->Flags()->GetFlag(FLAG_CARRY))
            result = 1 + d + s;
        else
    	    result = d + s;
    	
    	env->Flags()->SetFlag(FLAG_CARRY, result & 0x100);
    	env->Flags()->SetFlag(FLAG_ZERO, (result & 0xff) == 0);
    	env->Flags()->SetFlag(FLAG_SIGN, result & 0x80);
        env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
    	
        carryChain = ((s & d) | (~result)) & (s | d);
    	env->Flags()->SetFlag(FLAG_OVERFLOW, XOR2(carryChain >> 6));
        env->Flags()->SetFlag(FLAG_AUX, carryChain & 0x8);
        
    	return (u8)result;
    }
    
    u16 AddCarryWord(u16 d, u16 s, X86EmulatorEnvironment* env)
    {
        register u32 result;
    	register u32 carryChain;

    	if(env->Flags()->GetFlag(FLAG_CARRY))
            result = 1 + d + s;
        else
    	    result = d + s;
    	
    	env->Flags()->SetFlag(FLAG_CARRY, result & 0x10000);
    	env->Flags()->SetFlag(FLAG_ZERO, (result & 0xffff) == 0);
    	env->Flags()->SetFlag(FLAG_SIGN, result & 0x8000);
        env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
    	
        carryChain = ((s & d) | (~result)) & (s | d);
    	env->Flags()->SetFlag(FLAG_OVERFLOW, XOR2(carryChain >> 14));
        env->Flags()->SetFlag(FLAG_AUX, carryChain & 0x8);
        
    	return (u16)result;
    }
    
    u32 AddCarryDword(u32 d, u32 s, X86EmulatorEnvironment* env)
    {
        register u32 result;
    	register u32 carryChain;
    	register u32 low;
    	register u32 high;
    	
    	if(env->Flags()->GetFlag(FLAG_CARRY))
    	{
            low = 1 + (d & 0xFFFF) + (s & 0xFFFF);
            result = 1 + d + s;
        }
        else
        {
    	    low = (d & 0xFFFF) + (s & 0xFFFF);
        	result = d + s;
        }
        
    	high = (low >> 16) + (d >> 16) + (s >> 16);
    	
    	env->Flags()->SetFlag(FLAG_CARRY, high & 0x10000);
    	env->Flags()->SetFlag(FLAG_ZERO, (result & 0xffffffff) == 0);
    	env->Flags()->SetFlag(FLAG_SIGN, result & 0x80000000);
        env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
    	
        carryChain = ((s & d) | (~result)) & (s | d);
    	env->Flags()->SetFlag(FLAG_OVERFLOW, XOR2(carryChain >> 30));
        env->Flags()->SetFlag(FLAG_AUX, carryChain & 0x8);
        
    	return result;
    }
    
    u8 SubByte(u8 d, u8 s, X86EmulatorEnvironment* env)
    {
        register u32 result;
        register u32 borrowChain;
        
        result = d - s;
    	
    	env->Flags()->SetFlag(FLAG_SIGN, result & 0x80);
    	env->Flags()->SetFlag(FLAG_ZERO, (result & 0xff) == 0);
        env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
    	
    	borrowChain = (result & (~d | s)) | (~d & s);
    	env->Flags()->SetFlag(FLAG_CARRY, borrowChain & 0x80);
    	env->Flags()->SetFlag(FLAG_OVERFLOW, XOR2(borrowChain >> 6));
        env->Flags()->SetFlag(FLAG_AUX, borrowChain & 0x8);
        
    	return (u8)result;
    }
    
    u16 SubWord(u16 d, u16 s, X86EmulatorEnvironment* env)
    {
        register u32 result;
        register u32 borrowChain;
        
    	result = d - s;
    	
    	env->Flags()->SetFlag(FLAG_SIGN, result & 0x8000);
    	env->Flags()->SetFlag(FLAG_ZERO, (result & 0xffff) == 0);
        env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
    	
    	borrowChain = (result & (~d | s)) | (~d & s);
    	env->Flags()->SetFlag(FLAG_CARRY, borrowChain & 0x8000);
    	env->Flags()->SetFlag(FLAG_OVERFLOW, XOR2(borrowChain >> 14));
        env->Flags()->SetFlag(FLAG_AUX, borrowChain & 0x8);
        
    	return (u16)result;
    }
    
    u32 SubDword(u32 d, u32 s, X86EmulatorEnvironment* env)
    {
        register u32 result;
        register u32 borrowChain;
        
        result = d - s;
    	
    	env->Flags()->SetFlag(FLAG_SIGN, result & 0x80000000);
    	env->Flags()->SetFlag(FLAG_ZERO, (result & 0xffffffff) == 0);
        env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
    	
    	borrowChain = (result & (~d | s)) | (~d & s);
    	env->Flags()->SetFlag(FLAG_CARRY, borrowChain & 0x80000000);
    	env->Flags()->SetFlag(FLAG_OVERFLOW, XOR2(borrowChain >> 30));
        env->Flags()->SetFlag(FLAG_AUX, borrowChain & 0x8);
        
    	return (u32)result;
    }
    
    u8 SbbByte(u8 d, u8 s, X86EmulatorEnvironment* env)
    {
        register u32 result;
        register u32 borrowChain;
        
        if(env->Flags()->GetFlag(FLAG_CARRY))
            result = d - s - 1;
        else
    	    result = d - s;
    	
    	env->Flags()->SetFlag(FLAG_SIGN, result & 0x80);
    	env->Flags()->SetFlag(FLAG_ZERO, (result & 0xff) == 0);
        env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
    	
    	borrowChain = (result & (~d | s)) | (~d & s);
    	env->Flags()->SetFlag(FLAG_CARRY, borrowChain & 0x80);
    	env->Flags()->SetFlag(FLAG_OVERFLOW, XOR2(borrowChain >> 6));
        env->Flags()->SetFlag(FLAG_AUX, borrowChain & 0x8);
        
    	return (u8)result;
    }
    
    u16 SbbWord(u16 d, u16 s, X86EmulatorEnvironment* env)
    {
        register u32 result;
        register u32 borrowChain;
        
        if(env->Flags()->GetFlag(FLAG_CARRY))
            result = d - s - 1;
        else
    	    result = d - s;
    	
    	env->Flags()->SetFlag(FLAG_SIGN, result & 0x8000);
    	env->Flags()->SetFlag(FLAG_ZERO, (result & 0xffff) == 0);
        env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
    	
    	borrowChain = (result & (~d | s)) | (~d & s);
    	env->Flags()->SetFlag(FLAG_CARRY, borrowChain & 0x8000);
    	env->Flags()->SetFlag(FLAG_OVERFLOW, XOR2(borrowChain >> 14));
        env->Flags()->SetFlag(FLAG_AUX, borrowChain & 0x8);
        
    	return (u16)result;
    }
    
    u32 SbbDword(u32 d, u32 s, X86EmulatorEnvironment* env)
    {
        register u32 result;
        register u32 borrowChain;
        
        if(env->Flags()->GetFlag(FLAG_CARRY))
            result = d - s - 1;
        else
    	    result = d - s;
    	
    	env->Flags()->SetFlag(FLAG_SIGN, result & 0x80000000);
    	env->Flags()->SetFlag(FLAG_ZERO, (result & 0xffffffff) == 0);
        env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
    	
    	borrowChain = (result & (~d | s)) | (~d & s);
    	env->Flags()->SetFlag(FLAG_CARRY, borrowChain & 0x80000000);
    	env->Flags()->SetFlag(FLAG_OVERFLOW, XOR2(borrowChain >> 30));
        env->Flags()->SetFlag(FLAG_AUX, borrowChain & 0x8);
        
    	return (u32)result;
    }
    
    u8 OrByte(u8 d, u8 s, X86EmulatorEnvironment* env)
    {
        register u8 result;
        
        result = d | s;
        
        env->Flags()->SetFlag(FLAG_CARRY, false);
    	env->Flags()->SetFlag(FLAG_OVERFLOW, false);
        env->Flags()->SetFlag(FLAG_AUX, false);
        
    	env->Flags()->SetFlag(FLAG_SIGN, result & 0x80);
    	env->Flags()->SetFlag(FLAG_ZERO, result == 0);
    	env->Flags()->SetFlag(FLAG_PARITY, PARITY(result));
    	
        return result;
    }
    
    u16 OrWord(u16 d, u16 s, X86EmulatorEnvironment* env)
    {
        register u16 result;
        
        result = d | s;
        
        env->Flags()->SetFlag(FLAG_CARRY, false);
    	env->Flags()->SetFlag(FLAG_OVERFLOW, false);
        env->Flags()->SetFlag(FLAG_AUX, false);
        
    	env->Flags()->SetFlag(FLAG_SIGN, result & 0x8000);
    	env->Flags()->SetFlag(FLAG_ZERO, result == 0);
    	env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
    	
        return result;
    }
    
    u32 OrDword(u32 d, u32 s, X86EmulatorEnvironment* env)
    {
        register u32 result;
        
        result = d | s;
        
        env->Flags()->SetFlag(FLAG_CARRY, false);
    	env->Flags()->SetFlag(FLAG_OVERFLOW, false);
        env->Flags()->SetFlag(FLAG_AUX, false);
        
    	env->Flags()->SetFlag(FLAG_SIGN, result & 0x80000000);
    	env->Flags()->SetFlag(FLAG_ZERO, result == 0);
    	env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
    	
        return result;
    }
    
    u8 AndByte(u8 d, u8 s, X86EmulatorEnvironment* env)
    {
        register u8 result;
        
        result = d & s;
        
        env->Flags()->SetFlag(FLAG_CARRY, false);
    	env->Flags()->SetFlag(FLAG_OVERFLOW, false);
        env->Flags()->SetFlag(FLAG_AUX, false);
        
    	env->Flags()->SetFlag(FLAG_SIGN, result & 0x80);
    	env->Flags()->SetFlag(FLAG_ZERO, result == 0);
    	env->Flags()->SetFlag(FLAG_PARITY, PARITY(result));
    	
        return result;
    }
    
    u16 AndWord(u16 d, u16 s, X86EmulatorEnvironment* env)
    {
        register u16 result;
        
        result = d & s;
        
        env->Flags()->SetFlag(FLAG_CARRY, false);
    	env->Flags()->SetFlag(FLAG_OVERFLOW, false);
        env->Flags()->SetFlag(FLAG_AUX, false);
        
    	env->Flags()->SetFlag(FLAG_SIGN, result & 0x8000);
    	env->Flags()->SetFlag(FLAG_ZERO, result == 0);
    	env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
    	
        return result;
    }
    
    u32 AndDword(u32 d, u32 s, X86EmulatorEnvironment* env)
    {
        register u32 result;
        
        result = d & s;
        
        env->Flags()->SetFlag(FLAG_CARRY, false);
    	env->Flags()->SetFlag(FLAG_OVERFLOW, false);
        env->Flags()->SetFlag(FLAG_AUX, false);
        
    	env->Flags()->SetFlag(FLAG_SIGN, result & 0x80000000);
    	env->Flags()->SetFlag(FLAG_ZERO, result == 0);
    	env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
    	
        return result;
    }
    
    u8 XorByte(u8 d, u8 s, X86EmulatorEnvironment* env)
    {
        register u8 result;
        
        result = d ^ s;
        
        env->Flags()->SetFlag(FLAG_CARRY, false);
    	env->Flags()->SetFlag(FLAG_OVERFLOW, false);
        env->Flags()->SetFlag(FLAG_AUX, false);
        
    	env->Flags()->SetFlag(FLAG_SIGN, result & 0x80);
    	env->Flags()->SetFlag(FLAG_ZERO, result == 0);
    	env->Flags()->SetFlag(FLAG_PARITY, PARITY(result));
    	
        return result;
    }
    
    u16 XorWord(u16 d, u16 s, X86EmulatorEnvironment* env)
    {
        register u16 result;
        
        result = d ^ s;
        
        env->Flags()->SetFlag(FLAG_CARRY, false);
    	env->Flags()->SetFlag(FLAG_OVERFLOW, false);
        env->Flags()->SetFlag(FLAG_AUX, false);
        
    	env->Flags()->SetFlag(FLAG_SIGN, result & 0x8000);
    	env->Flags()->SetFlag(FLAG_ZERO, result == 0);
    	env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
    	
        return result;
    }
    
    u32 XorDword(u32 d, u32 s, X86EmulatorEnvironment* env)
    {
        register u32 result;
        
        result = d ^ s;
        
        env->Flags()->SetFlag(FLAG_CARRY, false);
    	env->Flags()->SetFlag(FLAG_OVERFLOW, false);
        env->Flags()->SetFlag(FLAG_AUX, false);
        
    	env->Flags()->SetFlag(FLAG_SIGN, result & 0x80000000);
    	env->Flags()->SetFlag(FLAG_ZERO, result == 0);
    	env->Flags()->SetFlag(FLAG_PARITY, PARITY(result & 0xff));
    	
        return result;
    }
    
    u16 AadWord(u16 d, X86EmulatorEnvironment* env)
    {
        u16 l;
    	u8 hb, lb;

    	hb = (u8)((d >> 8) & 0xff);
    	lb = (u8)((d & 0xff));
    	l = (u16)((lb + 10 * hb) & 0xFF);

    	env->Flags()->SetFlag(FLAG_CARRY, false);
        env->Flags()->SetFlag(FLAG_AUX, false);
    	env->Flags()->SetFlag(FLAG_OVERFLOW, false);
    	
    	env->Flags()->SetFlag(FLAG_SIGN, l & 0x80000000);
    	env->Flags()->SetFlag(FLAG_SIGN, l & 0x80000000);
    	env->Flags()->SetFlag(FLAG_PARITY, l & 0x80000000);
    	
    	env->Flags()->SetFlag(FLAG_SIGN, l & 0x80);
    	env->Flags()->SetFlag(FLAG_ZERO, l == 0);
    	env->Flags()->SetFlag(FLAG_PARITY, PARITY(l & 0xff));
    	
        return l;
    }
    
    u16 AamWord(u16 d, X86EmulatorEnvironment* env)
    {
        u16 h, l;

    	h = (u16)(d / 10);
    	l = (u16)(d % 10);
    	l |= (u16)(h << 8);

    	env->Flags()->SetFlag(FLAG_CARRY, false);
        env->Flags()->SetFlag(FLAG_AUX, false);
    	env->Flags()->SetFlag(FLAG_OVERFLOW, false);
    	
    	env->Flags()->SetFlag(FLAG_SIGN, l & 0x80000000);
    	env->Flags()->SetFlag(FLAG_SIGN, l & 0x80000000);
    	env->Flags()->SetFlag(FLAG_PARITY, l & 0x80000000);
    	
    	env->Flags()->SetFlag(FLAG_SIGN, l & 0x80);
    	env->Flags()->SetFlag(FLAG_ZERO, l == 0);
    	env->Flags()->SetFlag(FLAG_PARITY, PARITY(l & 0xff));
    	
        return l;
    }
    
    //FIXME: We need to be careful here. If we want to push any 32 bit register, then the 32 bit data
    //FIXME: prefix will be set. We are reading and writing SP here, so we are reading and writing ESP
    //FIXME: because of this prefix. How does the CPU behave? does it always use SP? Always ESP? Undocumented shit...
    //FIXME: Seems to operate on SP only
    
    void pushWord(u16 val, X86EmulatorEnvironment* env)
    {
        env->Registers()->SetRegister(SP, (u16)env->Registers()->GetRegister(SP) - 2);
        
        env->Memory()->WriteWord(
            (env->Registers()->GetSegmentRegister(SS) << 4) + env->Registers()->GetRegister(SP),
            val
        );
    }
    
    void pushDword(u32 val, X86EmulatorEnvironment* env)
    {
        env->Registers()->SetRegister(SP, (u16)env->Registers()->GetRegister(SP) - 4);
        
        env->Memory()->WriteDword(
            (env->Registers()->GetSegmentRegister(SS) << 4) + env->Registers()->GetRegister(SP),
            val
        );
    }
    
    u16 popWord(X86EmulatorEnvironment* env)
    {
        u16 val = env->Memory()->ReadWord((env->Registers()->GetSegmentRegister(SS) << 4) + env->Registers()->GetRegister(SP));
        env->Registers()->SetRegister(SP, (u16)env->Registers()->GetRegister(SP) + 2);
        return val;
    }
    
    u32 popDword(X86EmulatorEnvironment* env)
    {
        u32 val = env->Memory()->ReadDword((env->Registers()->GetSegmentRegister(SS) << 4) + env->Registers()->GetRegister(SP));
        env->Registers()->SetRegister(SP, (u16)env->Registers()->GetRegister(SP) + 4);
        return val;
    }
}

#endif
