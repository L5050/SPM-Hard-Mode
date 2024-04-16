#pragma once

#include <wii/types.h>

namespace spm::npcdrv {

#define NPC_TRIBE_COUNT 535
#define NPCTEMPLATE_MAX 435
typedef s32 EvtScriptCode;
typedef bool (EnemyCanSpawnFunction)();
typedef struct
{
/* 0x00 */ u8 unknown_0x0;
/* 0x01 */ u8 unknown_0x1;
/* 0x02 */ u8 unknown_0x2;
/* 0x03 */ u8 unknown_0x3;
/* 0x04 */ s32 instanceId; // left blank to be copied from SetupEnemy
/* 0x08 */ u32 unknown_0x8;
/* 0x0C */ EnemyCanSpawnFunction * canSpawnFunction;
/* 0x10 */ u8 unknown_0x10[0x14 - 0x10];
/* 0x14 */ s32 tribeId;
/* 0x18 */ const char * instanceName; // overrides instanceName of NPC spawned if not null
/* 0x1C */ const char * japaneseName; // unused debug information?
/* 0x20 */ wii::Vec3 pos; // left blank to be copied from SetupEnemy
/* 0x2C */ u32 flags; // ORd with NPCEntry's flags after spawning
/* 0x30 */ EvtScriptCode * onSpawnScript;
/* 0x34 */ EvtScriptCode * unkScript1;
/* 0x38 */ EvtScriptCode * unkScript2;
/* 0x3C */ EvtScriptCode * unkScript3;
/* 0x40 */ EvtScriptCode * unkScript4;
/* 0x44 */ EvtScriptCode * unkScript5;
/* 0x48 */ EvtScriptCode * unkScript6;
/* 0x4C */ EvtScriptCode * unkScript7;
/* 0x50 */ EvtScriptCode * unkScript8;
/* 0x54 */ EvtScriptCode * unkScript9;
/* 0x58 */ void * unknown_0x58;
/* 0x5C */ u8 unknown_0x5c[0x68 - 0x5c]; // all left blank to be copied from SetupEnemy
} NPCEnemyTemplate;
static_assert(sizeof(NPCEnemyTemplate) == 0x68);

typedef struct {
/* 0x00 */ s32 instanceId; // ignored if 0
/* 0x04 */ u8 unknown_0x4[0x10 - 0x4];
/* 0x10 */ s32 unitWork[16];
/* 0x50 */ u8 unknown_0x50[0x5c - 0x50];
/* 0x5C */ float gravityRotation;
} MiscSetupDataV6;
static_assert(sizeof(MiscSetupDataV6) == 0x60);
struct NPCTribeAnimDef
{
    s32 id;
    const char * animName;
};
static_assert(sizeof(NPCTribeAnimDef) == 0x8);

struct NPCDefense
{
    u8 type;
    u8 defense;
    u16 flags;
};
static_assert(sizeof(NPCDefense) == 0x4);

struct NPCPartDef
{
    u16 id;
    u8 unknown_0x2[0x28 - 0x2];
    NPCDefense * defenses;
    u32 unknown_0x2c;
    void * updateFunc;
    u8 unknown_0x34[0x48 - 0x34];
};
static_assert(sizeof(NPCPartDef) == 0x48);

struct DropItem
{
    s32 itemId;
    s32 chance; // relative
};
static_assert(sizeof(DropItem) == 0x8);

struct NPCTribe
{
    const char * animPoseName;
    NPCTribeAnimDef * animDefs;
    s32 catchCardItemId;
    u8 unknown_0xc[0x18 - 0xc];
    u8 maxHp;
    u8 partsCount;
    u8 unknown_0x1a[0x1c - 0x1a];
    NPCPartDef * parts;
    u8 unknown_0x20[0x38 - 0x20];
    s16 killXp;
    u8 unknown_0x3a[0x46 - 0x3a];
    u16 coinDropChance;
    u16 coinDropBaseCount;
    u16 coinDropExtraChance;
    u16 coinDropExtraMax;
    u16 dropItemChance;
    DropItem * dropItemList;
    u8 unknown_0x54[0x64 - 0x54];
    u8 attack;
    u8 unknown_0x65[0x68 - 0x65];
};
static_assert(sizeof(NPCTribe) == 0x68);

struct NPCAnim
{
    s32 m_nPoseId;
    char animPoseName[32];
    u8 unknown_0x24[0x44 - 0x24];
    u32 flags;
    NPCTribeAnimDef * tribeAnimDefs;
    u8 unknown_0x4c[0x244 - 0x4c];
};
static_assert(sizeof(NPCAnim) == 0x244);

struct NPCPart
{
    u8 unknown_0x0[0x8 - 0x0];
    wii::Vec3 pos;
    u8 unknown_0x14[0x2c - 0x14];
    u32 flag2c;
    u32 flag30;
    u8 unknown_0x34[0x38c - 0x34];
    NPCPart * next;
    NPCPart * prev;
    u8 unknown_0x394[0x398 - 0x394];
};
static_assert(sizeof(NPCPart) == 0x398);

struct NPCEntry
{
/* 0x000 */ s32 id;
/* 0x004 */ s32 setupFileIndex; // 1-based index, 0 if not spawned from a setup file
    /*
        0x1 is active
        Others unknown
    */
/* 0x008 */ u32 flag8;
/* 0x00C */ u32 flagC;
/* 0x010 */ u32 flag10;
/* 0x014 */ u8 unknown_0x14[0x24 - 0x14];
/* 0x024 */ char name[32]; // name of this instance, npc_XXXXXXXX for template-spawned ones
                           // where XXXXXXXX is id in hex
/* 0x044 */ NPCAnim m_Anim; // unknown size
/* ????? */ u8 unknown_unk[0x2a0 - 0x44 - sizeof(NPCAnim)];
/* 0x2A0 */ wii::Vec3 position;
/* 0x2AC */ u8 unknown_0x2ac[0x348 - 0x2ac];
/* 0x348 */ EvtScriptCode * templateUnkScript1; // unkScript1 from spawning SetupEnemyTemplate
                                                // (unknown for non-templated NPCs)
/* 0x34C */ u8 unknown_0x34c[0x360 - 0x34c];
/* 0x360 */ EvtScriptCode * templateUnkScript2; // unkScript2 from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x364 */ EvtScriptCode * templateUnkScript3; // unkScript3 from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x368 */ EvtScriptCode * templateUnkScript4; // unkScript4 from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x36C */ EvtScriptCode * templateUnkScript5; // unkScript5 from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x370 */ EvtScriptCode * templateUnkScript9; // unkScript9 from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x374 */ EvtScriptCode * templateUnkScript6; // unkScript6 from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x378 */ EvtScriptCode * templateUnkScript7; // unkScript7 from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x37C */ EvtScriptCode * templateUnkScript8; // unkScript8 from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x380 */ u8 unknown_0x380[0x390 - 0x380];
/* 0x390 */ s32 onSpawnEvtId; // id of the EvtEntry running a templated npc's onSpawn scripts
                              // (unknown for non-templated NPCs)
/* 0x394 */ u8 unknown_0x394[0x39c - 0x394];
/* 0x39C */ f32 tribeField0xE; // field 0xe of spawning NPCTribe cast to float
/* 0x3A0 */ f32 tribeField0x10; // field 0x10 of spawning NPCTribe cast to float
/* 0x3A4 */ f32 tribeField0x12; // field 0x12 of spawning NPCTribe cast to float
/* 0x3A8 */ u8 unknown_0x3a8[0x3ac - 0x3a8];
/* 0x3AC */ f32 unknown_0x3ac;
/* 0x3B0 */ u8 unknown_0x3b0[0x46c - 0x3b0];
    /*
        0x80000000 is frozen
        0x40000 is on different pane to Mario
        0x20000 is hidden & frozen
    */
/* 0x46C */ u32 flag46C;
/* 0x470 */ u8 unknown_0x470[0x478 - 0x470];
/* 0x478 */ u32 tribeField0x54; // field 0x54 of spawning NPCTribe
/* 0x47C */ u8 unknown_0x47c[0x49c - 0x47c];
/* 0x49C */ s32 tribeId; // id of the NPCTribe this NPC was spawned with
/* 0x4A0 */ s32 tribeId2; // seemingly just a copy of tribeId
/* 0x4A4 */ u8 unknown_0x4a4[0x4ec - 0x4a4];
/* 0x4EC */ u32 maxHp; // copied from spawning NPCTribe, 1 for NPCs not spawned by tribe
/* 0x4F0 */ u32 hp; // copied from spawning NPCTribe, 1 for NPCs not spawned by tribe
/* 0x4F4 */ u8 unknown_0x4f4[0x508 - 0x4f4];
/* 0x508 */ int axisMovementUnit;
/* 0x50c */ u8 unknown_0x50c[0x510 - 0x50c];
/* 0x510 */ s32 unitWork[16];
/* 0x550 */ u8 unknown_0x550[0x574 - 0x550];
/* 0x574 */ s32 dropItemId; // id of the item this npc would drop if killed (determined on spawn)
/* 0x578 */ u8 unknown_0x578[0x57c - 0x578];
/* 0x57C */ u32 templateField0x5C; // field 0x5c of spawning SetupEnemyTemplate
                                   // (unknown for non-templated NPCs)
/* 0x580 */ u32 templateField0x60; // field 0x60 of spawning SetupEnemyTemplate
                                   // (unknown for non-templated NPCs)
/* 0x584 */ u32 templateField0x64; // field 0x64 of spawning SetupEnemyTemplate
                                   // (unknown for non-templated NPCs)
/* 0x588 */ u8 unknown_0x588[0x714 - 0x588];
/* 0x714 */ NPCPart * parts; // made from tribe's NPCPartDef list, linked list
/* 0x718 */ EvtScriptCode * templateField0x58; // field 0x58 from spawning SetupEnemyTemplate
                                               // g(unknown for non-templated NPCs)
/* 0x71C */ u8 unknown_0x71c[0x748 - 0x71c];
};
static_assert(sizeof(NPCEntry) == 0x748);

struct NPCWork
{
    u32 flag;
    s32 num;
    NPCEntry * entries;
    u8 unknown_0xc[0xb48 - 0xc];
};
static_assert(sizeof(NPCWork) == 0xb48);

extern "C" {

extern NPCEnemyTemplate npcEnemyTemplates[NPCTEMPLATE_MAX];
extern NPCTribe npcTribes[NPC_TRIBE_COUNT];
NPCEntry * npcEntryFromTemplate(NPCEnemyTemplate * enemyTemplate);
NPCEntry * npcEntryFromSetupEnemy(s32 setupFileIndex, wii::Vec3 * pos, s32 nTemplateNo, MiscSetupDataV6 * miscSetupData);
void npcDispMain();
NPCEntry * npcNameToPtr(const char * name);
const char * npcSearchAnimDefs(NPCTribeAnimDef * defs, s32 id);
NPCWork * npcGetWorkPtr();
void npcDropItem(NPCEntry * npc, s32 itemType, s32 coinCount);

}

}
