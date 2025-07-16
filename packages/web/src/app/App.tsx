import { LocationProvider, Router, Route } from 'preact-iso';

import { ThemeProvider } from './theme/ThemeProvider';
import { Home } from './pages/Home';
import { Generator } from './pages/Generator';
import { GeneratorVersions } from './pages/GeneratorVersions';
import { NotFound } from './pages/NotFound';
import { Faq } from './pages/Faq';
import { Multiplayer } from './pages/Multiplayer';
import { Layout } from './layout/Layout';

export function App() {
	return (
    <ThemeProvider>
      <LocationProvider>
        <Layout>
          <Router>
            <Route path="/gen/old" component={GeneratorVersions} />
            <Route path="/gen/:id" component={Generator} />
            <Route path="/" component={Home} />
            <Route path="/faq" component={Faq}/>
            <Route path="/multiplayer" component={Multiplayer} />
            <Route default component={NotFound} />
          </Router>
        </Layout>
      </LocationProvider>
    </ThemeProvider>
	);
}
