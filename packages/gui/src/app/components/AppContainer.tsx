import { lazy, Suspense } from 'react';

import AppSkeleton from './AppSkeleton';
const App = lazy(() => import('./App'));

export default function AppContainer() {
  return (
    <Suspense fallback={<AppSkeleton/>}>
      <App/>
    </Suspense>
  );
}
