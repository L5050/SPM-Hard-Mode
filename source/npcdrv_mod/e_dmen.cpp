#include "e_dmen.h"
#include "evtpatch.h"
#include "patch.h"
#include "mod.h"
#include "evt_cmd.h"

#include <types.h>
#include <spm/evt_cam.h>
#include <spm/evt_sub.h>
#include <spm/evt_door.h>
#include <spm/evt_eff.h>
#include <spm/evt_fade.h>
#include <spm/evt_fairy.h>
#include <spm/evt_item.h>
#include <spm/evt_guide.h>
#include <spm/evt_mario.h>
#include <spm/evt_pouch.h>
#include <spm/evt_sub.h>
#include <spm/evt_mobj.h>
#include <spm/evt_msg.h>
#include <spm/evt_npc.h>
#include <spm/memory.h>
#include <spm/evt_env.h>
#include <spm/evt_seq.h>
#include <spm/evt_snd.h>
#include <spm/pausewin.h>
#include <spm/animdrv.h>
#include <spm/wpadmgr.h>
#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <spm/mario.h>
#include <spm/map_data.h>
#include <spm/evtmgr_cmd.h>
#include <spm/npcdrv.h>
#include <spm/mario.h>
#include <spm/evt_npc.h>
#include <spm/npc_ninja.h>
#include <spm/npc_dimeen_l.h>
#include <spm/evtmgr_cmd.h>
#include <wii/os.h>

using namespace spm;
using namespace npcdrv;
using namespace evt_npc;

namespace mod {

EVT_BEGIN(changeFlyingSpeed)
  USER_FUNC(spm::evt_npc::evt_npc_glide_to, PTR("me"), LW(5), LW(6), LW(7), 0, FLOAT(800.0), 0, 5, 0, 0)
RETURN_FROM_CALL()

EVT_BEGIN(runSaveScript)
  RUN_CHILD_EVT(evt_memcard_save)
RETURN_FROM_CALL()

EVT_BEGIN(makePeachDie)
  USER_FUNC(spm::npc_dimeen_l::npc_dimeen_l_box_deal_damage, 0, 1, LW(3), LW(0), LW(1), LW(2))
RETURN_FROM_CALL()

static void hookDimentioScripts()
{
  spm::evtmgr::EvtScriptCode* dimentioOnSpawn = spm::npcdrv::npcEnemyTemplates[142].onSpawnScript;
  spm::evtmgr::EvtScriptCode* part1 = getInstructionEvtArg(dimentioOnSpawn, 4, 3);
  spm::evtmgr::EvtScriptCode* part2 = getInstructionEvtArg(part1, 7, 0);
  spm::evtmgr::EvtScriptCode* dimentioBattleLink = getInstructionEvtArg(part2, 3, 0);
  evtpatch::hookEvtReplace(dimentioBattleLink, 6, (spm::evtmgr::EvtScriptCode*)changeFlyingSpeed);
  spm::evtmgr::EvtScriptCode* mainLogic = getInstructionEvtArg(dimentioOnSpawn, 5, 3);
  spm::evtmgr::EvtScriptCode* shootScript = getInstructionEvtArg(mainLogic, 54, 0);
  spm::evtmgr::EvtScriptCode* boxScript = getInstructionEvtArg(mainLogic, 65, 0);
  //spm::evtmgr::EvtScriptCode* boxScriptMain = getInstructionEvtArg(boxScript, 23, 0);
  #ifdef SPM_US0
  spm::evtmgr::EvtScriptCode* boxScriptMain = getInstructionEvtArg(boxScript, 18, 0);
  evtpatch::hookEvtReplace(boxScriptMain, 233, (spm::evtmgr::EvtScriptCode*)makePeachDie);
  #endif
  // nothing else needs to be done on US2

  #ifdef SPM_EU0
  //PAL has a different script offset
  spm::evtmgr::EvtScriptCode* boxScriptMain = getInstructionEvtArg(boxScript, 23, 0);
  evtpatch::hookEvtReplace(boxScriptMain, 238, (spm::evtmgr::EvtScriptCode*)makePeachDie);
  #endif
  
  evtpatch::hookEvtReplace(shootScript, 32, (spm::evtmgr::EvtScriptCode*)turnNull);
  spm::map_data::MapData * ls3_12_md = spm::map_data::mapDataPtr("ls3_12");
  spm::evtmgr::EvtScriptCode* postChase = getInstructionEvtArg(ls3_12_md->initScript, 77, 0);
  evtpatch::hookEvt(postChase, 26, (spm::evtmgr::EvtScriptCode*)runSaveScript);
  writeWord(&spm::npc_dimeen_l::npc_dimeen_l_box_deal_damage, 0x88, 0x38600000);
}

void e_dmenMain() {
  hookDimentioScripts();
}

}
