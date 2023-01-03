#include "mod.h"
#include "patch.h"
#include "scripting.cpp"

#include <spm/wpadmgr.h>
#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <spm/seq_game.h>
#include <spm/npcdrv.h>
#include <spm/mario.h>
#include <spm/mario_pouch.h>
#include <spm/seqdef.h>
#include <spm/item_data.h>
#include <spm/item_event_data.h>
#include <wii/OSError.h>
#include <patch.h>
#include <string>
#include <cstdio>
using namespace std;
namespace mod {

/*
    Title Screen Custom Text
    Prints "SPM Hard Mode" at the top of the title screen
*/

static spm::seqdef::SeqFunc *seq_titleMainReal;
static spm::seqdef::SeqFunc *seq_gameMainReal;
static void seq_titleMainOverride(spm::seqdrv::SeqWork *wp)
{
    wii::RGBA green {0, 255, 0, 255};
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
  spm::mario_pouch::MarioPouchWork* pouch = spm::mario_pouch::pouchGetPtr();
  int charms = pouch->charmsRemaining;
  if (charms > 0) {
    return charms;
  } else {
    return 0;
  }
}
void charmTextGenerator(spm::seqdrv::SeqWork *wp)
{
  int charmStats = checkCharmNum();
  if (charmStats > 0){
  wii::RGBA red {255, 0, 0, 255};
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
  wii::RGBA red {255, 0, 0, 255};
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
  wii::RGBA red {255, 0, 0, 255};
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
  wii::RGBA red {255, 0, 0, 255};
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
void merleeTextGenerator(spm::seqdrv::SeqWork *wp)
{
  charmTextGenerator(wp);
  charmKillsTextGenerator(wp);
  charmNumText(wp);
  charmNumLeftText(wp);
}
static void titleScreenCustomTextPatch()
{
    seq_titleMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main;
    seq_gameMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME].main;
    spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main = &seq_titleMainOverride;
    spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME].main = &merleeTextGenerator;
}
static void setBossHP() {
  spm::npcdrv::npcTribes[270].maxHp = 15; //O'Chunks 1
  spm::npcdrv::npcTribes[315].maxHp = 6; //Bowser 1
  spm::npcdrv::npcTribes[286].maxHp = 12; //Dimentio 1
  spm::npcdrv::npcTribes[318].maxHp = 100; //Francis
  spm::npcdrv::npcTribes[295].maxHp = 16; //Mr. L
  spm::npcdrv::npcTribes[271].maxHp = 20; //O'Chunks 2
  spm::npcdrv::npcTribes[272].maxHp = 15; //O'Cabbage
  //spm::npcdrv::npcTribes[319].maxHp = 20; //King Croacus
  spm::npcdrv::npcTribes[282].maxHp = 15; //Mimi
  spm::npcdrv::npcTribes[300].maxHp = 16; //Brobot L-Type
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
  /*for (int i = 0; i < 7; i++) {//o'chunks 1 defense
    if (spm::npcdrv::npcTribes[270].parts[i].id == 1) {
     spm::npcdrv::npcTribes[270].parts[i].defenses[0] = chunkDef;
    }
  }*/
  for (int i = 0; i < 2; i++) {//bowser 1 defense
    if (spm::npcdrv::npcTribes[315].parts[i].id == 1) {
     spm::npcdrv::npcTribes[315].parts[i].defenses[0] = def;
    }
  }
  for (int i = 0; i < 3; i++) {//Dimentio 1 defense
    if (spm::npcdrv::npcTribes[286].parts[i].id == 1) {
     spm::npcdrv::npcTribes[286].parts[i].defenses[0] = fireDef;
     spm::npcdrv::npcTribes[286].parts[i].defenses[5] = def;
    }
  }
   for (int i = 0; i < 7; i++) {//Brobot defense
      spm::npcdrv::npcTribes[296].parts[i].defenses[0] = def;
   }
   for (int i = 0; i < 7; i++) {//o'chunks 2 defense
     if (spm::npcdrv::npcTribes[271].parts[i].id == 1) {
      spm::npcdrv::npcTribes[271].parts[i].defenses[0] = fireDef;
      spm::npcdrv::npcTribes[271].parts[i].defenses[5] = def;
     }
   }
   for (int i = 0; i < 3; i++) {//king croacus defense
     if (spm::npcdrv::npcTribes[319].parts[i].id == 2) {
      spm::npcdrv::npcTribes[319].parts[i].defenses[0] = fireDef;
      spm::npcdrv::npcTribes[319].parts[i].defenses[5] = def;
     }
   }
   spm::npcdrv::npcTribes[282].parts[0].defenses[0] = def;//mimi defense
   for (int i = 0; i < 15; i++) {//Brobot L-Type defense
      spm::npcdrv::npcTribes[300].parts[i].defenses[0] = def;
   }
   for (int i = 0; i < 2; i++) {//bowser 2 defense
     if (spm::npcdrv::npcTribes[316].parts[i].id == 1) {
      spm::npcdrv::npcTribes[316].parts[i].defenses[0] = def;
     }
   }
   for (int i = 0; i < 9; i++) {//underchomp defense
     if (spm::npcdrv::npcTribes[316].parts[i].id == 1) {
      spm::npcdrv::npcTribes[316].parts[i].defenses[0] = def;
     }
   }
   for (int i = 0; i < 21; i++) {//bonechill defense
     if (spm::npcdrv::npcTribes[327].parts[i].id == 2) {
      spm::npcdrv::npcTribes[327].parts[i].defenses[2] = def;
     }
     if (spm::npcdrv::npcTribes[327].parts[i].id == 3) {
      spm::npcdrv::npcTribes[327].parts[i].defenses[2] = def;
     }
     if (spm::npcdrv::npcTribes[327].parts[i].id == 4) {
      spm::npcdrv::npcTribes[327].parts[i].defenses[2] = def;
     }
   }
   for (int i = 0; i < 3; i++) {//Dimentio 2 defense
      spm::npcdrv::npcTribes[292].parts[i].defenses[0] = def;
      spm::npcdrv::npcTribes[293].parts[i].defenses[0] = def;
   }
   for (int i = 0; i < 2; i++) {//Dark Mario defense
     if (spm::npcdrv::npcTribes[330].parts[i].id == 1) {
      spm::npcdrv::npcTribes[330].parts[i].defenses[0] = def;
     }
   }
   //Dark Peach and Luigi defense
   spm::npcdrv::npcTribes[331].parts[0].defenses[0] = def;
   spm::npcdrv::npcTribes[332].parts[0].defenses[0] = def;
   for (int i = 0; i < 2; i++) {//Dark bowser defense
     if (spm::npcdrv::npcTribes[333].parts[i].id == 1) {
      spm::npcdrv::npcTribes[333].parts[i].defenses[0] = def;
     }
   }

   }
/*
    Function patching
*/
void (*marioTakeDamage)(wii::Vec3 * position, u32 flags, s32 damage);
int (*marioCalcDamageToEnemy)(s32 damageType, s32 tribeId);
void (*seq_gameMain)(spm::seqdrv::SeqWork *param_1);
void (*pouchAddXp)(int increase);
spm::evtmgr::EvtEntry * (*evtEntry)(const spm::evtmgr::EvtScriptCode * script, u8 priority, u8 flags);
spm::evtmgr::EvtEntry * eTest(const spm::evtmgr::EvtScriptCode * script, u8 priority, u8 flags)
        {
            if (spm::item_event_data::getItemUseEvt(104) == script){
              spm::mario_pouch::MarioPouchWork* pouch = spm::mario_pouch::pouchGetPtr();
              pouch->killsBeforeNextCharm = pouch->killsBeforeNextCharm / 2;
            }
            return evtEntry(script, priority, flags);
        }
void patchGameMain() {
  seq_gameMain = patch::hookFunction(spm::seq_game::seq_gameMain,
    [](spm::seqdrv::SeqWork *param_1)
            {
                seq_gameMain(param_1);
            }
        );
}

void patchScripts() {
  evtEntry = patch::hookFunction(spm::evtmgr::evtEntry, eTest);
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

s32 itemCharm(spm::evtmgr::EvtEntry * evt, bool firstRun) {
  spm::mario_pouch::MarioPouchWork* pouch = spm::mario_pouch::pouchGetPtr();
  if (pouch->killsBeforeNextCharm > 5) {
    pouch->killsBeforeNextCharm = pouch->killsBeforeNextCharm - 5;
    pouch->charmsRemaining = pouch->charmsRemaining + 1;
  } else if (pouch->killsBeforeNextCharm < 6) {
    pouch->killsBeforeNextCharm = 5;
    pouch->charmsRemaining = pouch->charmsRemaining + 1;
  }
  return firstRun;
}

void patchItems() {
  for (int i = 0; i < 33; i++) {
if (spm::item_event_data::itemEventDataTable[i].itemId == 75) {
  spm::item_event_data::itemEventDataTable[i].useEvtScript = charmAdd;
}
if (spm::item_event_data::itemEventDataTable[i].itemId == 68) {
  spm::item_event_data::itemEventDataTable[i].useEvtScript = charmAdd;
}}
}
void patchMarioDamage(){
  marioTakeDamage = patch::hookFunction(spm::mario::marioTakeDamage,
    [](wii::Vec3 * position, u32 flags, s32 damage)
            {
                marioTakeDamage(position, flags, damage * 2);
            }
        );
  marioCalcDamageToEnemy = patch::hookFunction(spm::mario::marioCalcDamageToEnemy,
    [](s32 damageType, s32 tribeId)
            {
              int damage = 0;
              switch(tribeId) {
                case 295:
                damage = 1;
                break;
                case 273:
                damage = 1;
                break;
                case 272:
                damage = 1;
                break;
                case 270:
                damage = 100;
                break;
                case 305:
                damage = 1;
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
                default:
                damage = marioCalcDamageToEnemy(damageType, tribeId);
              if (spm::mario_pouch::pouchGetCardCount(spm::npcdrv::npcTribes[tribeId].catchCardItemId) > 0) {
                int cards = spm::mario_pouch::pouchGetCardCount(spm::npcdrv::npcTribes[tribeId].catchCardItemId);
                cards = cards + 1;
                damage = damage / cards;
              }
                              }
                return damage;
            }
        );
}
/*
    General mod functions
*/
void main() {
  wii::OSError::OSReport("SPM Rel Loader: the mod has ran!\n");
  titleScreenCustomTextPatch();
  setBossHP();
  setBossXp();
  setBossDef();
  patchMarioDamage();
  patchItems();
  patchAddXp();
  patchScripts();
  //patchGameMain();
}

}
