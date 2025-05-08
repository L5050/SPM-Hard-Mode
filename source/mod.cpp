#include "mod.h"
#include "patch.h"
#include "cutscene_helpers.h"
#include "evt_cmd.h"
#include "evtpatch.h"
#include "exception.h"
#include "exception.h"
#include "romfontexpand.h"
#include "shadoo.h"
#include "e_dmen.h"
#include "marioDamage.h"
#include "patchNinja.h"
#include "patch_dan.h"
#include "king_sammer.h"
#include "stdlib.h"

#include <common.h>
#include <spm/rel/dan.h>
#include <spm/evt_cam.h>
#include <spm/evt_sub.h>
#include <spm/evt_door.h>
#include <spm/evt_eff.h>
#include <spm/evt_fade.h>
#include <spm/evt_fairy.h>
#include <spm/evt_guide.h>
#include <spm/evt_mario.h>
#include <spm/evt_item.h>
#include <spm/evt_mobj.h>
#include <spm/evt_msg.h>
#include <spm/evt_npc.h>
#include <spm/item_event_data.h>
#include <spm/evt_pouch.h>
#include <spm/evt_sub.h>
#include <spm/memory.h>
#include <spm/evt_env.h>
#include <spm/evt_seq.h>
#include <spm/evt_snd.h>
#include <spm/pausewin.h>
#include <spm/animdrv.h>
#include <spm/wpadmgr.h>
#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <spm/map_data.h>
#include <spm/evtmgr_cmd.h>
#include <spm/seq_game.h>
#include <spm/npcdrv.h>
#include <spm/mario.h>
#include <spm/spmario.h>
#include <spm/mario_pouch.h>
#include <spm/mario_motion.h>
#include <spm/mario_status.h>
#include <spm/seqdef.h>
#include <spm/npc_ninja.h>
#include <spm/npc_dimeen_l.h>
#include <spm/item_data_ids.h>
#include <spm/item_data.h>
#include <spm/evt_shop.h>
#include <msl/string.h>
#include <wii/os/OSError.h>
#include <wii/gx.h>
#include <wii/mtx.h>
#include <wii/ipc.h>
#include <string>
#include <cstdio>
#include <limits>
#include <functional>
using namespace std;
namespace mod {

spm::mario::MarioWork * marioWork = spm::mario::marioGetPtr();
spm::spmario::SpmarioGlobals * globals = spm::spmario::gp;
int bossSequence = 1;
char newMapName[32] = {0};
u32* bossHealth = nullptr;

int* getBossSequence()
{
  return &bossSequence;
}

/*
    Title Screen Custom Text
    Prints "SPM Hard Mode" at the top of the title screen
*/
static spm::seqdef::SeqFunc *seq_titleMainReal;
static spm::seqdef::SeqFunc *seq_gameMainReal;

static void seq_titleMainOverride(spm::seqdrv::SeqWork *wp)
{
  wii::gx::GXColor green = {
    0,
    255,
    0,
    255
  };
    f32 scale = 0.8f;
    const char * msg = "SPM Hard Mode";
    spm::fontmgr::FontDrawStart();
    spm::fontmgr::FontDrawEdge();
    spm::fontmgr::FontDrawColor(&green);
    spm::fontmgr::FontDrawScale(scale);
    spm::fontmgr::FontDrawNoiseOff();
    spm::fontmgr::FontDrawRainbowColorOff();
    f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
    spm::fontmgr::FontDrawString(x, 200.0f, msg);
    seq_titleMainReal(wp);
}
int checkCharmStats()
{
  spm::mario_pouch::MarioPouchWork* pouch = spm::mario_pouch::pouchGetPtr();
  int charms = pouch->killsBeforeNextCharm;
  if (charms > 0) {
    return charms;
  } else {
    return 0;
  }
}

int checkCharmNum()
{
  if (spm::mario::marioKeyOffChk() == 1) {
    return 0;
  }
  spm::mario_pouch::MarioPouchWork* pouch = spm::mario_pouch::pouchGetPtr();
  int charms = pouch->charmsRemaining;
  if (charms > 0) {
    return charms;
  } else {
    return 0;
  }
}

int checkBossHealth() {
  if (spm::mario::marioKeyOffChk() == 1) {
    return 0;
  }
  if (bossHealth != nullptr) {
    if (bossHealth >= 1){
    return bossHealth;
  } else {
    bossHealth = nullptr;
  }
  }
  spm::npcdrv::NPCWork * NPCWork = spm::npcdrv::npcGetWorkPtr();
  u32 health = 0;
  s32 plotValue = globals->gsw0;
  if (globals->gsw[24] == 0) globals->gsw[24] = 1;
  s32 num = spm::npcdrv::npcGetMaxEntries();
    if (plotValue == 0x21){
    for (int i = 0; i < num; i++) {
      if (NPCWork->entries[i].tribeId == 270) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x66){
            bossSequence = 1;
              }
    if (plotValue == 0x67){
    for (int i = 0; i < num; i++) {
      if (NPCWork->entries[i].tribeId == 315) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0xb7){
      globals->gsw0 = 0xb9;
    }
    if (plotValue == 0x73){
    for (int i = 0; i < num; i++) {
      if (NPCWork->entries[i].tribeId == 286) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x7c){
    for (int i = 0; i < num; i++) {
      if (NPCWork->entries[i].tribeId == 318) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0xaa){
    for (int i = 0; i < num; i++) {
      if (NPCWork->entries[i].tribeId == 295) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0xab){
    for (int i = 0; i < num; i++) {
      if (NPCWork->entries[i].tribeId == 296) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0xbe){
    for (int i = 0; i < num; i++) {
      if (NPCWork->entries[i].tribeId == 271) {
        health = NPCWork->entries[i].hp;
      }
      if (NPCWork->entries[i].tribeId == 289) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0xd4){
        bossSequence = 1;
    }
    if (plotValue == 0xd5){
    for (int i = 0; i < num; i++) {
      if (NPCWork->entries[i].tribeId == 272) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0xdb){
    for (int i = 0; i < num; i++) {
      if (NPCWork->entries[i].tribeId == 319) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x112){
    for (int i = 0; i < num; i++) {
      if (NPCWork->entries[i].tribeId == 282) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x11d){
    for (int i = 0; i < num; i++) {
      if (NPCWork->entries[i].tribeId == 300) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x139){
    for (int i = 0; i < num; i++) {
      if (NPCWork->entries[i].tribeId == 316) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x160){
    for (int i = 0; i < num; i++) {
      if (NPCWork->entries[i].tribeId == 327) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x16c){
    for (int i = 0; i < num; i++) {
      if (NPCWork->entries[i].tribeId == 273) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x191){
    for (int i = 0; i < num; i++) {
      if (NPCWork->entries[i].tribeId == 292) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x19a){
    for (int i = 0; i < num; i++) {
      if (NPCWork->entries[i].tribeId == 305) {
        wii::os::OSReport("%d\n", NPCWork->entries[i].m_Anim.m_nPoseId);
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x19c){
    for (int i = 0; i < num; i++) {
      if (NPCWork->entries[i].tribeId == 309) {
        bossSequence = 3;
        return health = 99999;
      }
    }}
    if (plotValue == 0x19f){
    for (int i = 0; i < num; i++) {
      if (NPCWork->entries[i].tribeId == 309) {
        return health = NPCWork->entries[i].hp;
      }
    }}
    for (int i = 0; i < num; i++) {
      if (NPCWork->entries[i].tribeId == 332 ||NPCWork->entries[i].tribeId == 282 || NPCWork->entries[i].tribeId == 273 || NPCWork->entries[i].tribeId == 292 || NPCWork->entries[i].tribeId == 330 || NPCWork->entries[i].tribeId == 331 || NPCWork->entries[i].tribeId == 333 || NPCWork->entries[i].tribeId == 532) {
        if (NPCWork->entries[i].flag8 & 0x80000000 != 0)
        return NPCWork->entries[i].hp;
      }
    }
  return health;
}

static void bossHealthDisplay()
{
    if (checkBossHealth() > 0){
      wii::gx::GXColor green = {
        0,
        255,
        0,
        255
      };
    f32 scale = 0.8f;
    const char * msg = "Boss Health:";
    spm::fontmgr::FontDrawStart();
    spm::fontmgr::FontDrawEdge();
    spm::fontmgr::FontDrawColor(&green);
    spm::fontmgr::FontDrawScale(scale);
    spm::fontmgr::FontDrawNoiseOff();
    spm::fontmgr::FontDrawRainbowColorOff();
    f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
    spm::fontmgr::FontDrawString(x-300, 70.0f, msg);}
}
static void bossActualHealth()
{
    if (checkBossHealth() > 0){
      wii::gx::GXColor green = {
        0,
        255,
        0,
        255
      };
    f32 scale = 0.8f;
    char buffer [50];
    int health = checkBossHealth();
    sprintf(buffer, "%d", health);
    if (health < 99999) {
    const char * msg = buffer;
    spm::fontmgr::FontDrawStart();
    spm::fontmgr::FontDrawEdge();
    spm::fontmgr::FontDrawColor(&green);
    spm::fontmgr::FontDrawScale(scale);
    spm::fontmgr::FontDrawNoiseOff();
    spm::fontmgr::FontDrawRainbowColor();
    f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
    spm::fontmgr::FontDrawString(x-320, 50.0f, msg);
  } else if (health == 99999) {
    const char * msg = "Infinite";
    spm::fontmgr::FontDrawStart();
    spm::fontmgr::FontDrawEdge();
    spm::fontmgr::FontDrawColor(&green);
    spm::fontmgr::FontDrawScale(scale);
    spm::fontmgr::FontDrawNoiseOff();
    spm::fontmgr::FontDrawRainbowColor();
    f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
    spm::fontmgr::FontDrawString(x-320, 50.0f, msg);
  }
  }
    
}
static void charmTextGenerator()
{
  int charmStats = checkCharmNum();
  if (charmStats > 0){
  wii::gx::GXColor red = {
      255,
      0,
      0,
      255
  };
  f32 scale = 0.6f;
  const char * msg = "Enemies until next charm:";
  spm::fontmgr::FontDrawStart();
  spm::fontmgr::FontDrawEdge();
  spm::fontmgr::FontDrawColor(&red);
  spm::fontmgr::FontDrawScale(scale);
  spm::fontmgr::FontDrawNoiseOff();
  spm::fontmgr::FontDrawRainbowColorOff();
  f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
  spm::fontmgr::FontDrawString(x+265, 70.0f, msg);}
  
}
static void charmKillsTextGenerator()
{
  int charmNum = checkCharmNum();
  if (charmNum > 0){
  char buffer [50];
  int charmStats = checkCharmStats();
  wii::gx::GXColor red = {
      255,
      0,
      0,
      255
  };
  f32 scale = 0.6f;
  sprintf(buffer, "%d", charmStats);
  const char * msg = buffer;
  spm::fontmgr::FontDrawStart();
  spm::fontmgr::FontDrawEdge();
  spm::fontmgr::FontDrawColor(&red);
  spm::fontmgr::FontDrawScale(scale);
  spm::fontmgr::FontDrawNoiseOff();
  spm::fontmgr::FontDrawRainbowColor();
  f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
  spm::fontmgr::FontDrawString(x+350, 55.0f, msg);}
  
}
static void charmNumText()
{
  int charmStats = checkCharmNum();
  if (charmStats > 0){
  wii::gx::GXColor red = {
        255,
        0,
        0,
        255
  };
  f32 scale = 0.6f;
  const char * msg = "Charms left:";
  spm::fontmgr::FontDrawStart();
  spm::fontmgr::FontDrawEdge();
  spm::fontmgr::FontDrawColor(&red);
  spm::fontmgr::FontDrawScale(scale);
  spm::fontmgr::FontDrawNoiseOff();
  spm::fontmgr::FontDrawRainbowColorOff();
  f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
  spm::fontmgr::FontDrawString(x+322, 100.0f, msg);}
  
}
static void charmNumLeftText()
{
  int charmStats = checkCharmNum();
  if (charmStats > 0){
  char buffer [50];
  wii::gx::GXColor red = {
      255,
      0,
      0,
      255
  };
  f32 scale = 0.6f;
  sprintf(buffer, "%d", charmStats);
  const char * msg = buffer;
  spm::fontmgr::FontDrawStart();
  spm::fontmgr::FontDrawEdge();
  spm::fontmgr::FontDrawColor(&red);
  spm::fontmgr::FontDrawScale(scale);
  spm::fontmgr::FontDrawNoiseOff();
  spm::fontmgr::FontDrawRainbowColor();
  f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
  spm::fontmgr::FontDrawString(x+350, 85.0f, msg);}
  
}
static void customUI(spm::seqdrv::SeqWork *wp)
{
  charmTextGenerator();
  charmKillsTextGenerator();
  charmNumText();
  charmNumLeftText();
  bossHealthDisplay();
  bossActualHealth();
  seq_gameMainReal(wp);
}
static void titleScreenCustomTextPatch()
{
    seq_titleMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main;
    seq_gameMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME].main;
    spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main = &seq_titleMainOverride;
    spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME].main = &customUI;
}
static void setBossHP()
{
  spm::npcdrv::npcTribes[270].maxHp = 15; //O'Chunks 1
  spm::npcdrv::npcTribes[315].maxHp = 10; //Bowser 1
  spm::npcdrv::npcTribes[286].maxHp = 12; //Dimentio 1
  spm::npcdrv::npcTribes[318].maxHp = 25; //Francis
  spm::npcdrv::npcTribes[295].maxHp = 16; //Mr. L
  spm::npcdrv::npcTribes[271].maxHp = 20; //O'Chunks 2
  spm::npcdrv::npcTribes[289].maxHp = 10; //Dimentio 2
  spm::npcdrv::npcTribes[272].maxHp = 20; //O'Cabbage
  //spm::npcdrv::npcTribes[319].maxHp = 20; //King Croacus
  spm::npcdrv::npcTribes[282].maxHp = 20; //Mimi
  spm::npcdrv::npcTribes[300].maxHp = 150; //Brobot L-Type
  spm::npcdrv::npcTribes[316].maxHp = 12; //Bowser 2
  spm::npcdrv::npcTribes[327].maxHp = 90; //Bonechill
  spm::npcdrv::npcTribes[273].maxHp = 30; //O'Chunks 3
  spm::npcdrv::npcTribes[292].maxHp = 20; //Dimentio 3
  spm::npcdrv::npcTribes[305].maxHp = 25; //Count Bleck
  spm::npcdrv::npcTribes[309].maxHp = 170; //Super Dimentio
  spm::npcdrv::npcTribes[330].maxHp = 20; //Dark Mario
  spm::npcdrv::npcTribes[331].maxHp = 12; //Dark Luigi
  spm::npcdrv::npcTribes[332].maxHp = 20; //Dark Peach
  spm::npcdrv::npcTribes[333].maxHp = 12; //Dark Bowser
}

/*
  Gives all bosses the megabite defense stat
*/
spm::npcdrv::NPCDefense fireDef;
static void setBossDef() {
  spm::npcdrv::NPCDefense def;
  def.type = 0x0;
  def.defense = 0x2;
  def.flags = 0x2;
  fireDef.type = 0xA;
  fireDef.defense = 0x64;
  fireDef.flags = 0x0;
  spm::npcdrv::NPCDefense holeDef1;
  holeDef1.type = 0;
  holeDef1.defense = 99;
  holeDef1.flags = 0x4;
  spm::npcdrv::NPCDefense holeDef2;
  holeDef2.type = 41;
  holeDef2.defense = 0;
  holeDef2.flags = 0x4;
  /*for (int i = 0; i < 7; i++) {//O'chunks 1 defense
    if (spm::npcdrv::npcTribes[270].partsList[i].id == 1) {
     spm::npcdrv::npcTribes[270].partsList[i].defenses[0] = chunkDef;
    }
  }*/
  for (int i = 0; i < 2; i++) {//Bowser 1 defense
    if (spm::npcdrv::npcTribes[315].partsList[i].id == 1) {
     spm::npcdrv::npcTribes[315].partsList[i].defenses[0] = def;
    }
  }
  for (int i = 0; i < 3; i++) {//Dimentio 1 defense
    if (spm::npcdrv::npcTribes[286].partsList[i].id == 1) {
     spm::npcdrv::npcTribes[286].partsList[i].defenses[0] = fireDef;
    }
    if (spm::npcdrv::npcTribes[289].partsList[i].id == 1) {
     spm::npcdrv::npcTribes[289].partsList[i].defenses[0] = fireDef;
    }
  }
   for (int i = 0; i < 7; i++) {//Brobot defense
      spm::npcdrv::npcTribes[296].partsList[i].defenses[0] = def;
   }
   for (int i = 0; i < 7; i++) {//O'chunks 2 defense
     if (spm::npcdrv::npcTribes[271].partsList[i].id == 1) {
      spm::npcdrv::npcTribes[271].partsList[i].defenses[0] = fireDef;
      spm::npcdrv::npcTribes[271].partsList[i].defenses[5] = def;
     }
   }
   //spm::npcdrv::npcTribes[282].partsList[0].defenses[0] = def;  //mimi defense
   for (int i = 0; i < 15; i++) {//Brobot L-Type defense
      spm::npcdrv::npcTribes[300].partsList[i].defenses[0] = def;
   }
   for (int i = 0; i < 2; i++) {//Bowser 2 defense
     if (spm::npcdrv::npcTribes[316].partsList[i].id == 1) {
      spm::npcdrv::npcTribes[316].partsList[i].defenses[0] = def;
     }
   }
   for (int i = 0; i < 9; i++) {//Underchomp defense
     if (spm::npcdrv::npcTribes[316].partsList[i].id == 1) {
      spm::npcdrv::npcTribes[316].partsList[i].defenses[0] = def;
     }
   }
   for (int i = 0; i < 21; i++) {//Bonechill defense
     if (spm::npcdrv::npcTribes[327].partsList[i].id == 2) {
      spm::npcdrv::npcTribes[327].partsList[i].defenses[2] = def;
     }
     if (spm::npcdrv::npcTribes[327].partsList[i].id == 3) {
      spm::npcdrv::npcTribes[327].partsList[i].defenses[2] = def;
     }
     if (spm::npcdrv::npcTribes[327].partsList[i].id == 4) {
      spm::npcdrv::npcTribes[327].partsList[i].defenses[2] = def;
     }
   }
      spm::npcdrv::npcTribes[304].partsList[0].defenses[0] = def;
      spm::npcdrv::npcTribes[306].partsList[0].defenses[0] = holeDef1; // if we dont do this then bleck's black holes can be jumped on without being absorbed
      spm::npcdrv::npcTribes[306].partsList[0].defenses[1] = holeDef2; // its stupid and I fucking hate it

   }
/*
    Function patching
*/

s32 scriptTakeDamage(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
  spm::mario_pouch::MarioPouchWork * pouch = spm::mario_pouch::pouchGetPtr();
  pouch->hp = pouch->hp - 6;
  return firstRun;
}

void (*seq_gameMain)(spm::seqdrv::SeqWork *param_1);
/*spm::evtmgr::EvtEntry * newEntry(const spm::evtmgr::EvtScriptCode * script, u8 priority, u8 flags, s32 type)
        {
          spm::mario_pouch::MarioPouchWork * pouch = spm::mario_pouch::pouchGetPtr();
          //turtley leaf
            if (spm::item_event_data::getItemUseEvt(104) == script){
              pouch->killsBeforeNextCharm = pouch->killsBeforeNextCharm / 2;
            }
            //bone in cut and dayzee syrup
            if (spm::item_event_data::getItemUseEvt(113) == script || spm::item_event_data::getItemUseEvt(124) == script){
              pouch->charmsRemaining += 5;
              pouch->killsBeforeNextCharm += 1;
            }
            //hot sauce and spit roast
            if (spm::item_event_data::getItemUseEvt(109) == script || spm::item_event_data::getItemUseEvt(162) == script){
              pouch->charmsRemaining += 5;
              pouch->killsBeforeNextCharm += 1;
            }
            //spicy dinner and spicy pasta dish
            if (spm::item_event_data::getItemUseEvt(183) == script || spm::item_event_data::getItemUseEvt(155) == script){
              pouch->charmsRemaining += 5;
              pouch->killsBeforeNextCharm += 1;
            }
            //dangerous delight
            if (spm::item_event_data::getItemUseEvt(173) == script){
              pouch->killsBeforeNextCharm += 2;
              pouch->killsBeforeNextCharm = pouch->killsBeforeNextCharm / 2;
              pouch->charmsRemaining += 6;
            }*
            return evtEntryType(script, priority, flags, type);
        }*/

void patchGameMain() {
seq_gameMain = patch::hookFunction(spm::seq_game::seq_gameMain,
[](spm::seqdrv::SeqWork *param_1)
    {
        seq_gameMain(param_1);
    }
);
}

void reduceXpGained() {
  for (int i = 0; i < 535; i++) {
    if (spm::npcdrv::npcTribes[i].killXp >= 2) {
     int newXp = spm::npcdrv::npcTribes[i].killXp / 2;
     spm::npcdrv::npcTribes[i].killXp = newXp;
    }
    if (spm::npcdrv::npcTribes[i].stylishXp >= 2) {
       int newXp = spm::npcdrv::npcTribes[i].stylishXp / 2;
       spm::npcdrv::npcTribes[i].stylishXp = newXp;
    }
  }
  for (int i = 0; i < ITEM_ID_MAX; i++) {
    if (spm::item_data::itemDataTable[i].xpGain >= 2) {
      spm::item_data::itemDataTable[i].xpGain = spm::item_data::itemDataTable[i].xpGain / 2;
    }
  }
}

s32 itemCharm(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
  spm::mario_pouch::MarioPouchWork* pouch = spm::mario_pouch::pouchGetPtr();
  if (pouch->killsBeforeNextCharm > 5) {
    pouch->killsBeforeNextCharm = pouch->killsBeforeNextCharm - 5;
    pouch->charmsRemaining = pouch->charmsRemaining + 5;
  } else if (pouch->killsBeforeNextCharm < 6) {
    pouch->killsBeforeNextCharm = 5;
    pouch->charmsRemaining = pouch->charmsRemaining + 5;
  }
  return 2;
}

s32 reduceEnemyRequirements(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
  spm::mario_pouch::MarioPouchWork* pouch = spm::mario_pouch::pouchGetPtr();
  if (pouch->killsBeforeNextCharm > 1) {
    pouch->killsBeforeNextCharm = pouch->killsBeforeNextCharm / 2;
  }
  return 2;
}

s32 removeAbilities(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
  spm::mario_status::marioStatusApplyStatuses(4, 2);
  return 2;
}

s32 dimentioQuickTimeReset(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
  bossSequence = 3;
  return 2;
}

s32 unPauseGame(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
  spm::pausewin::pausewinUnpauseGame();
  spm::spmario::spmarioSystemLevel(0);
  return 2;
}

s32 giveHampterCheese(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
spm::mapdrv::mapGrpFlagOn(0, "A2_pas_01", 1);
spm::mapdrv::mapGrpFlagOn(0, "A3_pas_01", 1);
spm::mapdrv::mapGrpFlagOn(0, "A2_doa_05", 1);
spm::mapdrv::mapGrpFlagOn(0, "A3_doa_05", 1);
return 2;
}

s32 checkBboxChonkyBoi(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
  spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
  wii::mtx::Vec3 min;
  wii::mtx::Vec3 max;
  spm::hitdrv::hitGetMapEntryBbox(0, &min, &max);
  f32 x = spm::evtmgr_cmd::evtGetFloat(evtEntry, args[0]);
  if (x < min.x)
  {
    x += 300.0;
  }
  if (x > max.x)
  {
    x -= 300.0;
  }
  spm::evtmgr_cmd::evtSetFloat(evtEntry, args[0], x);
  return 2;
}

s32 checkBboxSammer(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
  spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
  wii::mtx::Vec3 min;
  wii::mtx::Vec3 max;
  spm::hitdrv::hitGetMapEntryBbox(0, &min, &max);
  f32 x = spm::evtmgr_cmd::evtGetFloat(evtEntry, args[0]);
  wii::os::OSReport("%f\n", x);
  if (x < min.x)
  {
    spm::evtmgr_cmd::evtSetFloat(evtEntry, args[0], 0.0);
  } else if (x > max.x)
  {
    spm::evtmgr_cmd::evtSetFloat(evtEntry, args[0], 0.0);
  }
  if (x >= -201.0 && x <= -111.0)
  {
    spm::evtmgr_cmd::evtSetFloat(evtEntry, args[0], 0.0);
  } else if (x >= 111.0 && x <= 202.0)
  {
    spm::evtmgr_cmd::evtSetFloat(evtEntry, args[0], 0.0);
  }
  return 2;
}

s32 compareStrings(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
        char *mapName = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        char *comparison = spm::evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        int compstrReturn = 1;
        char *result = msl::string::strstr(mapName, comparison);
        if (result != 0)
        {
            compstrReturn = 1;
        } else
        {
            compstrReturn = 0;
        }
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[2], compstrReturn);
        return 2;
    }

s32 returnNewMapName(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
{
    spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
    const char* mapName = globals->mapName;

    size_t len = msl::string::strlen(mapName);
    const char* lastTwo = mapName + (len - 2);
    int mapNumber = atoi(lastTwo);
    int value = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);

    mapNumber += value;

    // Make sure that the Ninja never sends you to a room that doesnt exist
    char *result = msl::string::strstr(mapName, "wa1");
    if (result != 0 && mapNumber > 26)
    {
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], mapNumber - 26);
        mapNumber = 26;
    } else if (mapNumber > 25)
    {
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], mapNumber - 25);
        mapNumber = 25;
    }


    // We need to do stupid math because otherwise it tries to set the map name to wa_3 when it should be wa_03
    char formattedDigits[3];
    if (mapNumber < 10) {
        formattedDigits[0] = '0';
        formattedDigits[1] = '0' + mapNumber;
    } else {
        formattedDigits[0] = '0' + (mapNumber / 10);
        formattedDigits[1] = '0' + (mapNumber % 10);
    }
    formattedDigits[2] = '\0';

    // Copy the original name excluding the map number
    msl::string::strncpy(newMapName, mapName, len - 2);
    newMapName[len - 2] = '\0';  // Null-terminate the bugger

    // Add new map number
    msl::string::strcat(newMapName, formattedDigits);

    return 2;
}

s32 displayBossHealth(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
{
  spm::npcdrv::NPCEntry *npc = evtEntry->ownerNPC;
  bossHealth = &npc->hp;
}

EVT_DECLARE_USER_FUNC(itemCharm, 0)
EVT_DECLARE_USER_FUNC(giveHampterCheese, 0)
EVT_DECLARE_USER_FUNC(reduceEnemyRequirements, 0)
EVT_DECLARE_USER_FUNC(removeAbilities, 0)
EVT_DECLARE_USER_FUNC(dimentioQuickTimeReset, 0)
EVT_DECLARE_USER_FUNC(checkBboxSammer, 1)
EVT_DECLARE_USER_FUNC(returnNewMapName, 1)

EVT_BEGIN(insertNop)
  SET(LW(0), 0)
RETURN_FROM_CALL()

EVT_BEGIN(runDimiSaveScript)
  SET(GSW(0), 414)
  RUN_CHILD_EVT(evt_memcard_save)
  USER_FUNC(dimentioQuickTimeReset)
  SET(GSW(0), 415)
RETURN_FROM_CALL()

EVT_BEGIN(summonMario)
  SET(LW(0), 0)
RETURN_FROM_CALL()

EVT_BEGIN(charmAdd)
  USER_FUNC(itemCharm)
RETURN_FROM_CALL()

EVT_BEGIN(reduceCharmEnemies)
  USER_FUNC(reduceEnemyRequirements)
RETURN_FROM_CALL()

EVT_BEGIN(increaseEggDamage)
  SET(LW(10), 7)
RETURN_FROM_CALL()

EVT_BEGIN(increaseSnowBunnyDamage1)
  SET(LW(10), 7)
RETURN_FROM_CALL()

EVT_BEGIN(increaseSnowBunnyDamage2)
  SET(LW(10), 8)
RETURN_FROM_CALL()

EVT_BEGIN(increaseDynamiteDamage)
  SET(LW(10), 13)
RETURN_FROM_CALL()

EVT_BEGIN(changeThunderDamage)
  IF_LARGE(LW(5), 4)
    SET(LW(5), 4)
  END_IF()
RETURN_FROM_CALL()

EVT_BEGIN(takeThatPiplup)
  USER_FUNC(spm::evt_seq::evt_seq_set_seq, spm::seqdrv::SEQ_MAPCHANGE, PTR("mi3_03"), PTR("doa_l"))
RETURN()
EVT_END()

EVT_BEGIN(takeThatPiplup2)
  USER_FUNC(spm::evt_snd::evt_snd_bgmon_f_d, 0, PTR("BGM_MAP_STG2"), 1000)
RETURN()
EVT_END()

static void patchItems() {
  spm::evtmgr::EvtScriptCode* mightyTonicUseScript = spm::item_event_data::getItemUseEvt(75);
  evtpatch::hookEvtReplace(mightyTonicUseScript, 17, (spm::evtmgr::EvtScriptCode*)charmAdd);
  spm::evtmgr::EvtScriptCode* default_item_script = spm::item_event_data::default_item_use_evt;
  evtpatch::hookEvt(default_item_script, 67, (spm::evtmgr::EvtScriptCode*)reduceCharmEnemies);
  evtpatch::hookEvt(default_item_script, 45, (spm::evtmgr::EvtScriptCode*)reduceCharmEnemies);
  evtpatch::hookEvt(default_item_script, 58, (spm::evtmgr::EvtScriptCode*)charmAdd);
  evtpatch::hookEvt(default_item_script, 78, (spm::evtmgr::EvtScriptCode*)charmAdd);
  evtpatch::hookEvt(default_item_script, 129, (spm::evtmgr::EvtScriptCode*)charmAdd);
  evtpatch::hookEvt(default_item_script, 70, (spm::evtmgr::EvtScriptCode*)charmAdd);
  evtpatch::hookEvt(default_item_script, 109, (spm::evtmgr::EvtScriptCode*)charmAdd);
  evtpatch::hookEvt(default_item_script, 113, (spm::evtmgr::EvtScriptCode*)charmAdd);
  evtpatch::hookEvt(default_item_script, 44, (spm::evtmgr::EvtScriptCode*)charmAdd);
  spm::evtmgr::EvtScriptCode* eggBomb = spm::item_event_data::getItemUseEvt(160);
  evtpatch::hookEvtReplace(eggBomb, 18, (spm::evtmgr::EvtScriptCode*)increaseEggDamage);
  evtpatch::hookEvtReplace(eggBomb, 20, (spm::evtmgr::EvtScriptCode*)increaseDynamiteDamage);
  spm::evtmgr::EvtScriptCode* snowBunny = spm::item_event_data::getItemUseEvt(148);
  evtpatch::hookEvtReplace(snowBunny, 16, (spm::evtmgr::EvtScriptCode*)increaseSnowBunnyDamage1);
  evtpatch::hookEvtReplace(snowBunny, 18, (spm::evtmgr::EvtScriptCode*)increaseSnowBunnyDamage2);
  evtpatch::hookEvtReplace(snowBunny, 12, (spm::evtmgr::EvtScriptCode*)increaseSnowBunnyDamage2);
  spm::evtmgr::EvtScriptCode* thunderRage = spm::item_event_data::getItemUseEvt(67);
  evtpatch::hookEvt(thunderRage, 46, (spm::evtmgr::EvtScriptCode*)changeThunderDamage);
}

static void patchCooking() {
  spm::evtmgr::EvtScriptCode* cookScript = spm::evt_shop::saffronCookingScript;
  evtpatch::hookEvtReplaceBlock(cookScript, 130, (spm::evtmgr::EvtScriptCode*)insertNop, 189);
  //evtpatch::hookEvtReplace(cookScript, 1, (spm::evtmgr::EvtScriptCode*)takeThatPiplup);
  //spm::map_data::MapData * mi3_03_md = spm::map_data::mapDataPtr("mi3_03");
  //evtpatch::hookEvtReplace(mi3_03_md->initScript, 1, (spm::evtmgr::EvtScriptCode*)takeThatPiplup2);

}



static void patchBarry()
{
  //check if the version is eu0, if not then change the assembly to point to the correct float which is 0x8 off between versions
  #ifdef SPM_EU0
    writeWord(spm::mario_motion::marioMotTbl[0x2A].mainFunc, 0x2BC, 0xC002BA98); //Barry patch 1
    writeWord(spm::mario_motion::marioMotTbl[0x2A].mainFunc, 0x298, 0xC022BA98); //Barry patch 2
  #else
    writeWord(spm::mario_motion::marioMotTbl[0x2A].mainFunc, 0x2BC, 0xC002BA90); //Barry patch 1
    writeWord(spm::mario_motion::marioMotTbl[0x2A].mainFunc, 0x298, 0xC022BA90); //Barry patch 2
  #endif
}

static void patchVariables() {
    writeWord(&spm::mario::marioCalcDamageToEnemy, 0x16C, 0x57FF003E);
      writeWord(&spm::mario::marioCalcDamageToEnemy, 0xC8, 0x57FF003C);
       writeWord(&spm::mario::marioCalcDamageToEnemy, 0x94, 0x57FF003C);
        writeWord(&spm::npc_dimeen_l::superDimentioHeadDefenses, 0x0, 0x00000002);
          writeWord(&spm::npc_dimeen_l::superDimentioBodyDefenses, 0x0, 0x00000002);
            patchBarry();
}

/*
    General mod functions
*/

spm::evtmgr::EvtScriptCode* getInstructionEvtArg(spm::evtmgr::EvtScriptCode* script, s32 line, int instruction)
{
  spm::evtmgr::EvtScriptCode* link = evtpatch::getEvtInstruction(script, line);
  wii::os::OSReport("%p\n", link);
  spm::evtmgr::EvtScriptCode* arg = evtpatch::getInstructionArgv(link)[instruction];
  wii::os::OSReport("%p\n", arg);
  return arg;
}

spm::npcdrv::NPCDefense * getInstructionEvtDefense(spm::evtmgr::EvtScriptCode* script, s32 line, int instruction)
{
  spm::evtmgr::EvtScriptCode* link = evtpatch::getEvtInstruction(script, line);
  wii::os::OSReport("%p\n", link);
  spm::npcdrv::NPCDefense * arg = evtpatch::getInstructionArgv(link)[instruction];
  wii::os::OSReport("%p\n", arg);
  return arg;
}

s32 mimiHittable = 1;
spm::evtmgr::EvtScriptCode* bleckMovementScript;
spm::evtmgr::EvtScriptCode* standard_death_script;

EVT_BEGIN(rollForNinjoe)
  USER_FUNC(spm::evt_sub::evt_sub_random, 90, LW(0))
  IF_EQUAL(LW(0), 1)
    USER_FUNC(spm::evt_sub::evt_sub_random, 9, LW(0))
    IF_LARGE(LW(0), 6)
      USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 178)
      USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 178, 0, -100, 0, LW(10), EVT_NULLPTR)
      USER_FUNC(spm::evt_npc::evt_npc_flip_to, LW(10), 1)
      USER_FUNC(spm::evt_npc::evt_npc_finish_flip_instant, LW(10))
      USER_FUNC(spm::evt_npc::evt_npc_flip, LW(10))
      USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(1), LW(2), LW(3))
      ADD(LW(2), 100)
      USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(10), LW(1), LW(2), LW(3))
      USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_NINJA_APPEAR1"), LW(10))
    ELSE()
      IF_LARGE(LW(0), 3)
        USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 259)
        USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 259, 0, -100, 0, LW(10), EVT_NULLPTR)
        USER_FUNC(spm::evt_npc::evt_npc_flip_to, LW(10), 1)
        USER_FUNC(spm::evt_npc::evt_npc_finish_flip_instant, LW(10))
        USER_FUNC(spm::evt_npc::evt_npc_flip, LW(10))
        USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(1), LW(2), LW(3))
        ADD(LW(2), 100)
        USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(10), LW(1), LW(2), LW(3))
        USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_NINJA_APPEAR1"), LW(10))
      ELSE()
        USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 262)
        USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 262, 0, -100, 0, LW(10), EVT_NULLPTR)
        USER_FUNC(spm::evt_npc::evt_npc_flip_to, LW(10), 1)
        USER_FUNC(spm::evt_npc::evt_npc_finish_flip_instant, LW(10))
        USER_FUNC(spm::evt_npc::evt_npc_flip, LW(10))
        USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(1), LW(2), LW(3))
        ADD(LW(2), 100)
        USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(10), LW(1), LW(2), LW(3))
        USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_NINJA_APPEAR1"), LW(10))
      END_IF()
    END_IF()
  END_IF()
RETURN_FROM_CALL()

void patchStandardDeathScript(){
  evtpatch::hookEvt(standard_death_script, 13, (spm::evtmgr::EvtScriptCode*)rollForNinjoe);
}

EVT_BEGIN(changeFlyingSpeedBleck)
  USER_FUNC(spm::evt_npc::evt_npc_glide_to, PTR("me"), LW(5), LW(6), LW(7), 0, FLOAT(180.0), 0, 11, 0, 0)
RETURN_FROM_CALL()

EVT_BEGIN(changeBoxAttack)
  USER_FUNC(spm::evt_npc::evt_npc_get_axis_movement_unit, PTR("me"), LW(1))
  ADD(LW(1), 1)
RETURN_FROM_CALL()

EVT_BEGIN(turnNull)
  NEXT()
RETURN_FROM_CALL()

EVT_BEGIN(changeSlowdown)
  USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 10, 0)
  USER_FUNC(spm::evt_npc::evt_npc_get_hp, PTR("me"), LW(0))
  IF_LARGE(LW(0), 5)
    ADD(LW(0), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_hp, PTR("me"), LW(0))
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 10, 0)
    SET(LW(0), 16)
    WAIT_MSEC(1000)
  ELSE()
    WAIT_MSEC(200)
  END_IF()
RETURN_FROM_CALL()

EVT_BEGIN(changeDashAttack)
  USER_FUNC(spm::evt_npc::evt_npc_glide_to, PTR("me"), LW(5), LW(6), LW(7), 0, FLOAT(800.0), 0, 2, 0, 0)
RETURN_FROM_CALL()

EVT_BEGIN(changeVoids)
  USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, PTR("me"), 0, 2048)
RETURN_FROM_CALL()

EVT_BEGIN(changeVoids2)
  SET(LW(10), 0)
RETURN_FROM_CALL()

EVT_BEGIN(skipBigVoid)
  SET(LW(6), 0)
RETURN_FROM_CALL()

EVT_BEGIN(returnToAttackCalc1)
  SET(LW(0), 16)
RETURN_FROM_CALL()

EVT_BEGIN(returnToAttackCalc2)
IF_LARGE(LW(0), 29)
IF_SMALL(LW(0), 39)
  SET(LW(0), 25)
END_IF()
IF_SMALL(LW(0), 45)
  SET(LW(0), 55)
END_IF()
END_IF()
RETURN_FROM_CALL()

spm::evtmgr::EvtScriptCode* mimiUnk2 = spm::npcdrv::npcEnemyTemplates[130].unkScript2;
spm::evtmgr::EvtScriptCode* mimiRollAttack = getInstructionEvtArg(mimiUnk2, 63, 0);
spm::npcdrv::NPCDefense * chaosHeartDefense = getInstructionEvtDefense(mimiRollAttack, 20, 3);

EVT_BEGIN(removeSkills)
  USER_FUNC(removeAbilities)
RETURN_FROM_CALL()

EVT_BEGIN(mimiChangeDefense)
  USER_FUNC(spm::evt_npc::evt_npc_modify_part, PTR("me"), 1, 15, PTR(chaosHeartDefense))
RETURN_FROM_CALL()

EVT_BEGIN(mimiFlag8_2048)
  USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, PTR("me"), 1, 2048)
RETURN_FROM_CALL()

EVT_BEGIN(mimiFlag8_turn_both_on)
  USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, PTR("me"), 1, 2048)
  USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, PTR("me"), 1, 32768)
RETURN_FROM_CALL()

EVT_BEGIN(changeMimiAi1)
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 0, 4)
RETURN_FROM_CALL()

EVT_BEGIN(changeMimiAi2)
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 0, 1)
RETURN_FROM_CALL()

EVT_BEGIN(changeMimiSpeed)
  SET(LW(7), 400)
RETURN_FROM_CALL()

EVT_BEGIN(changeMimiSpeed2)
  SET(LW(7), 600)
RETURN_FROM_CALL()

EVT_BEGIN(addMimiAttack)
  ADD(LW(10), 7)
RETURN_FROM_CALL()

EVT_BEGIN(removeMimiAnimation)
  SET(LW(0), FLOAT(0.0))
RETURN_FROM_CALL()

EVT_BEGIN(mimi_death_script)
  USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
  DO(10)
    USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("spm_explosion"), 0, LW(0), LW(1), LW(2), FLOAT(1.2), 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_BOMB_FIRE1"))
    WAIT_MSEC(200)
  WHILE()
  USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, PTR("me"), 14, LW(0))
  IF_EQUAL(LW(0), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("me"), 0, 5000, 0)
  END_IF()
  USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 14, 1)
  USER_FUNC(spm::evt_npc::evt_npc_set_move_mode, PTR("me"), 0)
  RETURN()
EVT_END()

EVT_BEGIN(checkForDan1)
USER_FUNC(spm::evt_sub::evt_sub_get_mapname, 0, LW(11))
USER_FUNC(compareStrings, LW(11), PTR("dan"), LW(13))
IF_EQUAL(LW(13), 1)
    RUN_CHILD_EVT(mimi_death_script)
    RETURN()
ELSE()
  SET(GSW(0), 94)
END_IF()
RETURN_FROM_CALL()

EVT_BEGIN(checkForDan2)
USER_FUNC(spm::evt_sub::evt_sub_get_mapname, 0, LW(11))
USER_FUNC(compareStrings, LW(11), PTR("dan"), LW(13))
IF_EQUAL(LW(13), 1)
    RUN_CHILD_EVT(mimi_death_script)
    RETURN()
ELSE()
  SET(GSW(0), 382)
END_IF()
RETURN_FROM_CALL()

EVT_BEGIN(changeBehaviorToAttack)
  IF_EQUAL(LW(0), 0)
    SET(LW(0), 1)
  END_IF()
RETURN_FROM_CALL()

EVT_BEGIN(increaseLuigiShits)
  SET(LW(0), 60000)
RETURN_FROM_CALL()

EVT_BEGIN(makePhase1LastLessTime)
  SET(LW(1), 250)
RETURN_FROM_CALL()

EVT_BEGIN(makeBowserJump)
  ADD(LW(1), 100)
  USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(4), LW(5))
  USER_FUNC(spm::evt_npc::evt_npc_jump_to, PTR("me"), LW(0), LW(1), LW(2), FLOAT(0.0), 180)
RETURN_FROM_CALL()

EVT_BEGIN(fixBowserAnims)
  USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 5, 1)
RETURN_FROM_CALL()

EVT_BEGIN(fixBowserOnSpawn)
  USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("me"), 14, PTR(spm::npcdrv::npcTribes[315].animDefs))
RETURN_FROM_CALL()

EVT_BEGIN(changeBowserScript)
  USER_FUNC(spm::evt_sub::evt_sub_random, 3, LW(0))
  IF_EQUAL(LW(0), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 0, 4)
  END_IF()
RETURN_FROM_CALL()



const char * dimentio_stg5_1 = "<p>\n"
"Ah ha ha ha, I'm surprised you\n"
"defeated him so easily...\n"
"<k>\n";

const char * dimentio_stg5_2 = "<p>\n"
"But I'm not going to leave\n"
"quite yet...\n"
"<k>\n";

const char * dimentio_stg5_3 = "<p>\n"
"<wave>En garde!</wave>\n"
"<k>\n";

const char * dimentio_stg5_4 = "<p>\n"
"<wave>Look how much effort it takes\n"
"for you to land 10 blows on me.</wave>\n"
"<k>\n";

const char * dimentio_stg5_5 = "<p>\n"
"Alas, I would let you entertain\n"
"me further...\n"
"<k>\n"
"<p>\n"
"But I have an appointment\n"
"with an old acquaintance\n"
"I must keep.\n"
"<k>\n";

const char * dimentio_stg5_6 = "<p>\n"
"I have one last surprise\n"
"for you.\n"
"<k>\n"
"<p>\n"
"Ah ha ha ha.\n"
"<wave>Ciao!</wave>\n"
"<k>\n";

spm::npcdrv::NPCTribeAnimDef animsDimen[] = {
  {0, "S_2"}, // Flying (ID 0 is idle animation)
  {1, "S_1"}, // Standing
  {20, "W_3"}, // Finger click
  {21, "A_1a"}, // Magic start
  {22, "W_2c"}, // Land
  {23, "A_1c"}, // Magic cast
  {24, "A_2a"}, // Raise hands
  {25, "A_2b"}, // Wave hands
  {26, "A_2c"}, // Raise hands cast
  {27, "W_2a"}, // Fly
  {3, "T_2"}, // Talking (ID 3 is the ID to use when talking)
  {-1, nullptr}
};

EVT_BEGIN(spawnDimiGn2)
  USER_FUNC(spm::evt_npc::evt_npc_entry, PTR("dimi"), PTR("e_dmen"), 0)
  USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("dimi"), 0xe, PTR(animsDimen))
  USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("dimi"), 0, 1)
  USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("dimi"), 450, 25, 0)
RETURN_FROM_CALL()

EVT_BEGIN(dimentioMusic1)
  USER_FUNC(spm::evt_snd::evt_snd_bgmon_f_d, 0, PTR("BGM_BTL_DIMEN1"), 2000)
RETURN_FROM_CALL()

EVT_BEGIN(dimiDeath)
  SET(LW(10), PTR("me"))
  USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)
  USER_FUNC(spm::evt_mario::evt_mario_fairy_reset)
  USER_FUNC(spm::evt_npc::evt_npc_set_move_mode, LW(10), 0)
  USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
  USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 1073741824)
  USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
  USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("dmen_warp"), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
  USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR("SFX_BS_DMN_GOOUT1"), LW(0), LW(1), LW(2))
  USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(10), 0, -1000, 0)
  WAIT_MSEC(700)
  USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
  ADD(LW(0), 100)
  ADD(LW(1), 75)
  USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("dimi"), LW(0), LW(1), LW(2))
  USER_FUNC(spm::evt_npc::evt_npc_teleport_effect, 1, PTR("dimi"))
  WAIT_MSEC(300)
  USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(dimentio_stg5_4), 0, PTR("dimi"))
  USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 1, LW(0), EVT_NULLPTR, 484, LW(0), EVT_NULLPTR, -16, 1000, 11)
  USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(dimentio_stg5_5), 0, PTR("dimi"))
  USER_FUNC(spm::evt_snd::evt_snd_bgmoff_f_d, 0, 2000)
  USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(dimentio_stg5_6), 0, PTR("dimi"))
  USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("dimi"), 24, 1)
  USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("dimi"), 1)
  USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_BS_DMN_MAGIC_HAND_UP1"), PTR("dimi"))
  USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("dimi"), 25, 1)
  WAIT_MSEC(1500)
  USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("dimi"), 26, 1)
  USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("dimi"), 1)
  USER_FUNC(spm::evt_fade::evt_set_transition, 26, 25)
  INLINE_EVT()
    USER_FUNC(spm::evt_env::evt_env_blur_on, 0, 1000)
  END_INLINE()
  SET(GSW(0), 191)
  USER_FUNC(spm::evt_seq::evt_seq_mapchange, PTR("gn2_04"), 0)
EVT_END()

EVT_BEGIN(dimentioDialogue1)
  USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(dimentio_stg5_1), 0, PTR("dimi"))
  USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, -1, 380, 60, 384, 380, 60, -16, 800, 11)
  WAIT_MSEC(800)
  USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(dimentio_stg5_2), 0, PTR("dimi"))
  USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(dimentio_stg5_3), 0, PTR("dimi"))
  USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 225)
  USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("dimi"), LW(0), LW(1), LW(2))
  USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 225, LW(0), LW(1), LW(2), LW(10), EVT_NULLPTR)
  USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(10), 8, PTR(dimiDeath))
  USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("dimi"), 0, -1000, 0)
  USER_FUNC(spm::evt_door::evt_door_enable_disable_map_door_desc, 0, PTR("doa1_l"))
  SET(GSW(0), 190)
RETURN_FROM_CALL()

EVT_BEGIN(checkForDimentio) //If stone tablet is active during dimentio fight it causes a crash, so check for dimentio via GSW and disable it if its too low
  IF_SMALL(GSW(0), 191)
    RETURN()
  END_IF()
RETURN_FROM_CALL()

void hookBowserScripts()
{
  spm::evtmgr::EvtScriptCode* bowserOnSpawn = spm::npcdrv::npcEnemyTemplates[285].onSpawnScript;
  spm::evtmgr::EvtScriptCode* mainLogic = spm::npcdrv::npcEnemyTemplates[285].unkScript7;
  evtpatch::hookEvt(mainLogic, 37, (spm::evtmgr::EvtScriptCode*)changeBowserScript);
  evtpatch::hookEvt(mainLogic, 172, (spm::evtmgr::EvtScriptCode*)makeBowserJump);
  evtpatch::hookEvt(mainLogic, 228, fixBowserAnims);
  evtpatch::hookEvt(bowserOnSpawn, 1, fixBowserOnSpawn);
}

void hookLs4_11()
{
  spm::map_data::MapData * ls4_11_md = spm::map_data::mapDataPtr("ls4_11");
  spm::evtmgr::EvtScriptCode* soupDimentio = getInstructionEvtArg(ls4_11_md->initScript, 249, 0);
  evtpatch::hookEvt(soupDimentio, 174, (spm::evtmgr::EvtScriptCode*)runDimiSaveScript);
}

void hookSuperDimentioScripts()
{
  spm::evtmgr::EvtScriptCode* mainLogic = spm::npcdrv::npcEnemyTemplates[255].unkScript7;
  evtpatch::hookEvtReplace(mainLogic, 10, (spm::evtmgr::EvtScriptCode*)increaseLuigiShits);
  evtpatch::hookEvtReplace(mainLogic, 34, (spm::evtmgr::EvtScriptCode*)makePhase1LastLessTime);
  //spm::evtmgr::EvtScriptCode* throwAttack = getInstructionEvtArg(mainLogic, 65, 0);
  //evtpatch::hookEvt(mainLogic, 21, (spm::evtmgr::EvtScriptCode*)throwAttack);
}

EVT_BEGIN(activateHampter)
  USER_FUNC(giveHampterCheese)
RETURN_FROM_CALL()

  spm::evtmgr::EvtScriptCode* onSpawn = spm::npcdrv::npcEnemyTemplates[183].onSpawnScript;
  spm::evtmgr::EvtScriptCode* chonkyAttackScript = getInstructionEvtArg(onSpawn, 41, 3);
  spm::evtmgr::EvtScriptCode* throwBlockScript = getInstructionEvtArg(chonkyAttackScript, 109, 0);
  spm::evtmgr::EvtScriptCode* fartAttack = getInstructionEvtArg(chonkyAttackScript, 164, 0);


EVT_BEGIN(oChunkSucks)
  SET(LW(10), 0)
RETURN_FROM_CALL()

EVT_BEGIN(oChunkSucks2)
  SET(LW(0), 75)
RETURN_FROM_CALL()

EVT_BEGIN(oChunkSucks3)
  USER_FUNC(spm::evt_sub::evt_sub_random, 100, LW(1))
RETURN_FROM_CALL()

EVT_BEGIN(sillyFunChunks)
  USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(1), LW(2), LW(3))
  USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(5), LW(6), LW(7))
  IF_LARGE(LW(1), LW(5))
    ADD(LW(1), 150)
  ELSE()
    SUB(LW(1), 150)
  END_IF()
  USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_BS_DDN_JUMP1"), PTR("me"))
  USER_FUNC(checkBboxChonkyBoi, LW(1))
  USER_FUNC(spm::evt_npc::evt_npc_arc_to, PTR("me"), LW(1), LW(2), LW(3), 500, 0, FLOAT(100.0), 0, 256, 0)
  USER_FUNC(spm::evt_sub::evt_sub_random, 100, LW(1))
  IF_LARGE(LW(1), 50)
    RUN_CHILD_EVT(throwBlockScript)
  ELSE()
    RUN_CHILD_EVT(fartAttack)
  END_IF()
RETURN_FROM_CALL()

EVT_BEGIN(sammerBboxCheck)
  USER_FUNC(spm::evt_sub::evt_sub_get_mapname, 0, LW(11))
  USER_FUNC(compareStrings, LW(11), PTR("dan"), LW(13))
  IF_EQUAL(LW(13), 0)
    USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(1), LW(2), LW(3))
    USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(5), LW(6), LW(7))
    IF_LARGE(LW(1), LW(5))
      ADD(LW(1), 150)
    ELSE()
      SUB(LW(1), 150)
    END_IF()
    USER_FUNC(checkBboxSammer, LW(1))
    IF_EQUAL(LW(1), FLOAT(0.0))
      USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(1), LW(2), LW(3))
      IF_LARGE(LW(1), LW(5))
        SUB(LW(1), 150)
      ELSE()
        ADD(LW(1), 150)
      END_IF()
      USER_FUNC(spm::evt_npc::evt_npc_arc_to, PTR("me"), LW(1), LW(2), LW(3), 500, 0, FLOAT(100.0), 0, 256, 0)
    END_IF()
  END_IF()
RETURN_FROM_CALL()

EVT_BEGIN(mimiInvFrames)
  USER_FUNC(spm::evt_npc::evt_npc_set_move_mode, PTR("me"), 2)
  WAIT_MSEC(800)
  USER_FUNC(spm::evt_npc::evt_npc_set_move_mode, PTR("me"), 1)
RETURN_FROM_CALL()

EVT_BEGIN(standardOnHit)
  USER_FUNC(spm::evt_npc::evt_npc_wait_for, PTR("me"), 200)
RETURN_FROM_CALL()

EVT_BEGIN(increaseFartSpeed)
  USER_FUNC(spm::evt_npc::evt_npc_glide_to, PTR("me"), LW(0), 400, LW(2), 600, 0, 0, 1, 0, 0)
RETURN_FROM_CALL()

void hampter()
{
    spm::map_data::MapData * mi3_01_md = spm::map_data::mapDataPtr("mi3_01");
    evtpatch::hookEvtReplaceBlock(mi3_01_md->initScript, 47, (spm::evtmgr::EvtScriptCode*)activateHampter, 57);
}

//spm::evtmgr::EvtScriptCode chunkFloat[] = { IF_LARGE_EQUAL(LW(1), FLOAT(900.0)) };

void hookChunkScripts()
{
  spm::map_data::MapData * gn2_03_md = spm::map_data::mapDataPtr("gn2_03");
  spm::evtmgr::EvtScriptCode* gn2_03_init = gn2_03_md->initScript;
  spm::evtmgr::EvtScriptCode* oChunksDeath = getInstructionEvtArg(gn2_03_init, 52, 0);
  spm::evtmgr::EvtScriptCode* stoneTabletScript = getInstructionEvtArg(gn2_03_init, 56, 0);
  evtpatch::hookEvtReplace(oChunksDeath, 99, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(oChunksDeath, 102, (spm::evtmgr::EvtScriptCode*)spawnDimiGn2);
  evtpatch::hookEvt(oChunksDeath, 143, (spm::evtmgr::EvtScriptCode*)dimentioDialogue1);
  evtpatch::hookEvt(stoneTabletScript, 1, (spm::evtmgr::EvtScriptCode*)checkForDimentio);
  evtpatch::hookEvtReplace(oChunksDeath, 146, (spm::evtmgr::EvtScriptCode*)dimentioMusic1);
  evtpatch::hookEvtReplaceBlock(oChunksDeath, 147, (spm::evtmgr::EvtScriptCode*)turnNull, 150);

  // 8-1

  spm::evtmgr::EvtScriptCode* grabScript = getInstructionEvtArg(chonkyAttackScript, 182, 0);
  spm::evtmgr::EvtScriptCode* jumpScript = getInstructionEvtArg(chonkyAttackScript, 112, 0);
  evtpatch::patchEvtInstruction(chonkyAttackScript, 174, EVT_CAST(IF_LARGE_EQUAL(LW(1), FLOAT(900.0))));
  evtpatch::hookEvtReplace(chonkyAttackScript, 155, (spm::evtmgr::EvtScriptCode*)sillyFunChunks);
  evtpatch::hookEvtReplace(chonkyAttackScript, 144, (spm::evtmgr::EvtScriptCode*)oChunkSucks3);
  evtpatch::hookEvt(chonkyAttackScript, 98, (spm::evtmgr::EvtScriptCode*)oChunkSucks);
  evtpatch::hookEvtReplace(chonkyAttackScript, 16, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(chonkyAttackScript, 15, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(fartAttack, 23, (spm::evtmgr::EvtScriptCode*)increaseFartSpeed);

  evtpatch::hookEvtReplace(fartAttack, 114, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(fartAttack, 116, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(fartAttack, 118, (spm::evtmgr::EvtScriptCode*)turnNull);

}

void hookBleckScripts()
{
  spm::evtmgr::EvtScriptCode* bleckUnk2 = spm::npcdrv::npcEnemyTemplates[196].unkScript2;
  spm::evtmgr::EvtScriptCode* mainLogic = spm::npcdrv::npcEnemyTemplates[196].unkScript7;
  spm::evtmgr::EvtScriptCode* bigVoidMainLogic = spm::npcdrv::npcEnemyTemplates[198].unkScript2;
  bleckMovementScript = getInstructionEvtArg(mainLogic, 34, 0);
  spm::evtmgr::EvtScriptCode* shootVoid = getInstructionEvtArg(mainLogic, 22, 0);
  //spm::evtmgr::EvtScriptCode* shootBigVoid = getInstructionEvtArg(mainLogic, 128, 0);
  spm::evtmgr::EvtScriptCode* dashAttack = getInstructionEvtArg(mainLogic, 42, 0);
  spm::evtmgr::EvtScriptCode* slowDown = getInstructionEvtArg(mainLogic, 69, 0);
  evtpatch::hookEvt(mainLogic, 13, (spm::evtmgr::EvtScriptCode*)returnToAttackCalc1);
  evtpatch::hookEvt(mainLogic, 118, (spm::evtmgr::EvtScriptCode*)returnToAttackCalc2);
  evtpatch::hookEvt(bigVoidMainLogic, 12, (spm::evtmgr::EvtScriptCode*)skipBigVoid);
  evtpatch::hookEvt(bigVoidMainLogic, 28, (spm::evtmgr::EvtScriptCode*)skipBigVoid);
  evtpatch::hookEvtReplace(shootVoid, 25, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(shootVoid, 52, (spm::evtmgr::EvtScriptCode*)changeVoids2);
  evtpatch::hookEvtReplaceBlock(shootVoid, 53, (spm::evtmgr::EvtScriptCode*)changeVoids, 61);
  evtpatch::hookEvtReplace(slowDown, 17, (spm::evtmgr::EvtScriptCode*)changeSlowdown);
  evtpatch::hookEvtReplace(dashAttack, 45, (spm::evtmgr::EvtScriptCode*)changeDashAttack);
  spm::evtmgr::EvtScriptCode* bleckMovement = getInstructionEvtArg(bleckUnk2, 8, 0);
  evtpatch::hookEvtReplace(bleckMovement, 52, (spm::evtmgr::EvtScriptCode*)changeFlyingSpeedBleck);
}

void hookMimiScripts()
{
  spm::evtmgr::EvtScriptCode* mimiOnHitScript = spm::npcdrv::npcEnemyTemplates[130].unkScript3;
  spm::evtmgr::EvtScriptCode* mimiMovement = getInstructionEvtArg(mimiUnk2, 56, 0);
  spm::evtmgr::EvtScriptCode* mimiMainAttack = getInstructionEvtArg(mimiUnk2, 58, 0);
  spm::evtmgr::EvtScriptCode* mimiCeilingMovement = getInstructionEvtArg(mimiUnk2, 60, 0);
  spm::evtmgr::EvtScriptCode* mimiCeilingAttack = getInstructionEvtArg(mimiCeilingMovement, 58, 0);
  spm::evtmgr::EvtScriptCode* mimiMoneyWave = getInstructionEvtArg(mimiUnk2, 65, 0);
  #ifdef SPM_EU0
    spm::evtmgr::EvtScriptCode* mimiTrueHit = getInstructionEvtArg(mimiOnHitScript, 55, 0);
    evtpatch::patchEvtInstruction(mimiTrueHit, 74, EVT_CAST(IF_LARGE(GSW(0), 0)));
  #else
    spm::evtmgr::EvtScriptCode* mimiTrueHit = getInstructionEvtArg(mimiOnHitScript, 54, 0);
    evtpatch::patchEvtInstruction(mimiTrueHit, 73, EVT_CAST(IF_LARGE(GSW(0), 0)));
  #endif
  //evtpatch::hookEvt(mimiUnk2, 54, (spm::evtmgr::EvtScriptCode*)removeMimiBasicWalk);
  evtpatch::hookEvtReplace(mimiMainAttack, 31, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(mimiTrueHit, 25, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(mimiTrueHit, 29, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(mimiTrueHit, 39, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(mimiTrueHit, 43, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(mimiTrueHit, 53, (spm::evtmgr::EvtScriptCode*)turnNull);
  #ifdef SPM_EU0
  evtpatch::hookEvtReplace(mimiTrueHit, 72, (spm::evtmgr::EvtScriptCode*)checkForDan1);
  evtpatch::hookEvtReplace(mimiTrueHit, 75, (spm::evtmgr::EvtScriptCode*)checkForDan2);
  #else
  evtpatch::hookEvtReplace(mimiTrueHit, 70, (spm::evtmgr::EvtScriptCode*)checkForDan1);
  evtpatch::hookEvtReplace(mimiTrueHit, 73, (spm::evtmgr::EvtScriptCode*)checkForDan2);
  #endif

  evtpatch::hookEvt(mimiTrueHit, 116, (spm::evtmgr::EvtScriptCode*)mimiFlag8_2048);
  evtpatch::hookEvt(mimiUnk2, 33, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiUnk2, 54, (spm::evtmgr::EvtScriptCode*)changeBehaviorToAttack);
  evtpatch::hookEvt(mimiMovement, 41, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvtReplace(mimiMainAttack, 34, (spm::evtmgr::EvtScriptCode*)addMimiAttack);
  evtpatch::hookEvt(mimiMainAttack, 41, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiMovement, 52, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiMovement, 57, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiMainAttack, 47, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiCeilingMovement, 49, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed2);
  evtpatch::hookEvt(mimiCeilingMovement, 53, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed2);
  evtpatch::hookEvt(mimiCeilingMovement, 71, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed2);
  evtpatch::hookEvt(mimiCeilingMovement, 76, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed2);
  //evtpatch::hookEvtReplace(mimiCeilingMovement, 30, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(mimiCeilingMovement, 60, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(mimiCeilingAttack, 19, (spm::evtmgr::EvtScriptCode*)removeMimiAnimation);
  evtpatch::hookEvt(mimiRollAttack, 58, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiRollAttack, 60, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiRollAttack, 75, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiRollAttack, 77, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiRollAttack, 92, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiRollAttack, 94, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiRollAttack, 135, (spm::evtmgr::EvtScriptCode*)changeMimiAi1);
  //evtpatch::hookEvt(mimiRollAttack, 1, (spm::evtmgr::EvtScriptCode*)removeSkills);
  //evtpatch::hookEvtReplace(mimiRollAttack, 114, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvt(mimiMoneyWave, 19, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed2);
  evtpatch::hookEvt(mimiMoneyWave, 52, (spm::evtmgr::EvtScriptCode*)changeMimiAi2);
  evtpatch::hookEvtReplace(mimiMoneyWave, 48, (spm::evtmgr::EvtScriptCode*)turnNull);
}

EVT_BEGIN(jumpToLbl50)
  WAIT_MSEC(100)
  GOTO(50)
RETURN_FROM_CALL()

void hookOtherMimiScripts()
{
  spm::evtmgr::EvtScriptCode* mimiOnHitScript = spm::npcdrv::npcEnemyTemplates[187].unkScript3;
  spm::evtmgr::EvtScriptCode* script_2 = getInstructionEvtArg(mimiOnHitScript, 10, 0);
  #ifdef SPM_EU0
  spm::evtmgr::EvtScriptCode* standard_onhit = getInstructionEvtArg(script_2, 1, 0);
  evtpatch::hookEvtReplace(script_2, 3, (spm::evtmgr::EvtScriptCode*)mimiInvFrames);
  evtpatch::hookEvtReplace(standard_onhit, 228, (spm::evtmgr::EvtScriptCode*)jumpToLbl50);
  evtpatch::hookEvtReplace(standard_onhit, 185, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 191, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 203, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 206, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 209, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 219, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 222, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 234, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 240, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 292, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 294, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 313, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 321, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  #else
  spm::evtmgr::EvtScriptCode* standard_onhit = getInstructionEvtArg(script_2, 1, 0);
  //spm::evtmgr::EvtScriptCode* standard_onhit = getInstructionEvtArg(script_2_1, 165, 0);
  evtpatch::hookEvtReplace(script_2, 3, (spm::evtmgr::EvtScriptCode*)mimiInvFrames);
  evtpatch::hookEvtReplace(standard_onhit, 225, (spm::evtmgr::EvtScriptCode*)jumpToLbl50);
  evtpatch::hookEvtReplace(standard_onhit, 182, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 188, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 200, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 203, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 206, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 216, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 219, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 231, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 237, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 289, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 291, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 310, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  evtpatch::hookEvtReplace(standard_onhit, 318, (spm::evtmgr::EvtScriptCode*)standardOnHit);
  #endif
}

static spm::npcdrv::NPCTribeAnimDef ninja_anims[] =  {
    {0, "S_1"}, // Standing (ID 0 is idle animation)
    {2, "R_1"}, // Running
    {3, "T_1"}, // Talking (ID 3 is the ID to use when talking)
    {5, "O_1"}, // Disappear
    {6, "J_1"}, // Begin jump
    {7, "J_3"}, // Hold jump
    {-1, nullptr}
  };

  const char * challenger_1 = "<p>\n"
  "<scale 0.7><wave><shake>\n"
  "Prove your honor by defeating my\n"
  "challenges, and you'll get a reward...\n"
  "<k>\n";

  const char * challenger_2 = "<p>\n"
  "<scale 0.7><wave><shake>\n"
  "I challenge you to...\n"
  "<wait 150>%s\n"
  "Do you accept?"
  "<o>\n";

const char challengeOptions[] =
  "<select 0 -1 160 20>\n"
  "Yes\n"
  "No";

  const char * challenger_refuse = "<p>\n"
  "<scale 0.7><wave><shake>\n"
  "Your cowardice will be your undoing, challenger...\n"
  "<wait 150>See you on the other side...\n"
  "<k>\n";

  const char * challenger_won = "<p>\n"
  "<scale 0.7><wave><shake>\n"
  "Bravo, warrior...\n"
  "You conquered my challenge...\n"
  "And shall now be rewarded for it...\n"
  "<k>\n"
  "<p>\n"
  "Skipping %d gates...\n"
  "<k>\n";

EVT_BEGIN(challenger_dialogue)
  USER_FUNC(spm::evt_mario::evt_mario_key_off, 1)
  USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("T_1"), 0)
  IF_EQUAL(GSWF(1700), 0)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(challenger_1), 0, PTR("me"))
    SET(GSWF(1700), 1)
  END_IF()
  USER_FUNC(spm::evt_sub::evt_sub_random, 4, LW(1))
  USER_FUNC(set_current_challenge, LW(1))
  SWITCH(LW(1))
    CASE_EQUAL(0)
      USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(challenger_2), 0, PTR("me"), PTR("Kill the sammer guy with 1 health..."))
      USER_FUNC(spm::evt_msg::evt_msg_select, 1, PTR(challengeOptions))
      USER_FUNC(spm::evt_msg::evt_msg_continue)
    CASE_EQUAL(1)
      USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(challenger_2), 0, PTR("me"), PTR("Kill the sammer guy in 10 seconds..."))
      USER_FUNC(spm::evt_msg::evt_msg_select, 1, PTR(challengeOptions))
      USER_FUNC(spm::evt_msg::evt_msg_continue)
    CASE_EQUAL(2)
      USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(challenger_2), 0, PTR("me"), PTR("Kill 3 ninjas before the sammer guy..."))
      USER_FUNC(spm::evt_msg::evt_msg_select, 1, PTR(challengeOptions))
      USER_FUNC(spm::evt_msg::evt_msg_continue)
    CASE_EQUAL(3)
      USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(challenger_2), 0, PTR("me"), PTR("Kill the sammer guy with 1 attack..."))
      USER_FUNC(spm::evt_msg::evt_msg_select, 1, PTR(challengeOptions))
      USER_FUNC(spm::evt_msg::evt_msg_continue)
    CASE_EQUAL(4)
      USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(challenger_2), 0, PTR("me"), PTR("Kill 3 ninjas with 1 attack and 1 health..."))
      USER_FUNC(spm::evt_msg::evt_msg_select, 1, PTR(challengeOptions))
      USER_FUNC(spm::evt_msg::evt_msg_continue)
  END_SWITCH()
  SWITCH(LW(0))
    CASE_EQUAL(0)
      USER_FUNC(get_challenge_script, LW(0))
      RUN_EVT(LW(0))
      USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
      ADD(LW(1), 100)
      USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 6, 1)
      USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
      USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 7, 1)
      USER_FUNC(spm::evt_npc::evt_npc_jump_to, PTR("me"), LW(0), LW(1), LW(2), FLOAT(100.0), 180)
      USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 5, 1)
      USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_NINJA_APPEAR1"), PTR("me"))
      USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
      USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("me"), LW(0), -1000, LW(2))
      USER_FUNC(accept_challenge)
    CASE_EQUAL(1)
      USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(challenger_refuse), 0, PTR("me"))
      USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
      ADD(LW(1), 100)
      USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 6, 1)
      USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
      USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 7, 1)
      USER_FUNC(spm::evt_npc::evt_npc_jump_to, PTR("me"), LW(0), LW(1), LW(2), FLOAT(100.0), 180)
      USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 5, 1)
      USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_NINJA_APPEAR1"), PTR("me"))
      USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
      USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("me"), LW(0), -1000, LW(2))
      USER_FUNC(refuse_challenge)
  END_SWITCH()
  USER_FUNC(spm::evt_mario::evt_mario_key_on)
  RETURN()
EVT_END()

EVT_BEGIN(fwd_challenger_dialogue)
  RUN_EVT(challenger_dialogue)
  RETURN()
EVT_END()

  EVT_BEGIN(delete_challenger)
    USER_FUNC(checkChallengeAccepted)
    IF_EQUAL(LW(0), 0)
      USER_FUNC(spm::evt_npc::evt_npc_delete, PTR("secret_challenges"))
    END_IF()
    RETURN()
  EVT_END()

EVT_BEGIN(wait_to_kill_challenger)
  DO(0)
      USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
      IF_LARGE(LW(0), -100)
          DO_BREAK()
      END_IF()
      WAIT_FRM(1)
  WHILE()
  RUN_EVT(delete_challenger)
RETURN()
EVT_END()

EVT_BEGIN(spawn_challenger)
  USER_FUNC(refuse_challenge)
  USER_FUNC(spm::evt_sub::evt_sub_random, 1, LW(0))
  IF_EQUAL(LW(0), 1)
    GOTO(1)
  END_IF()
  IF_SMALL(GSW(0), 420)
    GOTO(1)
  END_IF()
  USER_FUNC(spm::evt_sub::evt_sub_get_mapname, 0, LW(10))
  IF_STR_EQUAL(LW(10), PTR("wa1_25"))
    GOTO(1)
  END_IF()
  IF_STR_EQUAL(LW(10), PTR("wa1_26"))
    GOTO(1)
  END_IF()
  IF_STR_EQUAL(LW(10), PTR("wa2_24"))
    GOTO(1)
  END_IF()
  IF_STR_EQUAL(LW(10), PTR("wa2_25"))
    GOTO(1)
  END_IF()
  IF_STR_EQUAL(LW(10), PTR("wa3_24"))
    GOTO(1)
  END_IF()
  IF_STR_EQUAL(LW(10), PTR("wa3_25"))
    GOTO(1)
  END_IF()
  IF_STR_EQUAL(LW(10), PTR("wa4_24"))
    GOTO(1)
  END_IF()
  IF_STR_EQUAL(LW(10), PTR("wa4_25"))
    GOTO(1)
  END_IF()
  USER_FUNC(spm::evt_npc::evt_npc_entry, PTR("secret_challenges"), PTR("e_ninja"), 0)
  USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("secret_challenges"), -560, 0, 0)
  USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("secret_challenges"), 0xe, PTR(ninja_anims))
  USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("secret_challenges"), 0, 1)
  USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("secret_challenges"), 9, PTR(fwd_challenger_dialogue))
  RUN_EVT(wait_to_kill_challenger)
  LBL(1)
RETURN_FROM_CALL()

EVT_BEGIN(check_challenge_completion)
  USER_FUNC(checkChallengeAccepted)
  USER_FUNC(checkChallengeSucceeded, LW(0), LW(1))
  IF_EQUAL(LW(0), 1)
    USER_FUNC(returnNewMapName, LW(1))
    USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(challenger_won), 0, 0, LW(1))
    USER_FUNC(set_challenge_success, 0)
    USER_FUNC(spm::evt_sub::evt_sub_random, 5, LW(0))
    USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(5), LW(6), LW(7))
    SWITCH(LW(0))
      CASE_EQUAL(0)
        USER_FUNC(spm::evt_item::evt_item_entry, PTR("item"), 83, LW(5), LW(6), LW(7), 0, 0, 0, 0, 0)
        USER_FUNC(spm::evt_item::evt_item_flag_onoff, 1, PTR("item"), 8)
        USER_FUNC(spm::evt_item::evt_item_wait_collected, PTR("item"))
      CASE_EQUAL(1)
        USER_FUNC(spm::evt_item::evt_item_entry, PTR("item"), 80, LW(5), LW(6), LW(7), 0, 0, 0, 0, 0)
        USER_FUNC(spm::evt_item::evt_item_flag_onoff, 1, PTR("item"), 8)
        USER_FUNC(spm::evt_item::evt_item_wait_collected, PTR("item"))
      CASE_EQUAL(2)
        USER_FUNC(spm::evt_item::evt_item_entry, PTR("item"), 105, LW(5), LW(6), LW(7), 0, 0, 0, 0, 0)
        USER_FUNC(spm::evt_item::evt_item_flag_onoff, 1, PTR("item"), 8)
        USER_FUNC(spm::evt_item::evt_item_wait_collected, PTR("item"))
      CASE_EQUAL(3)
        USER_FUNC(spm::evt_item::evt_item_entry, PTR("item"), 118, LW(5), LW(6), LW(7), 0, 0, 0, 0, 0)
        USER_FUNC(spm::evt_item::evt_item_flag_onoff, 1, PTR("item"), 8)
        USER_FUNC(spm::evt_item::evt_item_wait_collected, PTR("item"))
      CASE_EQUAL(4)
        USER_FUNC(spm::evt_item::evt_item_entry, PTR("item"), 101, LW(5), LW(6), LW(7), 0, 0, 0, 0, 0)
        USER_FUNC(spm::evt_item::evt_item_flag_onoff, 1, PTR("item"), 8)
        USER_FUNC(spm::evt_item::evt_item_wait_collected, PTR("item"))
      CASE_EQUAL(5)
        USER_FUNC(spm::evt_item::evt_item_entry, PTR("item"), 96, LW(5), LW(6), LW(7), 0, 0, 0, 0, 0)
        USER_FUNC(spm::evt_item::evt_item_flag_onoff, 1, PTR("item"), 8)
        USER_FUNC(spm::evt_item::evt_item_wait_collected, PTR("item"))
    END_SWITCH()
    USER_FUNC(get_current_challenge, LW(0))
    SWITCH(LW(0))
      CASE_EQUAL(0)
        USER_FUNC(spm::evt_pouch::evt_pouch_get_max_hp, LW(2))
        USER_FUNC(spm::evt_pouch::evt_pouch_set_hp, LW(2))
      CASE_EQUAL(3)
        USER_FUNC(spm::evt_pouch::evt_pouch_set_attack, GSW(1700))
      CASE_EQUAL(4)
        USER_FUNC(spm::evt_pouch::evt_pouch_set_attack, GSW(1700))
        USER_FUNC(spm::evt_pouch::evt_pouch_get_max_hp, LW(2))
        USER_FUNC(spm::evt_pouch::evt_pouch_set_hp, LW(2))
    END_SWITCH()
    USER_FUNC(spm::evt_seq::evt_seq_set_seq, spm::seqdrv::SEQ_MAPCHANGE, PTR(newMapName), PTR("doa1_l"))
  END_IF()
  USER_FUNC(set_sammer_start, 0)
RETURN_FROM_CALL()

void hookSammerScripts()
{
  spm::evtmgr::EvtScriptCode* sammerIdle = spm::npcdrv::npcEnemyTemplates[426].unkScript3;
  evtpatch::hookEvt(sammerIdle, 2, (spm::evtmgr::EvtScriptCode*)sammerBboxCheck);
  spm::map_data::MapData * wa1_02_md = spm::map_data::mapDataPtr("wa1_02");
  spm::evtmgr::EvtScriptCode* sammer_fight_setup_evt = getInstructionEvtArg(wa1_02_md->initScript, 62, 0);
  spm::evtmgr::EvtScriptCode* sammer_fight_start_evt = getInstructionEvtArg(sammer_fight_setup_evt, 61, 5);
  spm::evtmgr::EvtScriptCode* sammer_door_disable_evt = getInstructionEvtArg(sammer_fight_start_evt, 46, 0);
  spm::evtmgr::EvtScriptCode* sammer_death_evt = getInstructionEvtArg(sammer_fight_setup_evt, 54, 3);
  //sammer_mario_x_wait_evt = getInstructionEvtArg(sammer_fight_setup_evt, 60, 0);
  evtpatch::hookEvt(sammer_fight_setup_evt, 1, (spm::evtmgr::EvtScriptCode*)spawn_challenger);
  evtpatch::hookEvt(sammer_door_disable_evt, 1, (spm::evtmgr::EvtScriptCode*)toggleSammerFightStart_fwd);
  #ifdef SPM_EU0
    evtpatch::hookEvt(sammer_death_evt, 91, (spm::evtmgr::EvtScriptCode*)check_challenge_completion);
  #else
    evtpatch::hookEvt(sammer_death_evt, 90, (spm::evtmgr::EvtScriptCode*)check_challenge_completion);
  #endif

}

spm::evtmgr::EvtScriptCode* getStandardDeathScript()
{
  spm::evtmgr::EvtScriptCode* dimentioOnDeath = spm::npcdrv::npcEnemyTemplates[225].unkScript6;
  return getInstructionEvtArg(dimentioOnDeath, 20, 0);
}

s32 dimen_determine_move_pos_new(spm::evtmgr::EvtEntry *entry, bool isFirstCall)
{
    spm::mario::MarioWork *marioWork = spm::mario::marioGetPtr();
    spm::npcdrv::NPCEntry *npc = entry->ownerNPC;
    double destYPos = 0;
    f32 marioZ = ((marioWork->position).z);
    f32 destXPos = 0;
    u32 dimenMoveRand = 0;
    wii::mtx::Vec3 min;
    wii::mtx::Vec3 max;
    spm::hitdrv::hitGetMapEntryBbox(0, &min, &max);
    s32 i = 0;
    do
    {
        while (true)
        {
            do
            {
                i = i + 1;
                dimenMoveRand = spm::system::irand(400);
                destXPos = ((marioWork->position).x + (f32)dimenMoveRand - 200);
                if (i > 50)
                {
                    destXPos = npc->position.x;
                    goto outOfBounds;
                }
            } while ((destXPos <= (min.x + 25)) || ((max.x - 25) <= destXPos));
        outOfBounds:
            u32 yMoveBehavior = spm::system::irand(100);
            if (yMoveBehavior < 67)
            {
                dimenMoveRand = spm::system::irand(4);
                destYPos = (10.0 * (f32)dimenMoveRand + 20.0);
            }
            else
            {
                dimenMoveRand = spm::system::irand(3);
                destYPos = (32.0 * (f32)dimenMoveRand + 40.0);
            }
            if (npc->flippedTo3d != 0)
                break;
            if ((100.0 < __builtin_abs((destXPos - (marioWork->position).x))) || (80.0 < destYPos))
                goto setFloats;
        }
        destYPos = spm::system::distABf(destXPos, marioZ, ((marioWork->position).x), marioZ);
    } while ((destYPos <= 120.0) && (destYPos <= 80.0));
setFloats:
    spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)entry->pCurData;
    spm::evtmgr_cmd::evtSetFloat(entry, args[0], destXPos);
    spm::evtmgr_cmd::evtSetFloat(entry, args[1], destYPos);
    spm::evtmgr_cmd::evtSetFloat(entry, args[2], marioZ);
    return 2;
}

    EVT_BEGIN(patch_dimen)
    SWITCH(GSW(0))
    CASE_SMALL(117) // 3-3 Dimentio, vanilla coordinates
    SET(LW(11), -350)
    SET(LW(12), 350)
    SWITCH_BREAK()
    CASE_SMALL(195) // 5-2 Dimentio, for use with Hard Mode R&M's gn2_03 Dimentio fight
    SET(LW(11), -958)
    SET(LW(12), 955)
    SWITCH_BREAK()
    CASE_SMALL(404) // 8-3 Dimentio, widened coordinates to fit the larger room
    SET(LW(11), -465)
    SET(LW(12), 465)
    SWITCH_BREAK()
    END_SWITCH()
    RETURN_FROM_CALL()

    static void dimenPatch()
        { //thanks to eemuh
            patch::hookFunction(spm::evt_npc::evt_npc_dimen_determine_move_pos, dimen_determine_move_pos_new);

            //evtpatch::hookEvt(spm::npc_dimeen_l::dimen_unk_fight_script_1, 1, patch_dimen);
            //evtpatch::hookEvt(spm::npc_dimeen_l::dimen_unk_fight_script_2, 1, patch_dimen);
            //evtpatch::hookEvt(spm::npc_dimeen_l::dimen_unk_fight_script_3, 1, patch_dimen);
        }

        struct TextureWork {
          u32 destId = 0;
          u32 srcId = 0;
          wii::tpl::TPLHeader *destTpl = nullptr;
          wii::tpl::TPLHeader *srcTpl = nullptr;
          const char *filePath;
          bool free = false;
          s32 heapType = spm::memory::Heap::HEAP_MAIN;
        };

        void patchTpl(TextureWork * textureWork) {

          // Loads the tpl if not already loaded by the stated filePath
          if (textureWork->srcTpl == nullptr){
          spm::filemgr::FileEntry * srcFile = spm::filemgr::fileAllocf(4, textureWork->filePath);
          s32 tplSize = srcFile->length;
          textureWork->srcTpl = (wii::tpl::TPLHeader *)spm::memory::__memAlloc(textureWork->heapType, tplSize);
          msl::string::memcpy(textureWork->srcTpl, srcFile->sp->data, tplSize);
          spm::filemgr::fileFree(srcFile);
          }

          // Patches the destination tpl with the one given by the mod.rel
          textureWork->destTpl->imageTable[textureWork->destId] = textureWork->srcTpl->imageTable[textureWork->srcId];

          // Free the memory of the tpl loaded from mod.rel to prevent a leak
          if (textureWork->free){
          spm::memory::__memFree(textureWork->heapType, textureWork->srcTpl);
          }
          return;
        }

static inline void patchStats()
{
  setBossHP();
  setBossDef();
}

void main() {
  wii::os::OSReport("SPM Rel Loader: the mod has ran!\n");
  titleScreenCustomTextPatch();
  standard_death_script = getStandardDeathScript();
  patchStats();
  marioDamageMain();
  patchItems();
  patchCooking();
  reduceXpGained();
  patchVariables();
  evtpatch::evtmgrExtensionInit(); // Initialize EVT scripting extension
  e_dmenMain();
  hookSuperDimentioScripts();
  hookBleckScripts();
  hookMimiScripts();
  hookOtherMimiScripts();
  hookChunkScripts();
  hookLs4_11();
  dimenPatch();
  ninjaPatch();
  kingSammerMain();
  patchStandardDeathScript();
  hookSammerScripts();
  shadooMain();
  hookBowserScripts();
  patch_dan::patch_dan_main();
  #ifdef SPM_EU0
  //wii::tpl::TPLHeader *myTplHeader = nullptr;
  TextureWork myTextures = {116, 0, (wii::tpl::TPLHeader *)spm::icondrv::icondrv_wp->wiconTpl->sp->data, nullptr, "./a/n_mg_flower-", true};
  patchTpl(&myTextures);
  #endif
  //hampter(); // will always live in our memories
}

}
