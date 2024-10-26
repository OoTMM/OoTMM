import { isArray } from 'lodash';
import { RAW_GI, RAW_DRAWGI, RAW_HINTS_DATA } from '@ootmm/data';

import { Game } from './config';
import { gameId } from './util';

const mapGossip = (game: Game, data: any[]) => {
  const result: {[k: string]: any} = {};
  for (const v of data) {
    const key = gameId(game, v.location, ' ');
    result[key] = { type: v.type, id: parseInt(v.id) }
  }
  return result;
};

export const DATA_HINTS_POOL = {
  oot: mapGossip('oot', RAW_HINTS_DATA.oot),
  mm: mapGossip('mm', RAW_HINTS_DATA.mm),
};

type DataGi = {
  index: number;
  id: string;
  item: string;
  type: string;
  flags: number;
  draw: string;
  drawParam: number;
  addFunc: string;
  addParam: string;
  object: { type: 'oot' | 'mm', id: number } | { type: 'custom', id: string } | null;
  name: string;
};

export let GI: {[k: string]: DataGi} = {};
for (let i = 0; i < RAW_GI.length; ++i) {
  const v = RAW_GI[i];
  const index = i + 1;
  const id = v.id;
  const item = `ITEM_${v.item || 'NONE'}`;
  const type = `ITT_${v.type || 'NONE'}`;
  const flags = v.flags;
  let draw: string;
  let drawParam: number;
  if (v.draw) {
    if (isArray(v.draw)) {
      draw = v.draw[0].toString();
      drawParam = v.draw[1] as number;
    } else {
      draw = v.draw;
      drawParam = 0;
    }
  } else {
    draw = 'NONE';
    drawParam = 0;
  }
  draw = `DRAWGI_${draw}`;
  let object: DataGi['object'];
  if (v.object) {
    if (isArray(v.object)) {
      object = { type: v.object[0], id: v.object[1] } as any;
    } else {
      object = { type: 'custom', id: v.object };
    }
  } else {
    object = null;
  }
  const name = v.name || "";
  let addFunc: string;
  let addParam: string;
  if (v.add) {
    if (isArray(v.add)) {
      addFunc = v.add[0].toString();
      addParam = v.add[1].toString();
    } else {
      addFunc = v.add;
      addParam = '0';
    }
  } else {
    addFunc = 'NONE';
    addParam = '0';
  }
  GI[id] = { index, id, item, type, flags, draw, drawParam, object, name, addFunc, addParam };
}

type DataDrawGi = {
  index: number;
  id: string;
  func: string;
  params: string[];
};

export let DRAWGI: {[k: string]: DataDrawGi} = {};
for (let i = 0; i < RAW_DRAWGI.length; ++i) {
  const v = RAW_DRAWGI[i];
  const index = i + 1;
  const id = v.id;
  let func: string;
  const params: string[] = [];

  if (typeof v.func === 'string') {
    func = `DrawGi_${v.func}`;
  } else {
    func = `0x${v.func.toString(16)}`;
  }

  if (v.params) {
    for (const p of v.params) {
      if (typeof p === 'string') {
        params.push(`CUSTOM_OBJECT_${p}`);
      } else {
        params.push(`0x${p.toString(16)}`);
      }
    }
  }

  DRAWGI[id] = { index, id, func, params };
}
