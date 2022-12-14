#include "mod.h"
#include "patch.h"

#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <spm/npcdrv.h>
#include <spm/seqdef.h>
#include <wii/OSError.h>

namespace mod {

/*
    Title Screen Custom Text
    Prints "SPM Rel Loader" at the top of the title screen
*/

static spm::seqdef::SeqFunc *seq_titleMainReal;
static void seq_titleMainOverride(spm::seqdrv::SeqWork *wp)
{
    wii::RGBA green {0, 255, 0, 255};
    f32 scale = 0.8f;
    const char * msg = "SPM Rel Loader";
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
  spm::npcdrv::npcTribes[305].maxHp = 16; //Count Bleck
  spm::npcdrv::npcTribes[309].maxHp = 40; //Super Dimentio
}

/*
    General mod functions
*/

void main() {
  wii::OSError::OSReport("SPM Rel Loader: the mod has ran!\n");
  titleScreenCustomTextPatch();
  setBossHP();
  spm::npcdrv::NPCDefense def;
  def.type = 0x0;
  def.defense = 0x0;
  def.flags = 0x2;
  for (int i = 0; i < 2; i++) {
    if (spm::npcdrv::npcTribes[305].parts[i].id == 1) {
          spm::npcdrv::npcTribes[305].parts[0].defenses[0] = def;
    }
  }
}

}
