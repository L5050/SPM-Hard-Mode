#include "shadoo.h"
#include "evtpatch.h"
#include "king_sammer.h"
#include "patch.h"
#include "mod.h"
#include "util.h"
#include "evt_cmd.h"

#include <types.h>
#include <spm/rel/dan.h>
#include <spm/evt_cam.h>
#include <spm/evt_sub.h>
#include <spm/evt_door.h>
#include <spm/evt_eff.h>
#include <spm/evt_fade.h>
#include <spm/evt_fairy.h>
#include <spm/evt_item.h>
#include <spm/evt_guide.h>
#include <spm/evt_mario.h>
#include <spm/evt_pouch.h>
#include <spm/evt_sub.h>
#include <spm/evt_mobj.h>
#include <spm/evt_msg.h>
#include <spm/evt_npc.h>
#include <spm/memory.h>
#include <spm/evt_env.h>
#include <spm/evt_seq.h>
#include <spm/evt_snd.h>
#include <spm/pausewin.h>
#include <spm/animdrv.h>
#include <spm/wpadmgr.h>
#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <spm/mario.h>
#include <spm/map_data.h>
#include <spm/evtmgr_cmd.h>
#include <spm/npcdrv.h>
#include <spm/mario.h>
#include <spm/evt_npc.h>
#include <spm/npc_ninja.h>
#include <spm/evtmgr_cmd.h>
#include <msl/stdio.h>
#include <wii/os.h>
#include <wii/mtx.h>

namespace mod {

static spm::npcdrv::NPCTribeAnimDef peachDefs[] = {
    {0, "S_1"},
    {1, "W_1"},
    {2, "R_1"},
    {3, "T_1"},
    {4, "D_2"},
    {6, "D_2"},
    {7, "D_2"},
    {8, "D_2"},
    {9, "D_2"},
    {10, "N_11"},
    {11, "N_12"},
    {12, "D_2"},
    {13, "D_2"},
    {14, "D_2"},
    {15, "D_2"},
    {16, "D_2"},
    {25, "J_1B"},
    {26, "J_1C"},
    {27, "J_2A"},
    {28, "J_2B"},
    {29, "J_2C"},
    {30, "J_2D"},
    {31, "N_11"},
    {32, "N_12"},
    {33, "G_2"}
};

static spm::npcdrv::NPCTribeAnimDef marioDefs[] = {
    {0, "mario_S_1"},
    {1, "mario_W_1"},
    {2, "mario_R_1"},
    {3, "mario_T_1"},
    {4, "mario_D_2"},
    {6, "mario_D_2"},
    {7, "mario_D_2"},
    {8, "mario_D_2"},
    {9, "mario_D_2"},
    {10, "D_2"},
    {11, "D_2"},
    {12, "D_2"},
    {13, "D_2"},
    {14, "D_2"},
    {15, "D_2"},
    {16, "D_2"},
    {25, "J_1B"},
    {26, "J_1C"},
    {27, "J_2A"},
    {28, "J_2B"},
    {29, "J_2C"},
    {30, "J_2D"},
    {31, "mario_N_11"},
    {32, "mario_N_12"},
    {33, "G_2"}
};

static spm::npcdrv::NPCTribeAnimDef boomDefs[] = {
    {0, "A_1B"},
    {1, "A_1A"},
    {25, "Z_1"}
};

static spm::npcdrv::NPCTribeAnimDef* ogMarioDefs = spm::npcdrv::npcTribes[330].animDefs;

EVT_BEGIN(ninja_pls_dont_crash)
DO(0)
    USER_FUNC(check_for_ninja, LW(0))
    IF_EQUAL(LW(0), 0)
        DO_BREAK()
    END_IF()
    WAIT_FRM(1)
WHILE()
RETURN()
EVT_END()

EVT_BEGIN(shadoo_fight_evt)
USER_FUNC(spm::evt_mobj::evt_mobj_delete, PTR("save"))
DO(0)
    USER_FUNC(spm::evt_npc::evt_npc_get_active_count, LW(0))
    IF_EQUAL(LW(0), 0)
        DO_BREAK()
    END_IF()
    IF_LARGE_EQUAL(LW(0), 1)
        USER_FUNC(check_for_ninja, LW(0))
        IF_LARGE_EQUAL(LW(0), 1)
            RUN_CHILD_EVT(ninja_pls_dont_crash)
            USER_FUNC(spm::evt_npc::evt_npc_get_active_count, LW(0))
            IF_EQUAL(LW(0), 0)
                DO_BREAK()
            END_IF()
        END_IF()
    END_IF()
    USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(10), LW(11), LW(12), LW(13))
    WAIT_FRM(1)
WHILE()/*
USER_FUNC(spm::evt_snd::evt_snd_bgmon_f_d, 0, PTR("BGM_BTL_BOSS_KOOPA1"), 1000)
USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)
USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 285)
USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 285, 0, -100, 0, LW(10), EVT_NULLPTR)
USER_FUNC(spm::evt_npc::evt_npc_set_rgba, LW(10), 0, 0, 0, 0xff)
USER_FUNC(spm::evt_npc::evt_npc_set_anim, LW(10), 0, 1)
USER_FUNC(spm::evt_npc::func_80107c38, LW(10), 0)
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 0, 8)
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 65536)
USER_FUNC(spm::evt_npc::evt_npc_flip_to, LW(10), 1)
USER_FUNC(spm::evt_npc::evt_npc_finish_flip_instant, LW(10))
IF_SMALL(LW(11), -480)
    SET(LW(11), -480)
END_IF()
IF_LARGE(LW(11), 480)
    SET(LW(11), 480)
END_IF()
IF_SMALL(LW(12), 0)
    SET(LW(12), 0)
END_IF()
IF_SMALL(LW(13), -140)
    SET(LW(13), -140)
END_IF()
IF_LARGE(LW(13), 140)
    SET(LW(13), 140)
END_IF()
USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(10), LW(11), LW(12), LW(13))
USER_FUNC(spm::evt_npc::func_800ff8f8, LW(10), LW(11), LW(12), LW(13))
USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_DRAW1"), LW(10))
USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_TURN1"), LW(10))
USER_FUNC(spm::evt_npc::evt_npc_flip, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_wait_flip_finished, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 8)
USER_FUNC(spm::evt_npc::evt_npc_restart_evt_id, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(10), 10, 0)
USER_FUNC(spm::evt_mario::evt_mario_key_on)
DO(0)
    USER_FUNC(spm::evt_npc::evt_npc_get_active_count, LW(0))
    IF_EQUAL(LW(0), 0)
        DO_BREAK()
    END_IF()
    IF_LARGE_EQUAL(LW(0), 1)
        USER_FUNC(check_for_ninja, LW(0))
        IF_LARGE_EQUAL(LW(0), 1)
            RUN_CHILD_EVT(ninja_pls_dont_crash)
            USER_FUNC(spm::evt_npc::evt_npc_get_active_count, LW(0))
            IF_EQUAL(LW(0), 0)
                DO_BREAK()
            END_IF()
        END_IF()
    END_IF()
    USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(10), LW(11), LW(12), LW(13))
    WAIT_FRM(1)
WHILE()
USER_FUNC(spm::evt_snd::evt_snd_bgmon_f_d, 0, PTR("BGM_BTL_DODONTAS1"), 1000)
USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)
USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 183)
USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 183, 0, -100, 0, LW(10), EVT_NULLPTR)
USER_FUNC(spm::evt_npc::evt_npc_set_rgba, LW(10), 0, 0, 0, 0xff)
USER_FUNC(spm::evt_npc::evt_npc_set_anim, LW(10), 0, 1)
USER_FUNC(spm::evt_npc::func_80107c38, LW(10), 0)
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 0, 8)
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 65536)
USER_FUNC(spm::evt_npc::evt_npc_flip_to, LW(10), 1)
USER_FUNC(spm::evt_npc::evt_npc_finish_flip_instant, LW(10))
IF_SMALL(LW(11), -480)
    SET(LW(11), -480)
END_IF()
IF_LARGE(LW(11), 480)
    SET(LW(11), 480)
END_IF()
IF_SMALL(LW(12), 0)
    SET(LW(12), 0)
END_IF()
IF_SMALL(LW(13), -140)
    SET(LW(13), -140)
END_IF()
IF_LARGE(LW(13), 140)
    SET(LW(13), 140)
END_IF()
USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(10), LW(11), LW(12), LW(13))
USER_FUNC(spm::evt_npc::func_800ff8f8, LW(10), LW(11), LW(12), LW(13))
USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_DRAW1"), LW(10))
USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_TURN1"), LW(10))
USER_FUNC(spm::evt_npc::evt_npc_flip, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_wait_flip_finished, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 8)
USER_FUNC(spm::evt_npc::evt_npc_restart_evt_id, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(10), 10, 0)
USER_FUNC(spm::evt_mario::evt_mario_key_on)
DO(0)
    USER_FUNC(spm::evt_npc::evt_npc_get_active_count, LW(0))
    IF_EQUAL(LW(0), 0)
        DO_BREAK()
    END_IF()
    IF_LARGE_EQUAL(LW(0), 1)
        USER_FUNC(check_for_ninja, LW(0))
        IF_LARGE_EQUAL(LW(0), 1)
            RUN_CHILD_EVT(ninja_pls_dont_crash)
            USER_FUNC(spm::evt_npc::evt_npc_get_active_count, LW(0))
            IF_EQUAL(LW(0), 0)
                DO_BREAK()
            END_IF()
        END_IF()
    END_IF()
    USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(10), LW(11), LW(12), LW(13))
    WAIT_FRM(1)
WHILE()
USER_FUNC(spm::evt_snd::evt_snd_bgmon_f_d, 0, PTR("BGM_BTL_MONEYLA1"), 1000)
USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)
USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 187)
USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 187, 0, -100, 0, LW(10), EVT_NULLPTR)
USER_FUNC(spm::evt_npc::evt_npc_set_rgba, LW(10), 0, 0, 0, 0xff)
USER_FUNC(spm::evt_npc::evt_npc_set_anim, LW(10), 0, 1)
USER_FUNC(spm::evt_npc::func_80107c38, LW(10), 0)
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 0, 8)
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 65536)
USER_FUNC(spm::evt_npc::evt_npc_flip_to, LW(10), 1)
USER_FUNC(spm::evt_npc::evt_npc_finish_flip_instant, LW(10))
IF_SMALL(LW(11), -480)
    SET(LW(11), -480)
END_IF()
IF_LARGE(LW(11), 480)
    SET(LW(11), 480)
END_IF()
IF_SMALL(LW(12), 0)
    SET(LW(12), 0)
END_IF()
IF_SMALL(LW(13), -140)
    SET(LW(13), -140)
END_IF()
IF_LARGE(LW(13), 140)
    SET(LW(13), 140)
END_IF()
USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(10), LW(11), LW(12), LW(13))
USER_FUNC(spm::evt_npc::func_800ff8f8, LW(10), LW(11), LW(12), LW(13))
USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_DRAW1"), LW(10))
USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_TURN1"), LW(10))
USER_FUNC(spm::evt_npc::evt_npc_flip, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_wait_flip_finished, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 8)
USER_FUNC(spm::evt_npc::evt_npc_restart_evt_id, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(10), 10, 0)
USER_FUNC(spm::evt_mario::evt_mario_key_on)
DO(0)
    USER_FUNC(spm::evt_npc::evt_npc_get_active_count, LW(0))
    IF_EQUAL(LW(0), 0)
        DO_BREAK()
    END_IF()
    IF_LARGE_EQUAL(LW(0), 1)
        USER_FUNC(check_for_ninja, LW(0))
        IF_LARGE_EQUAL(LW(0), 1)
            RUN_CHILD_EVT(ninja_pls_dont_crash)
            USER_FUNC(spm::evt_npc::evt_npc_get_active_count, LW(0))
            IF_EQUAL(LW(0), 0)
                DO_BREAK()
            END_IF()
        END_IF()
    END_IF()
    USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(10), LW(11), LW(12), LW(13))
    WAIT_FRM(1)
WHILE()*/
USER_FUNC(spm::evt_snd::evt_snd_bgmon_f_d, 0, PTR("BGM_BTL_MONEYLA1"), 1000)
USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)
USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 397)
USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 397, 0, -100, 0, LW(10), EVT_NULLPTR)
USER_FUNC(spm::evt_npc::evt_npc_set_rgba, LW(10), 0, 0, 0, 0xff)
USER_FUNC(spm::evt_npc::evt_npc_set_anim, LW(10), 0, 1)
USER_FUNC(spm::evt_npc::func_80107c38, LW(10), 0)
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 0, 8)
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 65536)
USER_FUNC(spm::evt_npc::evt_npc_flip_to, LW(10), 1)
USER_FUNC(spm::evt_npc::evt_npc_finish_flip_instant, LW(10))
IF_SMALL(LW(11), -480)
    SET(LW(11), -480)
END_IF()
IF_LARGE(LW(11), 480)
    SET(LW(11), 480)
END_IF()
IF_SMALL(LW(12), 0)
    SET(LW(12), 0)
END_IF()
IF_SMALL(LW(13), -140)
    SET(LW(13), -140)
END_IF()
IF_LARGE(LW(13), 140)
    SET(LW(13), 140)
END_IF()
USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(10), LW(11), LW(12), LW(13))
USER_FUNC(spm::evt_npc::func_800ff8f8, LW(10), LW(11), LW(12), LW(13))
USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_DRAW1"), LW(10))
USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_TURN1"), LW(10))
USER_FUNC(spm::evt_npc::evt_npc_flip, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_wait_flip_finished, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 8)
USER_FUNC(spm::evt_npc::evt_npc_restart_evt_id, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(10), 10, 0)
USER_FUNC(spm::evt_mario::evt_mario_key_on)
DO(0)
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(10), 14, LW(0))
    IF_EQUAL(LW(0), 1)
        DO_BREAK()
    END_IF()
    USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(10), LW(11), LW(12), LW(13))
    WAIT_FRM(1)
WHILE()
USER_FUNC(spm::evt_snd::evt_snd_bgmon_f_d, 0, PTR("BGM_BTL_DIMEN1"), 1000)
USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)
USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 226)
USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 226, 0, -100, 0, LW(10), EVT_NULLPTR)
USER_FUNC(spm::evt_npc::evt_npc_set_rgba, LW(10), 0, 0, 0, 0xff)
USER_FUNC(spm::evt_npc::evt_npc_set_anim, LW(10), 0, 1)
USER_FUNC(spm::evt_npc::func_80107c38, LW(10), 0)
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 0, 8)
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 65536)
USER_FUNC(spm::evt_npc::evt_npc_flip_to, LW(10), 1)
USER_FUNC(spm::evt_npc::evt_npc_finish_flip_instant, LW(10))
IF_SMALL(LW(11), -480)
    SET(LW(11), -480)
END_IF()
IF_LARGE(LW(11), 480)
    SET(LW(11), 480)
END_IF()
IF_SMALL(LW(12), 0)
    SET(LW(12), 0)
END_IF()
IF_SMALL(LW(13), -140)
    SET(LW(13), -140)
END_IF()
IF_LARGE(LW(13), 140)
    SET(LW(13), 140)
END_IF()
USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(10), LW(11), LW(12), LW(13))
USER_FUNC(spm::evt_npc::func_800ff8f8, LW(10), LW(11), LW(12), LW(13))
USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_DRAW1"), LW(10))
USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_TURN1"), LW(10))
USER_FUNC(spm::evt_npc::evt_npc_flip, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_wait_flip_finished, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 8)
USER_FUNC(spm::evt_npc::evt_npc_restart_evt_id, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(10), 10, 0)
USER_FUNC(spm::evt_mario::evt_mario_key_on)
DO(0)
    USER_FUNC(spm::evt_npc::evt_npc_get_active_count, LW(0))
    SUB(LW(0), 1)
    IF_EQUAL(LW(0), 0)
        DO_BREAK()
    END_IF()
    IF_LARGE_EQUAL(LW(0), 1)
        USER_FUNC(check_for_ninja, LW(0))
        IF_LARGE_EQUAL(LW(0), 1)
            RUN_CHILD_EVT(ninja_pls_dont_crash)
            USER_FUNC(spm::evt_npc::evt_npc_get_active_count, LW(0))
            IF_EQUAL(LW(0), 0)
                DO_BREAK()
            END_IF()
        END_IF()
    END_IF()
    USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(10), LW(11), LW(12), LW(13))
    WAIT_FRM(1)
WHILE()
IF_EQUAL(GSWF(1102), 1)
USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)
USER_FUNC(spm::evt_snd::evt_snd_bgmoff_f_d, 0, 500)
WAIT_MSEC(500)
USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 32)
USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 32, 0, -100, 0, LW(10), EVT_NULLPTR)
USER_FUNC(spm::evt_npc::evt_npc_set_rgba, LW(10), 0, 0, 0, 0xff)
USER_FUNC(spm::evt_npc::evt_npc_set_anim, LW(10), 0, 1)
USER_FUNC(spm::evt_npc::func_80107c38, LW(10), 0)
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 0, 8)
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 65536)
USER_FUNC(spm::evt_npc::evt_npc_flip_to, LW(10), 1)
USER_FUNC(spm::evt_npc::evt_npc_finish_flip_instant, LW(10))
IF_SMALL(LW(11), -480)
    SET(LW(11), -480)
END_IF()
IF_LARGE(LW(11), 480)
    SET(LW(11), 480)
END_IF()
IF_SMALL(LW(12), 0)
    SET(LW(12), 0)
END_IF()
IF_SMALL(LW(13), -140)
    SET(LW(13), -140)
END_IF()
IF_LARGE(LW(13), 140)
    SET(LW(13), 140)
END_IF()
USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(10), LW(11), FLOAT(0.0), LW(13))
USER_FUNC(spm::evt_npc::func_800ff8f8, LW(10), LW(11), LW(12), LW(13))
USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_DRAW1"), LW(10))
USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_TURN1"), LW(10))
USER_FUNC(spm::evt_npc::evt_npc_flip, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_wait_flip_finished, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_sammer_display_count, 102)
WAIT_MSEC(2500)
USER_FUNC(spm::evt_snd::evt_snd_bgmon, 0, PTR("BGM_MAP_STG6_2"))
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 8)
USER_FUNC(spm::evt_npc::evt_npc_restart_evt_id, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(10), 10, 0)
USER_FUNC(spm::evt_mario::evt_mario_key_on)
DO(0)
    USER_FUNC(spm::evt_npc::evt_npc_get_active_count, LW(0))
    SUB(LW(0), 1)
    IF_EQUAL(LW(0), 0)
        DO_BREAK()
    END_IF()
    IF_LARGE_EQUAL(LW(0), 1)
        USER_FUNC(check_for_ninja, LW(0))
        IF_LARGE_EQUAL(LW(0), 1)
            RUN_CHILD_EVT(ninja_pls_dont_crash)
            USER_FUNC(spm::evt_npc::evt_npc_get_active_count, LW(0))
            IF_EQUAL(LW(0), 0)
                DO_BREAK()
            END_IF()
        END_IF()
    END_IF()
    USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(10), LW(11), LW(12), LW(13))
    WAIT_FRM(1)
WHILE()
END_IF()
USER_FUNC(spm::evt_snd::evt_snd_bgmon_f_d, 0, PTR("BGM_BTL_BOSS_STG5"), 1000)
RETURN_FROM_CALL()

static s32 chunkFartId;

s32 grabChonkyFarts(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
  spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
  chunkFartId = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
  return 2;
}

s32 returnChonkyFarts(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
  spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
  chunkFartId = spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], chunkFartId);
  return 2;
}

s32 evt_npc_detect_player(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
  spm::npcdrv::NPCEntry *npc = evtEntry->ownerNPC;
  spm::npcdrv::NPCPart *part = spm::npcdrv::npcGetPartById(npc, 1);
  spm::mario::MarioWork *marioWork = spm::mario::marioGetPtr();

  f32 distance = wii::mtx::PSVECSquareDistance(&npc->position,&marioWork->position);
  spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
  f32 radius = spm::evtmgr_cmd::evtGetFloat(evtEntry, args[0]);
  if (distance < radius) {
    spm::evtmgr_cmd::evtSetValue(evtEntry, args[1], 1);
    return 2;
  }
  spm::evtmgr_cmd::evtSetValue(evtEntry, args[1], 0);
  return 2;
}

s32 evt_npc_entry_autoname(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
  spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
  char name[32];
  s32 id = spm::npcdrv::npcdrv_wp->nextTemplateNpcId;
  msl::stdio::sprintf(name,"npc_%08x", id);
  
  spm::npcdrv::NPCEntry* entry = spm::npcdrv::npcEntry(name, args[0], args[1], 0);
  spm::evtmgr_cmd::evtSetValue(evtEntry, args[2], entry->name);
  return 2;
}

EVT_DECLARE_USER_FUNC(grabChonkyFarts, 1)
EVT_DECLARE_USER_FUNC(returnChonkyFarts, 1)
EVT_DECLARE_USER_FUNC(evt_npc_detect_player, 2)

EVT_BEGIN(returnChunksDeathScript)
  USER_FUNC(grabChonkyFarts, LW(10))
RETURN_FROM_CALL()

EVT_BEGIN(hookChunksDeathScript)
  USER_FUNC(returnChonkyFarts, LW(10))
  DELETE_EVT(LW(10))
RETURN_FROM_CALL()

EVT_BEGIN(mariounk3)
  SET(LW(1), 50)
RETURN_FROM_CALL()

EVT_BEGIN(luigiReset)
  WAIT_MSEC(500)
  USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 12, 0)
RETURN_FROM_CALL()

EVT_BEGIN(luigiUnk7_2)
  USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, PTR("me"), 12, LW(15))
  IF_NOT_EQUAL(LW(15), 1)
    SET(LW(0), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 12, 1)
  ELSE()
    SET(LW(0), 0)
  END_IF()
RETURN_FROM_CALL()

EVT_BEGIN(luigiUnk7_1)
  USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
  ADD(LW(1), 75)
RETURN_FROM_CALL()

EVT_BEGIN(shadooSave)
  USER_FUNC(spm::evt_mobj::evt_mobj_save_blk, PTR("save"), FLOAT(-440.0), FLOAT(95.0), FLOAT(0.0), 0)
  USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 288)
RETURN_FROM_CALL()

EVT_BEGIN(peachGrab)
  USER_FUNC(evt_npc_entry_autoname, PTR("FRY_throw"), 0, LW(10))
  USER_FUNC(spm::evt_npc::evt_npc_set_rgba, LW(10), 0, 0, 0, 0xff)
  USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(11), LW(12), LW(13))
  USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(10), LW(11), LW(12), LW(13))
  LBL(0)
RETURN()

EVT_END()

EVT_BEGIN(peachReplacementAttackScript)
    USER_FUNC(spm::evt_npc::evt_npc_set_move_mode, PTR("me"), 1)
    USER_FUNC(spm::evt_npc::func_80224804)
    USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, PTR("me"), 1, 2048)
    USER_FUNC(spm::evt_npc::func_801049ec, PTR("me"))
    USER_FUNC(spm::evt_npc::func_80104a3c, PTR("me"))
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("me"), 37, FLOAT(1.0))
    USER_FUNC(spm::evt_npc::func_801072a4, PTR("me"))
    //USER_FUNC(spm::evt_sub::evt_sub_random, 2, LW(0))
    //RUN_EVT(peachGrab)
    IF_EQUAL(LW(0), 1)
      USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("me"), 14, PTR(peachDefs))
      USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(11), LW(12), LW(13))
      USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(5), LW(6), LW(7))
      IF_LARGE(LW(11), LW(5))
        SUB(LW(11), 40)
        USER_FUNC(spm::evt_npc::evt_npc_set_axis_movement_unit, PTR("me"), -1)
      ELSE()
        ADD(LW(11), 40)
        USER_FUNC(spm::evt_npc::evt_npc_set_axis_movement_unit, PTR("me"), 1)
      END_IF()
      USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 390, LW(11), LW(12), LW(13), LW(10), EVT_NULLPTR)
      USER_FUNC(spm::evt_npc::evt_npc_set_rgba, LW(10), 0, 0, 0, 0xff)
      USER_FUNC(spm::evt_npc::func_80108194, PTR("me"), 0)
      USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 31, 1)
      USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
      USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 32, 1)
      USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
      USER_FUNC(spm::evt_npc::evt_npc_animflag_onoff, LW(10), 0, 128)
      USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR("SFX_F_BOMB_SET2"), LW(11), LW(12), LW(13))
      WAIT_FRM(5)
      USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(10), 1, 1)
    END_IF()
    USER_FUNC(spm::evt_npc::func_80224804)
    USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, PTR("me"), 0, 2048)
    LBL(0)
    USER_FUNC(spm::evt_npc::func_80224874)
    USER_FUNC(spm::evt_npc::func_80225380)
    WAIT_FRM(1)
    USER_FUNC(spm::evt_mario::evt_mario_get_character, LW(0))
    IF_NOT_EQUAL(LW(0), 2)
      USER_FUNC(spm::evt_npc::evt_npc_set_move_mode, PTR("me"), 0)
      USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(11), LW(12), LW(13))
      USER_FUNC(spm::evt_npc::evt_npc_animflag_onoff, PTR("me"), 1, 128)
      USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("dmen_warp"), 0, LW(11), LW(12), LW(13), 0, 0, 0, 0, 0, 0, 0, 0)
      USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR("SFX_BS_DMN_GOOUT1"), LW(11), LW(12), LW(13))
      USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 287)
      WAIT_MSEC(250)
      USER_FUNC(spm::evt_npc::evt_npc_get_hp, PTR("me"), LW(0))
      USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 287, LW(11), LW(12), LW(13), LW(10), EVT_NULLPTR)
      USER_FUNC(spm::evt_npc::evt_npc_set_hp, LW(10), LW(0))
      USER_FUNC(spm::evt_npc::evt_npc_teleport_effect, 1, LW(10))
      USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(10), 8, spm::dan::dan_shadoo_defeat_evt)
      USER_FUNC(spm::evt_npc::evt_npc_delete, PTR("me"))
    END_IF()
    GOTO(0)
    RETURN()
EVT_END()

EVT_BEGIN(shadowBoomerTickDown)
LBL(0)
USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(10), LW(11), LW(12), LW(13))
USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(5), LW(6), LW(7))
SUB(LW(11), LW(5))
IF_SMALL(LW(11), 100)
  IF_LARGE(LW(11), -100)
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(10), 1, 1)
    RETURN()
  END_IF()
END_IF()
WAIT_FRM(1)
GOTO(0)
EVT_END()

EVT_BEGIN(marioReplacementAttackScript)
    USER_FUNC(spm::evt_npc::evt_npc_set_move_mode, PTR("me"), 1)
    USER_FUNC(spm::evt_npc::func_802154fc)
    USER_FUNC(spm::evt_npc::func_80215514)
    USER_FUNC(spm::evt_npc::func_801049ec, PTR("me"))
    USER_FUNC(spm::evt_npc::func_80104a3c, PTR("me"))
    USER_FUNC(spm::evt_npc::func_801072a4, PTR("me"))
    USER_FUNC(spm::evt_sub::evt_sub_random, 1, LW(0))
    IF_EQUAL(LW(0), 0)
      USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("me"), 14, PTR(marioDefs))
      USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(11), LW(12), LW(13))
      USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(5), LW(6), LW(7))
      IF_LARGE(LW(11), LW(5))
        SUB(LW(11), 25)
        USER_FUNC(spm::evt_npc::evt_npc_set_axis_movement_unit, PTR("me"), -1)
      ELSE()
        ADD(LW(11), 25)
        USER_FUNC(spm::evt_npc::evt_npc_set_axis_movement_unit, PTR("me"), 1)
      END_IF()
      USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 390, LW(11), LW(12), LW(13), LW(10), EVT_NULLPTR)
      USER_FUNC(spm::evt_npc::evt_npc_set_property, LW(10), 14, PTR(boomDefs))
      USER_FUNC(spm::evt_npc::func_80108194, LW(10), 0)
      USER_FUNC(spm::evt_npc::evt_npc_set_rgba, LW(10), 0, 0, 0, 0xff)
      USER_FUNC(spm::evt_npc::func_80108194, PTR("me"), 0)
      USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 31, 1)
      USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
      USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 32, 1)
      USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
      USER_FUNC(spm::evt_npc::evt_npc_set_anim, LW(10), 1, 1)
      USER_FUNC(spm::evt_npc::evt_npc_animflag_onoff, LW(10), 0, 128)
      USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR("SFX_F_BOMB_SET2"), LW(11), LW(12), LW(13))
      RUN_EVT(shadowBoomerTickDown)
      WAIT_FRM(5)
      USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("me"), 14, PTR(ogMarioDefs))
    END_IF()
    LBL(0)
    USER_FUNC(spm::evt_npc::func_80215540)
    USER_FUNC(spm::evt_npc::func_80215f44)
    WAIT_FRM(1)
    USER_FUNC(spm::evt_mario::evt_mario_get_character, LW(0))
    IF_EQUAL(LW(0), 2)
      USER_FUNC(spm::evt_npc::evt_npc_set_move_mode, PTR("me"), 0)
      USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(11), LW(12), LW(13))
      USER_FUNC(spm::evt_npc::evt_npc_animflag_onoff, PTR("me"), 1, 128)
      USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("dmen_warp"), 0, LW(11), LW(12), LW(13), 0, 0, 0, 0, 0, 0, 0, 0)
      USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR("SFX_BS_DMN_GOOUT1"), LW(11), LW(12), LW(13))
      USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 288)
      USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 390)
      WAIT_MSEC(250)
      USER_FUNC(spm::evt_npc::evt_npc_get_hp, PTR("me"), LW(0))
      USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 288, LW(11), LW(12), LW(13), LW(10), EVT_NULLPTR)
      USER_FUNC(spm::evt_npc::evt_npc_set_hp, LW(10), LW(0))
      USER_FUNC(spm::evt_npc::evt_npc_teleport_effect, 1, LW(10))
      USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(10), 8, spm::dan::dan_shadoo_defeat_evt)
      USER_FUNC(spm::evt_npc::evt_npc_delete, PTR("me"))
    END_IF()
    GOTO(0)
    RETURN()
EVT_END()

spm::evtmgr::EvtScriptCode luigiPatch[] = { IF_NOT_EQUAL(LW(0), 1) };
spm::evtmgr::EvtScriptCode luigiPatch2[] = { USER_FUNC(spm::evt_npc::evt_npc_arc_to, PTR("me"), LW(0), LW(1), LW(2), 500, FLOAT(0.0), FLOAT(55.0), 0, 0, 0) };

static void hookShadooScripts()
{
  spm::evtmgr::EvtScriptCode* luigi_attack_script = spm::npcdrv::npcEnemyTemplates[286].unkScript7;
  spm::evtmgr::EvtScriptCode* luigi_idle_script = spm::npcdrv::npcEnemyTemplates[286].unkScript2;
  evtpatch::hookEvt(luigi_idle_script, 6, (spm::evtmgr::EvtScriptCode*)luigiReset);
  evtpatch::hookEvtReplace(luigi_attack_script, 9, luigiUnk7_1);
  evtpatch::hookEvtReplace(luigi_attack_script, 2, luigiUnk7_2);
  evtpatch::patchEvtInstruction(luigi_attack_script, 3, luigiPatch);
  evtpatch::hookEvt(spm::dan::dan_70_init_evt, 39, (spm::evtmgr::EvtScriptCode*)shadooSave);
  
  spm::npcdrv::npcEnemyTemplates[288].unkScript7 = peachReplacementAttackScript;
  spm::npcdrv::npcTribes[332].voltShroomStunTime = 0;
  spm::npcdrv::npcEnemyTemplates[287].unkScript7 = marioReplacementAttackScript;

  evtpatch::hookEvtReplaceBlock(spm::dan::dan_shadoo_fight_evt, 1, (spm::evtmgr::EvtScriptCode*)shadoo_fight_evt, 91);
  evtpatch::patchEvtInstruction(spm::dan::dan_shadoo_main_evt, 126, EVT_CAST(USER_FUNC(spm::evt_snd::evt_snd_bgmon, 0, PTR("BGM_BTL_BOSS_STG4"))));
  evtpatch::hookEvt(spm::npcdrv::npcEnemyTemplates[183].onSpawnScript, 85, (spm::evtmgr::EvtScriptCode*)returnChunksDeathScript);
  evtpatch::hookEvt(spm::npcdrv::npcEnemyTemplates[183].unkScript6, 1, (spm::evtmgr::EvtScriptCode*)hookChunksDeathScript); //Fix for if O'Chunks is killed outside of boss rooms
  evtpatch::hookEvt(spm::npcdrv::npcEnemyTemplates[422].unkScript3, 71, (spm::evtmgr::EvtScriptCode*)mariounk3);
  evtpatch::hookEvt(spm::npcdrv::npcEnemyTemplates[288].unkScript3, 82, (spm::evtmgr::EvtScriptCode*)mariounk3);
  //spm::evtmgr::EvtScriptCode* dark_peach_on_hit = getInstructionEvtArg(spm::npcdrv::npcEnemyTemplates[288].unkScript3, 62, 0);
  evtpatch::hookEvtReplace(spm::npcdrv::npcEnemyTemplates[288].unkScript3, 62, turnNull);
}

void shadooMain() {
  hookShadooScripts();
}

}
