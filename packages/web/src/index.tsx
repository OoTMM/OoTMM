import { render } from 'preact';
import { LocationProvider, Router, Route } from 'preact-iso';

import { Header } from './app/components/Header';
import { Home } from './app/pages/Home';
import { Generator } from './app/pages/Generator';
import { GeneratorVersions } from './app/pages/GeneratorVersions';
import { NotFound } from './app/pages/NotFound';
import Faq from './pages/Faq.mdx';
import Multiplayer from './pages/Multiplayer.mdx';

import './index.css';

export function App() {
	return (
		<LocationProvider>
			<Header />
      <Router>
        <Route path="/gen/old" component={GeneratorVersions} />
        <Route path="/gen/:id" component={Generator} />
        <main path="/"><Home/></main>
        <main path="*">
          <Router>
            <Route path="/faq" component={Faq} />
            <Route path="/multiplayer" component={Multiplayer} />
            <Route default component={NotFound} />
          </Router>
        </main>
      </Router>
		</LocationProvider>
	);
}

render(<App />, document.getElementById('app'));
