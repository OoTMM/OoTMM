export class LogWriter {
  private buffer: string[];
  private indentLevel: number;

  constructor() {
    this.buffer = [];
    this.indentLevel = 0;
  }

  indent(str?: string) {
    if (str !== undefined) {
      this.write(str);
    }
    this.indentLevel++;
  }

  unindent(str?: string) {
    this.indentLevel--;
    if (this.indentLevel < 0) {
      throw new Error('unindent() called too many times');
    }
    if (str !== undefined) {
      this.write(str);
    }
  }

  write(str: string) {
    if (str === '') {
      this.buffer.push('');
      return;
    }
    this.buffer.push('  '.repeat(this.indentLevel) + str);
  }

  writeSectionHeader() {
    this.buffer.push('='.repeat(75));
  }

  emit() {
    return this.buffer.join("\n");
  }
}
