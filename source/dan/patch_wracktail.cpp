#include "patch_dan.h"
#include "evtpatch.h"
#include "patch.h"
#include "evt_cmd.h"
#include "mod.h"

#include <types.h>
#include <spm/npcdrv.h>
#include <spm/rel/dan.h>
#include <spm/mario.h>
#include <spm/spmario.h>
#include <spm/mario_pouch.h>
#include <spm/mario_motion.h>
#include <spm/evt_door.h>
#include <spm/evt_mario.h>
#include <spm/evt_npc.h>
#include <spm/evt_snd.h>
#include <spm/evt_sub.h>
#include <spm/evt_msg.h>
#include <spm/evt_eff.h>
#include <spm/evt_mobj.h>
#include <spm/seq_title.h>
#include <spm/evt_case.h>
#include <spm/evt_pouch.h>
#include <spm/map_data.h>
#include <spm/npc_ninja.h>
#include <spm/evtmgr_cmd.h>
#include <wii/os.h>

using namespace spm::npcdrv;
using namespace spm::evt_npc;

namespace mod::patch_dan {

EVT_BEGIN(wrackle_load_async)
    USER_FUNC(evt_npc_tribe_agb_async, 81)
RETURN_FROM_CALL()

EVT_BEGIN(wrackle_transform)
    USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("spm_explosion"), 0, LW(0), LW(1), LW(2), FLOAT(0.5), 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(evt_npc_entry_from_template, 0, 81, LW(0), LW(1), LW(2), LW(10), EVT_NULLPTR)
    USER_FUNC(evt_npc_delete, PTR("me"))
RETURN()
EVT_END()

void patch_wracktail_main()
{
    wii::os::OSReport("Wrackle unk7: %p\n", spm::npcdrv::npcEnemyTemplates[420].unkScript7);
    #ifdef SPM_US2
    evtpatch::hookEvtReplace(spm::npcdrv::npcEnemyTemplates[420].unkScript7, 20, wrackle_transform);
    #else
    evtpatch::hookEvtReplace(spm::npcdrv::npcEnemyTemplates[420].unkScript7, 32, wrackle_transform);
    #endif
    evtpatch::hookEvt(spm::dan::dan_wracktail_main_evt, 8, wrackle_load_async);
}

}