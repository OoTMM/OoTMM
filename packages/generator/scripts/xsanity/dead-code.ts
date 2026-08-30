/*
function outputPotsPoolOot(roomActors: RoomActors[]) {
  let lastSceneId = -1;
  let lastSetupId = -1;
  for (const room of roomActors) {
    for (const actor of room.actors) {
      if (actor.typeId === ACTORS_OOT.POT) {
        const item00 = (actor.params >> 0) & 0xff;
        let item: string;
        if (item00 >= 0x1a) {
          item = 'NOTHING';
        } else {
          item = ITEM00_DROPS_OOT[item00];
        }
        const key = ((room.setupId & 0x3) << 14) | (room.roomId << 8) | actor.actorId;
        if (room.sceneId != lastSceneId || room.setupId != lastSetupId) {
          console.log('');
          lastSceneId = room.sceneId;
          lastSetupId = room.setupId;
        }
        console.log(`Scene ${room.sceneId.toString(16)} Setup ${room.setupId} Room ${room.roomId} Pot ${actor.actorId}, pot,            NONE,                 SCENE_${room.sceneId.toString(16)}, 0x${key.toString(16)}, ${item}`);
      }

      if (actor.typeId === ACTORS_OOT.FLYING_POT) {
        const itemId = (actor.params >> 8) & 0xff;
        let item: string;
        if (itemId >= 0x07) {
          item = 'NOTHING';
        } else {
          item = FLYING_POT_DROPS[itemId];
        }
        const key = ((room.setupId & 0x3) << 14) | (room.roomId << 8) | actor.actorId;
        if (room.sceneId != lastSceneId || room.setupId != lastSetupId) {
          console.log('');
          lastSceneId = room.sceneId;
          lastSetupId = room.setupId;
        }
        console.log(`Scene ${room.sceneId.toString(16)} Setup ${room.setupId} Room ${room.roomId} Flying Pot ${actor.actorId}, pot,            NONE,                 SCENE_${room.sceneId.toString(16)}, 0x${key.toString(16)}, ${item}`);
      }
    }
  }
}
*/


// function outputShotSunOot(roomActors: RoomActors[]) {
//   let lastSceneId = -1;
//   let lastSetupId = -1;
//   for (const room of roomActors) {
//     for (const actor of room.actors) {
//       if (actor.typeId === ACTORS_OOT.SHOT_SUN) {
//         const params = actor.params & 0xFF;
//         var validFairy = params === 0x40 || params === 0x41;
//         if (!validFairy) {
//           console.log("Fairy not valid: " + JSON.stringify(actor));
//           continue;
//         }
//         const item = 'FAIRY_BIG';
//         if (room.sceneId != lastSceneId || room.setupId != lastSetupId) {
//           console.log('');
//           lastSceneId = room.sceneId;
//           lastSetupId = room.setupId;
//         }
//         const key = ((room.setupId & 0x3) << 14) | (room.roomId << 8) | actor.actorId;
//         console.log(`Scene ${room.sceneId.toString(16)} Setup ${room.setupId} Room ${hexPad(room.roomId, 2)} Fairy Spot ${decPad(actor.actorId + 1, 2)},             fairy_spot,            NONE,                 ${SCENES_BY_ID.oot[room.sceneId]}, ${hexPad(key, 5)}, ${item}`);
//         console.log(params === 0x40 ? "Suns Song" : "Song of Storms");
//       }
//     }
//   }
// }

// function outputFairyPoolOot(roomActors: RoomActors[]) {
//   let lastSceneId = -1;
//   let lastSetupId = -1;
//   for (const room of roomActors) {
//     for (const actor of room.actors) {
//       if (actor.typeId === ACTORS_OOT.EN_ELF || actor.typeId === ACTORS_OOT.BG_SPOT11_OASIS) {
//         var validFairy = actor.typeId === ACTORS_OOT.EN_ELF && actor.params === 4
//           || actor.typeId === ACTORS_OOT.BG_SPOT11_OASIS;
//         const fairyType = actor.params;
//         if (!validFairy) {
//           console.log("Fairy not valid: " + JSON.stringify(actor));
//           continue;
//         }
//         const item = 'FAIRY';
//         if (room.sceneId != lastSceneId || room.setupId != lastSetupId) {
//           console.log('');
//           lastSceneId = room.sceneId;
//           lastSetupId = room.setupId;
//         }
//         const count = 8;
//         for (let i = 0; i < count; ++i) {
//           const key = (i << 16) | ((room.setupId & 0x3) << 14) | (room.roomId << 8) | actor.actorId;
//           console.log(`Scene ${room.sceneId.toString(16)} Setup ${room.setupId} Room ${hexPad(room.roomId, 2)} Fairy Group ${decPad(actor.actorId + 1, 2)} Fairy ${decPad(i + 1, 2)},             fairy,            NONE,                 SCENE_${room.sceneId.toString(16)}, ${hexPad(key, 5)}, ${item}`);
//         }
//       }
//     }
//   }
// }

// function outputWonderMm(roomActors: RoomActors[]) {
//   let lastSceneId = -1;
//   let lastSetupId = -1;
//   for (const room of roomActors) {
//     for (const actor of room.actors) {
//       let count: number;
//       let item: string;
//       switch (actor.typeId) {
//       case ACTORS_MM.EN_HIT_TAG:
//         count = 3;
//         item = 'RUPEE_GREEN';
//         break;
//       case ACTORS_MM.EN_INVISIBLE_RUPPE:
//         count = 1;
//         item = ['RUPEE_GREEN', 'RUPEE_BLUE', 'RUPEE_RED', '???'][actor.params & 3];
//         break;
//       default:
//         count = 0;
//         item = '???';
//         break;
//       }
//       if (count === 0)
//         continue;
//       const keyBase = ((room.setupId & 0x3) << 14) | (room.roomId << 8) | actor.actorId;
//       if (room.sceneId != lastSceneId || room.setupId != lastSetupId) {
//         console.log('');
//         console.log(`### Scene: ${scenesById('mm')[room.sceneId]}`);
//         lastSceneId = room.sceneId;
//         lastSetupId = room.setupId;
//       }
//       for (let i = 0; i < count; ++i) {
//         const key = keyBase | (i << 16);
//         let post = count === 1 ? '' : ` Num ${i + 1}`;
//         console.log(`Scene ${room.sceneId.toString(16)} Setup ${room.setupId} Room ${hexPad(room.roomId, 2)} Wonder Item ${decPad(actor.actorId + 1, 2)}${post},        wonder,           NONE,                 SCENE_${room.sceneId.toString(16)}, ${hexPad(key, 5)}, ${item}`);
//       }
//     }
//   }
// }

// function outputCratesPoolMm(roomActors: RoomActors[]) {
//   let lastSceneId = -1;
//   let lastSetupId = -1;
//   for (const room of roomActors) {
//     for (const actor of room.actors) {
//       const key = ((room.setupId & 0x3) << 14) | (room.roomId << 8) | actor.actorId;
//       if (actor.typeId === ACTORS_MM.OBJ_KIBAKO || actor.typeId === ACTORS_MM.OBJ_KIBAKO2) {
//         if (room.sceneId != lastSceneId || room.setupId != lastSetupId) {
//           console.log('');
//           console.log(`### Scene: ${scenesById('mm')[room.sceneId]}`);
//           lastSceneId = room.sceneId;
//           lastSetupId = room.setupId;
//         }

//         /* Large crate */
//         if (actor.typeId === ACTORS_MM.OBJ_KIBAKO2) {
//           if (actor.params & 0x8000) continue;
//           const item = mmCollectibleDrop(actor.params & 0x3f);
//           if (item === 'STRAY_FAIRY') continue;
//           console.log(`Scene ${room.sceneId.toString(16)} Setup ${room.setupId} Room ${hexPad(room.roomId, 2)} Large Crate ${decPad(actor.actorId + 1, 2)},        crate,            NONE,                 SCENE_${room.sceneId.toString(16)}, ${hexPad(key, 5)}, ${item}`);
//         }

//         /* Small crate */
//         if (actor.typeId === ACTORS_MM.OBJ_KIBAKO) {
//           const item = mmCollectibleDrop(actor.params & 0x3f);
//           console.log(`Scene ${room.sceneId.toString(16)} Setup ${room.setupId} Room ${hexPad(room.roomId, 2)} Small Crate ${decPad(actor.actorId + 1, 2)},        crate,            NONE,                 SCENE_${room.sceneId.toString(16)}, ${hexPad(key, 5)}, ${item}`);
//         }
//       }
//     }
//   }
// }

// function outputPotsPoolMm(roomActors: RoomActors[]) {
//   let lastSceneId = -1;
//   let lastSetupId = -1;
//   for (const room of roomActors) {
//     for (const actor of room.actors) {
//       if (actor.typeId === ACTORS_MM.POT) {
//         let item: string;
//         const potType = (actor.params >> 7) & 3;
//         const potEnemy = (actor.rz >> 7) & 3;
//         if (potEnemy)
//           continue;
//         switch (potType) {
//         case 0:
//         case 2:
//           /* Set item */
//           item = MM_POTS_SET_DROPS[actor.params & 0x1f];
//           break;
//         case 1:
//           /* Magic Pot */
//           item = 'MAGIC_JAR_LARGE';
//           break;
//         case 3:
//           /* Random item */
//           if (actor.params & 0x10) {
//             item = 'NOTHING';
//           } else {
//             item = MM_POTS_RANDOM_DROPS[actor.params & 0x1f];
//           }
//           break;
//         default:
//           item = 'DUMMY';
//           break;
//         }
//         if (item === 'STRAY_FAIRY') {
//           continue;
//         }
//         const key = ((room.setupId & 0x3) << 14) | (room.roomId << 8) | actor.actorId;
//         if (room.sceneId != lastSceneId || room.setupId != lastSetupId) {
//           console.log('');
//           lastSceneId = room.sceneId;
//           lastSetupId = room.setupId;
//         }
//         console.log(`Scene ${room.sceneId.toString(16)} Setup ${room.setupId} Room ${room.roomId} Pot ${actor.actorId}, pot,            NONE,                 SCENE_${room.sceneId.toString(16)}, 0x${key.toString(16)}, ${item}`);
//       }

//       /*
//       if (actor.typeId === ACTORS_MM.FLYING_POT) {
//         const key = ((room.setupId & 0x3) << 14) | (room.roomId << 8) | actor.actorId;
//         const item = '???';
//         console.log(`Scene ${room.sceneId.toString(16)} Setup ${room.setupId} Room ${room.roomId} FLYING Pot ${actor.actorId}, pot,            NONE,                 SCENE_${room.sceneId.toString(16)}, 0x${key.toString(16)}, ${item}`);
//       }
//       */
//     }
//   }
// }
