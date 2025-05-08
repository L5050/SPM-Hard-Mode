#include "king_sammer.h"
#include "evtpatch.h"
#include "patch.h"
#include "mod.h"
#include "evt_cmd.h"

#include <types.h>
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
#include <wii/os.h>

using namespace spm;
using namespace npcdrv;
using namespace evt_npc;

namespace mod {

npcdrv::NPCWork * NPCWork = npcdrv::npcGetWorkPtr();
spm::evtmgr::EvtScriptCode* standard_death_script1;

static NPCTribeAnimDef tonoAnims[] = {
    {0, "S_1"}, // Standing (ID 0 is idle animation)
    {1, "W_1"}, // Slow jog lmao
    {2, "S_2"}, // Standing and grinning
    {3, "T_2"}, // Talking (ID 3 is the ID to use when talking)
    {4, "R_1"}, // Running
    {5, "O_1"}, // Damage/Surprise
    {6, "O_2a"}, // Begin dance
    {7, "O_2b"}, // Dance animation
    {8, "O_2c"}, // End dance
    {9, "O_3a"}, // Begin shouting
    {10, "O_3b"}, // Shouting animation
    {11, "O_3c"}, // End shouting
    {-1, nullptr}
  };

  static NPCTribeAnimDef tonoRunAnims[] = {
      {0, "S_1"}, // Standing (ID 0 is idle animation)
      {1, "W_1"}, // Slow jog lmao
      {4, "S_2"}, // Standing and grinning
      {3, "T_2"}, // Talking (ID 3 is the ID to use when talking)
      {2, "R_1"}, // Running
      {5, "O_1"}, // Damage/Surprise
      {6, "O_2a"}, // Begin dance
      {7, "O_2b"}, // Dance animation
      {8, "O_2c"}, // End dance
      {9, "O_3a"}, // Begin shouting
      {10, "O_3b"}, // Shouting animation
      {11, "O_3c"}, // End shouting
      {-1, nullptr}
    };

  spm::npcdrv::NPCDefense def;
  map_data::MapData * wa4_26_md = map_data::mapDataPtr("wa4_26");
  evtmgr::EvtScriptCode* wa4_26_init_evt = wa4_26_md->initScript;
  evtmgr::EvtScriptCode* give_cards_evt = getInstructionEvtArg(wa4_26_init_evt, 33, 0);

  EVT_BEGIN(sammerOnHit)
    USER_FUNC(evt_npc_set_property, PTR("me"), 14, PTR(tonoAnims))
    USER_FUNC(evt_npc_set_move_mode, PTR("me"), 0)
    USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
    ADD(LW(0), 150)
    USER_FUNC(spm::evt_npc::evt_npc_arc_to, PTR("me"), LW(0), LW(1), LW(2), 200, 0, FLOAT(100.0), 0, 256, 0)
    USER_FUNC(evt_npc_set_move_mode, PTR("me"), 1)
    RUN_CHILD_EVT(npcEnemyTemplates[2].unkScript3)
  RETURN()
  EVT_END()

  EVT_BEGIN(tono_check_for_fall)
  LBL(0)
  USER_FUNC(evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
  IF_SMALL(LW(1), FLOAT(-10.0))
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_BOMB_FIRE1"))
    USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, LW(0), LW(1), LW(2), FLOAT(5.0), 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(evt_npc_entry_from_template, 0, 32, 400, 75, 0, LW(10), EVT_NULLPTR)
    USER_FUNC(evt_npc_delete, PTR("me"))
    RETURN()
  END_IF()
  WAIT_FRM(1)
  GOTO(0)
  EVT_END()

  EVT_BEGIN(sammerOnSpawn)
    RUN_CHILD_EVT(npcEnemyTemplates[2].onSpawnScript)
    USER_FUNC(evt_npc_set_property, PTR("me"), 14, PTR(tonoAnims))
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 4)
    USER_FUNC(spm::evt_sub::evt_sub_get_mapname, 0, LW(11))
    USER_FUNC(compareStrings, LW(11), PTR("wa"), LW(13))
    IF_EQUAL(LW(13), 1)
      RUN_EVT(tono_check_for_fall)
    END_IF()
  RETURN()
  EVT_END()

  EVT_BEGIN(sammerDisappear)
  USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
  USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_BOMB_FIRE1"))
  USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, LW(0), LW(1), LW(2), FLOAT(5.0), 0, 0, 0, 0, 0, 0, 0)
  USER_FUNC(evt_npc_set_move_mode, PTR("me"), 0)
  USER_FUNC(evt_npc_animflag_onoff, PTR("me"), 1, 128)
  WAIT_MSEC(250)
  USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
  ADD(LW(1), 200)
  USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_BOMB_FIRE1"))
  USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, LW(0), LW(1), LW(2), FLOAT(5.0), 0, 0, 0, 0, 0, 0, 0)
  WAIT_MSEC(100)
  USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("me"), LW(0), LW(1), LW(2))
  USER_FUNC(evt_npc_set_move_mode, PTR("me"), 3)
  USER_FUNC(evt_npc_animflag_onoff, PTR("me"), 0, 128)
  SUB(LW(1), 200)
  INLINE_EVT()
    USER_FUNC(spm::evt_npc::evt_npc_arc_to, PTR("me"), LW(0), LW(1), LW(2), 250, 0, FLOAT(20.0), 0, 256, 0)
  END_INLINE()
  SET(LW(5), 0)
  LBL(30)
  IF_SMALL(LW(5), 30)
    ADD(LW(5), 1)
    SETF(LW(1), LW(0))
    DIVF(LW(1), FLOAT(300.0))
    MULF(LW(1), FLOAT(360.0))
    USER_FUNC(evt_npc_get_axis_movement_unit, PTR("me"), LW(2))
    IF_LARGE(LW(2), 0)
        MULF(LW(1), FLOAT(-1.0))
    END_IF()
    USER_FUNC(evt_npc_rotate, PTR("me"), LW(1), EVT_NULLPTR, EVT_NULLPTR)
    WAIT_FRM(1)
    GOTO(30)
  END_IF()
  USER_FUNC(evt_npc_rotate, PTR("me"), 0, 0, 0)
  WAIT_MSEC(80)
  RETURN()
  EVT_END()

  EVT_BEGIN(sammerDisappear2)
  USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
  USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_BOMB_FIRE1"))
  USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("spm_explosion"), 0, LW(0), LW(1), LW(2), FLOAT(0.5), 0, 0, 0, 0, 0, 0, 0)
  USER_FUNC(evt_npc_set_move_mode, PTR("me"), 0)
  USER_FUNC(evt_npc_animflag_onoff, PTR("me"), 1, 128)
  WAIT_MSEC(250)
  USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
  ADD(LW(0), 50)
  ADD(LW(1), 200)
  USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_BOMB_FIRE1"))
  USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("spm_explosion"), 0, LW(0), LW(1), LW(2), FLOAT(0.5), 0, 0, 0, 0, 0, 0, 0)
  WAIT_MSEC(100)
  USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("me"), LW(0), LW(1), LW(2))
  USER_FUNC(evt_npc_set_move_mode, PTR("me"), 3)
  USER_FUNC(evt_npc_animflag_onoff, PTR("me"), 0, 128)
  SUB(LW(0), 50)
  SUB(LW(1), 200)
  INLINE_EVT()
    USER_FUNC(spm::evt_npc::evt_npc_arc_to, PTR("me"), LW(0), LW(1), LW(2), 250, 0, FLOAT(20.0), 0, 256, 0)
  END_INLINE()
  SET(LW(5), 0)
  LBL(30)
  IF_SMALL(LW(5), 30)
    ADD(LW(5), 1)
    SETF(LW(1), LW(0))
    DIVF(LW(1), FLOAT(300.0))
    MULF(LW(1), FLOAT(360.0))
    USER_FUNC(evt_npc_get_axis_movement_unit, PTR("me"), LW(2))
    IF_LARGE(LW(2), 0)
        MULF(LW(1), FLOAT(-1.0))
    END_IF()
    USER_FUNC(evt_npc_rotate, PTR("me"), LW(1), EVT_NULLPTR, EVT_NULLPTR)
    WAIT_FRM(1)
    GOTO(30)
  END_IF()
  USER_FUNC(evt_npc_rotate, PTR("me"), 0, 0, 0)
  WAIT_MSEC(80)
  RETURN()
  EVT_END()

  EVT_BEGIN(sammerDisappear3)
  USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
  USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_BOMB_FIRE1"))
  USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("spm_explosion"), 0, LW(0), LW(1), LW(2), FLOAT(0.5), 0, 0, 0, 0, 0, 0, 0)
  USER_FUNC(evt_npc_set_move_mode, PTR("me"), 0)
  USER_FUNC(evt_npc_animflag_onoff, PTR("me"), 1, 128)
  WAIT_MSEC(250)
  USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
  SUB(LW(0), 50)
  ADD(LW(1), 200)
  USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_BOMB_FIRE1"))
  USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("spm_explosion"), 0, LW(0), LW(1), LW(2), FLOAT(0.5), 0, 0, 0, 0, 0, 0, 0)
  WAIT_MSEC(100)
  USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("me"), LW(0), LW(1), LW(2))
  USER_FUNC(evt_npc_set_move_mode, PTR("me"), 3)
  USER_FUNC(evt_npc_animflag_onoff, PTR("me"), 0, 128)
  ADD(LW(0), 50)
  SUB(LW(1), 200)
  INLINE_EVT()
    USER_FUNC(spm::evt_npc::evt_npc_arc_to, PTR("me"), LW(0), LW(1), LW(2), 250, 0, FLOAT(20.0), 0, 256, 0)
  END_INLINE()
  SET(LW(5), 0)
  LBL(30)
  IF_SMALL(LW(5), 30)
    ADD(LW(5), 1)
    SETF(LW(1), LW(0))
    DIVF(LW(1), FLOAT(300.0))
    MULF(LW(1), FLOAT(360.0))
    USER_FUNC(evt_npc_get_axis_movement_unit, PTR("me"), LW(2))
    IF_LARGE(LW(2), 0)
        MULF(LW(1), FLOAT(-1.0))
    END_IF()
    USER_FUNC(evt_npc_rotate, PTR("me"), LW(1), EVT_NULLPTR, EVT_NULLPTR)
    WAIT_FRM(1)
    GOTO(30)
  END_IF()
  USER_FUNC(evt_npc_rotate, PTR("me"), 0, 0, 0)
  WAIT_MSEC(80)
  RETURN()
  EVT_END()

  EVT_BEGIN(sammerRun)
  USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(1), LW(2), LW(3))
  USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(5), LW(6), LW(7))
  IF_LARGE(LW(1), LW(5))
    SUB(LW(1), 150)
  ELSE()
    ADD(LW(1), 150)
  END_IF()
  WAIT_MSEC(200)
  USER_FUNC(evt_npc_set_property, PTR("me"), 14, PTR(tonoRunAnims))
  USER_FUNC(spm::evt_npc::evt_npc_walk_to, PTR("me"), LW(1), LW(3), 0, FLOAT(180.0), 4, 0, 0)
  USER_FUNC(evt_npc_set_property, PTR("me"), 14, PTR(tonoAnims))
  RETURN()
  EVT_END()

  EVT_BEGIN(invertSammerRun)
  USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("tono"), LW(1), LW(2), LW(3))
  USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(5), LW(6), LW(7))
  IF_LARGE(LW(1), LW(5))
    ADD(LW(1), 150)
  ELSE()
    SUB(LW(1), 150)
  END_IF()
  WAIT_MSEC(200)
  USER_FUNC(evt_npc_set_property, PTR("tono"), 14, PTR(tonoRunAnims))
  USER_FUNC(spm::evt_npc::evt_npc_walk_to, PTR("tono"), LW(1), LW(3), 0, FLOAT(180.0), 4, 0, 0)
  USER_FUNC(evt_npc_set_property, PTR("tono"), 14, PTR(tonoAnims))
  RETURN()
  EVT_END()

  EVT_BEGIN(sammerAttack)
  LBL(1)
  USER_FUNC(evt_npc_animflag_onoff, PTR("me"), 0, 128)
  USER_FUNC(evt_npc_set_move_mode, PTR("me"), 1)
  USER_FUNC(spm::evt_sub::evt_sub_random, 5, LW(0))
  SWITCH(LW(0))
    CASE_EQUAL(0)
    CASE_EQUAL(1)
    CASE_EQUAL(2)
      USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
      SUB(LW(0), 150)
      USER_FUNC(spm::evt_npc::evt_npc_arc_to, PTR("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(100.0), 0, 256, 0)
      WAIT_MSEC(500)
      USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
      ADD(LW(0), 150)
      USER_FUNC(spm::evt_npc::evt_npc_arc_to, PTR("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(100.0), 0, 256, 0)
      WAIT_MSEC(500)
      USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
      USER_FUNC(evt_npc_set_move_mode, PTR("me"), 3)
      USER_FUNC(spm::evt_npc::evt_npc_glide_to, PTR("me"), LW(0), LW(1), LW(2), 0, FLOAT(800.0), 0, 11, 0, 0)
      USER_FUNC(evt_npc_set_move_mode, PTR("me"), 1)
      //USER_FUNC(spm::evt_npc::evt_npc_arc_to, PTR("me"), LW(0), LW(1), LW(2), 250, 0, FLOAT(10.0), 0, 256, 0)
      WAIT_MSEC(500)
    CASE_EQUAL(3)
      RUN_CHILD_EVT(sammerDisappear)
      USER_FUNC(evt_npc_set_move_mode, PTR("me"), 1)
      RUN_CHILD_EVT(sammerRun)
    CASE_EQUAL(4)
      RUN_CHILD_EVT(sammerDisappear3)
      RUN_CHILD_EVT(sammerDisappear)
      RUN_CHILD_EVT(sammerDisappear2)
      USER_FUNC(evt_npc_set_move_mode, PTR("me"), 1)
      USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
      SUB(LW(0), 150)
      USER_FUNC(spm::evt_npc::evt_npc_arc_to, PTR("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(100.0), 0, 256, 0)
    CASE_EQUAL(5)
      RUN_CHILD_EVT(sammerRun)
  END_SWITCH()
  WAIT_FRM(1)
  GOTO(1)
EVT_END()

const char * tono_loss = "<p>\n"
"<dynamic 3><scale 1.5><wave><shake>\n"
"I yield!\n"
"<k>\n";

const char * tono_loss_1 = "<p>\n"
"<scale 0.9>\n"
"Bravo challenger!\n"
"I haven't had a sparring\n"
"match like that in years!\n"
"<k>\n"
"<p>\n"
"Let us spar again someday!\n"
"In the meantime, here is the\n"
"treasure you've rightfully earned!\n"
"<k>\n";

const char * tono_loss_2 = "<p>\n"
"Let us spar again someday!\n"
"In the meantime, here is the\n"
"treasure you've rightfully earned!\n"
"<k>\n";

EVT_BEGIN(tono_after_win)
  USER_FUNC(evt_npc_set_anim, PTR("tono"), 10, 1)
  WAIT_MSEC(200)
  USER_FUNC(evt_msg::evt_msg_print, 1, PTR(tono_loss), 0, 0)
  USER_FUNC(evt_fade::evt_fade_entry, 2, 1000, 0, 0, 0, 255)
  USER_FUNC(evt_fade::evt_fade_end_wait, -1)
  USER_FUNC(evt_npc_set_position, PTR("tono"), 400, 75, 0)
  USER_FUNC(evt_cam::evt_cam3d_evt_zoom_in, 1, 340, 120, 484, 340, 120, -16, 500, 11)
  USER_FUNC(evt_mario::evt_mario_set_pos, 270, 35, 0)
  USER_FUNC(evt_mario::evt_mario_face_npc, PTR("tono"))
  USER_FUNC(evt_npc_set_anim, PTR("tono"), 2, 1)
  WAIT_FRM(5)
  USER_FUNC(evt_fade::evt_fade_entry, 1, 1000, 0, 0, 0, 255)
  USER_FUNC(evt_fade::evt_fade_end_wait, -1)
  USER_FUNC(evt_msg::evt_msg_print, 1, PTR(tono_loss_1), 0, PTR("tono"))
  CALL(give_cards_evt + evtpatch::getLineOffset(give_cards_evt, 30))
  /*USER_FUNC(evt_item::evt_item_entry, PTR("item"), 496, 0, 0, -1000, 0, 0, 0, 0, 0)
  USER_FUNC(evt_item::evt_item_flag_onoff, 1, PTR("item"), 8)
  USER_FUNC(evt_item::evt_item_wait_collected, PTR("item"))
  USER_FUNC(evt_item::evt_item_entry, PTR("item"), 497, 0, 0, -1000, 0, 0, 0, 0, 0)
  USER_FUNC(evt_item::evt_item_flag_onoff, 1, PTR("item"), 8)
  USER_FUNC(evt_item::evt_item_wait_collected, PTR("item"))
  USER_FUNC(evt_item::evt_item_entry, PTR("item"), 498, 0, 0, -1000, 0, 0, 0, 0, 0)
  USER_FUNC(evt_item::evt_item_flag_onoff, 1, PTR("item"), 8)
  USER_FUNC(evt_item::evt_item_wait_collected, PTR("item"))
  USER_FUNC(evt_item::evt_item_entry, PTR("item"), 499, 0, 0, -1000, 0, 0, 0, 0, 0)
  USER_FUNC(evt_item::evt_item_flag_onoff, 1, PTR("item"), 8)
  USER_FUNC(evt_item::evt_item_wait_collected, PTR("item"))
  USER_FUNC(evt_item::evt_item_entry, PTR("item"), 500, 0, 0, -1000, 0, 0, 0, 0, 0)
  USER_FUNC(evt_item::evt_item_flag_onoff, 1, PTR("item"), 8)
  USER_FUNC(evt_item::evt_item_wait_collected, PTR("item"))
  USER_FUNC(evt_item::evt_item_entry, PTR("item"), 501, 0, 0, -1000, 0, 0, 0, 0, 0)
  USER_FUNC(evt_item::evt_item_flag_onoff, 1, PTR("item"), 8)
  USER_FUNC(evt_item::evt_item_wait_collected, PTR("item"))
  USER_FUNC(evt_item::evt_item_entry, PTR("item"), 502, 0, 0, -1000, 0, 0, 0, 0, 0)
  USER_FUNC(evt_item::evt_item_flag_onoff, 1, PTR("item"), 8)
  USER_FUNC(evt_item::evt_item_wait_collected, PTR("item"))
  USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg6_1_513"), 0, PTR("tono"))
  USER_FUNC(evt_npc_walk_to, PTR("tono"), 410, 0, 0, FLOAT(80.0), 0, 0, 0)
  USER_FUNC(evt_msg::evt_msg_print, 0, PTR("stg6_1_514"), 0, PTR("tono"))
  SET(GSWF(1102), 1)
  USER_FUNC(evt_cam::evt_cam_zoom_to_coords, 500, 11)
  USER_FUNC(evt_mario::evt_mario_key_on)*/
RETURN()
EVT_END()

EVT_BEGIN(tono_on_kill)
  USER_FUNC(evt_npc_set_property, PTR("tono"), 14, PTR(tonoAnims))
  USER_FUNC(evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
  USER_FUNC(evt_npc_set_position, PTR("tono"), LW(0), LW(1), LW(2))
  USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)
  RUN_EVT(tono_after_win)
  USER_FUNC(evt_npc_delete, PTR("me"))
  RETURN()
EVT_END()

EVT_BEGIN(checkForDanSammer)
USER_FUNC(spm::evt_sub::evt_sub_get_mapname, 0, LW(11))
USER_FUNC(compareStrings, LW(11), PTR("dan"), LW(13))
IF_EQUAL(LW(13), 1)
  USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
  DO(10)
    USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("spm_explosion"), 0, LW(0), LW(1), LW(2), FLOAT(0.8), 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon, PTR("SFX_F_BOMB_FIRE1"))
    WAIT_MSEC(200)
  WHILE()
  USER_FUNC(evt_npc_delete, PTR("me"))
  RETURN()
ELSE()
  RUN_CHILD_EVT(tono_on_kill)
END_IF()
RETURN()
EVT_END()

static void createKingNpc() {
  def.type = 0x0;
  def.defense = 0x2;
  npcEnemyTemplates[32].tribeId = 532;
  npcEnemyTemplates[32].unknown_0x0 = 1;
  npcEnemyTemplates[32].unknown_0x1 = 1;
  npcEnemyTemplates[32].unknown_0x2 = 0;
  npcEnemyTemplates[32].unknown_0x3 = 0;
  npcEnemyTemplates[32].unknown_0x8 = 0;
  npcEnemyTemplates[32].flags = 0x18;
  npcEnemyTemplates[32].onSpawnScript = sammerOnSpawn;
  npcEnemyTemplates[32].unkScript1 = npcEnemyTemplates[2].unkScript1;

  npcEnemyTemplates[32].unkScript2 = sammerAttack;
  npcEnemyTemplates[32].unkScript3 = sammerOnHit;
  npcEnemyTemplates[32].unkScript4 = npcEnemyTemplates[2].unkScript4;
  npcEnemyTemplates[32].unkScript5 = npcEnemyTemplates[2].unkScript5;
  npcEnemyTemplates[32].unkScript6 = checkForDanSammer;
  npcEnemyTemplates[32].unkScript7 = nullptr;
  npcEnemyTemplates[32].unkScript8 = nullptr;
  npcEnemyTemplates[32].unkScript9 = nullptr;
  npcEnemyTemplates[32].unkDefinitionTable = nullptr;
  npcTribes[532].catchCardItemId = 299;
  npcTribes[532].catchCardDefense = 1;
  npcTribes[532].maxHp = 20;
  npcTribes[532].killXp = 5000;
  npcTribes[532].coinDropChance = 100;
  npcTribes[532].coinDropBaseCount = 50;
  npcTribes[532].coinDropExtraChance = 45;
  npcTribes[532].coinDropExtraMax = 30;
  npcTribes[532].attackStrength = 4;
  npcTribes[532].animPoseName = "n_stg6_tono";
  npcTribes[532].animDefs = tonoAnims;
  npcTribes[532].hitXp = 0;
}

const char * tono_1 = "<p>\n"
"I never imagined you'd be\n"
"the victor. Quite the feat\n"
"you've achieved, indeed!\n"
"<k>\n"
"<p>\n"
"However, I cannot give you my\n"
"treasure, for you have yet to\n"
"defeat my final challenger...\n"
"<k>\n";

const char * tono_2 = "<p>\n"
"<dynamic 3><scale 2.5><wave><shake>\n"
"Me!\n"
"<k>\n";


EVT_BEGIN(tono_begin_fight)
  USER_FUNC(evt_npc_set_anim, PTR("tono"), 0, 1)
  USER_FUNC(evt_npc_set_property, PTR("tono"), 14, PTR(tonoAnims))
  USER_FUNC(evt_msg::evt_msg_print, 1, PTR(tono_1), 0, PTR("tono"))
  USER_FUNC(evt_snd::evt_snd_bgmoff_f_d, 0, 1000)
  USER_FUNC(evt_npc_set_anim, PTR("tono"), 1, 1)
  USER_FUNC(evt_npc_flag8_onoff, PTR("tono"), 1, 8)
  USER_FUNC(evt_npc_flag8_onoff, PTR("tono"), 0, 134217728)
  USER_FUNC(evt_npc_walk_to, PTR("tono"), 325, 0, 0, FLOAT(40.0), 0, 0, 0)
  USER_FUNC(evt_npc_set_anim, PTR("tono"), 0, 1)
  USER_FUNC(evt_msg::evt_msg_print, 1, PTR(tono_2), 0, PTR("tono"))
  USER_FUNC(evt_npc_sammer_display_count, 101)
  WAIT_MSEC(2500)
  USER_FUNC(evt_snd::evt_snd_bgmon, 0, PTR("BGM_MAP_STG6_2"))
  USER_FUNC(evt_npc_get_position, PTR("tono"), LW(0), LW(1), LW(2))
  USER_FUNC(evt_npc_entry_from_template, 0, 32, LW(0), LW(1), LW(2), LW(10), EVT_NULLPTR)
  USER_FUNC(evt_npc_flag8_onoff, PTR("tono"), 0, 8)
  USER_FUNC(evt_npc_flag8_onoff, PTR("tono"), 1, 134217728)
  USER_FUNC(evt_npc_set_position, PTR("tono"), 0, -1000, 0)
  USER_FUNC(evt_cam::evt_cam_zoom_to_coords, 500, 11)
  USER_FUNC(evt_mario::evt_mario_key_on)
RETURN()
EVT_END()

s32 get_cur_key(evtmgr::EvtEntry * evtEntry, bool firstRun) {
  evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
  evtmgr_cmd::evtSetValue(evtEntry, args[0], mario::marioKeyOffChk());
  return 2;
}
EVT_DECLARE_USER_FUNC(get_cur_key, 1)

EVT_BEGIN(toggleSammerFightStart)
  USER_FUNC(set_sammer_start, 1)
RETURN()
EVT_END()

EVT_BEGIN(toggleSammerFightStart_fwd)
  RUN_CHILD_EVT(toggleSammerFightStart)
RETURN_FROM_CALL()

EVT_BEGIN(challengeScript_1)
  USER_FUNC(set_challenge_success, 1)
  USER_FUNC(evt_mario::evt_mario_set_pose, PTR("D_2"), 0)
  USER_FUNC(evt_pouch::evt_pouch_set_hp, 1)
  USER_FUNC(evt_snd::evt_snd_sfxon_character, PTR("SFX_P_V_MARIO_DAMEGE1"), PTR("SFX_P_V_PEACH_DAMEGE1"), PTR("SFX_P_V_KOOPA_DAMEGE1"), PTR("SFX_P_V_LUIGI_DAMEGE1"))
  DO(0)
      USER_FUNC(evt_pouch::evt_pouch_get_hp, LW(0))
      IF_LARGE(LW(0), 1)
          DO_BREAK()
      END_IF()
      WAIT_FRM(1)
  WHILE()
  USER_FUNC(set_challenge_success, 0)
  RETURN()
EVT_END()

EVT_BEGIN(challengeScript_2)
  USER_FUNC(set_challenge_success, 1)
  DO(0)
      USER_FUNC(get_sammer_start, LW(0))
      IF_EQUAL(LW(0), 1)
          DO_BREAK()
      END_IF()
      WAIT_FRM(1)
  WHILE()
  DO(0)
      USER_FUNC(get_cur_key, LW(0))
      IF_EQUAL(LW(0), 0)
          DO_BREAK()
      END_IF()
      WAIT_FRM(1)
  WHILE()
  WAIT_MSEC(10000)
  USER_FUNC(get_cur_key, LW(0))
  IF_EQUAL(LW(0), 1)
      RETURN()
  END_IF()
  USER_FUNC(set_challenge_success, 0)
  RETURN()
EVT_END()

EVT_BEGIN(challengeScript_3)
  USER_FUNC(set_challenge_success, 0)
  DO(0)
      USER_FUNC(get_sammer_start, LW(0))
      IF_EQUAL(LW(0), 1)
          DO_BREAK()
      END_IF()
      WAIT_FRM(1)
  WHILE()
  DO(0)
      USER_FUNC(get_cur_key, LW(0))
      IF_EQUAL(LW(0), 0)
          DO_BREAK()
      END_IF()
      WAIT_FRM(1)
  WHILE()
  WAIT_FRM(5)
  DO(0)
      USER_FUNC(evt_npc::evt_npc_get_active_count, LW(0))
      IF_EQUAL(LW(0), 1)
          DO_BREAK()
      END_IF()
      WAIT_FRM(1)
  WHILE()
  USER_FUNC(evt_npc::evt_npc_tribe_agb_async, 178)
  USER_FUNC(evt_npc::evt_npc_entry_from_template, 0, 178, 0, -100, 0, LW(10), EVT_NULLPTR)
  USER_FUNC(evt_npc::evt_npc_flip_to, LW(10), 1)
  USER_FUNC(evt_npc::evt_npc_finish_flip_instant, LW(10))
  USER_FUNC(evt_npc::evt_npc_flip, LW(10))
  USER_FUNC(evt_mario::evt_mario_get_pos, LW(1), LW(2), LW(3))
  ADD(LW(1), 75)
  ADD(LW(2), 100)
  USER_FUNC(evt_npc::evt_npc_set_position, LW(10), LW(1), LW(2), LW(3))
  USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_NINJA_APPEAR1"), LW(10))
  DO(0)
      USER_FUNC(evt_npc::evt_npc_get_active_count, LW(0))
      IF_EQUAL(LW(0), 1)
          DO_BREAK()
      END_IF()
      WAIT_FRM(1)
  WHILE()
  USER_FUNC(evt_npc::evt_npc_tribe_agb_async, 259)
  USER_FUNC(evt_npc::evt_npc_entry_from_template, 0, 259, 0, -100, 0, LW(10), EVT_NULLPTR)
  USER_FUNC(evt_npc::evt_npc_flip_to, LW(10), 1)
  USER_FUNC(evt_npc::evt_npc_finish_flip_instant, LW(10))
  USER_FUNC(evt_npc::evt_npc_flip, LW(10))
  USER_FUNC(evt_mario::evt_mario_get_pos, LW(1), LW(2), LW(3))
  ADD(LW(1), 75)
  ADD(LW(2), 100)
  USER_FUNC(evt_npc::evt_npc_set_position, LW(10), LW(1), LW(2), LW(3))
  USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_NINJA_APPEAR1"), LW(10))
  DO(0)
      USER_FUNC(evt_npc::evt_npc_get_active_count, LW(0))
      IF_EQUAL(LW(0), 1)
          DO_BREAK()
      END_IF()
      WAIT_FRM(1)
  WHILE()
  USER_FUNC(evt_npc::evt_npc_tribe_agb_async, 262)
  USER_FUNC(evt_npc::evt_npc_entry_from_template, 0, 262, 0, -100, 0, LW(10), EVT_NULLPTR)
  USER_FUNC(evt_npc::evt_npc_flip_to, LW(10), 1)
  USER_FUNC(evt_npc::evt_npc_finish_flip_instant, LW(10))
  USER_FUNC(evt_npc::evt_npc_flip, LW(10))
  USER_FUNC(evt_mario::evt_mario_get_pos, LW(1), LW(2), LW(3))
  ADD(LW(1), 75)
  ADD(LW(2), 100)
  USER_FUNC(evt_npc::evt_npc_set_position, LW(10), LW(1), LW(2), LW(3))
  USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_NINJA_APPEAR1"), LW(10))
  DO(0)
      USER_FUNC(evt_npc::evt_npc_get_active_count, LW(0))
      IF_EQUAL(LW(0), 1)
          DO_BREAK()
      END_IF()
      WAIT_FRM(1)
  WHILE()
  USER_FUNC(set_challenge_success, 1)
  RETURN()
EVT_END()

EVT_BEGIN(challengeScript_4)
  USER_FUNC(set_challenge_success, 1)
  USER_FUNC(evt_mario::evt_mario_set_pose, PTR("D_2"), 0)
  USER_FUNC(evt_pouch::evt_pouch_get_attack, LW(0))
  SET(GSW(1700), LW(0))
  USER_FUNC(evt_pouch::evt_pouch_set_attack, 1)
  USER_FUNC(evt_snd::evt_snd_sfxon_character, PTR("SFX_P_V_MARIO_DAMEGE1"), PTR("SFX_P_V_PEACH_DAMEGE1"), PTR("SFX_P_V_KOOPA_DAMEGE1"), PTR("SFX_P_V_LUIGI_DAMEGE1"))
  RETURN()
EVT_END()

EVT_BEGIN(challengeScript_5)
  USER_FUNC(set_challenge_success, 0)
  USER_FUNC(evt_mario::evt_mario_set_pose, PTR("D_2"), 0)
  USER_FUNC(evt_pouch::evt_pouch_get_attack, LW(0))
  SET(GSW(1700), LW(0))
  USER_FUNC(evt_pouch::evt_pouch_set_attack, 1)
  USER_FUNC(evt_pouch::evt_pouch_set_hp, 1)
  USER_FUNC(evt_snd::evt_snd_sfxon_character, PTR("SFX_P_V_MARIO_DAMEGE1"), PTR("SFX_P_V_PEACH_DAMEGE1"), PTR("SFX_P_V_KOOPA_DAMEGE1"), PTR("SFX_P_V_LUIGI_DAMEGE1"))
  DO(0)
      USER_FUNC(get_sammer_start, LW(0))
      IF_EQUAL(LW(0), 1)
          DO_BREAK()
      END_IF()
      WAIT_FRM(1)
  WHILE()
  DO(0)
      USER_FUNC(get_cur_key, LW(0))
      IF_EQUAL(LW(0), 0)
          DO_BREAK()
      END_IF()
      WAIT_FRM(1)
  WHILE()
  WAIT_FRM(5)
  DO(0)
      USER_FUNC(evt_npc::evt_npc_get_active_count, LW(0))
      IF_EQUAL(LW(0), 1)
          DO_BREAK()
      END_IF()
      WAIT_FRM(1)
  WHILE()
  USER_FUNC(evt_npc::evt_npc_tribe_agb_async, 178)
  USER_FUNC(evt_npc::evt_npc_entry_from_template, 0, 178, 0, -100, 0, LW(10), EVT_NULLPTR)
  USER_FUNC(evt_npc::evt_npc_flip_to, LW(10), 1)
  USER_FUNC(evt_npc::evt_npc_finish_flip_instant, LW(10))
  USER_FUNC(evt_npc::evt_npc_flip, LW(10))
  USER_FUNC(evt_mario::evt_mario_get_pos, LW(1), LW(2), LW(3))
  ADD(LW(1), 75)
  ADD(LW(2), 100)
  USER_FUNC(evt_npc::evt_npc_set_position, LW(10), LW(1), LW(2), LW(3))
  USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_NINJA_APPEAR1"), LW(10))
  DO(0)
      USER_FUNC(evt_npc::evt_npc_get_active_count, LW(0))
      IF_EQUAL(LW(0), 1)
          DO_BREAK()
      END_IF()
      WAIT_FRM(1)
  WHILE()
  USER_FUNC(evt_npc::evt_npc_tribe_agb_async, 259)
  USER_FUNC(evt_npc::evt_npc_entry_from_template, 0, 259, 0, -100, 0, LW(10), EVT_NULLPTR)
  USER_FUNC(evt_npc::evt_npc_flip_to, LW(10), 1)
  USER_FUNC(evt_npc::evt_npc_finish_flip_instant, LW(10))
  USER_FUNC(evt_npc::evt_npc_flip, LW(10))
  USER_FUNC(evt_mario::evt_mario_get_pos, LW(1), LW(2), LW(3))
  ADD(LW(1), 75)
  ADD(LW(2), 100)
  USER_FUNC(evt_npc::evt_npc_set_position, LW(10), LW(1), LW(2), LW(3))
  USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_NINJA_APPEAR1"), LW(10))
  DO(0)
      USER_FUNC(evt_npc::evt_npc_get_active_count, LW(0))
      IF_EQUAL(LW(0), 1)
          DO_BREAK()
      END_IF()
      WAIT_FRM(1)
  WHILE()
  USER_FUNC(evt_npc::evt_npc_tribe_agb_async, 262)
  USER_FUNC(evt_npc::evt_npc_entry_from_template, 0, 262, 0, -100, 0, LW(10), EVT_NULLPTR)
  USER_FUNC(evt_npc::evt_npc_flip_to, LW(10), 1)
  USER_FUNC(evt_npc::evt_npc_finish_flip_instant, LW(10))
  USER_FUNC(evt_npc::evt_npc_flip, LW(10))
  USER_FUNC(evt_mario::evt_mario_get_pos, LW(1), LW(2), LW(3))
  ADD(LW(1), 75)
  ADD(LW(2), 100)
  USER_FUNC(evt_npc::evt_npc_set_position, LW(10), LW(1), LW(2), LW(3))
  USER_FUNC(evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_NINJA_APPEAR1"), LW(10))
  DO(0)
      USER_FUNC(evt_npc::evt_npc_get_active_count, LW(0))
      IF_EQUAL(LW(0), 1)
          DO_BREAK()
      END_IF()
      WAIT_FRM(1)
  WHILE()
  USER_FUNC(set_challenge_success, 1)
  RETURN()
EVT_END()

s32 check_for_ninja(evtmgr::EvtEntry * evtEntry, bool firstRun) {
  evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
  wii::os::OSReport("ninja check\n");
  for (int i = 0; i < 535; i++) {
    switch(NPCWork->entries[i].tribeId) {
      case 211:
      case 214:
      case 217:
      if (NPCWork->entries[i].flag8 & 0x80000000 != 0) {
      evtmgr_cmd::evtSetValue(evtEntry, args[0], 1);
      return 2;
      }
      break;
    }
  }
  evtmgr_cmd::evtSetValue(evtEntry, args[0], 0);
  return 2;
}

s32 evt_npc_to_pointer(evtmgr::EvtEntry * evtEntry, bool firstRun) {
  evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
  spm::npcdrv::NPCEntry* npc = evtNpcNameToPtr_NoAssert(evtEntry, (char *)args[0]);
  wii::os::OSReport("tono_ptr: %p\n", npc);
  evtmgr_cmd::evtSetValue(evtEntry, args[1], npc);
  return 2;
}

s32 set_challenge_success(evtmgr::EvtEntry * evtEntry, bool firstRun) {
  evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
  sammerKingdom_Work.succeededChallenge = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
  return 2;
}

s32 accept_challenge(evtmgr::EvtEntry *evtEntry, bool firstRun)
{
sammerKingdom_Work.acceptedChallenge = true;
return 2;
}

s32 refuse_challenge(evtmgr::EvtEntry *evtEntry, bool firstRun)
{
  sammerKingdom_Work.acceptedChallenge = false;
  return 2;
}

s32 checkChallengeAccepted(evtmgr::EvtEntry * evtEntry, bool firstRun) {
  if (sammerKingdom_Work.acceptedChallenge == false) {
    evtEntry -> lw[0] = 0;
  } else {
    evtEntry -> lw[0] = 1;
  }
  return 2;
}

s32 checkChallengeSucceeded(evtmgr::EvtEntry * evtEntry, bool firstRun) {
  evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
  if (sammerKingdom_Work.succeededChallenge == false) {
    evtmgr_cmd::evtSetValue(evtEntry, args[0], 0);
  } else {
    evtmgr_cmd::evtSetValue(evtEntry, args[0], 1);
    evtmgr_cmd::evtSetValue(evtEntry, args[1], sammerKingdom_Work.amountToSkip[sammerKingdom_Work.currentChallenge]);
  }
  return 2;
}

s32 set_current_challenge(evtmgr::EvtEntry * evtEntry, bool firstRun) {
  evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
  sammerKingdom_Work.currentChallenge = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
  return 2;
}

s32 get_current_challenge(evtmgr::EvtEntry * evtEntry, bool firstRun) {
  evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
  evtmgr_cmd::evtSetValue(evtEntry, args[0], sammerKingdom_Work.currentChallenge);
  return 2;
}

s32 get_challenge_script(evtmgr::EvtEntry * evtEntry, bool firstRun) {
  evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
  evtmgr_cmd::evtSetValue(evtEntry, args[0], sammerKingdom_Work.challengeScripts[sammerKingdom_Work.currentChallenge]);
  return 2;
}

s32 get_sammer_start(evtmgr::EvtEntry * evtEntry, bool firstRun) {
  evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
  evtmgr_cmd::evtSetValue(evtEntry, args[0], (int)sammerKingdom_Work.fightingSammer);
  return 2;
}

s32 set_sammer_start(evtmgr::EvtEntry * evtEntry, bool firstRun) {
  evtmgr::EvtVar *args = (evtmgr::EvtVar *)evtEntry->pCurData;
  sammerKingdom_Work.fightingSammer = evtmgr_cmd::evtGetValue(evtEntry, args[0]);
  return 2;
}

EVT_BEGIN(runTonoSaveScript)
  IF_EQUAL(GSWF(1701), 0)
    SET(GSWF(1701), 1)
    RUN_CHILD_EVT(evt_memcard_save)
  END_IF()
RETURN()
EVT_END()

EVT_BEGIN(runTonoSaveScript_fwd)
  USER_FUNC(spm::evt_pouch::evt_pouch_get_max_hp, LW(2))
  USER_FUNC(spm::evt_pouch::evt_pouch_set_hp, LW(2))
  RUN_CHILD_EVT(runTonoSaveScript)
RETURN_FROM_CALL()

static void patchFinalSammerRoom() {
  evtpatch::hookEvtReplace(give_cards_evt, 29, tono_begin_fight);
  evtpatch::hookEvt(give_cards_evt, 9, runTonoSaveScript_fwd);
}

static void kingdomMain() {
  sammerKingdom_Work.acceptedChallenge = false;
  sammerKingdom_Work.fightingSammer = false;
  sammerKingdom_Work.challengeScripts[0] = challengeScript_1;
  sammerKingdom_Work.challengeScripts[1] = challengeScript_2;
  sammerKingdom_Work.challengeScripts[2] = challengeScript_3;
  sammerKingdom_Work.challengeScripts[3] = challengeScript_4;
  sammerKingdom_Work.challengeScripts[4] = challengeScript_5;
  sammerKingdom_Work.amountToSkip[0] = 5;
  sammerKingdom_Work.amountToSkip[1] = 2;
  sammerKingdom_Work.amountToSkip[2] = 7;
  sammerKingdom_Work.amountToSkip[3] = 5;
  sammerKingdom_Work.amountToSkip[4] = 14;
}

void kingSammerMain() {
  standard_death_script1 = getStandardDeathScript();
  createKingNpc();
  patchFinalSammerRoom();
  kingdomMain();
}

}
