import { ReactNode } from 'react';

export function Container({ children }: { children: ReactNode }) {
  return (
    <main className="w-[1024px] ml-auto mr-auto mt-4">
      {children}
    </main>
  );
}
