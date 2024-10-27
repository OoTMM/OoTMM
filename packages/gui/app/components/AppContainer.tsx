import { lazy, Suspense } from 'preact/compat';

import AppSkeleton from './AppSkeleton';
const App = lazy(() => import('./App'));

export default function AppContainer() {
  return (
    <Suspense fallback={<AppSkeleton/>}>
      <App/>
    </Suspense>
  );
}
