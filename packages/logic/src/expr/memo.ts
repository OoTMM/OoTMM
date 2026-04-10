import type { ExprNode } from './types';

export function exprMemoKey(expr: ExprNode): string {
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

