#include "marioDamage.h"
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
#include <spm/item_event_data.h>
#include <spm/mario_pouch.h>
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
#include <spm/spmario.h>
#include <spm/evt_npc.h>
#include <spm/npc_ninja.h>
#include <spm/evtmgr_cmd.h>
#include <msl/string.h>
#include <wii/os.h>

namespace mod {
int * _bossSequence = getBossSequence();
void (*marioTakeDamage)(wii::mtx::Vec3 * position, u32 flags, s32 damage);
s32 (*npcDamageMario)(spm::npcdrv::NPCEntry *npcEntry, spm::npcdrv::NPCPart *part, wii::mtx::Vec3 *position, u32 param_4, s32 damage, u32 flags);
int (*marioCalcDamageToEnemy)(s32 damageType, s32 tribeId);

s32 new_npcDamageMario(spm::npcdrv::NPCEntry *npcEntry, spm::npcdrv::NPCPart *part, wii::mtx::Vec3 *position, u32 param_4, s32 damage, u32 flags) {
  if (npcEntry == nullptr) return npcDamageMario(npcEntry, part, position, param_4, damage, flags);
      if (npcEntry->tribeId == 284) {
        wii::os::OSReport("unitwork %d\n", npcEntry->unitWork[0]);
        if (npcEntry->unitWork[0] == 3) {
          return npcDamageMario(npcEntry, part, position, param_4, damage, 1);
        }
      }
      if (npcEntry->tribeId == 532) {
        return npcDamageMario(npcEntry, part, position, param_4, 4, flags);
      }
  return npcDamageMario(npcEntry, part, position, param_4, damage, flags);
}

static void patchMarioDamage(){
  npcDamageMario = patch::hookFunction(spm::npcdrv::npcDamageMario, new_npcDamageMario);

  marioTakeDamage = patch::hookFunction(spm::mario::marioTakeDamage,
    [](wii::mtx::Vec3 * position, u32 flags, s32 damage) {
      if (marioWork -> character == 2) {
        damage = damage - 1;
        if (damage <= 0) {
          damage = 1;
        }
      }
      damage = damage * 2;
      s16 pixl = spm::mario_pouch::pouchGetCurPixl();
      if (pixl == 0x0DF)
      {
        damage -= spm::spmario::gp->gsw[1704];
      }
      if (pixl == 0x0E4)
      {
        damage -= spm::spmario::gp->gsw[1705];
      }
      if (damage < 0) damage = 0;
      //adds the rpg elements to boss fights
      int health = checkBossHealth();
      s32 plotValue = globals -> gsw0;
      if (plotValue == 0x67 && * _bossSequence > 0) {
        if (health > 0 && health <= 3 && * _bossSequence == 1) {
          //Bowser
          * _bossSequence -= 1;
          damage = 0;
          flags = 0x4;
          marioTakeDamage(position, flags, damage);
          spm::pausewin::pausewinPauseGame();
          spm::spmario::spmarioSystemLevel(1);
          for (int i = 0; i < 33; i++) {
            if (spm::item_event_data::itemEventDataTable[i].itemId == 68) {
              spm::evtmgr::evtEntryType(iceStorm, 0, 0, 0);
            }
          }
        } else {
          marioTakeDamage(position, flags, damage);
        }
      } else if (plotValue == 0xd5 && * _bossSequence > 0) {
        if (health <= 10 && * _bossSequence == 1 && health >= 1) {
          //O'Cabbage
          * _bossSequence -= 1;
          damage = 0;
          flags = 0x4;
          marioTakeDamage(position, flags, damage);
          spm::pausewin::pausewinPauseGame();
          spm::spmario::spmarioSystemLevel(1);
          for (int i = 0; i < 33; i++) {
            if (spm::item_event_data::itemEventDataTable[i].itemId == 68) {
              spm::evtmgr::evtEntryType(fireBurst, 0, 0, 0);
            }
          }
        } else {
          marioTakeDamage(position, flags, damage);
        }
      } else if (plotValue == 0xd5 && * _bossSequence > 0) {
        if (health <= 6 && * _bossSequence == 1) {
          //King Croacus
          * _bossSequence -= 1;
          damage = 0;
          flags = 0x4;
          marioTakeDamage(position, flags, damage);
          spm::pausewin::pausewinPauseGame();
          spm::spmario::spmarioSystemLevel(1);
          for (int i = 0; i < 33; i++) {
            if (spm::item_event_data::itemEventDataTable[i].itemId == 68) {
              spm::evtmgr::evtEntryType(fireBurst, 0, 0, 0);
            }
          }
        } else {
          marioTakeDamage(position, flags, damage);
        }
      } else if (plotValue == 0x19f && * _bossSequence > 0) {
        if (health <= 150 && * _bossSequence == 3) {
          //Super Dimentio
          * _bossSequence -= 1;
          damage = 0;
          flags = 0x4;
          marioTakeDamage(position, flags, damage);
          spm::pausewin::pausewinPauseGame();
          spm::spmario::spmarioSystemLevel(1);
          for (int i = 0; i < 33; i++) {
            if (spm::item_event_data::itemEventDataTable[i].itemId == 68) {
              spm::evtmgr::evtEntryType(shootingStar, 0, 0, 0);
            }
          }
        } else if (health <= 100 && * _bossSequence == 2) {
          * _bossSequence -= 1;
          damage = 0;
          flags = 0x4;
          marioTakeDamage(position, flags, damage);
          spm::pausewin::pausewinPauseGame();
          spm::spmario::spmarioSystemLevel(1);
          for (int i = 0; i < 33; i++) {
            if (spm::item_event_data::itemEventDataTable[i].itemId == 68) {
              spm::evtmgr::evtEntryType(fireBurst, 0, 0, 0);
            }
          }
        } else if (health <= 50 && * _bossSequence == 1) {
          * _bossSequence -= 1;
          damage = 0;
          flags = 0x4;
          marioTakeDamage(position, flags, damage);
          spm::pausewin::pausewinPauseGame();
          spm::spmario::spmarioSystemLevel(1);
          for (int i = 0; i < 33; i++) {
            if (spm::item_event_data::itemEventDataTable[i].itemId == 68) {
              spm::evtmgr::evtEntryType(shootingStar, 0, 0, 0);
            }
          }
        } else {
          marioTakeDamage(position, flags, damage);
        }
      } else {
        marioTakeDamage(position, flags, damage);
      }
    }
  );

  marioCalcDamageToEnemy = patch::hookFunction(spm::mario::marioCalcDamageToEnemy,
    [](s32 damageType, s32 tribeId)
            {
              //spm::npcdrv::NPCWork * NPCWork = spm::npcdrv::npcGetWorkPtr();
              //wii::os::OSReport("tribe %d\n", tribeId);
              wii::os::OSReport("damageType %d, tribeId %d\n", tribeId);
              if (damageType == 8 && tribeId == 381) return 0;
              if (damageType == 8 && tribeId == 386) return 0;
              if (damageType == 8 && tribeId == 396) return 0;
              if (damageType == 20 && tribeId > 0) return 2; //Shell Shock Damage Nerf
              wii::os::OSReport("damageTypeeeeee %d, tribeId %d\n", tribeId);
              int curBossHealth = checkBossHealth();
              if (damageType == 20 && tribeId < 0 && curBossHealth >= 1) 
              {
              return 2;
              }
              int damage = 0;
              switch(tribeId) {
                case 211:
                damage = 1;
                break;
                case 214:
                damage = 1;
                break;
                case 217:
                damage = 1;
                break;
                case 270:
                damage = 100;
                break;
                case 272:
                damage = 1;
                break;
                case 273:
                damage = 1;
                if (spm::mario::marioGetPtr()->miscFlags & 0x4000) {return 0;}
                //if (damageType == 8) {return 1;}
                break;
                case 282:
                damage = 1;
                break;
                case 286:
                damage = 1;
                break;
                case 289:
                damage = 1;
                break;
                case 292:
                return 1;
                break;
                case 295:
                damage = 1;
                break;
                case 305:
                damage = 1;
                break;
                case 306:
                return 0;
                break;
                case 309:
                /*if (damageType == 1) {
                  for (int i = 0; i < 535; i++) {
                    if (NPCWork->entries[i].tribeId == 309) {
                      NPCWork->entries[i].hp = NPCWork->entries[i].hp - 1;
                    }
                  }
                  damage = 1;
                } else {return 1;}*/
                break;
                case 316:
                damage = 3;
                break;
                case 318:
                damage = 1;
                break;
                case 319:
                damage = 2;
                break;
                case 330:
                damage = 1;
                break;
                case 331:
                damage = 1;
                break;
                case 332:
                damage = 1;
                break;
                case 333:
                damage = 5;
                break;
                case 532:
                damage = 1;
                break;
              }
              if (damageType == 0xf) {
                if (damage == 0) {
                  damage = marioCalcDamageToEnemy(0, tribeId) + 2;
                }
                if (damage > 0) {
                  damage = damage + 1;
                }
              }
              const char* mapName = spm::spmario::gp->mapName;
              char *comparison = "dan";
              char *result = msl::string::strstr(mapName, comparison);

              // pit only behavior
              if (result != 0)
              {
                wii::os::OSReport("dan running %d\n", damageType);
                if (damage == 0)
                {
                  if (tribeId > -1)  {
                  spm::npcdrv::NPCTribe* tribe = &spm::npcdrv::npcTribes[tribeId];
                  s32 id = tribe->catchCardItemId;
                  if (id != NULL && spm::mario_pouch::pouchCheckHaveItem(id) == true) {
                  int cards = spm::mario_pouch::pouchGetCardCount(spm::npcdrv::npcTribes[tribeId].catchCardItemId);
                  cards = cards + 1;
                  damage = marioCalcDamageToEnemy(damageType, tribeId) / cards;
                  } else {
                    damage = marioCalcDamageToEnemy(damageType, tribeId);
                  }
                  }
                  switch (damageType)
                  {
                    case 1:
                    damage += spm::spmario::gp->gsw[1700] * 2;
                    break;
                    case 19:
                    damage += spm::spmario::gp->gsw[1701] * 2;
                    break;
                    case 15:
                    damage += spm::spmario::gp->gsw[1702] * 2;
                    break;
                    case 8:
                    damage += spm::spmario::gp->gsw[1703];
                    break;
                  }
                } else {
                  switch (damageType)
                  {
                    case 1:
                    if (spm::spmario::gp->gsw[1700] > 0) {
                    damage += 1;
                    }
                    break;
                    case 19:
                    if (spm::spmario::gp->gsw[1701] > 0) {
                    damage += 1;
                    }
                    break;
                    case 15:
                    if (spm::spmario::gp->gsw[1702] > 0) {
                    damage += 1;
                    }
                    break;
                    case 8:
                    if (spm::spmario::gp->gsw[1703] > 0) {
                    damage += 1;
                    }
                    break;
                  }
                }
              }
              if (damage > 0) return damage;
              if (tribeId > -1)  {
                spm::npcdrv::NPCTribe* tribe = &spm::npcdrv::npcTribes[tribeId];
                s32 id = tribe->catchCardItemId;
                if (id != NULL && spm::mario_pouch::pouchCheckHaveItem(id) == true) {
                int cards = spm::mario_pouch::pouchGetCardCount(spm::npcdrv::npcTribes[tribeId].catchCardItemId);
                cards = cards + 1;
                return marioCalcDamageToEnemy(damageType, tribeId) / cards;
                }
              }
          return marioCalcDamageToEnemy(damageType, tribeId);
            }
        );
}

void marioDamageMain() {
  patchMarioDamage();
}

}
