import { defaultEnemyShuffleWeight, enemyShuffleRoster } from '@ootmm/core';
import { useStore } from '@/app/store';
import { SettingsPanel } from '../settings/SettingsEditor';
import { Card, Tooltip } from '../ui';

const gameLabels = { oot: 'OoT', mm: 'MM' };

type Game = keyof typeof gameLabels;

function sortedRoster(game: Game) {
  return enemyShuffleRoster(game, true)
    .map(row => ({ ...row, label: row.donor === game ? row.name : `${row.name} (${gameLabels[row.donor]})` }))
    .sort((a, b) => a.label.localeCompare(b.label));
}

const rosters = { oot: sortedRoster('oot'), mm: sortedRoster('mm') };

const minEnemyWeight = 0.01;

const maxEnemyWeight = 1000;

function EnemyWeightInput({ name, weight, disabled, onChange }: {
  name: string;
  weight: number;
  disabled: boolean;
  onChange: (weight: number) => void;
}) {
  return (
    <input type="number" min={minEnemyWeight} max={maxEnemyWeight} step="any"
      className="w-20 rounded border px-2 py-1" aria-label={name + ' weight'}
      key={weight} defaultValue={weight} disabled={disabled}
      onKeyDown={event => {
        if (event.key === 'Enter') event.currentTarget.blur();
      }}
      onBlur={event => {
        const value = event.currentTarget.valueAsNumber;
        if (Number.isFinite(value) && value >= minEnemyWeight && value <= maxEnemyWeight) onChange(value);
        else event.currentTarget.value = String(weight);
      }} />
  );
}

const headerClass = 'sticky top-0 z-20 border-b border-gray-300 bg-white p-2 dark:border-gray-700 dark:bg-gray-900';

function EnemyTable({ game }: { game: Game }) {
  const settings = useStore(state => state.settings);
  const patchSettings = useStore(state => state.patchSettings);
  const enabled = (settings.games === 'ootmm' || settings.games === game) &&
    (game === 'oot' ? settings.enemyShuffleOot || settings.enemyShuffleOotMinibossOnly : settings.enemyShuffleMm || settings.enemyShuffleMmMinibossOnly);
  const otherRoster = rosters[game === 'oot' ? 'mm' : 'oot'];
  const legacyPrefix = game + ':';

  return (
    <section className="flex min-w-0 flex-col gap-3" aria-label={gameLabels[game]}>
      <h2 className="text-lg font-bold">{gameLabels[game]}</h2>
      <Card className="overflow-hidden p-0">
        <div className="isolate max-h-[36rem] overflow-auto">
          <table className="w-full border-separate border-spacing-0 text-left text-sm">
            <thead>
              <tr className="ux-border-b">
                <th scope="col" className={headerClass}>Enemy</th>
                <th scope="col" className={headerClass + ' text-center'}>
                  <span className="inline-flex items-center gap-1">
                    Exclude
                    <Tooltip>Prevent this enemy from being chosen as a replacement.</Tooltip>
                  </span>
                </th>
                <th scope="col" className={headerClass}>
                  <span className="inline-flex items-center gap-1">
                    Weight
                    <Tooltip>Weight controls how often an eligible enemy type is chosen. Variants under one entry share that weight.</Tooltip>
                  </span>
                </th>
              </tr>
            </thead>
            <tbody>
              {rosters[game].map(row => {
                const disabled = !enabled || (row.donor !== game && !settings.crossGameEnemies && !(game === 'oot' ? settings.enemyShuffleOotMinibossOnly : settings.enemyShuffleMmMinibossOnly)) ||
                  (row.miniboss && !settings.enemyShuffleMinibosses && !(game === 'oot' ? settings.enemyShuffleOotMinibossOnly : settings.enemyShuffleMmMinibossOnly)) ||
                  (!row.miniboss && (game === 'oot' ? settings.enemyShuffleOotMinibossOnly : settings.enemyShuffleMmMinibossOnly));
                const keys = settings.enemyShuffleExcluded.filter(key =>
                  key === row.key || row.legacyKeys.includes(key) ||
                  (key.startsWith(legacyPrefix) && row.legacyKeys.includes(key.slice(legacyPrefix.length))),
                );
                const defaultWeight = defaultEnemyShuffleWeight(row.miniboss, row.name);
                const weight = settings.enemyShuffleWeights[row.key] ?? defaultWeight;
                return (
                  <tr key={row.key} className={'ux-border-b last:border-b-0' + (disabled ? ' opacity-50' : '')}>
                    <td className="p-2">{row.label}</td>
                    <td className="p-2 text-center">
                      <input type="checkbox" checked={keys.length > 0} disabled={disabled}
                        aria-label={'Exclude ' + row.label}
                        onChange={event => {
                          if (event.currentTarget.checked) {
                            patchSettings({ enemyShuffleExcluded: { add: [row.key] } });
                            return;
                          }
                          const removed = new Set(keys);
                          const preserve = otherRoster.filter(other => other.legacyKeys.some(key => removed.has(key)));
                          patchSettings({
                            enemyShuffleExcluded: {
                              set: [
                                ...settings.enemyShuffleExcluded.filter(key => !removed.has(key)),
                                ...preserve.map(other => other.key),
                              ]
                            }
                          });
                        }} />
                    </td>
                    <td className="p-2">
                      <EnemyWeightInput name={row.label} weight={weight} disabled={disabled} onChange={value => {
                        if (value === weight) return;
                        const weights = { ...settings.enemyShuffleWeights };
                        if (value === defaultWeight) delete weights[row.key];
                        else weights[row.key] = value;
                        patchSettings({ enemyShuffleWeights: weights });
                      }} />
                    </td>
                  </tr>
                );
              })}
            </tbody>
          </table>
        </div>
      </Card>
    </section>
  );
}

export function GeneratorEnemies() {
  return (
    <main className="flex flex-col gap-8 p-8">
      <SettingsPanel category="enemies" />
      <div className="flex flex-col gap-4">
        <h1 className="text-xl font-bold">Enemy Pool</h1>
        <div className="grid grid-cols-1 gap-6 lg:grid-cols-2">
          <EnemyTable game="oot" />
          <EnemyTable game="mm" />
        </div>
      </div>
    </main>
  );
}
