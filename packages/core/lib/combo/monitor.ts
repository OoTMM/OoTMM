type MonitorOnProgress = (progress: number, total: number) => void;
type MonitorOnLog = (log: string) => void;

export type MonitorCallbacks = {
  onProgress?: MonitorOnProgress;
  onLog?: MonitorOnLog;
};

export class Monitor {
  private onLog: MonitorOnLog;
  private onProgress: MonitorOnProgress;
  private percent = 0;
  private lastMsg: string | null = null;

  constructor(callbacks: MonitorCallbacks, private isDebug?: boolean) {
    this.onLog = callbacks.onLog || (x => console.log(x));
    this.onProgress = callbacks.onProgress || ((progress: number, total: number) => { this.defaultProgress(progress, total); });
  }

  setProgress(progress: number, total: number) {
    this.onProgress(progress, total);
  }

  defaultProgress(progress: number, total: number) {
    const newPercent = Math.floor(progress / total * 100);
    if (newPercent !== this.percent) {
      this.percent = newPercent;
      if (this.lastMsg) {
        this.onLog(`${this.lastMsg} - ${this.percent}%`);
      }
    }
  }

  log(message: string) {
    this.percent = 0;
    this.lastMsg = message;
    this.onLog(message);
  }

  debug(message: string) {
    if (this.isDebug) {
      this.onLog(message);
    }
  }
}
