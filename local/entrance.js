const num = parseInt(process.argv[2], 16);
console.log(`0x${((num - 3) << 9).toString(16)}`);
