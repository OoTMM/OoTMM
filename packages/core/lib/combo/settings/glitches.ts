export const GLITCHES = {
  OOT_EQUIP_SWAP: "Equip Swap (OoT)",
  OOT_OCARINA_ITEMS: "Ocarina Items (OoT)",
  MM_SAKONS_HIDEOUT_CLIP: "Sakon's Hideout Clip",
  MM_FD_ANYWHERE: "Fierce Deity Anywhere (Night 3)"
}

export type Glitch = keyof typeof GLITCHES;

export const DEFAULT_GLITCHES: Glitch[] = [];
