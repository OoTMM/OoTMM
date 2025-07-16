import { LocationProvider, Router, Route } from 'preact-iso';

import { ThemeProvider } from './theme/ThemeProvider';
import { Header } from './components/Header';
import { Home } from './pages/Home';
import { Generator } from './pages/Generator';
import { GeneratorVersions } from './pages/GeneratorVersions';
import { NotFound } from './pages/NotFound';
import Faq from './pages/Faq.mdx';
import Multiplayer from './pages/Multiplayer.mdx';

export function App() {
	return (
    <ThemeProvider>
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
    </ThemeProvider>
	);
}
