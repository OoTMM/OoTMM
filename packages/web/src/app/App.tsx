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
        <Header/>
        <Router>
          <Route path="/gen/old" component={GeneratorVersions} />
          <Route path="/gen/:id" component={Generator} />
          <Route path="/" component={Home} />
          <Route path="/faq" component={() => <main><Faq/></main>} />
          <Route path="/multiplayer" component={() => <main><Multiplayer/></main>} />
          <Route default component={NotFound} />
        </Router>
      </LocationProvider>
    </ThemeProvider>
	);
}
