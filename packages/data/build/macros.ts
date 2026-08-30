import { loadYaml } from './helpers';

export async function parseMacros() {
  const [common, oot, mm] = await Promise.all([
    loadYaml('macros/macros_common.yml'),
    loadYaml('macros/macros_oot.yml'),
    loadYaml('macros/macros_mm.yml'),
  ]);

  return {
    common,
    oot,
    mm,
  };
}
