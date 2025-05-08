#pragma once
#include "evt_cmd.h"
#include <spm/spmario.h>
#include <spm/mario.h>

namespace mod {

#define MOD_VERSION "Rubies and Magic"

extern bool gIsDolphin;

extern "C" {

EVT_DECLARE(evt_cutscene_pure_heart)
EVT_DECLARE(evt_memcard_save)

}

extern spm::mario::MarioWork * marioWork;
extern spm::spmario::SpmarioGlobals * globals;
EVT_DECLARE(turnNull)
EVT_DECLARE(iceStorm)
EVT_DECLARE(fireBurst)
EVT_DECLARE(shootingStar)

spm::evtmgr::EvtScriptCode* getStandardDeathScript();
spm::evtmgr::EvtScriptCode* getInstructionEvtArg(spm::evtmgr::EvtScriptCode* script, s32 line, int instruction);
int checkBossHealth();
int* getBossSequence();
EVT_DECLARE_USER_FUNC(compareStrings, 3)
EVT_DECLARE_USER_FUNC(displayBossHealth, 0)
EVT_DECLARE_USER_FUNC(checkBboxChonkyBoi, 1)
EVT_DECLARE_USER_FUNC(evt_npc_entry_autoname, 3)
void main();

}
