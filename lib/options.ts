export type Options = {
  debug: boolean;
};

export const makeOptions = (args: string[]): Options => {
  const opts: Options = {
    debug: false,
  };
  for (let i = 0; i < args.length; i++) {
    const opt = args[i];
    if (opt === '--debug') {
      opts.debug = true;
    } else {
      throw new Error(`Unknown option: ${opt}`);
    }
  }
  return opts;
};
