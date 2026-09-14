import type { TabViewRoute } from '../nav';
import { LuPackage, LuPuzzle, LuClock } from 'react-icons/lu';

import { useStore } from '@/app/store';
import { TabView } from '../nav';
import { StartingItems } from '../StartingItems';
import { SettingsEditor } from '../settings';
import { ExtendedItemsEditor } from '../ExtendedItemsEditor.tsx';
import {MmAgeRequirements} from "@/app/components/mmAgeRequirements.tsx";

const PageExtensions = () => <ExtendedItemsEditor/>;
const PageAgeless = () => <SettingsEditor category='items.ageless'/>;
const PageMmAgeRequirements = () => <MmAgeRequirements />;

export function GeneratorItems() {
  const { games, adultMaskMm, crossAge } = useStore(state => state.settings);

  const routes: TabViewRoute[] = [
    { name: 'Extended Items', icon: LuPuzzle, component: PageExtensions },
    { name: 'Ageless', icon: LuClock, component: PageAgeless, disabled: games === 'mm' },
    { name: 'MM Age Req.', icon: LuClock, component: PageMmAgeRequirements, disabled: games === 'oot' || (!adultMaskMm && !crossAge) },
    { name: 'Starting Items', icon: LuPackage, component: StartingItems }
  ];

  return <TabView routes={routes}/>;
}
