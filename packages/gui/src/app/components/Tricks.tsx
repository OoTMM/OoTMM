import { useCallback, useMemo } from 'react';
import { FaYoutube, FaFileLines } from 'react-icons/fa6';
import { TRICKS } from '@ootmm/core';
import { TrickKey } from '@ootmm/core/lib/combo/settings';

import { DoubleList } from './DoubleList';
import { Tooltip } from './ui/Tooltip';
import { usePatchSettings, useSetting } from '../contexts/SettingsContext';
import { TabView, TabViewRoute } from './nav';
import { IconMaskMajora, IconTriforce } from './icons';

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

  return <DoubleList onAdd={add} onRemove={remove} onReset={reset} options={options} selected={selectedTrickKeys}/>;
}

const OotTricks = () => <GameTricks glitches={false} game="oot"/>;
const MmTricks = () => <GameTricks glitches={false} game="mm"/>
const OotGlitches = () => <GameTricks glitches={true} game="oot"/>;
const MmGlitches = () => <GameTricks glitches={true} game="mm"/>

type TricksProps = {
  glitches?: boolean;
}
export function Tricks({ glitches }: TricksProps) {
  const games = useSetting('games');

  if (games === 'oot') {
    return glitches ? <OotGlitches/> : <OotTricks/>;
  } else if (games === 'mm') {
    return glitches ? <MmGlitches/> : <MmTricks/>;
  }

  const routes: TabViewRoute[] = [
    { name: 'Ocarina of Time', icon: IconTriforce, component: glitches ? OotGlitches : OotTricks },
    { name: 'Majora\'s Mask', icon: IconMaskMajora, component: glitches ? MmGlitches : MmTricks },
  ];

  return <TabView routes={routes}/>;
}
