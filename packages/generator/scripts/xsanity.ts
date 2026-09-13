import { run } from './xsanity/main';

run().catch(e => {
  console.error(e);
  process.exit(1);
});
