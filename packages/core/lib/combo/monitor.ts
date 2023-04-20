export type MonitorCallbacks = {
  onProgress?: (progress: number, total: number) => void;
  onLog?: (log: string) => void;
};

export class Monitor {
  private progress = 0;
  private total = 0;

  constructor(private callbacks: MonitorCallbacks, private isDebug?: boolean) {
  }

  setProgress(progress: number, total: number) {
    this.progress = progress;
    this.total = total;
    if (this.callbacks.onProgress) {
      this.callbacks.onProgress(progress, total);
    }
  }

  log(message: string) {
    if (this.callbacks.onLog) {
      this.callbacks.onLog(message);
    } else {
      console.log(message);
    }
  }

  debug(message: string) {
    if (this.isDebug) {
      this.log(message);
    }
  }
}
