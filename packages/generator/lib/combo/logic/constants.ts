declare const __brand: unique symbol;
export type Brand<T, U> = T & { readonly __brand: U };

export const AGE_CHILD = 0 as Brand<0, 'Age'>;
export const AGE_ADULT = 1 as Brand<1, 'Age'>;
export type Age = typeof AGE_CHILD | typeof AGE_ADULT;
export const AGES: readonly Age[] = [AGE_CHILD, AGE_ADULT];
