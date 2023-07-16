export const GLITCHES = {
  OOT_EQUIP_SWAP: "Equip Swap (OoT)",
  OOT_OCARINA_ITEMS: "Ocarina Items (OoT)",
}

export type Glitch = keyof typeof GLITCHES;

export const DEFAULT_GLITCHES: Glitch[] = [];
