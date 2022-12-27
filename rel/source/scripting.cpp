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
USER_FUNC(itemCharm)
RETURN()
EVT_END()


}
