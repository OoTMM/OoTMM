import { ComponentType } from 'preact';
import clsx from 'clsx';
import { IconType } from 'react-icons';
import { LuSettings, LuGamepad2, LuBox, LuRoute, LuCpu, LuCompass, LuSparkle } from 'react-icons/lu';

import { useRouter } from '@/app/hooks/useRouter';
import { GeneratorGeneral } from './GeneratorGeneral';
import { CosmeticsEditor } from '../Cosmetics';
import { GeneratorSettings } from './GeneratorSettings';
import { GeneratorItems } from './GeneratorItems';
import { GeneratorLogic } from './GeneratorLogic';
import { GeneratorEntrances } from './GeneratorEntrances';
import { GeneratorAdvanced } from './GeneratorAdvanced';
import { useRomConfig } from '@/app/contexts/GeneratorContext';

type GeneratorRoute = {
  name: string;
  component: ComponentType;
  icon: IconType;
  disabled?: boolean;
};

export function Generator() {
  const { romConfig } = useRomConfig();
  const isPatch = romConfig.mode === 'patch';
  const isRandom = romConfig.mode === 'random';

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
    <div className="flex flex-row h-full w-full overflow-hidden">
      <nav className="w-[200px] flex flex-col bg-gray-950 text-white border-r border-gray-800">
        {router.routes.map((route, i) =>
          <a
            key={i}
            aria-disabled={route.disabled}
            tabIndex={route.disabled ? -1 : 0}
            className={clsx(
              "inline-flex items-center gap-3 [font-variant:small-caps] m-2 p-2 transition-all rounded",
              i == router.currentIndex ? 'font-semibold bg-gray-800' : 'hover:bg-gray-900',
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
      <div className="h-full w-full gutter-stable overflow-y-auto">
        {<router.currentRoute.component/>}
      </div>
    </div>
  )
}
