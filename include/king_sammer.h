#pragma once
#include <types.h>
#include <spm/npcdrv.h>
#include <spm/evtmgr.h>
#include <spm/mario.h>
#include <spm/evt_npc.h>
#include <spm/npc_ninja.h>
#include <spm/evtmgr_cmd.h>
#include <wii/os.h>
namespace mod {

#define CHALLENGES_MAX 5

struct {
  u8 currentChallenge = 0;
  bool acceptedChallenge = false;
  bool fightingSammer = false;
  bool succeededChallenge = false;
  spm::evtmgr::EvtScriptCode * challengeScripts[CHALLENGES_MAX];
  u8 amountToSkip[CHALLENGES_MAX];
} sammerKingdom_Work;

EVT_DECLARE(toggleSammerFightStart_fwd)
EVT_DECLARE(check_challenge_completion)
EVT_DECLARE_USER_FUNC(set_current_challenge, 1)
EVT_DECLARE_USER_FUNC(evt_npc_to_pointer, 2)
EVT_DECLARE_USER_FUNC(get_current_challenge, 1)
EVT_DECLARE_USER_FUNC(set_challenge_success, 1)
EVT_DECLARE_USER_FUNC(get_challenge_script, 1)
EVT_DECLARE_USER_FUNC(check_for_ninja, 1)
EVT_DECLARE_USER_FUNC(set_sammer_start, 1)
EVT_DECLARE_USER_FUNC(get_sammer_start, 1)
EVT_DECLARE_USER_FUNC(checkChallengeSucceeded, 2)
EVT_DECLARE_USER_FUNC(checkChallengeAccepted, 0)
EVT_DECLARE_USER_FUNC(accept_challenge, 0)
EVT_DECLARE_USER_FUNC(refuse_challenge, 0)

void kingSammerMain();

}
