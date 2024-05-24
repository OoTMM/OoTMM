import React from 'react';
import ReactGA from 'react-ga4';

import { GeneratorContextProvider, useGenerator } from '../contexts/GeneratorContext';
import { Generator } from './Generator';
import { Progress } from './Progress';

ReactGA.initialize('G-4S4Y8RTZ7T');

function AppContent() {
  const { isGenerating, message, progress } = useGenerator();

  if (isGenerating) {
    return <div className="container"><Progress message={message || ''} progress={progress}/></div>;
  }

  return <Generator/>;
}

export const App = () =>
  <GeneratorContextProvider>
    <AppContent/>
  </GeneratorContextProvider>;
