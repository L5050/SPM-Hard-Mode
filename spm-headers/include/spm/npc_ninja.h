#pragma once

#include <common.h>
#include <spm/npcdrv.h>

CPP_WRAPPER(spm::npc_ninja)

extern "C"{

s32 ninjoe_bomb_calc_chance(spm::mario::MarioWork *marioWork, spm::npcdrv::NPCPart *npcPart, s32 lastAttackedDefenseType, s32 defenseType, s32 power, u8 param_6);
s32 ninjoe_thoreau_override(spm::npcdrv::NPCEntry *npcEntry, int param_2, int *grabbedNpc, int *grabbedNpcPart);


}
CPP_WRAPPER_END()
