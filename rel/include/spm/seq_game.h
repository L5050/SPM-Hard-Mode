#pragma once

#include <spm/seqdrv.h>

namespace spm::seq_game {

extern "C" {

void seq_gameInit(spm::seqdrv::SeqWork * wp);
void seq_gameMain(spm::seqdrv::SeqWork *param_1);

}

}
