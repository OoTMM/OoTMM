import clsx from 'clsx';
import { useRouter } from '@/app/hooks/useRouter';
import { ComponentType } from 'preact';
import { IconType } from 'react-icons';

export type TabViewRoute = {
  name: string;
  component: ComponentType;
  disabled?: boolean;
  icon?: IconType;
};

export function TabView({ routes }: { routes: TabViewRoute[] }) {
  const router = useRouter(routes);

  return (
    <div className="p-4 flex flex-col gap-4 h-full">
      <nav className="mx-auto ux-bg ux-border inline-flex justify-center max-w-full">
        {router.routes.map((route, i) =>
          <button
            data-text={route.name}
            disabled={route.disabled}
            className={clsx(
              "p-2 w-[200px] flex items-center justify-center gap-2",
              "cursor-pointer disabled:opacity-50 disabled:cursor-not-allowed",
              i === router.currentIndex
                ? "font-semibold text-white bg-neutral-400 dark:bg-gray-800"
                : "hover:bg-neutral-300 hover:dark:bg-gray-900",
              i !== 0 && "border-l border-gray-400 dark:border-gray-600"
            )}
            key={i}
            onClick={() => router.navigate(i)}
          >
            {route.icon && <route.icon size={20}/>}
            <span className="reserve-semibold" data-text={route.name}>
              <span>{route.name}</span>
            </span>
          </button>
        )}
      </nav>
      <div className="h-full w-full">
        {<router.currentRoute.component/>}
      </div>
    </div>
  )
};
