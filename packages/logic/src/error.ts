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

export class EnemyCompatibilityError extends Error {
  constructor(readonly failures: import('./pathfind').EnemyCompatibilityFailure[], readonly worlds: import('./world').World[]) {
    super('Enemy replacements are incompatible with All Locations logic');
    this.name = 'EnemyCompatibilityError';
  }
}
