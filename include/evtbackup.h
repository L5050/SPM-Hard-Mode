#pragma once

#include <evt_cmd.h>
#include "stack.hh"

namespace mod::evtbackup {
  using namespace spm::evtmgr;
  void backupLocalWork(s32 index, EvtVar value, EvtEntry * evtEntry);
  EvtVar restoreLocalWork(s32 index, EvtEntry * evtEntry);
  void destroyStack(s32 index);

  // evt_backupLocalWork(s32 index, EvtVar value)
  EVT_DECLARE_USER_FUNC(evt_backupLocalWork, 2)

  // evt_restoreLocalWork(EvtVar value, s32 index)
  EVT_DECLARE_USER_FUNC(evt_restoreLocalWork, 2)

}
