#pragma once

#include <common.h>
#include <evt_cmd.h>
#include <spm/evtmgr.h>
#include <spm/npcdrv.h>

CPP_WRAPPER(spm::npc_ninja)

extern "C"{

s32 ninjoe_bomb_calc_chance(spm::mario::MarioWork *marioWork, spm::npcdrv::NPCPart *npcPart, s32 lastAttackedDefenseType, s32 defenseType, s32 power, u8 param_6);
s32 ninjoe_thoreau_override(spm::npcdrv::NPCEntry *npcEntry, int param_2, int *grabbedNpc, int *grabbedNpcPart);

EVT_DECLARE_USER_FUNC(bomb_damage_mario, 0)


}
CPP_WRAPPER_END()
