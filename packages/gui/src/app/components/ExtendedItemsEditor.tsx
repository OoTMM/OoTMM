import { SETTINGS } from '@ootmm/generator';
import { useStore } from '@/app/store';
import { Setting } from './settings/Setting.tsx';

type SettingDefinition = (typeof SETTINGS)[number];
type SettingKey = SettingDefinition['key'];
type Game = 'oot' | 'mm';
type ItemGame = Game | 'shared';
type ItemCellValue = SettingKey | 'native';

type ItemSetting = Extract<
    SettingDefinition,
    { game: ItemGame; group: string }
>;

type ItemRow = {
  name: string;
  oot?: ItemCellValue;
  mm?: ItemCellValue;
  shared?: SettingKey;
  ootLabel?: string;
  mmLabel?: string;
};

type ItemSectionDefinition = {
  name: string;
  description: string;
  category: string;
  nativeMissing?: boolean;
};

type SimpleSectionDefinition = {
  name: string;
  description: string;
  settings: readonly SettingKey[];
};

function isItemSetting(setting: SettingDefinition): setting is ItemSetting {
  return 'game' in setting && 'group' in setting;
}

function settingsForCategory(category: string): SettingKey[] {
  return SETTINGS
      .filter(setting => setting.category === category)
      .map(setting => setting.key);
}

function rowsForCategory(
    category: string,
    nativeMissing = false,
): ItemRow[] {
  const rows = new Map<string, ItemRow>();

  for (const setting of SETTINGS) {
    if (setting.category !== category || !isItemSetting(setting)) {
      continue;
    }

    let row = rows.get(setting.group);

    if (!row) {
      row = {
        name: setting.group,
        oot: nativeMissing ? 'native' : undefined,
        mm: nativeMissing ? 'native' : undefined,
      };
      rows.set(setting.group, row);
    }

    const label = setting.type === 'enum' ? 'Behavior' : undefined;

    switch (setting.game) {
      case 'oot':
        row.oot = setting.key;
        row.ootLabel = label;
        break;

      case 'mm':
        row.mm = setting.key;
        row.mmLabel = label;
        break;

      case 'shared':
        row.shared = setting.key;
        break;
    }
  }

  return [...rows.values()];
}

const ITEM_SECTIONS = [
  {
    name: 'Shared Items',
    description:
        'Items that already exist in both games, but can be shared to combine them into one item for both games.',
    category: 'items.extensions.shared',
    nativeMissing: true,
  },
  {
    name: 'Cross-Game Items',
    description:
        'Items that have been ported from one game into the other.',
    category: 'items.extensions.crossGame.items',
    nativeMissing: true,
  },
  {
    name: 'Cross-Game Masks',
    description:
        'Masks that have been imported from one game into the other.',
    category: 'items.extensions.crossGame.masks',
    nativeMissing: true,
  },
  {
    name: 'Cross-Game Songs',
    description:
        'Songs that have been imported from one game into the other.',
    category: 'items.extensions.crossGame.songs',
    nativeMissing: true,
  },
  {
    name: 'Extended Item Systems',
    description:
        'Additional item systems such as keys, clocks, souls, and other new progression items.',
    category: 'items.extensions.systems',
  },
] as const satisfies readonly ItemSectionDefinition[];

const PROGRESSIVE_SECTION = {
  name: 'Progressive Items',
  description:
      'Configure how related items and upgrades are combined into progressive item chains.',
  settings: settingsForCategory('items.progressive'),
} satisfies SimpleSectionDefinition;

const SIMPLE_SECTIONS = [
  {
    name: 'Ocarina of Time Extensions',
    description:
        'Additional OoT-specific item behavior and progression options.',
    settings: settingsForCategory('items.extensions.oot'),
  },
  {
    name: "Majora's Mask Extensions",
    description:
        'Additional MM-specific item behavior and progression options.',
    settings: settingsForCategory('items.extensions.mm'),
  },
  {
    name: 'General Item Extensions',
    description:
        'Extensions that affect both games or the item pool as a whole.',
    settings: settingsForCategory('items.extensions.general'),
  },
  {
    name: 'Coins',
    description:
        'Enable coin items and configure how many of each color are added to the pool.',
    settings: settingsForCategory('items.extensions.coins'),
  },
  {
    name: 'Traps',
    description:
        'Configure which traps exist and how densely they replace junk items.',
    settings: settingsForCategory('items.extensions.traps'),
  },
] as const satisfies readonly SimpleSectionDefinition[];

function gameEnabled(games: string, game: Game) {
  return games === 'ootmm' || games === game;
}

function NativeItem({ enabled }: { enabled: boolean }) {
  return (
      <span
          className={[
            'inline-flex min-h-8 items-center rounded-md bg-muted px-3',
            'text-sm font-medium text-muted-foreground',
            !enabled && 'opacity-40',
          ]
              .filter(Boolean)
              .join(' ')}
          aria-disabled={!enabled || undefined}
          title="Already present in this game"
      >
        Native
      </span>
  );
}

function ItemCell({
                    value,
                    heading,
                    label = 'Add',
                    enabled = true,
                  }: {
  value?: ItemCellValue;
  heading: string;
  label?: string;
  enabled?: boolean;
}) {
  return (
      <div className="min-w-0 text-center">
        <div className="mb-1.5 text-xs font-medium text-muted-foreground lg:hidden">
          {heading}
        </div>

        <div className="flex min-h-8 items-center justify-center">
          {!value ? (
              <span
                  className="text-sm text-muted-foreground/50"
                  aria-label="Not available"
              >
                —
              </span>
          ) : value === 'native' ? (
              <NativeItem enabled={enabled}/>
          ) : (
              <Setting
                  setting={value}
                  showDisabled
                  label={label}
              />
          )}
        </div>
      </div>
  );
}

function ItemSectionView({
                           section,
                           games,
                         }: {
  section: ItemSectionDefinition;
  games: string;
}) {
  const ootEnabled = gameEnabled(games, 'oot');
  const mmEnabled = gameEnabled(games, 'mm');
  const rows = rowsForCategory(section.category, section.nativeMissing);

  return (
      <section className="rounded-xl border bg-card">
        <header className="border-b bg-muted/20 px-5 py-4">
          <h2 className="text-lg font-semibold">
            {section.name}
          </h2>

          <p className="mt-1 max-w-3xl text-sm leading-relaxed text-muted-foreground">
            {section.description}
          </p>
        </header>

        <div
            className="
              hidden
              grid-cols-[minmax(13rem,1.3fr)_repeat(3,minmax(10rem,1fr))]
              gap-4
              border-b
              bg-muted/40
              px-5
              py-2.5
              text-xs
              font-semibold
              uppercase
              tracking-wide
              text-muted-foreground
              lg:grid
            "
        >
          <div>Item</div>
          <div className="text-center">Ocarina of Time</div>
          <div className="text-center">Majora's Mask</div>
          <div className="text-center">Shared</div>
        </div>

        <div>
          {rows.map((row, index) => (
              <div
                  key={row.name}
                  className={[
                    'grid grid-cols-1 gap-3 px-4 py-3',
                    'sm:grid-cols-3',
                    'lg:grid-cols-[minmax(13rem,1.3fr)_repeat(3,minmax(10rem,1fr))]',
                    'lg:items-center lg:gap-4',
                    index % 2 === 1
                        ? 'bg-black/5 dark:bg-white/5'
                        : '',
                  ].join(' ')}
              >
                <div className="font-medium sm:col-span-3 lg:col-span-1">
                  {row.name}
                </div>

                <ItemCell
                    value={row.oot}
                    heading="OoT"
                    label={row.ootLabel}
                    enabled={ootEnabled}
                />

                <ItemCell
                    value={row.mm}
                    heading="MM"
                    label={row.mmLabel}
                    enabled={mmEnabled}
                />

                <ItemCell
                    value={row.shared}
                    heading="Shared"
                    label="Shared"
                />
              </div>
          ))}
        </div>
      </section>
  );
}

function SimpleSection({
                         name,
                         description,
                         settings,
                       }: SimpleSectionDefinition) {
  return (
      <section className="rounded-xl border bg-card p-5">
        <header>
          <h2 className="text-lg font-semibold">
            {name}
          </h2>

          <p className="mt-1 max-w-3xl text-sm leading-relaxed text-muted-foreground">
            {description}
          </p>
        </header>

        <div className="mt-5 grid grid-cols-1 items-center gap-x-6 gap-y-4 md:grid-cols-2 xl:grid-cols-3">
          {settings.map(setting => (
              <Setting
                  key={setting}
                  setting={setting}
                  showDisabled
              />
          ))}
        </div>
      </section>
  );
}

export function ExtendedItemsEditor() {
  const games = useStore(state => state.settings.games);

  return (
      <main className="mx-auto flex w-full max-w-7xl flex-col gap-6 p-4 md:p-8">
        <header className="space-y-2">
          <h1 className="text-2xl font-bold tracking-tight">
            Extended Items
          </h1>

          <p className="max-w-3xl text-sm leading-relaxed text-muted-foreground">
            Configure progressive items, cross-game imports, shared progression,
            and other extensions to the randomized item pool.
          </p>
        </header>

        {ITEM_SECTIONS.map(section => (
            <ItemSectionView
                key={section.category}
                section={section}
                games={games}
            />
        ))}

        <SimpleSection {...PROGRESSIVE_SECTION}/>

        {SIMPLE_SECTIONS.map(section => (
            <SimpleSection
                key={section.name}
                {...section}
            />
        ))}
      </main>
  );
}