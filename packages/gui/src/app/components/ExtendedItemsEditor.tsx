import { SETTINGS } from '@ootmm/generator';
import { useStore } from '@/app/store';
import { Setting } from './settings/Setting.tsx';

type SettingKey = (typeof SETTINGS)[number]['key'];
type Game = 'oot' | 'mm';
type ItemCellValue = SettingKey | 'native';

type ItemRow = {
  name: string;
  oot?: ItemCellValue;
  mm?: ItemCellValue;
  shared?: SettingKey;
  ootLabel?: string;
  mmLabel?: string;
};

type ItemSection = {
  name: string;
  description: string;
  rows: readonly ItemRow[];
};

type SimpleSectionDefinition = {
  name: string;
  description: string;
  settings: readonly SettingKey[];
};

function sharedNative(
    name: string,
    shared: SettingKey,
): ItemRow {
  return {
    name,
    oot: 'native',
    mm: 'native',
    shared,
  };
}

function ootItem(
    name: string,
    mm: SettingKey,
    shared?: SettingKey,
): ItemRow {
  return {
    name,
    oot: 'native',
    mm,
    shared,
  };
}

function mmItem(
    name: string,
    oot: SettingKey,
    shared?: SettingKey,
): ItemRow {
  return {
    name,
    oot,
    mm: 'native',
    shared,
  };
}

function settingsForCategory(category: string): SettingKey[] {
  return SETTINGS
      .filter(setting => setting.category === category)
      .map(setting => setting.key);
}

const PROGRESSIVE_SECTION = {
  name: 'Progressive Items',
  description:
      'Configure how related items and upgrades are combined into progressive item chains.',
  settings: settingsForCategory('items.progressive'),
} satisfies SimpleSectionDefinition;

const ITEM_SECTIONS = [
  {
    name: 'Shared Items',
    description:
        'Items that already exist in both games, but can be shared to combine them into one item for both games.',
    rows: [
      sharedNative('Bows', 'sharedBows'),
      sharedNative('Bomb Bags', 'sharedBombBags'),
      sharedNative('Magic', 'sharedMagic'),
      sharedNative('Fire Arrows', 'sharedMagicArrowFire'),
      sharedNative('Ice Arrows', 'sharedMagicArrowIce'),
      sharedNative('Light Arrows', 'sharedMagicArrowLight'),
      sharedNative("Epona's Song", 'sharedSongEpona'),
      sharedNative('Song of Storms', 'sharedSongStorms'),
      sharedNative('Song of Time', 'sharedSongTime'),
      sharedNative('Lens of Truth', 'sharedLens'),
      sharedNative('Goron Mask', 'sharedMaskGoron'),
      sharedNative('Zora Mask', 'sharedMaskZora'),
      sharedNative('Bunny Hood', 'sharedMaskBunny'),
      sharedNative('Keaton Mask', 'sharedMaskKeaton'),
      sharedNative('Mask of Truth', 'sharedMaskTruth'),
      sharedNative('Wallets', 'sharedWallets'),
      sharedNative('Health', 'sharedHealth'),
      sharedNative('Bottles', 'sharedBottles'),
    ],
  },
  {
    name: 'Cross-Game Items',
    description:
        'Items that have been ported from one game into the other.',
    rows: [
      ootItem(
          'Stone of Agony',
          'stoneAgonyMm',
          'sharedStoneAgony',
      ),
      mmItem(
          'Spin Attack Upgrade',
          'spinUpgradeOot',
          'sharedSpinUpgrade',
      ),
      ootItem(
          'Deku Stick & Nut Upgrades',
          'sticksNutsUpgradesMm',
          'sharedNutsSticks',
      ),
      ootItem(
          'Fairy Ocarina',
          'fairyOcarinaMm',
          'sharedOcarina',
      ),
      ootItem(
          'Short Hookshot',
          'shortHookshotMm',
          'sharedHookshot',
      ),
      ootItem(
          "Din's Fire",
          'spellFireMm',
          'sharedSpellFire',
      ),
      ootItem(
          "Farore's Wind",
          'spellWindMm',
          'sharedSpellWind',
      ),
      ootItem(
          "Nayru's Love",
          'spellLoveMm',
          'sharedSpellLove',
      ),
      ootItem(
          'Iron Boots',
          'bootsIronMm',
          'sharedBootsIron',
      ),
      ootItem(
          'Hover Boots',
          'bootsHoverMm',
          'sharedBootsHover',
      ),
      ootItem(
          'Goron Tunic',
          'tunicGoronMm',
          'sharedTunicGoron',
      ),
      ootItem(
          'Zora Tunic',
          'tunicZoraMm',
          'sharedTunicZora',
      ),
      ootItem(
          'Scales',
          'scalesMm',
          'sharedScales',
      ),
      ootItem(
          'Strength',
          'strengthMm',
          'sharedStrength',
      ),
      ootItem(
          'Megaton Hammer',
          'hammerMm',
          'sharedHammer',
      ),
      ootItem(
          'Boomerang',
          'boomerangMm',
          'sharedBoomerang',
      ),
      mmItem(
          "Great Fairy's Sword",
          'gfsOot',
          'sharedGFS',
      ),
      ootItem(
          'Slingshot',
          'slingshotMm',
          'sharedSlingshot',
      ),
      mmItem(
          'Child Swords',
          'extraChildSwordsOot',
          'sharedSwords',
      ),
      ootItem(
          'Deku Shield',
          'dekuShieldMm',
          'sharedShields',
      ),
      mmItem(
          'Powder Keg',
          'powderKegOot',
          'sharedPowderKeg',
      ),
    ],
  },
  {
    name: 'Cross-Game Masks',
    description:
        'Masks that have been imported from one game into the other.',
    rows: [
      mmItem(
          'Blast Mask',
          'blastMaskOot',
          'sharedMaskBlast',
      ),
      mmItem(
          'Stone Mask',
          'stoneMaskOot',
          'sharedMaskStone',
      ),
      mmItem(
          'Kamaro Mask',
          'kamaroMaskOot',
          'sharedMaskKamaro',
      ),
      ootItem(
          'Gerudo Mask',
          'gerudoMaskMm',
          'sharedMaskGerudo',
      ),
      ootItem(
          'Skull Mask',
          'skullMaskMm',
          'sharedMaskSkull',
      ),
      ootItem(
          'Spooky Mask',
          'spookyMaskMm',
          'sharedMaskSpooky',
      ),
    ],
  },
  {
    name: 'Cross-Game Songs',
    description:
        'Songs that have been imported from one game into the other.',
    rows: [
      ootItem(
          "Sun's Song",
          'songSunMm',
          'sharedSongSun',
      ),
      mmItem(
          'Elegy of Emptiness',
          'elegyOot',
          'sharedSongElegy',
      ),
      mmItem(
          'Song of Healing',
          'songHealingOot',
          'sharedSongHealing',
      ),
      mmItem(
          'Song of Soaring',
          'songSoaringOot',
          'sharedSongSoaring',
      ),
      mmItem(
          'Sonata of Awakening',
          'songAwakeningOot',
          'sharedSongAwakening',
      ),
      mmItem(
          'Goron Lullaby',
          'songGoronOot',
          'sharedSongGoron',
      ),
      mmItem(
          'New Wave Bossa Nova',
          'songZoraOot',
          'sharedSongZora',
      ),
      mmItem(
          'Oath to Order',
          'songOrderOot',
          'sharedSongOrder',
      ),
      ootItem(
          "Zelda's Lullaby",
          'songZeldaLullabyMm',
          'sharedSongZeldaLullaby',
      ),
      ootItem(
          "Saria's Song",
          'songSariasMm',
          'sharedSongSarias',
      ),
      ootItem(
          'Minuet of Forest',
          'songMinuetMm',
          'sharedSongMinuet',
      ),
      ootItem(
          'Bolero of Fire',
          'songBoleroMm',
          'sharedSongBolero',
      ),
      ootItem(
          'Serenade of Water',
          'songSerenadeMm',
          'sharedSongSerenade',
      ),
      ootItem(
          'Requiem of Spirit',
          'songRequiemMm',
          'sharedSongRequiem',
      ),
      ootItem(
          'Nocturne of Shadow',
          'songNocturneMm',
          'sharedSongNocturne',
      ),
      ootItem(
          'Prelude of Light',
          'songPreludeMm',
          'sharedSongPrelude',
      ),
    ],
  },
  {
    name: 'Extended Item Systems',
    description:
        'Additional item systems such as keys, clocks, souls, and other new progression items.',
    rows: [
      {
        name: 'Skeleton Key',
        oot: 'skeletonKeyOot',
        mm: 'skeletonKeyMm',
        shared: 'sharedSkeletonKey',
      },
      {
        name: 'Platinum Token',
        oot: 'platinumTokenOot',
        mm: 'platinumTokenMm',
        shared: 'sharedPlatinumToken',
      },
      {
        name: 'Ocarina Buttons',
        oot: 'ocarinaButtonsShuffleOot',
        mm: 'ocarinaButtonsShuffleMm',
        shared: 'sharedOcarinaButtons',
      },
      {
        name: 'Shovel',
        oot: 'shovelOot',
        mm: 'shovelMm',
        shared: 'sharedShovel',
      },
      {
        name: 'Bombchu',
        oot: 'bombchuBehaviorOot',
        mm: 'bombchuBehaviorMm',
        shared: 'sharedBombchu',
        ootLabel: 'Behavior',
        mmLabel: 'Behavior',
      },
      {
        name: 'Enemy Souls',
        oot: 'soulsEnemyOot',
        mm: 'soulsEnemyMm',
        shared: 'sharedSoulsEnemy',
      },
      {
        name: 'Boss Souls',
        oot: 'soulsBossOot',
        mm: 'soulsBossMm',
      },
      {
        name: 'NPC Souls',
        oot: 'soulsNpcOot',
        mm: 'soulsNpcMm',
        shared: 'sharedSoulsNpc',
      },
      {
        name: 'Animal Souls',
        oot: 'soulsAnimalOot',
        mm: 'soulsAnimalMm',
        shared: 'sharedSoulsAnimal',
      },
      {
        name: 'Misc. Souls',
        oot: 'soulsMiscOot',
        mm: 'soulsMiscMm',
        shared: 'sharedSoulsMisc',
      },
      {
        name: 'Rusty Keys',
        oot: 'rustyKeysOot',
        mm: 'rustyKeysMm',
      },
      {
        name: 'Clocks',
        oot: 'clocksOot',
        mm: 'clocksMm',
        ootLabel: 'Behavior',
      },
    ],
  },
] as const satisfies readonly ItemSection[];

const SIMPLE_SECTIONS = [
  {
    name: 'Ocarina of Time Extensions',
    description:
        'Additional OoT-specific item behavior and progression options.',
    settings: [
      'blueFireArrows',
      'iceArrowPlatformsOot',
      'sunlightArrows',
      'magicalRupee',
      'songOfDoubleTimeOot',
    ],
  },
  {
    name: "Majora's Mask Extensions",
    description:
        'Additional MM-specific item behavior and progression options.',
    settings: [
      'kegStrength3',
      'adultMaskMm',
      'transcendentFairy',
      'menuNotebook',
    ],
  },
  {
    name: 'General Item Extensions',
    description:
        'Extensions that affect both games or the item pool as a whole.',
    settings: [
      'fillWallets',
      'bottleContentShuffle',
      'sticksNutsUpgradesInitial',
      'bronzeScale',
      'childWallets',
      'colossalWallets',
      'bottomlessWallets',
      'rupeeScaling',
    ],
  },
  {
    name: 'Coins',
    description:
        'Enable coin items and configure how many of each color are added to the pool.',
    settings: [
      'coins',
      'coinsRed',
      'coinsGreen',
      'coinsBlue',
      'coinsYellow',
    ],
  },
  {
    name: 'Traps',
    description:
        'Configure which traps exist and how densely they replace junk items.',
    settings: [
      'trapIce',
      'trapFire',
      'trapShock',
      'trapDrain',
      'trapAntiMagic',
      'trapKnockback',
      'trapRupoor',
      'cloakTraps',
      'trapsQuantity',
    ],
  },
] as const satisfies readonly SimpleSectionDefinition[];

/*
 * Helpers
 */

function gameEnabled(games: string, game: Game) {
  return games === 'ootmm' || games === game;
}

/*
 * Item table
 */

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

function ItemSectionView({section, games,}: {
  section: ItemSection;
  games: string;
}) {
  const ootEnabled = gameEnabled(games, 'oot');
  const mmEnabled = gameEnabled(games, 'mm');

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
          {section.rows.map((row, index) => (
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

function SimpleSection({name, description, settings,}: SimpleSectionDefinition)
{
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
                key={section.name}
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