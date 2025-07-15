import ReactGA from 'react-ga4';

import { GeneratorContextProvider, useGenerator } from '../contexts/GeneratorContext';
import { Generator } from './Generator';
import { Progress } from './Progress';
import { composeProviders } from '../utils/composeProviders';
import { CosmeticsProvider } from '../contexts/CosmeticsContext';
import { RandomSettingsProvider } from '../contexts/RandomSettingsContext';

ReactGA.initialize('G-4S4Y8RTZ7T');

const AppProviders = composeProviders([
  RandomSettingsProvider,
  CosmeticsProvider,
  GeneratorContextProvider,
]);

function AppContent() {
  const { isGenerating, message, progress } = useGenerator();

  if (isGenerating) {
    return <div className="container"><Progress message={message || ''} progress={progress}/></div>;
  }

  return <Generator/>;
}

const App = () =>
  <AppProviders>
    <AppContent/>
  </AppProviders>;

export default App;
