import React from 'react';

import { GeneratorContextProvider, useGenerator } from '../contexts/GeneratorContext';
import { Generator } from './Generator';
import { Progress } from './Progress';
import { Result } from './Result';

function AppContent() {
  const { isGenerating, message, result } = useGenerator();

  if (result) {
    return <Result result={result}/>
  }
  if (isGenerating) {
    return <Progress message={message || ''}/>
  }

  return <Generator/>;
}

export const App = () =>
  <GeneratorContextProvider>
    <AppContent/>
  </GeneratorContextProvider>;
