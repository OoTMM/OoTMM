export const COSMETICS = [{
  key: 'defaultHold',
  name: 'Default Hold Target',
  description: 'If enabled the targetting setting will be set to "Hold"',
  type: 'boolean',
  default: false,
}, {
  key: 'applyFreestandings',
  name: 'Apply Cosmetics to Freestanding Items',
  description: 'If enabled, items that are modified by cosmetics will also appear modified when they are freestanding in the world.<br>Currently only applies to tunics and mirror shield.',
  type: 'boolean',
  default: true,
}, {
  key: 'musicNames',
  name: 'Display Music Names',
  type: 'boolean',
  default: true,
}, {
  key: 'nightBgm',
  name: 'Enable Nighttime BGM (Experimental!)',
  description: 'Enable the background music at night time.<br>May cause crashes on Everdrive.',
  type: 'boolean',
  default: false,
}, {
  key: 'noLowHealthBeep',
  name: 'Remove Low Health Beep',
  description: 'Disable the low health warning beep sound.',
  type: 'boolean',
  default: false,
}, {
  key: 'ootTunicKokiri',
  name: 'OoT Kokiri Tunic',
  type: 'color',
}, {
  key: 'ootTunicGoron',
  name: 'OoT Goron Tunic',
  type: 'color',
}, {
  key: 'ootTunicZora',
  name: 'OoT Zora Tunic',
  type: 'color',
}, {
  key: 'ootShieldMirror',
  name: 'OoT Mirror Shield',
  type: 'color',
}, {
  key: 'mmTunicHuman',
  name: 'MM Human Tunic',
  type: 'color',
}, {
  key: 'mmTunicDeku',
  name: 'MM Deku Tunic',
  type: 'color',
}, {
  key: 'mmTunicGoron',
  name: 'MM Goron Tunic',
  type: 'color',
}, {
  key: 'mmTunicZora',
  name: 'MM Zora Tunic',
  type: 'color',
}, {
  key: 'mmTunicFierceDeity',
  name: 'MM Fierce Deity Tunic',
  type: 'color',
}, {
  key: 'dpad',
  name: 'D-Pad',
  type: 'color',
}, {
  key: 'fileSelect',
  name: 'File Select',
  type: 'color',
}, {
  key: 'modelOotChildLink',
  name: 'Player Model - Child Link (OoT)',
  type: 'file',
  ext: 'zobj',
}, {
  key: 'modelOotAdultLink',
  name: 'Player Model - Adult Link (OoT)',
  type: 'file',
  ext: 'zobj',
}, {
  key: 'music',
  name: 'Custom Music ZIP',
  type: 'file',
  ext: 'zip',
},] as const;
