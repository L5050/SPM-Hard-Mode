#include "mod.h"
#include "patch.h"

#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <spm/npcdrv.h>
#include <spm/mario.h>
#include <spm/mario_pouch.h>
#include <spm/seqdef.h>
#include <wii/OSError.h>
#include <patch.h>
namespace mod {

/*
    Title Screen Custom Text
    Prints "SPM Hard Mode" at the top of the title screen
*/

static spm::seqdef::SeqFunc *seq_titleMainReal;
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
static void titleScreenCustomTextPatch()
{
    seq_titleMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main;
    spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main = &seq_titleMainOverride;
}
static void setBossHP() {
  spm::npcdrv::npcTribes[270].maxHp = 15; //O'Chunks 1
  spm::npcdrv::npcTribes[315].maxHp = 6; //Bowser 1
  spm::npcdrv::npcTribes[286].maxHp = 12; //Dimentio 1
  spm::npcdrv::npcTribes[318].maxHp = 100; //Francis
  spm::npcdrv::npcTribes[295].maxHp = 120; //Mr. L
  spm::npcdrv::npcTribes[271].maxHp = 20; //O'Chunks 2
  spm::npcdrv::npcTribes[272].maxHp = 100; //O'Cabbage
  spm::npcdrv::npcTribes[319].maxHp = 20; //King Croacus
  spm::npcdrv::npcTribes[282].maxHp = 10; //Mimi
  spm::npcdrv::npcTribes[300].maxHp = 16; //Brobot L-Type
  spm::npcdrv::npcTribes[316].maxHp = 12; //Bowser 2
  //spm::npcdrv::npcTribes[327].maxHp = 30; //Bonechill
  spm::npcdrv::npcTribes[273].maxHp = 200; //O'Chunks 3
  spm::npcdrv::npcTribes[292].maxHp = 160; //Dimentio 2
  spm::npcdrv::npcTribes[305].maxHp = 16; //Count Bleck
  spm::npcdrv::npcTribes[330].maxHp = 12; //Dark Mario
  spm::npcdrv::npcTribes[331].maxHp = 12; //Dark Luigi
  spm::npcdrv::npcTribes[332].maxHp = 12; //Dark Peach
  spm::npcdrv::npcTribes[333].maxHp = 12; //Dark Bowser
}
static void setBossXp() {
  for (int i = 0; i < 535; i++) {
    if (spm::npcdrv::npcTribes[i].killXp >= 3) {
     int newXp = spm::npcdrv::npcTribes[i].killXp / 3;
     spm::npcdrv::npcTribes[i].killXp = newXp;
    }
  }
}
/*
  Gives all bosses the megabite defense stat
*/
static void setBossDef() {
  spm::npcdrv::NPCDefense def;
  def.type = 0x0;
  def.defense = 0x2;
  def.flags = 0x2;
  spm::npcdrv::NPCDefense chunkDef;
  chunkDef.type = 0x0;
  chunkDef.defense = 0x0;
  chunkDef.flags = 0x2;
  spm::npcdrv::NPCDefense fireDef;
  fireDef.type = 0xA;
  fireDef.defense = 0x64;
  fireDef.flags = 0x0;
  for (int i = 0; i < 7; i++) {//o'chunks 1 defense
    if (spm::npcdrv::npcTribes[270].parts[i].id == 1) {
     spm::npcdrv::npcTribes[270].parts[i].defenses[0] = fireDef;
     spm::npcdrv::npcTribes[270].parts[i].defenses[5] = chunkDef;
    }
  }
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
   /*for (int i = 0; i < 7; i++) {//o'cabbage defense
    spm::npcdrv::npcTribes[272].parts[i].defenses[0] = fireDef;
    spm::npcdrv::npcTribes[272].parts[i].defenses[1] = def;
  }*/
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
//static void newMarioTakeDamage() {

//}
/*
    General mod functions
*/

void main() {
  wii::OSError::OSReport("SPM Rel Loader: the mod has ran!\n");
  titleScreenCustomTextPatch();
  setBossHP();
  setBossXp();
  setBossDef();
}

}
