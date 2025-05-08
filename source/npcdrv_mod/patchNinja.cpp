#include "patchNinja.h"
#include "evtpatch.h"
#include "patch.h"

#include <types.h>
#include <spm/npcdrv.h>
#include <spm/mario.h>
#include <spm/evt_npc.h>
#include <spm/npc_ninja.h>
#include <spm/evtmgr_cmd.h>
#include <wii/os.h>

using namespace spm::npcdrv;

namespace mod {

  spm::npcdrv::NPCEntryUnkDef ninjaUnkdefOverride[3];

    static s32 ninjaBombLogic(spm::mario::MarioWork *marioWork, NPCPart *npcPart, s32 lastAttackedDefenseType, s32 defenseType, s32 power, u8 param_6) {
      NPCEntry *npcEntry = npcPart->owner;
      if (defenseType == 29) { //barry nerf
        return 1;
      }
      if (npcEntry->moveMode != 1) {
        return 4;
      }
      if (npcEntry->unitWork[7] != 0) {
        return 3;
      }
      if (npcEntry->unitWork[15] > 0 && (u32)power >= npcEntry->hp) {
        npcEntry->unitWork[15] -= 1;
        npcEntry->unitWork[7] = 1;
        npcEntry->flag8 = npcEntry->flag8 & 0xfffff7ff;
        npcEntry->flags_398 = npcEntry->flags_398 | 1;
        return 1;
      }
      return 4;
    }

    s32 ninjoe_thoreau_override(spm::npcdrv::NPCEntry *npcEntry, int param_2, int *grabbedNpc, int *grabbedNpcPart)
    {
      return 2;
    }

  EVT_BEGIN(patch_ninjoe) // Set the maximum number of feigns in one life
    USER_FUNC(spm::evt_npc::evt_npc_get_property, PTR("me"), 13, LW(0))
    SWITCH(LW(0))
      CASE_EQUAL(211)
        USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 15, 1)
      SWITCH_BREAK()
      CASE_EQUAL(214)
        USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 15, 1)
      SWITCH_BREAK()
      CASE_EQUAL(217) //Ninjerry and Dark Ninjoe get one extra feign
        USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 15, 2)
      SWITCH_BREAK()
      CASE_EQUAL(220) //Ninjerry and Dark Ninjoe get one extra feign
        USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 15, 2)
      SWITCH_BREAK()
    END_SWITCH()
  RETURN_FROM_CALL()

void ninjaPatch()
{
    //writeWord(&spm::npc_ninja::ninjoe_bomb_calc_chance, 0x16C, 0x57FF003E);
    //writeBranch(&spm::npc_ninja::ninjoe_bomb_calc_chance, 0x50, branchToNewLogic);
    evtpatch::hookEvt(spm::npcdrv::npcEnemyTemplates[178].onSpawnScript, 1, (spm::evtmgr::EvtScriptCode*)patch_ninjoe);
    evtpatch::hookEvt(spm::npcdrv::npcEnemyTemplates[259].onSpawnScript, 1, (spm::evtmgr::EvtScriptCode*)patch_ninjoe);
    evtpatch::hookEvt(spm::npcdrv::npcEnemyTemplates[388].onSpawnScript, 1, (spm::evtmgr::EvtScriptCode*)patch_ninjoe);
    patch::hookFunction(spm::npc_ninja::ninjoe_bomb_calc_chance, ninjaBombLogic);
    patch::hookFunction(spm::npc_ninja::ninjoe_thoreau_override, ninjoe_thoreau_override);
    spm::npcdrv::npcTribes[211].coinDropExtraMax = 10;
    spm::npcdrv::npcTribes[211].coinDropExtraChance = 100;
    spm::npcdrv::npcTribes[214].coinDropExtraMax = 15;
    spm::npcdrv::npcTribes[214].coinDropExtraChance = 100;
    spm::npcdrv::npcTribes[217].coinDropExtraMax = 20;
    spm::npcdrv::npcTribes[217].coinDropExtraChance = 100;
}

}
