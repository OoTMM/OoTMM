import { render } from 'preact';
import { LocationProvider, Router, Route } from 'preact-iso';

import { Header } from './components/Header';
import { Home } from './pages/Home';
import { Generator } from './pages/Generator';
import { NotFound } from './pages/NotFound';
import Faq from './pages/Faq.mdx';
import Multiplayer from './pages/Multiplayer.mdx';

import './styles/index.css';

export function App() {
	return (
		<LocationProvider>
			<Header />
      <Router>
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
