import type { LogicPassSolverState, SolverState } from './types';
import { ItemPlacer } from './place-items';

type Placers = {
  item: ItemPlacer;
};

class Solver {
  private state: SolverState;
  private placers: Placers;

  constructor(private input: LogicPassSolverState) {
    this.state = {
      startingItems: new Map(),
      items: new Map(),
      pools: {
        extra: new Map(),
        required: new Map(),
        nice: new Map(),
        junk: new Map(),
        nothing: new Map(),
      },
      criticalRenewables: new Set(),
      placedCount: 0,
    };
    this.placers = {
      item: new ItemPlacer(input, this.state),
    }
  }

  run() {
    this.input.monitor.log('Logic: Solve');
    this.placers.item.init();

    for (;;) {
      this.placers.item.run();
    }
  }
}

export function logicPassSolver(input: LogicPassSolverState) {
  const solver = new Solver(input);
  return solver.run();
}

export function logicPassSolverValidate(_input: LogicPassSolverState) {
}
