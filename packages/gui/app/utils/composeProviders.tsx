import { ReactNode, ComponentType } from 'react';

type ProviderProps = { children: ReactNode };

export function composeProviders(providers: ComponentType<ProviderProps>[]) {
  return function ComposedProvider({ children }: ProviderProps) {
    return providers.reduceRight(
      (acc, Provider) => <Provider>{acc}</Provider>,
      children
    );
  };
}
