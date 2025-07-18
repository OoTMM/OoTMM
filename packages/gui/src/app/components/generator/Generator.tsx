import { ComponentType } from 'preact';
import clsx from 'clsx';
import { IconType } from 'react-icons';
import { LuSettings, LuGamepad2, LuBox, LuRoute, LuCpu, LuCompass, LuSparkle } from 'react-icons/lu';

import { useRouter } from '@/app/hooks/useRouter';
import { useIsPatch } from '../../contexts/GeneratorContext';
import { useRandomSettings } from '../../contexts/RandomSettingsContext';
import { GeneratorGeneral } from './GeneratorGeneral';
import { CosmeticsEditor } from '../Cosmetics';
import { GeneratorSettings } from './GeneratorSettings';
import { GeneratorItems } from './GeneratorItems';
import { GeneratorLogic } from './GeneratorLogic';
import { GeneratorEntrances } from './GeneratorEntrances';
import { GeneratorAdvanced } from './GeneratorAdvanced';

type GeneratorRoute = {
  name: string;
  component: ComponentType;
  icon: IconType;
  disabled?: boolean;
};

export function Generator() {
  const [isPatch] = useIsPatch();
  const randomSettings = useRandomSettings();
  const isRandom = randomSettings.enabled;

  const router = useRouter<GeneratorRoute>([
    { name: 'General', icon: LuSettings, component: GeneratorGeneral },
    { name: 'Settings', icon: LuGamepad2, disabled: isPatch || isRandom, component: GeneratorSettings },
    { name: 'Items', icon: LuBox, disabled: isPatch || isRandom, component: GeneratorItems },
    { name: 'Logic', icon: LuRoute, disabled: isPatch, component: GeneratorLogic },
    { name: 'Entrances', icon: LuCompass, disabled: isPatch || isRandom, component: GeneratorEntrances },
    { name: 'Advanced', icon: LuCpu, disabled: isPatch, component: GeneratorAdvanced },
    { name: 'Cosmetics', icon: LuSparkle, component: CosmeticsEditor },
  ]);

  return (
    <div className="flex flex-row h-full w-full min-h-screen">
      <nav className="w-[200px] flex flex-col dark:bg-gray-950 border-r dark:border-gray-800">
        {router.routes.map((route, i) =>
          <a
            key={i}
            aria-disabled={route.disabled}
            tabIndex={route.disabled ? -1 : 0}
            className={clsx(
              "inline-flex items-center gap-3 [font-variant:small-caps] m-2 p-2 transition-all rounded",
              i == router.currentIndex ? 'font-semibold dark:bg-gray-800' : 'hover:dark:bg-gray-900',
              route.disabled && 'opacity-50 cursor-not-allowed',
            )}
            href="#"
            onClick={(e) => {
              e.preventDefault();
              if (route.disabled) return;
              router.navigate(i);
            }}
          >
            {route.icon && <route.icon size={20}/>}
            {route.name}
          </a>
        )}
      </nav>
      <div className="h-full w-full overflow-y-auto">
        {<router.currentRoute.component/>}
      </div>
    </div>
  )
}
