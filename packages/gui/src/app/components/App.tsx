import ReactGA from 'react-ga4';

import { Generator } from './generator/Generator';
import { Progress } from './Progress';
import { useStore } from '../store';

ReactGA.initialize('G-4S4Y8RTZ7T');

function App() {
  const { isGenerating, message, progress } = useStore(state => state.generator);

  if (isGenerating) {
    return (
      <div className="h-full -wfull flex items-center justify-center">
        <Progress message={message || ''} progress={progress}/>
      </div>
    );
  }

  return <Generator/>;
}

export default App;
