import { LuLightbulb, LuWrench } from 'react-icons/lu';

import { TabView, TabViewRoute } from '../nav';
import { Hints } from '../Hints';
import { Plando } from '../Plando';
import { useRomConfig } from '@/app/contexts/GeneratorContext';

export function GeneratorAdvanced() {
  const { romConfig } = useRomConfig();
  const isRandom = romConfig.mode === 'random';
  const routes: TabViewRoute[] = [
    { name: 'Hints', icon: LuLightbulb, component: Hints },
    { name: 'Plando', icon: LuWrench, component: Plando, disabled: isRandom },
  ];

  return (
    <TabView routes={routes}/>
  );
}
