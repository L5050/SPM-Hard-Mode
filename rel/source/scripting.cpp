#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"

#include <spm/evtmgr.h>
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

EVT_DECLARE_USER_FUNC(itemCharm, 0)
EVT_BEGIN(charmAdd)
SET(LW(0), 1)
RUN_CHILD_EVT(spm::item_event_data::eventUseAnimation)
RUN_CHILD_EVT(spm::item_event_data::eventFreezeGame)
USER_FUNC(spm::item_event_data::verticalShakeActionCommand, PTR("ac"), 15)
USER_FUNC(spm::item_event_data::evt_unknownScript_4, PTR("ac"), LW(10))
USER_FUNC(spm::item_event_data::closeVerticleActionCommand, PTR("ac"))
RUN_CHILD_EVT(spm::item_event_data::evt_unknownScript_1)
USER_FUNC(itemCharm)
USER_FUNC(spm::item_event_data::drawBox, -1)
USER_FUNC(spm::item_event_data::evt_unknownScript_2, -1)
USER_FUNC(spm::item_event_data::evt_unknownScript_3, LW(0), LW(1), LW(2))
USER_FUNC(spm::item_event_data::playSndEffect, PTR("SFX_I_TUYOTUYO1"), LW(0), LW(1), LW(2))
USER_FUNC(spm::item_event_data::drawRedSparkles)
WAIT_MSEC(1500)
USER_FUNC(spm::item_event_data::removeBox, -1)
USER_FUNC(spm::item_event_data::addStatusEffect, 64, LW(10))
USER_FUNC(spm::item_event_data::evt_mario_flag8_onoff, 1, 131072)
USER_FUNC(spm::item_event_data::postActionMessage, 75)
RUN_CHILD_EVT(spm::item_event_data::eventUnfreezeGame)
USER_FUNC(spm::item_event_data::evt_mario_flag8_onoff, 0, 131072)
RETURN()
EVT_END()

}
