import { LuZap, LuBug, LuTrash2 } from 'react-icons/lu';

import { TabView, TabViewRoute } from '../nav';
import { Tricks } from '../Tricks';
import { JunkLocations } from '../JunkLocations';

const PageTricks = () => <Tricks glitches={false}/>;
const PageGlitches = () => <Tricks glitches={true}/>;

export function GeneratorLogic() {
  const routes: TabViewRoute[] = [
    { name: 'Tricks', icon: LuZap, component: PageTricks },
    { name: 'Glitches', icon: LuBug, component: PageGlitches },
    { name: 'Junk Locations', icon: LuTrash2, component: JunkLocations },
  ];

  return <TabView routes={routes}/>;
}
