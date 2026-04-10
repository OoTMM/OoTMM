import type { ExprData, ExprNode } from './types';

let nextExprId = 0;
const keyToId = new Map<string, number>();
const isToExpr: ExprData[] = [];

function memoKey(expr: ExprNode): string {
  switch (expr.type) {
    case 'true': return 'TRUE';
    case 'false': return 'FALSE';
    case 'or': return `OR:${expr.exprs.map(x => x.id).sort().join(',')}`;
    case 'and': return `AND:${expr.exprs.map(x => x.id).sort().join(',')}`;
    case 'age': return `AGE:${expr.age}`;
    case 'item': return `ITEM:${expr.item.id}:${expr.count}`;
    case 'renewable': return `RENEWABLE:${expr.item.id}`;
    case 'license': return `LICENSE:${expr.item.id}`;
    case 'event': return `EVENT:${expr.event}`;
    case 'masks': return `MASKS:${expr.count}`;
    case 'special': return `SPECIAL:${expr.specialId}`;
    case 'time-oot': return `TIME_OOT:${expr.flag}`;
    case 'time-mm': return `TIME_MM:${expr.value}:${expr.value2}`;
    case 'price': return `PRICE:${expr.slot}:${expr.max}`;
    case 'song-event': return `SONG_EVENT:${expr.songId}:${expr.cmp}`;
    case 'flag-on': return `FLAG_ON:${expr.flag}`;
    case 'flag-off': return `FLAG_OFF:${expr.flag}`;
  }
}

export function memoExpr(expr: ExprNode): ExprData {
  let e: ExprData;
  const key = memoKey(expr);
  let id = keyToId.get(key);
  if (id === undefined) {
    id = nextExprId++;
    keyToId.set(key, id);
    e = Object.freeze({ ...expr, id });
    isToExpr[id] = e;
  } else {
    e = isToExpr[id];
  }
  return e;
};
