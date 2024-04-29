#pragma once
#include "evt_cmd.h"
namespace mod {

#define MOD_VERSION "SPM April Fools 2024"

extern bool gIsDolphin;

extern "C" {

EVT_DECLARE(evt_cutscene_pure_heart)
EVT_DECLARE(evt_memcard_save)

}

void main();

}
