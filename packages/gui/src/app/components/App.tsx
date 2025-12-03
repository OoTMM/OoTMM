import ReactGA from 'react-ga4';

import { Generator } from './generator/Generator';
import { Progress } from './Progress';
import { composeProviders } from '../utils/composeProviders';
import { GeneratorContextProvider, useGenerator } from '../contexts/GeneratorContext';

ReactGA.initialize('G-4S4Y8RTZ7T');

const AppProviders = composeProviders([
  GeneratorContextProvider,
]);

function AppContent() {
  const { isGenerating, message, progress } = useGenerator();

  if (isGenerating) {
    return (
      <div className="h-full -wfull flex items-center justify-center">
        <Progress message={message || ''} progress={progress}/>
      </div>
    );
  }

  return <Generator/>;
}

const App = () =>
  <AppProviders>
    <AppContent/>
  </AppProviders>;

export default App;
