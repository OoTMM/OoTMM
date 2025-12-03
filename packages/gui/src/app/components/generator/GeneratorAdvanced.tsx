import { LuLightbulb, LuWrench } from 'react-icons/lu';

import { TabView, TabViewRoute } from '../nav';
import { Hints } from '../Hints';
import { Plando } from '../Plando';
import { useStore } from '@/app/store';

export function GeneratorAdvanced() {
  const mode = useStore(state => state.config.mode);
  const isRandom = mode === 'random';
  const routes: TabViewRoute[] = [
    { name: 'Hints', icon: LuLightbulb, component: Hints },
    { name: 'Plando', icon: LuWrench, component: Plando, disabled: isRandom },
  ];

  return (
    <TabView routes={routes}/>
  );
}
