import { useCallback, useMemo } from 'react';
import { FaYoutube, FaFileLines } from 'react-icons/fa6';
import { TRICKS } from '@ootmm/core';
import { TrickKey } from '@ootmm/core/lib/combo/settings';

import { DoubleList } from './DoubleList';
import { Tooltip } from './ui/Tooltip';
import { GameName } from './GameName';
import { usePatchSettings, useSetting } from '../contexts/SettingsContext';

function trickExtra(trick: TrickKey) {
  const t = TRICKS[trick];
  return (
    <>
      {t.linkVideo && <a target="_blank" href={t.linkVideo} title="Video Example"><FaYoutube/></a>}
      {t.linkText && <a target="_blank" href={t.linkText} title="Relevant Written Documentation"><FaFileLines/></a>}
      {t.tooltip && <Tooltip>{t.tooltip}</Tooltip>}
    </>
  );
}

type GameTricksProps = {
  glitches?: boolean;
  game: 'oot' | 'mm';
}
function GameTricks({ glitches, game }: GameTricksProps) {
  const tricks = useSetting('tricks');
  const trickKeys = useMemo(() => Object.keys(TRICKS).filter((x) => TRICKS[x].game === game && !!(TRICKS[x].glitch) === !!glitches), [game, glitches]);
  const selectedTrickKeys = trickKeys.filter((x) => tricks.includes(x));
  const options = trickKeys.map((trickKey) => ({ key: trickKey, label: TRICKS[trickKey].name, extra: trickExtra(trickKey) }));
  const patchSettings = usePatchSettings();

  const add = useCallback((t: string[]) => {
    patchSettings({ tricks: { add: t } });
  }, []);

  const remove = useCallback((t: string[]) => {
    patchSettings({ tricks: { remove: t } });
  }, []);

  const reset = useCallback(() => {
    patchSettings({ tricks: { remove: trickKeys } });
  }, []);

  return (
    <div>
      <GameName game={game}/>
      <DoubleList onAdd={add} onRemove={remove} onReset={reset} options={options} selected={selectedTrickKeys}/>
    </div>
  );
}

type TricksProps = {
  glitches?: boolean;
}
export function Tricks({ glitches }: TricksProps) {
  return (
    <main>
      <h1>{glitches ? "Glitches" : "Tricks"}</h1>
      <div className="dual-panels">
        <GameTricks glitches={glitches} game="oot"/>
        <GameTricks glitches={glitches} game="mm"/>
      </div>
    </main>
  );
}
