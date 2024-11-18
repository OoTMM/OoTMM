type ConfigEntryGenerator = {
  type: 'iframe';
  path: string;
};

type ConfigEntry = {
  generator?: ConfigEntryGenerator;
};

type Config = Record<string, ConfigEntry>;

const config: Config;
export default config;
