export const DUNGEONS = {
  DT: "Deku Tree",
  DC: "Dodongo's Cavern",
  JJ: "Jabu-Jabu",
  Forest: "Forest Temple",
  Fire: "Fire Temple",
  Water: "Water Temple",
  Spirit: "Spirit Temple",
  Shadow: "Shadow Temple",
  BotW: "Bottom of the Well",
  IC: "Ice Cavern",
  GTG: "Gerudo Training Grounds",
  Ganon: "Ganon's Castle",
} as const;

export type DungeonSettings = {[k in keyof typeof DUNGEONS]: 'vanilla' | 'mq' | 'random'};

export const DEFAULT_DUNGEONS = Object.keys(DUNGEONS).reduce((dungeons, dungeon) => {
  dungeons[dungeon] = 'vanilla';
  return dungeons;
}, {} as any) as DungeonSettings;
