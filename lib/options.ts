export type Options = {
  debug: boolean;
  seed?: string;
};

export const makeOptions = (args: string[]): Options => {
  const opts: Options = {
    debug: false
  };
  for (let i = 0; i < args.length; i++) {
    const opt = args[i];
    switch (opt) {
    case "--debug":
      opts.debug = true;
      break;
    case "--seed":
      opts.seed = args[++i];
      break;
    default:
      throw new Error(`Unknown option: ${opt}`);
    }
  }
  return opts;
};
