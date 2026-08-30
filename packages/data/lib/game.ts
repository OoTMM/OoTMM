export const GAMES = ['oot', 'mm'] as const;
export type Game = typeof GAMES[number];

export function gameId(game: Game | 'shared', id: string, char: string) {
  const prefixes = ['OOT', 'MM', 'SHARED'];
  if (!char) {
    char = '_';
  }
  for (const p of prefixes) {
    const pp = p + char;
    if (id.startsWith(pp)) {
      return id;
    }
  }
  return [game.toUpperCase(), id].join(char);
}
