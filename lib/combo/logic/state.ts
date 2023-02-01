export const AGES = ['child', 'adult'] as const;

export type Items = {[k: string]: number};
export type Age = typeof AGES[number];

export type State = {
  items: Items;
  age: Age;
  events: Set<string>;
  ignoreItems: boolean;
};
