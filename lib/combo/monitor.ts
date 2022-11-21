export type MonitorCallbackParams = {
  message: string;
  progress: number;
  total: number;
};

export type MonitorConfig = {
  log: (msg: string) => void;
};

export type MonitorCallback = (params: MonitorCallbackParams) => void;

export class Monitor {
  private progress = 0;
  private total = 0;

  constructor(private callback: MonitorCallback) {
    this.callback = callback;
  }

  setProgress(progress: number, total: number) {
    this.progress = progress;
    this.total = total;
  }
}
