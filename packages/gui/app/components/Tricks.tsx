import React from 'react';
import { DoubleList } from './DoubleList';
import { TRICKS } from '@ootmm/core';
import { TrickKey } from '@ootmm/core/lib/combo/settings';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faYoutube } from '@fortawesome/free-brands-svg-icons';
import { faFileLines } from '@fortawesome/free-solid-svg-icons';
import { useSettings } from '../contexts/GeneratorContext';
import { Tooltip } from './Tooltip';
import { GameName } from './GameName';

const GAMES_NAMES = {
  oot: "Ocarina of Time",
  mm: "Majora's Mask",
};

function trickExtra(trick: TrickKey) {
  const t = TRICKS[trick];
  return (
    <>
      {t.linkVideo && <a target="_blank" href={t.linkVideo} title="Video Example"><FontAwesomeIcon icon={faYoutube}/></a>}
      {t.linkText && <a target="_blank" href={t.linkText} title="Relevant Written Documentation"><FontAwesomeIcon icon={faFileLines}/></a>}
      {t.tooltip && <Tooltip>{t.tooltip}</Tooltip>}
    </>
  );
}

type GameTricksProps = {
  glitches?: boolean;
  game: 'oot' | 'mm';
}
function GameTricks({ glitches, game }: GameTricksProps) {
  const [settings, setSettings] = useSettings();
  const tricks = Object.keys(TRICKS).filter((x) => TRICKS[x].game === game && !!(TRICKS[x].glitch) === !!glitches);
  const selectedTricks = tricks.filter((x) => settings.tricks.includes(x));
  const options = tricks.map((trickKey) => ({ key: trickKey, label: TRICKS[trickKey].name, extra: trickExtra(trickKey) }));

  const add = (t: string[]) => {
    setSettings({ tricks: { add: t } });
  };

  const remove = (t: string[]) => {
    setSettings({ tricks: { remove: t } });
  };

  const reset = () => {
    setSettings({ tricks: { remove: tricks } });
  };

  return (
    <div>
      <GameName game={game}/>
      <DoubleList onAdd={add} onRemove={remove} onReset={reset} options={options} selected={selectedTricks}/>
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
