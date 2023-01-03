#pragma once

#include <types.h>
#include <spm/evtmgr.h>

namespace spm::item_event_data {

#define ITEM_EVENT_DATA_COUNT 33

struct ItemEventData
{
    int itemId;
    const spm::evtmgr::EvtScriptCode * useEvtScript;
    const char * useMsg;
};
static_assert(sizeof(ItemEventData) == 0xc);

extern "C" {

extern char mightyTonicPointer;
extern ItemEventData itemEventDataTable[ITEM_EVENT_DATA_COUNT];
spm::evtmgr::EvtScriptCode * getItemUseEvt(s32 itemId);
EVT_DECLARE(eventUseAnimation)
EVT_DECLARE(eventFreezeGame)
EVT_DECLARE(eventUnfreezeGame)
EVT_DECLARE(evt_unknownScript_1)
EVT_DECLARE_USER_FUNC(drawBox, 1)
EVT_DECLARE_USER_FUNC(drawRedSparkles, 0)
EVT_DECLARE_USER_FUNC(removeBox, 1)
EVT_DECLARE_USER_FUNC(actionCommand, 2)
EVT_DECLARE_USER_FUNC(evt_unknownFunc_3, 2)
EVT_DECLARE_USER_FUNC(evt_unknownFunc_1, 1)
EVT_DECLARE_USER_FUNC(evt_unknownFunc_2, 3)
EVT_DECLARE_USER_FUNC(playSndEffect, 4)
EVT_DECLARE_USER_FUNC(evt_mario_flag8_onoff, 2)
EVT_DECLARE_USER_FUNC(addStatusEffect, 2)
EVT_DECLARE_USER_FUNC(postActionMessage, 1)
EVT_DECLARE_USER_FUNC(closeActionCommand, 1)
}

}
