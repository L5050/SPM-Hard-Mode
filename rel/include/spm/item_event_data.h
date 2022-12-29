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

extern s32 mightyTonicPointer;

extern ItemEventData itemEventDataTable[ITEM_EVENT_DATA_COUNT];
EVT_DECLARE(eventUseAnimation)
EVT_DECLARE(eventFreezeGame)
EVT_DECLARE(eventUnfreezeGame)
EVT_DECLARE(evt_unknownScript_1)
EVT_DECLARE_USER_FUNC(drawBox, 1)
EVT_DECLARE_USER_FUNC(drawRedSparkles, 0)
EVT_DECLARE_USER_FUNC(removeBox, 1)
EVT_DECLARE_USER_FUNC(verticalShakeActionCommand, 2)
EVT_DECLARE_USER_FUNC(evt_unknownScript_6, 2)
EVT_DECLARE_USER_FUNC(evt_unknownScript_2, 1)
EVT_DECLARE_USER_FUNC(evt_unknownScript_3, 3)
EVT_DECLARE_USER_FUNC(evt_mario_flag8_onoff, 2)
EVT_DECLARE_USER_FUNC(redHaze, 2)
EVT_DECLARE_USER_FUNC(postActionMessage, 1)
EVT_DECLARE_USER_FUNC(closeVerticleActionCommand, 1)
}

}
