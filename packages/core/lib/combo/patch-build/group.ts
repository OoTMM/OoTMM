export const PATCH_GROUPS = [
  'MM_FD_ANYWHERE',
  'MM_HOOKSHOT_ANYWHERE',
] as const;

export type PatchGroup = typeof PATCH_GROUPS[number];

export const PATCH_GROUP_VALUES: {[k: string]: number} = {};

for (let i = 0; i < PATCH_GROUPS.length; i++) {
  const k = PATCH_GROUPS[i];
  PATCH_GROUP_VALUES[k] = i + 1;
};
