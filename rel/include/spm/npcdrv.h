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
    s32 id;
    s32 setupFileIndex;
    u32 flags_8;
    u32 flags_c;
    u32 flags_10;
    u8 unknown_0x14[0x24 - 0x14];
    char name[32];
    NPCAnim m_Anim;
    u8 unknown_0x288[0x2a0 - 0x288];
    wii::Vec3 position;
    u8 unknown_0x2ac[0x3ac - 0x2ac];
    float unknown_0x3ac;
    u8 unknown_0x3b0[0x46c - 0x3b0];
    u32 flag46C;
    u8 unknown_0x470[0x49c - 0x470];
    s32 tribeId;
    u8 unknown_0x4a0[0x4ec - 0x4a0];
    u32 maxHp;
    u32 hp;
    u8 unknown_0x4f4[0x510 - 0x4f4];
    s32 unitWork[16];
    u8 unknown_0x550[0x574 - 0x550];
    s32 dropItemId;
    u8 unknown_0x578[0x714 - 0x578];
    NPCPart * parts;
    u8 unknown_0x718[0x748 - 0x718];
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
void npcDispMain();
NPCEntry * npcNameToPtr(const char * name);
const char * npcSearchAnimDefs(NPCTribeAnimDef * defs, s32 id);
NPCWork * npcGetWorkPtr();
void npcDropItem(NPCEntry * npc, s32 itemType, s32 coinCount);

}

}
