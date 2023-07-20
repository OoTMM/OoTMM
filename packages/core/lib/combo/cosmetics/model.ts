import { Patchfile } from "../patch-build/patchfile";

const OOT_FILES = {
  CODE: 0x00A87000,
}

const OOT_LINK_CHILD_OFFSETS = {
  LUT_DL_SHIELD_DEKU: 0x060050D0,
  LUT_DL_WAIST: 0x060050D8,
  LUT_DL_RTHIGH: 0x060050E0,
  LUT_DL_RSHIN: 0x060050E8,
  LUT_DL_RFOOT: 0x060050F0,
  LUT_DL_LTHIGH: 0x060050F8,
  LUT_DL_LSHIN: 0x06005100,
  LUT_DL_LFOOT: 0x06005108,
  LUT_DL_HEAD: 0x06005110,
  LUT_DL_HAT: 0x06005118,
  LUT_DL_COLLAR: 0x06005120,
  LUT_DL_LSHOULDER: 0x06005128,
  LUT_DL_LFOREARM: 0x06005130,
  LUT_DL_RSHOULDER: 0x06005138,
  LUT_DL_RFOREARM: 0x06005140,
  LUT_DL_TORSO: 0x06005148,
  LUT_DL_LHAND: 0x06005150,
  LUT_DL_LFIST: 0x06005158,
  LUT_DL_LHAND_BOTTLE: 0x06005160,
  LUT_DL_RHAND: 0x06005168,
  LUT_DL_RFIST: 0x06005170,
  LUT_DL_SWORD_SHEATH: 0x06005178,
  LUT_DL_SWORD_HILT: 0x06005180,
  LUT_DL_SWORD_BLADE: 0x06005188,
  LUT_DL_SLINGSHOT: 0x06005190,
  LUT_DL_OCARINA_FAIRY: 0x06005198,
  LUT_DL_OCARINA_TIME: 0x060051A0,
  LUT_DL_DEKU_STICK: 0x060051A8,
  LUT_DL_BOOMERANG: 0x060051B0,
  LUT_DL_SHIELD_HYLIAN_BACK: 0x060051B8,
  LUT_DL_BOTTLE: 0x060051C0,
  LUT_DL_MASTER_SWORD: 0x060051C8,
  LUT_DL_GORON_BRACELET: 0x060051D0,
  LUT_DL_FPS_RIGHT_ARM: 0x060051D8,
  LUT_DL_SLINGSHOT_STRING: 0x060051E0,
  LUT_DL_MASK_BUNNY: 0x060051E8,
  LUT_DL_MASK_GERUDO: 0x060051F0,
  LUT_DL_MASK_GORON: 0x060051F8,
  LUT_DL_MASK_KEATON: 0x06005200,
  LUT_DL_MASK_SPOOKY: 0x06005208,
  LUT_DL_MASK_TRUTH: 0x06005210,
  LUT_DL_MASK_ZORA: 0x06005218,
  LUT_DL_MASK_SKULL: 0x06005220,
  DL_SWORD_SHEATHED: 0x06005228,
  LUT_DL_SWORD_SHEATHED: 0x06005248,
  DL_SHIELD_DEKU_ODD: 0x06005250,
  LUT_DL_SHIELD_DEKU_ODD: 0x06005260,
  DL_SHIELD_DEKU_BACK: 0x06005268,
  LUT_DL_SHIELD_DEKU_BACK: 0x06005278,
  DL_SWORD_SHIELD_HYLIAN: 0x06005280,
  LUT_DL_SWORD_SHIELD_HYLIAN: 0x06005290,
  DL_SWORD_SHIELD_DEKU: 0x06005298,
  LUT_DL_SWORD_SHIELD_DEKU: 0x060052A8,
  DL_SHEATH0_HYLIAN: 0x060052B0,
  LUT_DL_SHEATH0_HYLIAN: 0x060052C0,
  DL_SHEATH0_DEKU: 0x060052C8,
  LUT_DL_SHEATH0_DEKU: 0x060052D8,
  DL_LFIST_SWORD: 0x060052E0,
  LUT_DL_LFIST_SWORD: 0x060052F8,
  DL_LHAND_PEDESTALSWORD: 0x06005300,
  LUT_DL_LHAND_PEDESTALSWORD: 0x06005310,
  DL_LFIST_BOOMERANG: 0x06005318,
  LUT_DL_LFIST_BOOMERANG: 0x06005328,
  DL_RFIST_SHIELD_DEKU: 0x06005330,
  LUT_DL_RFIST_SHIELD_DEKU: 0x06005340,
  DL_RFIST_SLINGSHOT: 0x06005348,
  LUT_DL_RFIST_SLINGSHOT: 0x06005358,
  DL_RHAND_OCARINA_FAIRY: 0x06005360,
  LUT_DL_RHAND_OCARINA_FAIRY: 0x06005370,
  DL_RHAND_OCARINA_TIME: 0x06005378,
  LUT_DL_RHAND_OCARINA_TIME: 0x06005388,
  DL_FPS_RARM_SLINGSHOT: 0x06005390,
  LUT_DL_FPS_RARM_SLINGSHOT: 0x060053A0,
  HIERARCHY: 0x060053A8,
};

function patchPtrOotHi(patch: Patchfile, base: number, offset: number, value: number) {
  const buf = Buffer.alloc(2);
  buf.writeUInt16BE(value >>> 16, 0);
  patch.addDataPatch('oot', base + offset, buf);
}

function patchPtrOotLo(patch: Patchfile, base: number, offset: number, value: number) {
  const buf = Buffer.alloc(2);
  buf.writeUInt16BE((value & 0xffff) >>> 0, 0);
  patch.addDataPatch('oot', base + offset, buf);
}

function patchPtrOot(patch: Patchfile, base: number, offset: number, value: number) {
  const buf = Buffer.alloc(4);
  buf.writeUInt32BE(value, 0);
  patch.addDataPatch('oot', base + offset, buf);
}

export function enableModelOotLink(patch: Patchfile, dfAddr: number) {
  dfAddr |= 0x06000000;
  let base: number;

  base = OOT_FILES.CODE + 0xE671C;
  patchPtrOot(patch, base, 0x0000, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST);
  patchPtrOot(patch, base, 0x0008, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST);
  patchPtrOot(patch, base, 0x0010, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST_SHIELD_DEKU);
  patchPtrOot(patch, base, 0x0018, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST_SHIELD_DEKU);
  patchPtrOot(patch, base, 0x0020, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST);
  patchPtrOot(patch, base, 0x0028, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST);
  patchPtrOot(patch, base, 0x0030, dfAddr);
  patchPtrOot(patch, base, 0x0038, dfAddr);
  patchPtrOot(patch, base, 0x0040, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHEATHED);
  patchPtrOot(patch, base, 0x0048, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHEATHED);
  patchPtrOot(patch, base, 0x0050, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHIELD_DEKU);
  patchPtrOot(patch, base, 0x0058, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHIELD_DEKU);
  patchPtrOot(patch, base, 0x0060, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHIELD_HYLIAN);
  patchPtrOot(patch, base, 0x0068, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHIELD_HYLIAN);
  patchPtrOot(patch, base, 0x0070, dfAddr);
  patchPtrOot(patch, base, 0x0078, dfAddr);
  patchPtrOot(patch, base, 0x0080, 0x00000000);
  patchPtrOot(patch, base, 0x0088, 0x00000000);
  patchPtrOot(patch, base, 0x0090, OOT_LINK_CHILD_OFFSETS.LUT_DL_SHIELD_DEKU_BACK);
  patchPtrOot(patch, base, 0x0098, OOT_LINK_CHILD_OFFSETS.LUT_DL_SHIELD_DEKU_BACK);
  patchPtrOot(patch, base, 0x00a0, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHEATH);
  patchPtrOot(patch, base, 0x00a8, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHEATH);
  patchPtrOot(patch, base, 0x00b0, OOT_LINK_CHILD_OFFSETS.LUT_DL_SHEATH0_DEKU);
  patchPtrOot(patch, base, 0x00b8, OOT_LINK_CHILD_OFFSETS.LUT_DL_SHEATH0_DEKU);
  patchPtrOot(patch, base, 0x00c0, OOT_LINK_CHILD_OFFSETS.LUT_DL_SHEATH0_HYLIAN);
  patchPtrOot(patch, base, 0x00c8, OOT_LINK_CHILD_OFFSETS.LUT_DL_SHEATH0_HYLIAN);
  patchPtrOot(patch, base, 0x00d0, dfAddr);
  patchPtrOot(patch, base, 0x00d8, dfAddr);
  patchPtrOot(patch, base, 0x00e0, 0x00000000);
  patchPtrOot(patch, base, 0x00e8, 0x00000000);
  patchPtrOot(patch, base, 0x00f0, OOT_LINK_CHILD_OFFSETS.LUT_DL_SHIELD_DEKU_BACK);
  patchPtrOot(patch, base, 0x00f8, OOT_LINK_CHILD_OFFSETS.LUT_DL_SHIELD_DEKU_BACK);
  patchPtrOot(patch, base, 0x0100, OOT_LINK_CHILD_OFFSETS.LUT_DL_LHAND_PEDESTALSWORD);
  patchPtrOot(patch, base, 0x0108, OOT_LINK_CHILD_OFFSETS.LUT_DL_LHAND_PEDESTALSWORD);
  patchPtrOot(patch, base, 0x0110, OOT_LINK_CHILD_OFFSETS.LUT_DL_LHAND_PEDESTALSWORD);
  patchPtrOot(patch, base, 0x0118, OOT_LINK_CHILD_OFFSETS.LUT_DL_LHAND_PEDESTALSWORD);
  patchPtrOot(patch, base, 0x0120, OOT_LINK_CHILD_OFFSETS.LUT_DL_LHAND);
  patchPtrOot(patch, base, 0x0128, OOT_LINK_CHILD_OFFSETS.LUT_DL_LHAND);
  patchPtrOot(patch, base, 0x0130, OOT_LINK_CHILD_OFFSETS.LUT_DL_LFIST);
  patchPtrOot(patch, base, 0x0138, OOT_LINK_CHILD_OFFSETS.LUT_DL_LFIST);
  patchPtrOot(patch, base, 0x0140, OOT_LINK_CHILD_OFFSETS.LUT_DL_LFIST_SWORD);
  patchPtrOot(patch, base, 0x0148, OOT_LINK_CHILD_OFFSETS.LUT_DL_LFIST_SWORD);
  patchPtrOot(patch, base, 0x0150, OOT_LINK_CHILD_OFFSETS.LUT_DL_LFIST_SWORD);
  patchPtrOot(patch, base, 0x0158, OOT_LINK_CHILD_OFFSETS.LUT_DL_LFIST_SWORD);
  patchPtrOot(patch, base, 0x0160, OOT_LINK_CHILD_OFFSETS.LUT_DL_RHAND);
  patchPtrOot(patch, base, 0x0168, OOT_LINK_CHILD_OFFSETS.LUT_DL_RHAND);
  patchPtrOot(patch, base, 0x0170, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST);
  patchPtrOot(patch, base, 0x0178, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST);
  patchPtrOot(patch, base, 0x0180, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST_SLINGSHOT);
  patchPtrOot(patch, base, 0x0188, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST_SLINGSHOT);
  patchPtrOot(patch, base, 0x0190, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHEATHED);
  patchPtrOot(patch, base, 0x0198, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHEATHED);
  patchPtrOot(patch, base, 0x01a0, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHEATH);
  patchPtrOot(patch, base, 0x01a8, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHEATH);
  patchPtrOot(patch, base, 0x01b0, OOT_LINK_CHILD_OFFSETS.LUT_DL_WAIST);
  patchPtrOot(patch, base, 0x01b8, OOT_LINK_CHILD_OFFSETS.LUT_DL_WAIST);
  patchPtrOot(patch, base, 0x01c0, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST_SLINGSHOT);
  patchPtrOot(patch, base, 0x01c8, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST_SLINGSHOT);
  patchPtrOot(patch, base, 0x01d0, OOT_LINK_CHILD_OFFSETS.LUT_DL_RHAND_OCARINA_FAIRY);
  patchPtrOot(patch, base, 0x01d8, OOT_LINK_CHILD_OFFSETS.LUT_DL_RHAND_OCARINA_FAIRY);
  patchPtrOot(patch, base, 0x01e0, OOT_LINK_CHILD_OFFSETS.LUT_DL_RHAND_OCARINA_TIME);
  patchPtrOot(patch, base, 0x01e8, OOT_LINK_CHILD_OFFSETS.LUT_DL_RHAND_OCARINA_TIME);
  patchPtrOot(patch, base, 0x01f0, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST);
  patchPtrOot(patch, base, 0x01f8, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST);
  patchPtrOot(patch, base, 0x0200, OOT_LINK_CHILD_OFFSETS.LUT_DL_LFIST);
  patchPtrOot(patch, base, 0x0208, OOT_LINK_CHILD_OFFSETS.LUT_DL_LFIST);
  patchPtrOot(patch, base, 0x0210, OOT_LINK_CHILD_OFFSETS.LUT_DL_LFIST_BOOMERANG);
  patchPtrOot(patch, base, 0x0218, OOT_LINK_CHILD_OFFSETS.LUT_DL_LFIST_BOOMERANG);
  patchPtrOot(patch, base, 0x0220, OOT_LINK_CHILD_OFFSETS.LUT_DL_LHAND_BOTTLE);
  patchPtrOot(patch, base, 0x0228, OOT_LINK_CHILD_OFFSETS.LUT_DL_LHAND_BOTTLE);
  patchPtrOot(patch, base, 0x0230, 0x00000000);
  patchPtrOot(patch, base, 0x0238, 0x00000000);
  patchPtrOot(patch, base, 0x0240, OOT_LINK_CHILD_OFFSETS.LUT_DL_RSHOULDER);
  patchPtrOot(patch, base, 0x0248, 0x00000000);
  patchPtrOot(patch, base, 0x0250, OOT_LINK_CHILD_OFFSETS.LUT_DL_FPS_RARM_SLINGSHOT);

  base = OOT_FILES.CODE + 0xE6B2C;
  patchPtrOot(patch, base, 0x0000, OOT_LINK_CHILD_OFFSETS.LUT_DL_BOTTLE);

  base = OOT_FILES.CODE + 0xE6B74;
  patchPtrOot(patch, base, 0x0000, OOT_LINK_CHILD_OFFSETS.LUT_DL_SLINGSHOT_STRING)
  patchPtrOot(patch, base, 0x0004, 0x44178000);
  patchPtrOot(patch, base, 0x0008, 0x436C0000);

  patchPtrOotHi(patch, OOT_FILES.CODE, 0x6922E, OOT_LINK_CHILD_OFFSETS.LUT_DL_GORON_BRACELET);
  patchPtrOotLo(patch, OOT_FILES.CODE, 0x69232, OOT_LINK_CHILD_OFFSETS.LUT_DL_GORON_BRACELET);
  patchPtrOotHi(patch, OOT_FILES.CODE, 0x6A80E, OOT_LINK_CHILD_OFFSETS.LUT_DL_DEKU_STICK);
  patchPtrOotLo(patch, OOT_FILES.CODE, 0x6A812, OOT_LINK_CHILD_OFFSETS.LUT_DL_DEKU_STICK);

  /*
  writer.SetBase('Stick')
  writer.GoTo(0x334)
  writer.WriteModelData(Offsets.CHILD_LINK_LUT_DL_DEKU_STICK)
  writer.GoTo(0x330)
  writer.WriteModelData16(0x0015)

  writer.SetBase('Shield')
  writer.GoTo(0x7EE)
  writer.WriteModelDataHi(Offsets.CHILD_LINK_LUT_DL_SHIELD_DEKU_ODD)
  writer.GoTo(0x7F2)
  writer.WriteModelDataLo(Offsets.CHILD_LINK_LUT_DL_SHIELD_DEKU_ODD)

  writer.SetBase('Player')
  writer.GoTo(0x2253C)
  writer.SetAdvance(4)
  writer.WriteModelData(Offsets.CHILD_LINK_LUT_DL_MASK_KEATON)
  writer.WriteModelData(Offsets.CHILD_LINK_LUT_DL_MASK_SKULL)
  writer.WriteModelData(Offsets.CHILD_LINK_LUT_DL_MASK_SPOOKY)
  writer.WriteModelData(Offsets.CHILD_LINK_LUT_DL_MASK_BUNNY)
  writer.WriteModelData(Offsets.CHILD_LINK_LUT_DL_MASK_GORON)
  writer.WriteModelData(Offsets.CHILD_LINK_LUT_DL_MASK_ZORA)
  writer.WriteModelData(Offsets.CHILD_LINK_LUT_DL_MASK_GERUDO)
  writer.WriteModelData(Offsets.CHILD_LINK_LUT_DL_MASK_TRUTH)

  writer.SetBase('GraveyardKid')
  writer.GoTo(0xE62)
  writer.WriteModelDataHi(Offsets.CHILD_LINK_LUT_DL_MASK_SPOOKY)
  writer.GoTo(0xE66)
  writer.WriteModelDataLo(Offsets.CHILD_LINK_LUT_DL_MASK_SPOOKY)

  writer.SetBase('Guard')
  writer.GoTo(0x1EA2)
  writer.WriteModelDataHi(Offsets.CHILD_LINK_LUT_DL_MASK_KEATON)
  writer.GoTo(0x1EA6)
  writer.WriteModelDataLo(Offsets.CHILD_LINK_LUT_DL_MASK_KEATON)

  writer.SetBase('RunningMan')
  writer.GoTo(0x1142)
  writer.WriteModelDataHi(Offsets.CHILD_LINK_LUT_DL_MASK_BUNNY)
  writer.GoTo(0x1146)
  writer.WriteModelDataLo(Offsets.CHILD_LINK_LUT_DL_MASK_BUNNY)
  */

  base = 0x00A87000 + 0xE65A4;
  patchPtrOot(patch, base, 0x0000, OOT_LINK_CHILD_OFFSETS.HIERARCHY);
}
