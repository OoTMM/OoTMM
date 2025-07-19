import clsx from 'clsx';
import { useRouter } from '@/app/hooks/useRouter';
import { ComponentType } from 'preact';

export type TabViewRoute = {
  name: string;
  component: ComponentType;
  disabled?: boolean;
};

export function TabView({ routes }: { routes: TabViewRoute[] }) {
  const router = useRouter(routes);

  return (
    <div className="p-4 flex flex-col gap-4">
      <nav className="ux-bg ux-border flex">
        {router.routes.map((route, i) =>
          <button
            disabled={route.disabled}
            className={clsx(
              'cursor-pointer not-first:border-l dark:border-gray-600',
              'disabled:opacity-50 disabled:cursor-not-allowed',
              (i === router.currentIndex ? 'font-semibold dark:bg-gray-800' : 'not-disabled:hover:dark:bg-gray-900'),
              "flex-1 p-2"
            )}
            key={i}
            onClick={() => router.navigate(i)}
          >
            {route.name}
          </button>
        )}
      </nav>
      <div>
        {<router.currentRoute.component/>}
      </div>
    </div>
  )
};
