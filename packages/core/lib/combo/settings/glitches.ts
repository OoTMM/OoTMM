export const GLITCHES = {
  OOT_EQUIP_SWAP: "Equip Swap (OoT)",
  OOT_OCARINA_ITEMS: "Ocarina Items (OoT)",
  OOT_MEGAFLIP: "Megaflips (OoT)",
}

export type Glitch = keyof typeof GLITCHES;

export const DEFAULT_GLITCHES: Glitch[] = [];
