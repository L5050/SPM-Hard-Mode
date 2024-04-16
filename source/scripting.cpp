#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"

#include <spm/evt_ac.h>
#include <spm/evtmgr.h>
#include <spm/evt_msg.h>
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
#include <patch.h>
#include <string>
#include <cstdio>
using namespace std;
namespace mod {

EVT_DECLARE_USER_FUNC(itemCharm, 0)
EVT_DECLARE_USER_FUNC(unPauseGame, 0)
EVT_DECLARE_USER_FUNC(scriptTakeDamage, 0)

EVT_BEGIN(charmAdd)
SET(LW(0), 1)/*
RUN_CHILD_EVT(spm::item_event_data::eventUseAnimation)
RUN_CHILD_EVT(spm::item_event_data::eventFreezeGame)
//USER_FUNC(spm::evt_ac::evt_ac_entry, PTR("ac"), 15)
//USER_FUNC(spm::evt_ac::evt_ac_return_results, PTR("ac"), LW(10))
//USER_FUNC(spm::evt_ac::evt_ac_delete, PTR("ac"))
RUN_CHILD_EVT(spm::item_event_data::evt_unknownScript_1)*/
USER_FUNC(itemCharm)/*
USER_FUNC(spm::item_event_data::drawBox, -1)
USER_FUNC(spm::item_event_data::evt_unknownFunc_1, -1)
USER_FUNC(spm::item_event_data::evt_unknownFunc_2, LW(0), LW(1), LW(2))
USER_FUNC(spm::item_event_data::playSndEffect, PTR("SFX_I_TUYOTUYO1"), LW(0), LW(1), LW(2))
USER_FUNC(spm::item_event_data::drawRedSparkles)
WAIT_MSEC(1500)
USER_FUNC(spm::item_event_data::removeBox, -1)
USER_FUNC(spm::item_event_data::addStatusEffect, 64, 30)
USER_FUNC(spm::item_event_data::evt_mario_flag8_onoff, 1, 131072)
USER_FUNC(spm::item_event_data::postActionMessage, 75)
RUN_CHILD_EVT(spm::item_event_data::eventUnfreezeGame)
USER_FUNC(spm::item_event_data::evt_mario_flag8_onoff, 0, 131072)*/
RETURN()
EVT_END()

EVT_BEGIN(iceStorm)
USER_FUNC(spm::evt_msg::evt_msg_print, 0x1, PTR("When a boss reaches a certain\nhealth threshhold and you take\ndamage <wait 100>"), 0, PTR("me"))
USER_FUNC(spm::evt_msg::evt_msg_print, 0x1, PTR("Press inputs on the screen \ncorrectly to avoid the hit\nand get something in return! <wait 200>"), 0, PTR("me"))
USER_FUNC(spm::evt_ac::evt_ac_entry, PTR("ac"), 0)
USER_FUNC(spm::evt_ac::evt_ac_return_results, PTR("ac"), LW(10))
USER_FUNC(spm::evt_ac::evt_ac_delete, PTR("ac"))
IF_EQUAL(LW(10), 7)
RUN_CHILD_EVT(spm::item_event_data::itemEventDataTable[1].useScript)
USER_FUNC(unPauseGame)
RETURN()
ELSE()
USER_FUNC(unPauseGame)
USER_FUNC(scriptTakeDamage)
RETURN()
EVT_END()

EVT_BEGIN(fireBurst)
USER_FUNC(spm::evt_ac::evt_ac_entry, PTR("ac"), 0)
USER_FUNC(spm::evt_ac::evt_ac_return_results, PTR("ac"), LW(13))
USER_FUNC(spm::evt_ac::evt_ac_delete, PTR("ac"))
IF_EQUAL(LW(13), 7)
RUN_CHILD_EVT(spm::item_event_data::itemEventDataTable[2].useScript)
USER_FUNC(unPauseGame)
RETURN()
ELSE()
USER_FUNC(unPauseGame)
USER_FUNC(scriptTakeDamage)
RETURN()
EVT_END()

EVT_BEGIN(shootingStar)
SET(LW(10), 0)
USER_FUNC(spm::evt_ac::evt_ac_entry, PTR("ac"), 1)
USER_FUNC(spm::evt_ac::evt_ac_return_results, PTR("ac"), LW(10))
USER_FUNC(spm::evt_ac::evt_ac_delete, PTR("ac"))
IF_EQUAL(LW(10), 7)
RUN_CHILD_EVT(spm::item_event_data::itemEventDataTable[3].useScript)
USER_FUNC(unPauseGame)
RETURN()
ELSE()
USER_FUNC(unPauseGame)
USER_FUNC(scriptTakeDamage)
RETURN()
EVT_END()
}
