type MonitorOnProgress = (progress: number, total: number) => void;
type MonitorMessage = (log: string) => void;

export type MonitorCallbacks = {
  onProgress?: MonitorOnProgress;
  onLog?: MonitorMessage;
  onWarn?: MonitorMessage;
};

export class Monitor {
  private onLog: MonitorMessage;
  private onWarn: MonitorMessage;
  private onProgress: MonitorOnProgress;
  private percent = 0;
  private lastMsg: string | null = null;

  constructor(callbacks: MonitorCallbacks, private isDebug?: boolean) {
    this.onLog = callbacks.onLog || (x => console.log(x));
    this.onProgress = callbacks.onProgress || ((progress: number, total: number) => { this.defaultProgress(progress, total); });
    this.onWarn = callbacks.onWarn || (x => console.warn(x));
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

  warn(message: string) {
    this.onWarn(message);
  }
}
