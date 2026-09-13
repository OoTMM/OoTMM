import { LocationProvider, Router, Route } from 'preact-iso';

import { ThemeProvider } from './theme/ThemeProvider';
import { Layout } from './layout/Layout';
import * as Pages from './pages';

export function App() {
	return (
    <ThemeProvider>
      <LocationProvider>
        <Layout>
          <Router>
            <Route path="/docs/multiplayer-api" component={Pages.MultiplayerAPI}/>
            <Route path="/gen/old" component={Pages.GeneratorVersions}/>
            <Route path="/gen/:id" component={Pages.Generator}/>
            <Route path="/" component={Pages.Home}/>
            <Route path="/faq" component={Pages.Faq}/>
            <Route path="/multiplayer/legacy" component={Pages.MultiplayerLegacy}/>
            <Route path="/multiplayer" component={Pages.Multiplayer}/>
            <Route default component={Pages.NotFound}/>
          </Router>
        </Layout>
      </LocationProvider>
    </ThemeProvider>
	);
}
