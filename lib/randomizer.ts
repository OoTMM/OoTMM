import { logic } from "./logic";

export const randomize = async (rom: Buffer) => {
  console.log("Randomizing...");
  const res = await logic();
  return res.log;
}
