import { LuZap, LuBug, LuTrash2 } from 'react-icons/lu';

import { TabView, TabViewRoute } from '../nav';
import { Tricks } from '../Tricks';
import { JunkLocations } from '../JunkLocations';

export function GeneratorLogic() {
  const routes: TabViewRoute[] = [
    { name: 'Tricks', icon: LuZap, component: () => <Tricks glitches={false}/> },
    { name: 'Glitches', icon: LuBug, component: () => <Tricks glitches={true}/> },
    { name: 'Junk Locations', icon: LuTrash2, component: JunkLocations },
  ];

  return <TabView routes={routes}/>;
}
