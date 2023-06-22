export type Region = string & { __brand: 'Region' };

/* Regions registry */
type RegionDescriptor = {
  id: string;
  world: number;
}
const registry: {[k: Region]: RegionDescriptor} = {};

export function makeRegion(id: string, world: number): Region {
  const x = `${id}@${world}` as Region;
  registry[x] = { id, world };
  return x;
}

export function regionData(region: Region): RegionDescriptor {
  return registry[region];
}
