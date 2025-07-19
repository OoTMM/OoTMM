import { TabView, TabViewRoute } from '../nav';
import { Tricks } from '../Tricks';
import { JunkLocations } from '../JunkLocations';

export function GeneratorLogic() {
  const routes: TabViewRoute[] = [
    { name: 'Tricks', component: () => <Tricks glitches={false}/> },
    { name: 'Glitches', component: () => <Tricks glitches={true}/> },
    { name: 'Junk Locations', component: JunkLocations },
  ];

  return <TabView routes={routes}/>;
}
