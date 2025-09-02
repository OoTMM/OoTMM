import { ReactNode } from 'react';

import { Header } from './Header';
import { Container } from './Container';
import { useLocation } from 'preact-iso';

function LayoutContainer({ children }: { children: ReactNode }) {
  const loc = useLocation();
  const noContainer = loc.path.startsWith('/gen/') && loc.path !== '/gen/old';

  if (noContainer) {
    return <>{children}</>;
  }

  return (
    <Container>
      {children}
    </Container>
  );
}

export function Layout({ children }: { children: ReactNode }) {
  return (
    <div className="flex flex-col h-screen">
      <Header/>
      <LayoutContainer>
        {children}
      </LayoutContainer>
    </div>
  );
}
