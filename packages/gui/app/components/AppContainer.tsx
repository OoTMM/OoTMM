import React from 'react';

import AppSkeleton from './AppSkeleton';

const App = React.lazy(() => import('./App'));

export default function AppContainer() {
  return (
    <React.Suspense fallback={<AppSkeleton/>}>
      <App/>
    </React.Suspense>
  );
}
