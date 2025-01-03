#include "mod.h"
#include "patch.h"
#include "scripting.cpp"

#include <spm/pausewin.h>
#include <spm/animdrv.h>
#include <spm/wpadmgr.h>
#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <spm/evtmgr_cmd.h>
#include <spm/seq_game.h>
#include <spm/npcdrv.h>
#include <spm/mario.h>
#include <spm/spmario.h>
#include <spm/mario_pouch.h>
#include <spm/seqdef.h>
#include <spm/others.h>
#include <spm/item_data.h>
//#include <spm/item_event_data.h>
#include <wii/os/OSError.h>
#include <wii/gx.h>
#include <wii/mtx.h>
#include <string>
#include <cstdio>
#include <limits>
#include <functional>
using namespace std;
namespace mod {
spm::mario::MarioWork * marioWork = spm::mario::marioGetPtr();
spm::spmario::SpmarioGlobals * globals = spm::spmario::gp;
spm::evtmgr::EvtEntry * eventEntry;
int bossSequence = 1;
int holee = 0;
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
  spm::npcdrv::NPCWork * NPCWork = spm::npcdrv::npcGetWorkPtr();
  int health = 0;
  s32 plotValue = globals->gsw0;
  //wii::os::OSReport("%x\n", spm::evtmgr_cmd::evtGetValue(eventEntry, LW(10)));
    if (plotValue == 0x21){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 270) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x66){
            bossSequence = 1;
              }
    if (plotValue == 0x67){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 315) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x72){
      globals->gsw0 = 0x73;
    }
    if (plotValue == 0xb8){
      globals->gsw0 = 0xb9;
    }
    if (plotValue == 0x73){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 286) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x7c){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 318) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0xaa){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 295) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0xab){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 296) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0xbe){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 271) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0xd4){
        bossSequence = 1;
    }
    if (plotValue == 0xd5){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 272) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0xda){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 319) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x112){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 282) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x11d){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 300) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x160){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 327) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x16c){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 273) {
        health = NPCWork->entries[i].hp;
      }
    }}
    /*if (plotValue == 0x17D){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 284 && holee == 0) {
        //wii::os::OSReport("%d\n", NPCWork->entries[i].m_Anim.m_nPoseId);
        holee = 1;
        double rate = 2;
        spm::animdrv::animPoseSetLocalTimeRate(NPCWork->entries[i].m_Anim.m_nPoseId, rate);
      }
    }}*/
    if (plotValue == 0x191){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 292) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x19a){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 305) {
        wii::os::OSReport("%d\n", NPCWork->entries[i].m_Anim.m_nPoseId);
        health = NPCWork->entries[i].hp;
      }
    }
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 307 && holee == 0) {
        holee = 1;
        for (int j = 0; j < 16; j++) {
        wii::os::OSReport("%x\n", NPCWork->entries[i].unitWork[j]);
      }
      //wii::os::OSReport("%x\n", NPCWork->entries[i]); wtf is this anyways
      }
    }}
    if (plotValue == 0x19c){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 309) {
        bossSequence = 3;
        health = 99999;
      }
    }}
    if (plotValue == 0x19f){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 309) {
        health = NPCWork->entries[i].hp;
      }
    }}
  return health;
}
static void bossHealth(spm::seqdrv::SeqWork *wp)
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
    seq_gameMainReal(wp);
}
static void bossActualHealth(spm::seqdrv::SeqWork *wp)
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
    seq_gameMainReal(wp);
}
void charmTextGenerator(spm::seqdrv::SeqWork *wp)
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
  seq_gameMainReal(wp);
}
void charmKillsTextGenerator(spm::seqdrv::SeqWork *wp)
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
  seq_gameMainReal(wp);
}
void charmNumText(spm::seqdrv::SeqWork *wp)
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
  seq_gameMainReal(wp);
}
void charmNumLeftText(spm::seqdrv::SeqWork *wp)
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
  seq_gameMainReal(wp);
}
void customUI(spm::seqdrv::SeqWork *wp)
{
  charmTextGenerator(wp);
  charmKillsTextGenerator(wp);
  charmNumText(wp);
  charmNumLeftText(wp);
  bossHealth(wp);
  bossActualHealth(wp);
}
static void titleScreenCustomTextPatch()
{
    seq_titleMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main;
    seq_gameMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME].main;
    spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main = &seq_titleMainOverride;
    spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME].main = &customUI;
}
static void setBossHP() {
  spm::npcdrv::npcTribes[270].maxHp = 15; //O'Chunks 1
  spm::npcdrv::npcTribes[315].maxHp = 10; //Bowser 1
  spm::npcdrv::npcTribes[286].maxHp = 12; //Dimentio 1
  spm::npcdrv::npcTribes[318].maxHp = 25; //Francis
  spm::npcdrv::npcTribes[295].maxHp = 16; //Mr. L
  spm::npcdrv::npcTribes[271].maxHp = 20; //O'Chunks 2
  spm::npcdrv::npcTribes[272].maxHp = 20; //O'Cabbage
  //spm::npcdrv::npcTribes[319].maxHp = 20; //King Croacus
  spm::npcdrv::npcTribes[282].maxHp = 15; //Mimi
  spm::npcdrv::npcTribes[300].maxHp = 36; //Brobot L-Type
  spm::npcdrv::npcTribes[316].maxHp = 12; //Bowser 2
  //spm::npcdrv::npcTribes[327].maxHp = 30; //Bonechill
  spm::npcdrv::npcTribes[273].maxHp = 100; //O'Chunks 3
  spm::npcdrv::npcTribes[292].maxHp = 160; //Dimentio 2
  spm::npcdrv::npcTribes[305].maxHp = 16; //Count Bleck
  spm::npcdrv::npcTribes[330].maxHp = 12; //Dark Mario
  spm::npcdrv::npcTribes[331].maxHp = 12; //Dark Luigi
  spm::npcdrv::npcTribes[332].maxHp = 12; //Dark Peach
  spm::npcdrv::npcTribes[333].maxHp = 12; //Dark Bowser
}
static void setBossXp() {
  /*for (int i = 0; i < 535; i++) {
    if (spm::npcdrv::npcTribes[i].killXp >= 3) {
     int newXp = spm::npcdrv::npcTribes[i].killXp / 3;
     spm::npcdrv::npcTribes[i].killXp = newXp;
    }
  }*/
}
/*
  Gives all bosses the megabite defense stat
*/
static void setBossDef() {
  spm::npcdrv::NPCDefense def;
  def.type = 0x0;
  def.defense = 0x2;
  def.flags = 0x2;
  spm::npcdrv::NPCDefense fireDef;
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
   spm::npcdrv::npcTribes[282].partsList[0].defenses[0] = def;//mimi defense
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
   /*for (int i = 0; i < 3; i++) {//Dimentio 2 defense
      spm::npcdrv::npcTribes[292].partsList[i].defenses[0] = def;
      spm::npcdrv::npcTribes[293].partsList[i].defenses[0] = def;
   }*/
      spm::npcdrv::npcTribes[306].partsList[0].defenses[0] = holeDef1;
      spm::npcdrv::npcTribes[306].partsList[0].defenses[1] = holeDef2;

   }
/*
    Function patching
*/
void (*marioTakeDamage)(wii::mtx::Vec3 * position, u32 flags, s32 damage);

s32 scriptTakeDamage(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
  spm::mario_pouch::MarioPouchWork * pouch = spm::mario_pouch::pouchGetPtr();
  pouch->hp = pouch->hp - 6;
  return firstRun;
}

int (*marioCalcDamageToEnemy)(s32 damageType, s32 tribeId);
void (*seq_gameMain)(spm::seqdrv::SeqWork *param_1);
void (*pouchAddXp)(int increase);
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

void patchAddXp() {
  pouchAddXp = patch::hookFunction(spm::mario_pouch::pouchAddXp,
    [](int increase)
            {
              increase = increase / 2;
                pouchAddXp(increase);
            }
        );
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
  return firstRun;
}

s32 unPauseGame(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
  spm::pausewin::pausewinUnpauseGame();
  spm::spmario::spmarioSystemLevel(0);
  return firstRun;
}

void patchItems() {
  for (int i = 0; i < 33; i++) {
if (spm::item_event_data::itemEventDataTable[i].itemId == 75) {
  //spm::item_event_data::itemEventDataTable[i].useScript = charmAdd;
}}
}
void patchMarioDamage(){
  marioTakeDamage = patch::hookFunction(spm::mario::marioTakeDamage,
    [](wii::mtx::Vec3 * position, u32 flags, s32 damage)
            {
              if (marioWork->character == 2) {
                damage = damage - 1;
                if (damage <= 0) {
                  damage = 1;
                }
              }
              //adds the rpg elements to boss fights
              int health = checkBossHealth();
              s32 plotValue = globals->gsw0;
              if (plotValue == 0x67 && bossSequence > 0){
                if (health <= 3 && bossSequence == 1){
                  //Bowser
              bossSequence -= 1;
              damage = 0;
              flags = 0x4;
              marioTakeDamage(position, flags, damage);
              spm::pausewin::pausewinPauseGame();
              spm::spmario::spmarioSystemLevel(1);
              for (int i = 0; i < 33; i++) {
            if (spm::item_event_data::itemEventDataTable[i].itemId == 68) {
            eventEntry = spm::evtmgr::evtEntryType(iceStorm, 0, 0, 0);
          }}} else {
            marioTakeDamage(position, flags, damage * 2);
          }} else if (plotValue == 0xd5 && bossSequence > 0){
            if (health <= 10 && bossSequence == 1){
              //O'Cabbage
          bossSequence -= 1;
          damage = 0;
          flags = 0x4;
          marioTakeDamage(position, flags, damage);
          spm::pausewin::pausewinPauseGame();
          spm::spmario::spmarioSystemLevel(1);
          for (int i = 0; i < 33; i++) {
        if (spm::item_event_data::itemEventDataTable[i].itemId == 68) {
        eventEntry = spm::evtmgr::evtEntryType(fireBurst, 0, 0, 0);
      }}} else {
        marioTakeDamage(position, flags, damage * 2);
      }} else if (plotValue == 0xd5 && bossSequence > 0){
        if (health <= 6 && bossSequence == 1){
          //King Croacus
      bossSequence -= 1;
      damage = 0;
      flags = 0x4;
      marioTakeDamage(position, flags, damage);
      spm::pausewin::pausewinPauseGame();
      spm::spmario::spmarioSystemLevel(1);
      for (int i = 0; i < 33; i++) {
    if (spm::item_event_data::itemEventDataTable[i].itemId == 68) {
    eventEntry = spm::evtmgr::evtEntryType(fireBurst, 0, 0, 0);
  }}} else {
    marioTakeDamage(position, flags, damage * 2);
  }} else if (plotValue == 0x19f && bossSequence > 0){
                if (health <= 150 && bossSequence == 3){
                  //Super Dimentio
              bossSequence -= 1;
              damage = 0;
              flags = 0x4;
              marioTakeDamage(position, flags, damage);
              spm::pausewin::pausewinPauseGame();
              spm::spmario::spmarioSystemLevel(1);
              for (int i = 0; i < 33; i++) {
            if (spm::item_event_data::itemEventDataTable[i].itemId == 68) {
            eventEntry = spm::evtmgr::evtEntryType(shootingStar, 0, 0, 0);
          }}} else if (health <= 100 && bossSequence == 2){
        bossSequence -= 1;
        damage = 0;
        flags = 0x4;
        marioTakeDamage(position, flags, damage);
        spm::pausewin::pausewinPauseGame();
        spm::spmario::spmarioSystemLevel(1);
        for (int i = 0; i < 33; i++) {
      if (spm::item_event_data::itemEventDataTable[i].itemId == 68) {
      eventEntry = spm::evtmgr::evtEntryType(fireBurst, 0, 0, 0);
    }}} else if (health <= 50 && bossSequence == 1){
    bossSequence -= 1;
    damage = 0;
    flags = 0x4;
    marioTakeDamage(position, flags, damage);
    spm::pausewin::pausewinPauseGame();
    spm::spmario::spmarioSystemLevel(1);
    for (int i = 0; i < 33; i++) {
    if (spm::item_event_data::itemEventDataTable[i].itemId == 68) {
    eventEntry = spm::evtmgr::evtEntryType(shootingStar, 0, 0, 0);
    }}} else {
      marioTakeDamage(position, flags, damage * 2);
    }
        }
            else {
              marioTakeDamage(position, flags, damage * 2);
            }
            }
        );
  marioCalcDamageToEnemy = patch::hookFunction(spm::mario::marioCalcDamageToEnemy,
    [](s32 damageType, s32 tribeId)
            {
              //spm::npcdrv::NPCWork * NPCWork = spm::npcdrv::npcGetWorkPtr();
              //wii::os::OSReport("%x\n", damageType);
              /*if (damageType == 1) {
                spm::npcdrv::MiscSetupDataV6 miscSetupData;
                s32 zero = 0;
                u8 one = 0;
                s32 test1 = 0x80a7cfc0;
                s32 test2 = 0x33c00000;
                miscSetupData.instanceId = zero;
                miscSetupData.gravityRotation = 0;
                for (int i = 0; i < 16; i++) {
                  miscSetupData.unitWork[i] = zero;
                }
                miscSetupData.unitWork[2] = test1;
                miscSetupData.unitWork[3] = test2;
                wii::mtx::Vec3 pos = marioWork->position;
                spm::npcdrv::NPCEntry * voidEntry = spm::npcdrv::npcEntryFromSetupEnemy(0, &pos, 198, &miscSetupData);
                voidEntry->partsList = spm::npcdrv::npcTribes[307].partsList;
              }*/
              if (damageType == 12) {
                //barry damage type
              }
              int damage = 0;
              switch(tribeId) {
                case 295:
                damage = 1;
                break;
                case 273:
                damage = 2;
                break;
                case 272:
                damage = 1;
                break;
                case 270:
                damage = 100;
                break;
                case 286:
                damage = 1;
                break;
                case 305:
                damage = 1;
                break;
                case 306:
                return 0;
                break;
                case 309:
                /*if (damageType == 1) {
                  for (int i = 0; i < 535; i++) {
                    if (NPCWork->entries[i].tribeId == 309) {
                      NPCWork->entries[i].hp = NPCWork->entries[i].hp - 1;
                    }
                  }
                  damage = 1;
                } else {return 1;}*/
                break;
                case 318:
                damage = 1;
                break;
                case 319:
                damage = 2;
                break;
                case 330:
                damage = 1;
                break;
                case 331:
                damage = 1;
                break;
                case 332:
                damage = 1;
                break;
                case 333:
                damage = 5;
                break;
              }
              if (damageType == 0xf) {
                if (damage == 0) {
                  damage = marioCalcDamageToEnemy(0, tribeId) + 2;
                }
                if (damage > 0) {
                  damage = damage + 1;
                }
              }
              if (damage > 0) return damage;
              if (tribeId > -1){
              if (spm::npcdrv::npcTribes[tribeId].catchCardItemId != 0){
              int cards = spm::mario_pouch::pouchGetCardCount(spm::npcdrv::npcTribes[tribeId].catchCardItemId);
              cards = cards + 1;
              return marioCalcDamageToEnemy(damageType, tribeId) / cards;
            }
          }
          return marioCalcDamageToEnemy(damageType, tribeId);
            }
        );
}

void patchVariables() {
    writeWord(&spm::mario::marioCalcDamageToEnemy, 0x16C, 0x57FF003E);
      writeWord(&spm::mario::marioCalcDamageToEnemy, 0xC8, 0x57FF003C);
       writeWord(&spm::mario::marioCalcDamageToEnemy, 0x94, 0x57FF003C);
        writeWord(&spm::others::superDimentioHeadDefenses, 0x0, 0x00000002);
          writeWord(&spm::others::superDimentioBodyDefenses, 0x0, 0x00000002);
}

/*
    General mod functions
*/
void main() {
  wii::os::OSReport("SPM Rel Loader: the mod has ran!\n");
  titleScreenCustomTextPatch();
  setBossHP();
  setBossXp();
  setBossDef();
  patchMarioDamage();
  patchItems();
  patchAddXp();
  patchVariables();
}

}
