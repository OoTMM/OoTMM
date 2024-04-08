import path from 'path';
import { ENTRANCES, NPC, SCENES } from '@ootmm/data';

import { DRAWGI, GI } from './data';
import { Monitor } from './monitor';
import { PATCH_GROUP_VALUES } from './patch-build/group';
import { CONFVARS_VALUES } from './confvars';
import { PRICE_RANGES } from './logic/price';
import { CodeGen } from './util/codegen';

const ENTRANCES_DEBUG_CATEGORIES = {
  COMMON: 'Common',
  REGION: 'Region',
  DUNGEON: 'Dungeon',
  BOSS: 'Boss',
  WARP: 'Songs/Owls',
  INTERIOR: 'Interiors',
  OOT_GROTTO: 'Grottos (OoT)',
  MM_GROTTO: 'Grottos (MM)',
  SPECIAL: 'Special',
};

const codegenFile = async (data: {[k: string]: number}, prefix: string, filename: string, guard: string) => {
  if (!process.env.BROWSER) {
    const cg = new CodeGen(path.resolve('build', 'include', 'combo', filename), guard);
    for (const [k, v] of Object.entries(data)) {
      cg.define(prefix + "_" + k, v);
    }
    await cg.emit();
  }
};

/* Split on <> tags and extract the inner macro */
/* TODO: Make this less ugly */
function textMacro(data: string) {
  data = `"${data}"`;
  data = data.replace(/<[^>]+>/g, (match) => {
    return `" TEXT_${match.substring(1, match.length - 1)} "`;
  });
  return data;
}

async function genGI() {
  if (process.env.BROWSER)
    return;

  /* Header */
  const cgHeader = new CodeGen(path.resolve('build', 'include', 'combo', 'gi_data.h'), "GENERATED_GI_DATA_H");
  cgHeader.define('GI_NONE', 0);
  for (const gi of Object.values(GI)) {
    cgHeader.define(`GI_${gi.id}`, gi.index);
  }
  await cgHeader.emit();

  /* Source */
  const cgSource = new CodeGen(path.resolve('build', 'src', 'common', 'gi_data.c'));
  cgSource.include('combo.h');
  cgSource.include('combo/custom.h');
  cgSource.include('combo/item.h');
  cgSource.include('combo/sr.h');
  cgSource.raw('');
  cgSource.raw('#if defined(GAME_OOT)');
  cgSource.raw('# define OBJECT_OOT(x) x');
  cgSource.raw('# define OBJECT_MM(x) ((x) ^ MASK_FOREIGN_OBJECT)');
  cgSource.raw('#else');
  cgSource.raw('# define OBJECT_OOT(x) ((x) ^ MASK_FOREIGN_OBJECT)');
  cgSource.raw('# define OBJECT_MM(x) x');
  cgSource.raw('#endif');
  cgSource.raw('');
  cgSource.raw('const GetItem kExtendedGetItems[] = {');
  for (const gi of Object.values(GI)) {
    let fields: string[] = [];
    fields.push(gi.item);
    fields.push(`0x${gi.flags.toString(16)}`);
    fields.push(gi.draw);
    fields.push(gi.type);
    if (!gi.object) {
      fields.push('0x0000');
    } else {
      if (gi.object.type === 'custom') {
        fields.push(`CUSTOM_OBJECT_ID_${gi.object.id}`);
      } else {
        fields.push(`OBJECT_${gi.object.type.toUpperCase()}(0x${gi.object.id.toString(16)})`);
      }
    }
    cgSource.raw(`    { ${fields.join(', ')} },`);
  }
  cgSource.raw('};');
  cgSource.raw('');
  cgSource.table('kGetItemDrawGiParam', 'u8', Object.values(GI).map(gi => gi.drawParam));
  cgSource.raw('');
  cgSource.raw('const char* const kItemNames[] = {');
  for (const gi of Object.values(GI)) {
    cgSource.raw(`    ${textMacro(gi.name)},`);
  }
  cgSource.raw('};');
  cgSource.raw('const u8 kAddItemFuncs[] = {');
  for (const gi of Object.values(GI)) {
    cgSource.raw(`    IA_${gi.addFunc},`);
  }
  cgSource.raw('};');
  cgSource.raw('const u16 kAddItemParams[] = {');
  for (const gi of Object.values(GI)) {
    cgSource.raw(`    ${gi.addParam},`);
  }
  cgSource.raw('};');
  await cgSource.emit();
}

async function genDrawGI() {
  if (process.env.BROWSER)
    return;

  /* Header */
  const cgHeader = new CodeGen(path.resolve('build', 'include', 'combo', 'drawgi_data.h'), "GENERATED_DRAWGI_DATA_H");
  cgHeader.define('DRAWGI_NONE', 0);
  for (const dgi of Object.values(DRAWGI)) {
    cgHeader.define(`DRAWGI_${dgi.id}`, dgi.index);
  }
  await cgHeader.emit();

  /* Source */
  const cgSource = new CodeGen(path.resolve('build', 'src', 'common', 'drawgi_data.c'));
  cgSource.include('combo.h');
  cgSource.include('combo/custom.h');
  cgSource.raw('const DrawGi kDrawGi[] = {');
  for (const dgi of Object.values(DRAWGI)) {
    cgSource.raw(`    { (void*)${dgi.func}, { ${dgi.params.join(', ')} } },`);
  }
  cgSource.raw('};');
  await cgSource.emit();
}

async function genEntrances() {
  /* Codegen header */
  const data: {[k: string]: number} = {};
  data['NONE'] = 0xffff;
  for (const [k, v] of Object.entries(ENTRANCES)) {
    data[k] = v.id;
  }
  await codegenFile(data, "ENTR", "entrances.h", "GENERATED_ENTRANCES_H");

  /* Codegen debug data */
  const debug = new CodeGen(path.resolve('build', 'src', 'common', 'entrances_debug.c'));
  debug.include('combo.h');
  debug.raw('');
  debug.raw('#if defined(DEBUG)');
  for (const [name, _] of Object.entries(ENTRANCES_DEBUG_CATEGORIES)) {
    debug.raw(`static const DebugMenuEntry kDebugMenuWarp_${name}[] = {`);
    for (const [k, v] of Object.entries(ENTRANCES)) {
      if (v.debug && v.debug[0] === name) {
        debug.raw(`  { "${v.debug[1]}", ENTR_${k} ${v.game === 'mm' ? '| MASK_FOREIGN_ENTRANCE ' : ''}}, `);
      }
    }
    debug.raw('  { NULL, 0 },');
    debug.raw('};');
    debug.raw('');
  }
  debug.raw(`const DebugMenuEntry kDebugMenuWarp[] = {`);
  for (const [name, str] of Object.entries(ENTRANCES_DEBUG_CATEGORIES)) {
    debug.raw(`  { "${str}", (u32)kDebugMenuWarp_${name} }, `);
  }
  debug.raw('  { NULL, 0 },');
  debug.raw('};');
  debug.raw('#endif');
  await debug.emit();
}

export const codegen = async (monitor: Monitor) => {
  monitor.log("Codegen");
  return Promise.all([
    genGI(),
    genDrawGI(),
    genEntrances(),
    codegenFile(SCENES,               "SCE",      "scenes.h",       "GENERATED_SCENES_H"),
    codegenFile(NPC,                  "NPC",      "npc.h",          "GENERATED_NPC_H"),
    codegenFile(CONFVARS_VALUES,      "CFG",      "config.h",       "GENERATED_CONFIG_H"),
    codegenFile(PATCH_GROUP_VALUES,   "PG",       "patch_group.h",  "GENERATED_PATCH_GROUP_H"),
    codegenFile(PRICE_RANGES,         "PRICES",   "prices.h",       "GENERATED_PRICES_H"),
  ]);
};
