import { build } from "./build";
import { codegen } from "./codegen";
import { custom } from "./custom";
import { decompressGames } from "./decompress";
import { Options } from "./options";
import { pack } from "./pack";

export class Generator {
  constructor(
    private oot: string | Buffer,
    private mm: string | Buffer,
    private opts: Options
  ) {
  }

  async run() {
    await decompressGames();
    await codegen();
    await custom();
    await build(this.opts);
    await pack(this.opts);
  }
};
