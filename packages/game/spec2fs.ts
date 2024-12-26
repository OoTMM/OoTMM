import { promises as fs } from 'fs';
import { exec } from 'child_process';

async function getSymbols(elfFilename: string): Promise<Map<string, number>> {
  const output = await new Promise<string>((resolve, reject) => {
    exec(`nm ${elfFilename}`, { maxBuffer: 1024 * 1024 * 32 }, (err, stdout, stderr) => {
      if (err) {
        reject(err);
      } else {
        resolve(stdout);
      }
    });
  });

  const symbols = new Map<string, number>();
  for (let line of output.split("\n")) {
    const parts = line.split(" ");
    if (parts.length === 3) {
      const addr = parseInt(parts[0], 16);
      const name = parts[2];
      symbols.set(name, addr);
    }
  }

  return symbols;
}

function findSymbol(symbols: Map<string, number>, name: string): number {
  if (!symbols.has(name)) {
    throw new Error(`Symbol not found: ${name}`);
  }
  return symbols.get(name)!;
}

async function run() {
  let segs: any[] = [];
  const specFilename = process.argv[2];
  const elfFilename = process.argv[3];
  const outFilename = process.argv[4];
  const specLines = (await fs.readFile(specFilename, 'utf8')).split("\n");
  const symbols = await getSymbols(elfFilename);

  let name = "";
  let noload = false;
  let compressed = false;
  let syms = false;

  for (let line of specLines) {
    line = line.trim();
    if (line === 'beginseg') {
      name = "";
      compressed = false;
      noload = false;
      syms = false;
    } else if (line.startsWith("name")) {
      name = line.split(" ")[1].substring(1, line.split(" ")[1].length - 1);
    } else if (line.startsWith("flags")) {
      if (line.includes("NOLOAD")) {
        noload = true;
      }
      if (line.includes("SYMS")) {
        syms = true;
      }
    } else if (line === "compress") {
      compressed = true;
    } else if (line === "endseg") {
      if (!noload) {
        const start = findSymbol(symbols, `_${name}SegmentRomStart`);
        const end = findSymbol(symbols, `_${name}SegmentRomEnd`);
        const size = end - start;
        segs.push({ name, compressed, syms, start, size });
      }
    }
  }

  const bootproc = findSymbol(symbols, "bootproc");
  const bootram = findSymbol(symbols, "_bootSegmentStart");
  const loaderStart = findSymbol(symbols, "_loaderSegmentStart");
  const loaderOffsets = {
    configs: findSymbol(symbols, "gLoaderGameConfigs") - loaderStart,
  };
  const game = {
    bootproc,
    ram: bootram,
  };

  await fs.writeFile(outFilename, JSON.stringify({ game, loaderOffsets, files: segs }));
}

run().catch((err) => {
  console.error(err);
  process.exit(1);
});
