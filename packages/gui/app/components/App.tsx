import React from 'react';

import { GeneratorContextProvider, useGenerator } from '../contexts/GeneratorContext';
import { Generator } from './Generator';
import { Progress } from './Progress';
import { Result } from './Result';

function AppContent() {
  const { isGenerating, message } = useGenerator();

  if (isGenerating) {
    return <div className="container"><Progress message={message || ''}/></div>;
  }

  return <Generator/>;
}

export const App = () =>
  <GeneratorContextProvider>
    <AppContent/>
  </GeneratorContextProvider>;
