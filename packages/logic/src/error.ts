export class LogicError extends Error {
  constructor(message: string) {
    super(message);
    this.name = "LogicError";
  }
};

export class LogicEntranceError extends LogicError {
  constructor(message: string) {
    super(message);
    this.name = "LogicEntranceError";
  }
};

export class LogicSeedError extends LogicError {
  constructor(message: string) {
    super(message);
    this.name = "LogicSeedError";
  }
};
