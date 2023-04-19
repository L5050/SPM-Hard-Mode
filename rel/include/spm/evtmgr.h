#pragma once

#include <types.h>

namespace spm::evtmgr {

#define EVT_max 0x78
#define EVT_ENTRY_MAX 0x80
#define MAX_EVT_JMPTBL 16

struct EvtEntry;
typedef s32 (user_func)(struct EvtEntry * entry, bool firstRun);
typedef s32 EvtScriptCode;

#define EVT_FLAG_IN_USE (1 << 0)
#define EVT_FLAG_PAUSED (1 << 1)
typedef s32 (UserFunc)(struct _EvtEntry * entry, bool firstRun);
struct EvtEntry
{
/* 0x000 */ s64 lifetime; // Time this script has been running & not paused
/* 0x008 */ u8 flags; // Bit flags, see defines above
/* 0x009 */ u8 curDataLength; // Number of pieces of data (4 bytes each) belonging to the
                              // current instruction
/* 0x00A */ u8 curOpcode; // Opcode of the current instruction
/* 0x00B */ u8 priority; // Priority for script to run (higher value -> earlier)
/* 0x00C */ u8 type; // Type mask for stopping/starting
/* 0x00D */ s8 blocked; // Keep re-exucuting the current instruction if true
                        // (decomp doesn't match as bool)
/* 0x00E */ s8 doWhileDepth; // Number of do-while loops the current instruction is nested into
/* 0x00F */ s8 switchDepth; // Number of switches the current instruction is nested into
/* 0x010 */ EvtScriptCode * pCurInstruction; // Current instruction being executed
/* 0x014 */ EvtScriptCode * pCurData; // Args for the current instruction being executed
/* 0x018 */ s8 labelIds[MAX_EVT_JMPTBL]; // Each correspond to an address in the jump table
/* 0x028 */ void * jumptable[MAX_EVT_JMPTBL]; // Addresses for each label
/* 0x068 */ struct _EvtEntry * parent; // Script that started this one as a child
/* 0x06C */ struct _EvtEntry * childEntry; // Script started by this one, pausing this until it's
                                           // finished
/* 0x070 */ struct _EvtEntry * brotherEntry; // Script started by this one which will be stopped if
                                             // this one is
/* 0x074 */ union
            {
                s32 tempS[3];
                u32 tempU[3];
            }; // Temp values with meanings varying by instruction / user func
/* 0x080 */ u8 unknown_0x80[0x94 - 0x80];
/* 0x094 */ UserFunc * userFunc; // Function set & called by the user_func opcode
/* 0x098 */ s32 lw[16]; // Local Work variables
/* 0x0D8 */ u32 lf[3]; // Local Flag variables
/* 0x0E4 */ void * doWhileStartPtrs[8]; // Pointer to do opcodes for active while loops
/* 0x104 */ s32 doWhileCounters[8]; // Counters for active while loops
/* 0x124 */ s8 switchStates[8]; // Sates of active switch statements
/* 0x12C */ s32 switchValues[8]; // Values passed into active switch statements
/* 0x14C */ union
            {
                s32 * readAddr;
                f32 * readfAddr;
            }; // Current addr for read/readf instructions
/* 0x150 */ s32 * uw; // User Work variables pointer
/* 0x154 */ u32 * uf; // User Flag variables pointer
/* 0x158 */ s32 id; // Script id
/* 0x15C */ f32 speed; // timeToRun given per frame
/* 0x160 */ f32 timeToRun; // Number of instructions to execute this frame
/* 0x164 */ s32 casedrvId;
/* 0x168 */ void * ownerNPC;
/* 0x16C */ void * ownerMOBJ;
/* 0x170 */ s32 unknown_0x170;
/* 0x174 */ s32 msgWindowId;
/* 0x178 */ s32 unknown_0x178;
/* 0x17C */ f32 unknown_0x17c;
/* 0x180 */ f32 unknown_0x180;
/* 0x184 */ f32 unknown_0x184;
/* 0x188 */ s32 msgPri;
/* 0x18C */ u8 unknown_0x18c[0x198 - 0x18c];
/* 0x198 */ EvtScriptCode * scriptStart; // Pointer to the start of this script
/* 0x19C */ char * name; // debug thing, unused?
/* 0x1A0 */ void * pPrevInstruction; // Pointer to last instruction executed
/* 0x1A4 */ u8 unknown_0x1a4[0x1a8 - 0x1a4];
};
static_assert(sizeof(EvtEntry) == 0x1a8);

struct EvtWork
{
    s32 entryCount;
    s32 gw[32];
    u32 gf[3];
    EvtEntry * entries;
    u8 unknown_0x94[0x98 - 0x94];
    s64 time;
};
static_assert(sizeof(EvtWork) == 0xa0);

extern "C" {

EvtWork * evtGetWork();
EvtEntry * evtEntry(const EvtScriptCode * script, u8 priority, u8 flags);
EvtEntry * evtEntryType(const EvtScriptCode * script, u8 priority, u8 flags, s32 type);
EvtEntry * evtChildEntry(EvtEntry * entry, const EvtScriptCode * script, u8 flags);
EvtEntry * evtBrotherEntry(EvtEntry * entry, const EvtScriptCode * script, u8 flags);
bool evtCheckId(s32 id);
void evtSetSpeed(EvtEntry *entry, float multiplier);
// more

}

#define EVT_GET_GW(id) \
    (spm::evtmgr::evtGetWork()->gw[(id)])

#define EVT_SET_GW(id, val) \
    spm::evtmgr::evtGetWork()->gw[(id)] = (val)

#define EVT_GET_GF(id) \
    ((spm::evtmgr::evtGetWork()->gf[(id) / 32] >> ((id) % 32)) & 1)

#define EVT_SET_GF(id) \
    spm::evtmgr::evtGetWork()->gf[(id) / 32] |= 1 << ((id) % 32)

#define EVT_CLEAR_GF(id) \
    spm::evtmgr::evtGetWork()->gf[(id) / 32] &= ~(1 << ((id) % 32))

#define EVT_DECLARE_USER_FUNC(name, parameter_count) \
    constexpr int name##_parameter_count = (parameter_count); \
    s32 name(spm::evtmgr::EvtEntry * evt, bool isFirstCall);

#define EVT_DEFINE_USER_FUNC(name) \
    s32 name(spm::evtmgr::EvtEntry * evt, bool isFirstCall)

#define EVT_DECLARE(name) \
    extern const spm::evtmgr::EvtScriptCode name[];

}
