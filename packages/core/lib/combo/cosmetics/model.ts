import { Patchfile } from "../patch-build/patchfile";
import { RomBuilder } from "../rom-builder";

const OOT_FILES = {
  CODE:               0x00A87000,
  PLAYER:             0x00BCDB70,
  HOOK:               0x00CAD2C0,
  SHIELD:             0x00DB1F40,
  STICK:              0x00EAD0F0,
  GRAVEYARD_KID:      0x00E60920,
  GUARD:              0x00D1A690,
  RUNNING_MAN:        0x00E50440,
};

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

const OOT_LINK_ADULT_OFFSETS = {
  LUT_DL_WAIST: 0x06005090,
  LUT_DL_RTHIGH: 0x06005098,
  LUT_DL_RSHIN: 0x060050A0,
  LUT_DL_RFOOT: 0x060050A8,
  LUT_DL_LTHIGH: 0x060050B0,
  LUT_DL_LSHIN: 0x060050B8,
  LUT_DL_LFOOT: 0x060050C0,
  LUT_DL_HEAD: 0x060050C8,
  LUT_DL_HAT: 0x060050D0,
  LUT_DL_COLLAR: 0x060050D8,
  LUT_DL_LSHOULDER: 0x060050E0,
  LUT_DL_LFOREARM: 0x060050E8,
  LUT_DL_RSHOULDER: 0x060050F0,
  LUT_DL_RFOREARM: 0x060050F8,
  LUT_DL_TORSO: 0x06005100,
  LUT_DL_LHAND: 0x06005108,
  LUT_DL_LFIST: 0x06005110,
  LUT_DL_LHAND_BOTTLE: 0x06005118,
  LUT_DL_RHAND: 0x06005120,
  LUT_DL_RFIST: 0x06005128,
  LUT_DL_SWORD_SHEATH: 0x06005130,
  LUT_DL_SWORD_HILT: 0x06005138,
  LUT_DL_SWORD_BLADE: 0x06005140,
  LUT_DL_LONGSWORD_HILT: 0x06005148,
  LUT_DL_LONGSWORD_BLADE: 0x06005150,
  LUT_DL_LONGSWORD_BROKEN: 0x06005158,
  LUT_DL_SHIELD_HYLIAN: 0x06005160,
  LUT_DL_SHIELD_MIRROR: 0x06005168,
  LUT_DL_HAMMER: 0x06005170,
  LUT_DL_BOTTLE: 0x06005178,
  LUT_DL_BOW: 0x06005180,
  LUT_DL_OCARINA_TIME: 0x06005188,
  LUT_DL_HOOKSHOT: 0x06005190,
  LUT_DL_UPGRADE_LFOREARM: 0x06005198,
  LUT_DL_UPGRADE_LHAND: 0x060051A0,
  LUT_DL_UPGRADE_LFIST: 0x060051A8,
  LUT_DL_UPGRADE_RFOREARM: 0x060051B0,
  LUT_DL_UPGRADE_RHAND: 0x060051B8,
  LUT_DL_UPGRADE_RFIST: 0x060051C0,
  LUT_DL_BOOT_LIRON: 0x060051C8,
  LUT_DL_BOOT_RIRON: 0x060051D0,
  LUT_DL_BOOT_LHOVER: 0x060051D8,
  LUT_DL_BOOT_RHOVER: 0x060051E0,
  LUT_DL_FPS_LFOREARM: 0x060051E8,
  LUT_DL_FPS_LHAND: 0x060051F0,
  LUT_DL_FPS_RFOREARM: 0x060051F8,
  LUT_DL_FPS_RHAND: 0x06005200,
  LUT_DL_FPS_HOOKSHOT: 0x06005208,
  LUT_DL_HOOKSHOT_CHAIN: 0x06005210,
  LUT_DL_HOOKSHOT_HOOK: 0x06005218,
  LUT_DL_HOOKSHOT_AIM: 0x06005220,
  LUT_DL_BOW_STRING: 0x06005228,
  LUT_DL_BLADEBREAK: 0x06005230,
  LUT_DL_SWORD_SHEATHED: 0x06005238,
  LUT_DL_SHIELD_HYLIAN_BACK: 0x06005258,
  LUT_DL_SHIELD_MIRROR_BACK: 0x06005268,
  LUT_DL_SWORD_SHIELD_HYLIAN: 0x06005278,
  LUT_DL_SWORD_SHIELD_MIRROR: 0x06005288,
  LUT_DL_SHEATH0_HYLIAN: 0x06005298,
  LUT_DL_SHEATH0_MIRROR: 0x060052A8,
  LUT_DL_LFIST_SWORD: 0x060052B8,
  LUT_DL_LFIST_LONGSWORD: 0x060052D0,
  LUT_DL_LFIST_LONGSWORD_BROKEN: 0x060052E8,
  LUT_DL_LFIST_HAMMER: 0x06005300,
  LUT_DL_RFIST_SHIELD_HYLIAN: 0x06005310,
  LUT_DL_RFIST_SHIELD_MIRROR: 0x06005320,
  LUT_DL_RFIST_BOW: 0x06005330,
  LUT_DL_RFIST_HOOKSHOT: 0x06005340,
  LUT_DL_RHAND_OCARINA_TIME: 0x06005350,
  LUT_DL_FPS_RHAND_BOW: 0x06005360,
  LUT_DL_FPS_LHAND_HOOKSHOT: 0x06005370,
  HIERARCHY: 0x06005380,
};

function patchPtrHi(file: Buffer, offset: number, value: number) {
  const buf = Buffer.alloc(2);
  buf.writeUInt16BE(value >>> 16, 0);
  buf.copy(file, offset);
}

function patchPtrLo(file: Buffer, offset: number, value: number) {
  const buf = Buffer.alloc(2);
  buf.writeUInt16BE((value & 0xffff) >>> 0, 0);
  buf.copy(file, offset);
}

function patchPtr16(file: Buffer, offset: number, value: number) {
  const buf = Buffer.alloc(2);
  buf.writeUInt16BE(value, 0);
  buf.copy(file, offset);
}

function patchPtr(file: Buffer, offset: number, value: number) {
  const buf = Buffer.alloc(4);
  buf.writeUInt32BE(value, 0);
  buf.copy(file, offset);
}

export function enableModelOotLinkChild(builder: RomBuilder, dfAddr: number) {
  dfAddr |= 0x06000000;
  let base: number;

  /* Patch code */
  const fileCode = builder.fileByNameRequired('oot/code');
  const fileEffStick = builder.fileByNameRequired('oot/actors/ovl_Effect_Ss_Stick');
  const fileItemShield = builder.fileByNameRequired('oot/actors/ovl_Item_Shield'); /* Isn't this unused? */
  const filePlayer = builder.fileByNameRequired('oot/ovl_player_actor');
  const fileEnCs = builder.fileByNameRequired('oot/actors/ovl_En_Cs');
  const fileEnHeishi2 = builder.fileByNameRequired('oot/actors/ovl_En_Heishi2');
  const fileEnMm = builder.fileByNameRequired('oot/actors/ovl_En_Mm');

  base = 0xe671c;
  patchPtr(fileCode.data, base + 0x0000, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST);
  patchPtr(fileCode.data, base + 0x0008, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST);
  patchPtr(fileCode.data, base + 0x0010, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST);
  patchPtr(fileCode.data, base + 0x0018, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST);
  patchPtr(fileCode.data, base + 0x0010, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST_SHIELD_DEKU);
  patchPtr(fileCode.data, base + 0x0018, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST_SHIELD_DEKU);
  patchPtr(fileCode.data, base + 0x0020, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST);
  patchPtr(fileCode.data, base + 0x0028, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST);
  patchPtr(fileCode.data, base + 0x0030, dfAddr);
  patchPtr(fileCode.data, base + 0x0038, dfAddr);
  patchPtr(fileCode.data, base + 0x0040, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHEATHED);
  patchPtr(fileCode.data, base + 0x0048, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHEATHED);
  patchPtr(fileCode.data, base + 0x0050, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHIELD_DEKU);
  patchPtr(fileCode.data, base + 0x0058, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHIELD_DEKU);
  patchPtr(fileCode.data, base + 0x0060, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHIELD_HYLIAN);
  patchPtr(fileCode.data, base + 0x0068, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHIELD_HYLIAN);
  patchPtr(fileCode.data, base + 0x0070, dfAddr);
  patchPtr(fileCode.data, base + 0x0078, dfAddr);
  patchPtr(fileCode.data, base + 0x0080, 0x00000000);
  patchPtr(fileCode.data, base + 0x0088, 0x00000000);
  patchPtr(fileCode.data, base + 0x0090, OOT_LINK_CHILD_OFFSETS.LUT_DL_SHIELD_DEKU_BACK);
  patchPtr(fileCode.data, base + 0x0098, OOT_LINK_CHILD_OFFSETS.LUT_DL_SHIELD_DEKU_BACK);
  patchPtr(fileCode.data, base + 0x00a0, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHEATH);
  patchPtr(fileCode.data, base + 0x00a8, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHEATH);
  patchPtr(fileCode.data, base + 0x00b0, OOT_LINK_CHILD_OFFSETS.LUT_DL_SHEATH0_DEKU);
  patchPtr(fileCode.data, base + 0x00b8, OOT_LINK_CHILD_OFFSETS.LUT_DL_SHEATH0_DEKU);
  patchPtr(fileCode.data, base + 0x00c0, OOT_LINK_CHILD_OFFSETS.LUT_DL_SHEATH0_HYLIAN);
  patchPtr(fileCode.data, base + 0x00c8, OOT_LINK_CHILD_OFFSETS.LUT_DL_SHEATH0_HYLIAN);
  patchPtr(fileCode.data, base + 0x00d0, dfAddr);
  patchPtr(fileCode.data, base + 0x00d8, dfAddr);
  patchPtr(fileCode.data, base + 0x00e0, 0x00000000);
  patchPtr(fileCode.data, base + 0x00e8, 0x00000000);
  patchPtr(fileCode.data, base + 0x00f0, OOT_LINK_CHILD_OFFSETS.LUT_DL_SHIELD_DEKU_BACK);
  patchPtr(fileCode.data, base + 0x00f8, OOT_LINK_CHILD_OFFSETS.LUT_DL_SHIELD_DEKU_BACK);
  patchPtr(fileCode.data, base + 0x0100, OOT_LINK_CHILD_OFFSETS.LUT_DL_LHAND_PEDESTALSWORD);
  patchPtr(fileCode.data, base + 0x0108, OOT_LINK_CHILD_OFFSETS.LUT_DL_LHAND_PEDESTALSWORD);
  patchPtr(fileCode.data, base + 0x0110, OOT_LINK_CHILD_OFFSETS.LUT_DL_LHAND_PEDESTALSWORD);
  patchPtr(fileCode.data, base + 0x0118, OOT_LINK_CHILD_OFFSETS.LUT_DL_LHAND_PEDESTALSWORD);
  patchPtr(fileCode.data, base + 0x0120, OOT_LINK_CHILD_OFFSETS.LUT_DL_LHAND);
  patchPtr(fileCode.data, base + 0x0128, OOT_LINK_CHILD_OFFSETS.LUT_DL_LHAND);
  patchPtr(fileCode.data, base + 0x0130, OOT_LINK_CHILD_OFFSETS.LUT_DL_LFIST);
  patchPtr(fileCode.data, base + 0x0138, OOT_LINK_CHILD_OFFSETS.LUT_DL_LFIST);
  patchPtr(fileCode.data, base + 0x0140, OOT_LINK_CHILD_OFFSETS.LUT_DL_LFIST_SWORD);
  patchPtr(fileCode.data, base + 0x0148, OOT_LINK_CHILD_OFFSETS.LUT_DL_LFIST_SWORD);
  patchPtr(fileCode.data, base + 0x0150, OOT_LINK_CHILD_OFFSETS.LUT_DL_LFIST_SWORD);
  patchPtr(fileCode.data, base + 0x0158, OOT_LINK_CHILD_OFFSETS.LUT_DL_LFIST_SWORD);
  patchPtr(fileCode.data, base + 0x0160, OOT_LINK_CHILD_OFFSETS.LUT_DL_RHAND);
  patchPtr(fileCode.data, base + 0x0168, OOT_LINK_CHILD_OFFSETS.LUT_DL_RHAND);
  patchPtr(fileCode.data, base + 0x0170, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST);
  patchPtr(fileCode.data, base + 0x0178, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST);
  patchPtr(fileCode.data, base + 0x0180, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST_SLINGSHOT);
  patchPtr(fileCode.data, base + 0x0188, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST_SLINGSHOT);
  patchPtr(fileCode.data, base + 0x0190, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHEATHED);
  patchPtr(fileCode.data, base + 0x0198, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHEATHED);
  patchPtr(fileCode.data, base + 0x01a0, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHEATH);
  patchPtr(fileCode.data, base + 0x01a8, OOT_LINK_CHILD_OFFSETS.LUT_DL_SWORD_SHEATH);
  patchPtr(fileCode.data, base + 0x01b0, OOT_LINK_CHILD_OFFSETS.LUT_DL_WAIST);
  patchPtr(fileCode.data, base + 0x01b8, OOT_LINK_CHILD_OFFSETS.LUT_DL_WAIST);
  patchPtr(fileCode.data, base + 0x01c0, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST_SLINGSHOT);
  patchPtr(fileCode.data, base + 0x01c8, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST_SLINGSHOT);
  patchPtr(fileCode.data, base + 0x01d0, OOT_LINK_CHILD_OFFSETS.LUT_DL_RHAND_OCARINA_FAIRY);
  patchPtr(fileCode.data, base + 0x01d8, OOT_LINK_CHILD_OFFSETS.LUT_DL_RHAND_OCARINA_FAIRY);
  patchPtr(fileCode.data, base + 0x01e0, OOT_LINK_CHILD_OFFSETS.LUT_DL_RHAND_OCARINA_TIME);
  patchPtr(fileCode.data, base + 0x01e8, OOT_LINK_CHILD_OFFSETS.LUT_DL_RHAND_OCARINA_TIME);
  patchPtr(fileCode.data, base + 0x01f0, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST);
  patchPtr(fileCode.data, base + 0x01f8, OOT_LINK_CHILD_OFFSETS.LUT_DL_RFIST);
  patchPtr(fileCode.data, base + 0x0200, OOT_LINK_CHILD_OFFSETS.LUT_DL_LFIST);
  patchPtr(fileCode.data, base + 0x0208, OOT_LINK_CHILD_OFFSETS.LUT_DL_LFIST);
  patchPtr(fileCode.data, base + 0x0210, OOT_LINK_CHILD_OFFSETS.LUT_DL_LFIST_BOOMERANG);
  patchPtr(fileCode.data, base + 0x0218, OOT_LINK_CHILD_OFFSETS.LUT_DL_LFIST_BOOMERANG);
  patchPtr(fileCode.data, base + 0x0220, OOT_LINK_CHILD_OFFSETS.LUT_DL_LHAND_BOTTLE);
  patchPtr(fileCode.data, base + 0x0228, OOT_LINK_CHILD_OFFSETS.LUT_DL_LHAND_BOTTLE);
  patchPtr(fileCode.data, base + 0x0230, 0x00000000);
  patchPtr(fileCode.data, base + 0x0238, 0x00000000);
  patchPtr(fileCode.data, base + 0x0240, OOT_LINK_CHILD_OFFSETS.LUT_DL_RSHOULDER);
  patchPtr(fileCode.data, base + 0x0248, 0x00000000);
  patchPtr(fileCode.data, base + 0x0250, OOT_LINK_CHILD_OFFSETS.LUT_DL_FPS_RARM_SLINGSHOT);

  base = 0xe6b2c;
  patchPtr(fileCode.data, base + 0x0000, OOT_LINK_CHILD_OFFSETS.LUT_DL_BOTTLE);

  base = 0xe6b74;
  patchPtr(fileCode.data, base + 0x0000, OOT_LINK_CHILD_OFFSETS.LUT_DL_SLINGSHOT_STRING);
  patchPtr(fileCode.data, base + 0x0004, 0x44178000);
  patchPtr(fileCode.data, base + 0x0008, 0x436C0000);

  patchPtrHi(fileCode.data, 0x6922e, OOT_LINK_CHILD_OFFSETS.LUT_DL_GORON_BRACELET);
  patchPtrLo(fileCode.data, 0x69232, OOT_LINK_CHILD_OFFSETS.LUT_DL_GORON_BRACELET);
  patchPtrHi(fileCode.data, 0x6a80e, OOT_LINK_CHILD_OFFSETS.LUT_DL_DEKU_STICK);
  patchPtrLo(fileCode.data, 0x6a812, OOT_LINK_CHILD_OFFSETS.LUT_DL_DEKU_STICK);

  patchPtr(fileEffStick.data, 0x334, OOT_LINK_CHILD_OFFSETS.LUT_DL_DEKU_STICK);
  patchPtr16(fileEffStick.data, 0x330, 0x0015);

  patchPtrHi(fileItemShield.data, 0x7ee, OOT_LINK_CHILD_OFFSETS.LUT_DL_SHIELD_DEKU_ODD);
  patchPtrLo(fileItemShield.data, 0x7f2, OOT_LINK_CHILD_OFFSETS.LUT_DL_SHIELD_DEKU_ODD);

  base = 0x2253c;
  patchPtr(filePlayer.data, base + 0x0000, OOT_LINK_CHILD_OFFSETS.LUT_DL_MASK_KEATON);
  patchPtr(filePlayer.data, base + 0x0004, OOT_LINK_CHILD_OFFSETS.LUT_DL_MASK_SKULL);
  patchPtr(filePlayer.data, base + 0x0008, OOT_LINK_CHILD_OFFSETS.LUT_DL_MASK_SPOOKY);
  patchPtr(filePlayer.data, base + 0x000c, OOT_LINK_CHILD_OFFSETS.LUT_DL_MASK_BUNNY);
  patchPtr(filePlayer.data, base + 0x0010, OOT_LINK_CHILD_OFFSETS.LUT_DL_MASK_GORON);
  patchPtr(filePlayer.data, base + 0x0014, OOT_LINK_CHILD_OFFSETS.LUT_DL_MASK_ZORA);
  patchPtr(filePlayer.data, base + 0x0018, OOT_LINK_CHILD_OFFSETS.LUT_DL_MASK_GERUDO);
  patchPtr(filePlayer.data, base + 0x001c, OOT_LINK_CHILD_OFFSETS.LUT_DL_MASK_TRUTH);

  patchPtrHi(fileEnCs.data, 0xe62, OOT_LINK_CHILD_OFFSETS.LUT_DL_MASK_SPOOKY);
  patchPtrLo(fileEnCs.data, 0xe66, OOT_LINK_CHILD_OFFSETS.LUT_DL_MASK_SPOOKY);

  patchPtrHi(fileEnHeishi2.data, 0x1ea2, OOT_LINK_CHILD_OFFSETS.LUT_DL_MASK_KEATON);
  patchPtrLo(fileEnHeishi2.data, 0x1ea6, OOT_LINK_CHILD_OFFSETS.LUT_DL_MASK_KEATON);

  patchPtrHi(fileEnMm.data, 0x1142, OOT_LINK_CHILD_OFFSETS.LUT_DL_MASK_BUNNY);
  patchPtrLo(fileEnMm.data, 0x1146, OOT_LINK_CHILD_OFFSETS.LUT_DL_MASK_BUNNY);

  patchPtr(fileCode.data, 0xe65a4, OOT_LINK_CHILD_OFFSETS.HIERARCHY);
}

export function enableModelOotLinkAdult(builder: RomBuilder, dfAddr: number) {
  dfAddr |= 0x06000000;
  let base: number;

  const fileCode = builder.fileByNameRequired('oot/code');
  const fileArmsHook = builder.fileByNameRequired('oot/actors/ovl_Arms_Hook');
  const fileEffStick = builder.fileByNameRequired('oot/actors/ovl_Effect_Ss_Stick');

  base = 0xe6718;
  patchPtr(fileCode.data, base + 0x0000, OOT_LINK_ADULT_OFFSETS.LUT_DL_RFIST);
  patchPtr(fileCode.data, base + 0x0008, OOT_LINK_ADULT_OFFSETS.LUT_DL_RFIST);
  patchPtr(fileCode.data, base + 0x0010, dfAddr);
  patchPtr(fileCode.data, base + 0x0018, dfAddr);
  patchPtr(fileCode.data, base + 0x0020, OOT_LINK_ADULT_OFFSETS.LUT_DL_RFIST_SHIELD_HYLIAN);
  patchPtr(fileCode.data, base + 0x0028, OOT_LINK_ADULT_OFFSETS.LUT_DL_RFIST_SHIELD_HYLIAN);
  patchPtr(fileCode.data, base + 0x0030, OOT_LINK_ADULT_OFFSETS.LUT_DL_RFIST_SHIELD_MIRROR);
  patchPtr(fileCode.data, base + 0x0038, OOT_LINK_ADULT_OFFSETS.LUT_DL_RFIST_SHIELD_MIRROR);
  patchPtr(fileCode.data, base + 0x0040, OOT_LINK_ADULT_OFFSETS.LUT_DL_SWORD_SHEATHED);
  patchPtr(fileCode.data, base + 0x0048, OOT_LINK_ADULT_OFFSETS.LUT_DL_SWORD_SHEATHED);
  patchPtr(fileCode.data, base + 0x0050, dfAddr);
  patchPtr(fileCode.data, base + 0x0058, dfAddr);
  patchPtr(fileCode.data, base + 0x0060, OOT_LINK_ADULT_OFFSETS.LUT_DL_SWORD_SHIELD_HYLIAN);
  patchPtr(fileCode.data, base + 0x0068, OOT_LINK_ADULT_OFFSETS.LUT_DL_SWORD_SHIELD_HYLIAN);
  patchPtr(fileCode.data, base + 0x0070, OOT_LINK_ADULT_OFFSETS.LUT_DL_SWORD_SHIELD_MIRROR);
  patchPtr(fileCode.data, base + 0x0078, OOT_LINK_ADULT_OFFSETS.LUT_DL_SWORD_SHIELD_MIRROR);
  patchPtr(fileCode.data, base + 0x0080, 0x00000000);
  patchPtr(fileCode.data, base + 0x0088, 0x00000000);
  patchPtr(fileCode.data, base + 0x0090, 0x00000000);
  patchPtr(fileCode.data, base + 0x0098, 0x00000000);
  patchPtr(fileCode.data, base + 0x00a0, OOT_LINK_ADULT_OFFSETS.LUT_DL_SWORD_SHEATH);
  patchPtr(fileCode.data, base + 0x00a8, OOT_LINK_ADULT_OFFSETS.LUT_DL_SWORD_SHEATH);
  patchPtr(fileCode.data, base + 0x00b0, dfAddr);
  patchPtr(fileCode.data, base + 0x00b8, dfAddr);
  patchPtr(fileCode.data, base + 0x00c0, OOT_LINK_ADULT_OFFSETS.LUT_DL_SHEATH0_HYLIAN);
  patchPtr(fileCode.data, base + 0x00c8, OOT_LINK_ADULT_OFFSETS.LUT_DL_SHEATH0_HYLIAN);
  patchPtr(fileCode.data, base + 0x00d0, OOT_LINK_ADULT_OFFSETS.LUT_DL_SHEATH0_MIRROR);
  patchPtr(fileCode.data, base + 0x00d8, OOT_LINK_ADULT_OFFSETS.LUT_DL_SHEATH0_MIRROR);
  patchPtr(fileCode.data, base + 0x00e0, 0x00000000);
  patchPtr(fileCode.data, base + 0x00e8, 0x00000000);
  patchPtr(fileCode.data, base + 0x00f0, 0x00000000);
  patchPtr(fileCode.data, base + 0x00f8, 0x00000000);
  patchPtr(fileCode.data, base + 0x0100, OOT_LINK_ADULT_OFFSETS.LUT_DL_LFIST_LONGSWORD);
  patchPtr(fileCode.data, base + 0x0108, OOT_LINK_ADULT_OFFSETS.LUT_DL_LFIST_LONGSWORD);
  patchPtr(fileCode.data, base + 0x0110, OOT_LINK_ADULT_OFFSETS.LUT_DL_LFIST_LONGSWORD_BROKEN);
  patchPtr(fileCode.data, base + 0x0118, OOT_LINK_ADULT_OFFSETS.LUT_DL_LFIST_LONGSWORD_BROKEN);
  patchPtr(fileCode.data, base + 0x0120, OOT_LINK_ADULT_OFFSETS.LUT_DL_LHAND);
  patchPtr(fileCode.data, base + 0x0128, OOT_LINK_ADULT_OFFSETS.LUT_DL_LHAND);
  patchPtr(fileCode.data, base + 0x0130, OOT_LINK_ADULT_OFFSETS.LUT_DL_LFIST);
  patchPtr(fileCode.data, base + 0x0138, OOT_LINK_ADULT_OFFSETS.LUT_DL_LFIST);
  patchPtr(fileCode.data, base + 0x0140, dfAddr);
  patchPtr(fileCode.data, base + 0x0148, dfAddr);
  patchPtr(fileCode.data, base + 0x0150, OOT_LINK_ADULT_OFFSETS.LUT_DL_LFIST_SWORD);
  patchPtr(fileCode.data, base + 0x0158, OOT_LINK_ADULT_OFFSETS.LUT_DL_LFIST_SWORD);
  patchPtr(fileCode.data, base + 0x0160, OOT_LINK_ADULT_OFFSETS.LUT_DL_RHAND);
  patchPtr(fileCode.data, base + 0x0168, OOT_LINK_ADULT_OFFSETS.LUT_DL_RHAND);
  patchPtr(fileCode.data, base + 0x0170, OOT_LINK_ADULT_OFFSETS.LUT_DL_RFIST);
  patchPtr(fileCode.data, base + 0x0178, OOT_LINK_ADULT_OFFSETS.LUT_DL_RFIST);
  patchPtr(fileCode.data, base + 0x0180, OOT_LINK_ADULT_OFFSETS.LUT_DL_RFIST_BOW);
  patchPtr(fileCode.data, base + 0x0188, OOT_LINK_ADULT_OFFSETS.LUT_DL_RFIST_BOW);
  patchPtr(fileCode.data, base + 0x0190, OOT_LINK_ADULT_OFFSETS.LUT_DL_SWORD_SHEATHED);
  patchPtr(fileCode.data, base + 0x0198, OOT_LINK_ADULT_OFFSETS.LUT_DL_SWORD_SHEATHED);
  patchPtr(fileCode.data, base + 0x01a0, OOT_LINK_ADULT_OFFSETS.LUT_DL_SWORD_SHEATH);
  patchPtr(fileCode.data, base + 0x01a8, OOT_LINK_ADULT_OFFSETS.LUT_DL_SWORD_SHEATH);
  patchPtr(fileCode.data, base + 0x01b0, OOT_LINK_ADULT_OFFSETS.LUT_DL_WAIST);
  patchPtr(fileCode.data, base + 0x01b8, OOT_LINK_ADULT_OFFSETS.LUT_DL_WAIST);
  patchPtr(fileCode.data, base + 0x01c0, OOT_LINK_ADULT_OFFSETS.LUT_DL_RFIST_BOW);
  patchPtr(fileCode.data, base + 0x01c8, OOT_LINK_ADULT_OFFSETS.LUT_DL_RFIST_BOW);
  patchPtr(fileCode.data, base + 0x01d0, OOT_LINK_ADULT_OFFSETS.LUT_DL_RHAND_OCARINA_TIME);
  patchPtr(fileCode.data, base + 0x01d8, OOT_LINK_ADULT_OFFSETS.LUT_DL_RHAND_OCARINA_TIME);
  patchPtr(fileCode.data, base + 0x01e0, OOT_LINK_ADULT_OFFSETS.LUT_DL_RHAND_OCARINA_TIME);
  patchPtr(fileCode.data, base + 0x01e8, OOT_LINK_ADULT_OFFSETS.LUT_DL_RHAND_OCARINA_TIME);
  patchPtr(fileCode.data, base + 0x01f0, OOT_LINK_ADULT_OFFSETS.LUT_DL_RFIST_HOOKSHOT);
  patchPtr(fileCode.data, base + 0x01f8, OOT_LINK_ADULT_OFFSETS.LUT_DL_RFIST_HOOKSHOT);
  patchPtr(fileCode.data, base + 0x0200, OOT_LINK_ADULT_OFFSETS.LUT_DL_LFIST_HAMMER);
  patchPtr(fileCode.data, base + 0x0208, OOT_LINK_ADULT_OFFSETS.LUT_DL_LFIST_HAMMER);
  patchPtr(fileCode.data, base + 0x0210, dfAddr);
  patchPtr(fileCode.data, base + 0x0218, dfAddr);
  patchPtr(fileCode.data, base + 0x0220, OOT_LINK_ADULT_OFFSETS.LUT_DL_LHAND_BOTTLE);
  patchPtr(fileCode.data, base + 0x0228, OOT_LINK_ADULT_OFFSETS.LUT_DL_LHAND_BOTTLE);
  patchPtr(fileCode.data, base + 0x0230, OOT_LINK_ADULT_OFFSETS.LUT_DL_FPS_LFOREARM);
  patchPtr(fileCode.data, base + 0x0238, OOT_LINK_ADULT_OFFSETS.LUT_DL_FPS_LHAND);
  patchPtr(fileCode.data, base + 0x0240, OOT_LINK_ADULT_OFFSETS.LUT_DL_RSHOULDER);
  patchPtr(fileCode.data, base + 0x0248, OOT_LINK_ADULT_OFFSETS.LUT_DL_FPS_RFOREARM);
  patchPtr(fileCode.data, base + 0x0250, OOT_LINK_ADULT_OFFSETS.LUT_DL_FPS_RHAND_BOW);

  base = 0xe6a4c;
  patchPtr(fileCode.data, base + 0x0000, OOT_LINK_ADULT_OFFSETS.LUT_DL_BOOT_LIRON);
  patchPtr(fileCode.data, base + 0x0004, OOT_LINK_ADULT_OFFSETS.LUT_DL_BOOT_RIRON);
  patchPtr(fileCode.data, base + 0x0008, OOT_LINK_ADULT_OFFSETS.LUT_DL_BOOT_LHOVER);
  patchPtr(fileCode.data, base + 0x000c, OOT_LINK_ADULT_OFFSETS.LUT_DL_BOOT_RHOVER);

  patchPtr(fileCode.data, 0xe6b28, OOT_LINK_ADULT_OFFSETS.LUT_DL_BOTTLE);

  base = 0xe6b64;
  patchPtr(fileCode.data, base + 0x0000, OOT_LINK_ADULT_OFFSETS.LUT_DL_BOW_STRING);
  patchPtr(fileCode.data, base + 0x0004, 0x00000000);
  patchPtr(fileCode.data, base + 0x0008, 0xC3B43333);

  patchPtrHi(fileCode.data, 0x69112, OOT_LINK_ADULT_OFFSETS.LUT_DL_UPGRADE_LFOREARM);
  patchPtrLo(fileCode.data, 0x69116, OOT_LINK_ADULT_OFFSETS.LUT_DL_UPGRADE_LFOREARM);
  patchPtrHi(fileCode.data, 0x6912E, OOT_LINK_ADULT_OFFSETS.LUT_DL_UPGRADE_RFOREARM);
  patchPtrLo(fileCode.data, 0x69132, OOT_LINK_ADULT_OFFSETS.LUT_DL_UPGRADE_RFOREARM);
  patchPtrHi(fileCode.data, 0x6914E, OOT_LINK_ADULT_OFFSETS.LUT_DL_UPGRADE_LFIST);
  patchPtrLo(fileCode.data, 0x69162, OOT_LINK_ADULT_OFFSETS.LUT_DL_UPGRADE_LFIST);
  patchPtrHi(fileCode.data, 0x69166, OOT_LINK_ADULT_OFFSETS.LUT_DL_UPGRADE_LHAND);
  patchPtrLo(fileCode.data, 0x69172, OOT_LINK_ADULT_OFFSETS.LUT_DL_UPGRADE_LHAND);
  patchPtrHi(fileCode.data, 0x6919E, OOT_LINK_ADULT_OFFSETS.LUT_DL_UPGRADE_RFIST);
  patchPtrLo(fileCode.data, 0x691A2, OOT_LINK_ADULT_OFFSETS.LUT_DL_UPGRADE_RFIST);
  patchPtrHi(fileCode.data, 0x691AE, OOT_LINK_ADULT_OFFSETS.LUT_DL_UPGRADE_RHAND);
  patchPtrLo(fileCode.data, 0x691B2, OOT_LINK_ADULT_OFFSETS.LUT_DL_UPGRADE_RHAND);
  patchPtrHi(fileCode.data, 0x69DEA, OOT_LINK_ADULT_OFFSETS.LUT_DL_FPS_LHAND_HOOKSHOT);
  patchPtrLo(fileCode.data, 0x69DEE, OOT_LINK_ADULT_OFFSETS.LUT_DL_FPS_LHAND_HOOKSHOT);
  patchPtrHi(fileCode.data, 0x6A666, OOT_LINK_ADULT_OFFSETS.LUT_DL_HOOKSHOT_AIM);
  patchPtrLo(fileCode.data, 0x6A66A, OOT_LINK_ADULT_OFFSETS.LUT_DL_HOOKSHOT_AIM);

  patchPtrHi(fileArmsHook.data, 0xa72, OOT_LINK_ADULT_OFFSETS.LUT_DL_HOOKSHOT_HOOK);
  patchPtrLo(fileArmsHook.data, 0xa76, OOT_LINK_ADULT_OFFSETS.LUT_DL_HOOKSHOT_HOOK);
  patchPtrHi(fileArmsHook.data, 0xb66, OOT_LINK_ADULT_OFFSETS.LUT_DL_HOOKSHOT_CHAIN);
  patchPtrLo(fileArmsHook.data, 0xb6a, OOT_LINK_ADULT_OFFSETS.LUT_DL_HOOKSHOT_CHAIN);

  patchPtr(fileEffStick.data, 0x32C, OOT_LINK_ADULT_OFFSETS.LUT_DL_BLADEBREAK);
  patchPtr16(fileEffStick.data, 0x328, 0x0014);

  patchPtr(fileCode.data, 0xe65a0, OOT_LINK_ADULT_OFFSETS.HIERARCHY);
}
