#ifndef _X86EMU_X86EMUSEGMENTOVERRIDES_H
#define _X86EMU_X86EMUSEGMENTOVERRIDES_H

#include <x86emu/x86EmulatorTypes.h>
#include <string.h>

namespace X86Emu
{
    #define NUM_SEG_OVERRIDES       10
    enum x86SegmentOverride
    {
        SEGOVR_CS       = 0,
        SEGOVR_DS       = 1,
        SEGOVR_ES       = 2,
        SEGOVR_FS       = 3,
        SEGOVR_GS       = 4,
        SEGOVR_SS       = 5,
        PREFIX_DATA32   = 6,
        PREFIX_ADDR32   = 7,
        PREFIX_REPE     = 8,
        PREFIX_REPNE    = 9
    };
    
    class X86SegmentOverrides
    {
    private:
        bool overrideValues[NUM_SEG_OVERRIDES];
        
    public:
        X86SegmentOverrides()
        {
            memset(&overrideValues, false, sizeof(overrideValues));
        }
        
        void Clear()
        {
            memset(&overrideValues, false, sizeof(overrideValues - 2)); //don't reset REPE/REPNE
        }
        
        bool GetOverride(x86SegmentOverride Override)
        {
            return overrideValues[Override];
        }
        
        void SetFlag(x86SegmentOverride Override, bool Value)
        {
            overrideValues[Override] = Value;
        }
    };
}

#endif
