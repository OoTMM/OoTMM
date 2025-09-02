export const COSMETICS = [{
  key: 'defaultHold',
  name: 'Default Hold Target',
  type: 'boolean',
  default: false,
}, {
  key: 'applyFreestandings',
  name: 'Apply Cosmetics to Freestanding Items',
  type: 'boolean',
  default: true,
}, {
  key: 'musicNames',
  name: 'Display Music Names',
  type: 'boolean',
  default: true,
// }, {
//   key: 'nightBgm',
//   name: 'Enable Nighttime BGM',
//   type: 'boolean',
//   default: false,
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
