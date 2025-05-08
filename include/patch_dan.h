#pragma once
#include <types.h>
#include <spm/npcdrv.h>
#include <spm/mario.h>
#include <spm/evt_npc.h>
#include <spm/npc_ninja.h>
#include <spm/evtmgr_cmd.h>
#include <wii/os.h>

namespace mod::patch_dan {

EVT_DECLARE(reset_piccolo)
EVT_DECLARE_USER_FUNC(resetPiccolo, 0)

void patch_wracktail_main();
void patch_dan_main();

}
