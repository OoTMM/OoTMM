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
  game: 'oot' | 'mm';
}
function GameTricks({ game }: GameTricksProps) {
  const [settings, setSettings] = useSettings();
  const gameTricks = Object.keys(TRICKS).filter((x) => x.startsWith(game.toUpperCase()));
  const selectedGameTricks = gameTricks.filter((x) => settings.tricks.includes(x));
  const options = gameTricks.map((trickKey) => ({ key: trickKey, label: TRICKS[trickKey].name, extra: trickExtra(trickKey) }));

  const add = (t: string[]) => {
    setSettings({ tricks: { add: t } });
  };

  const remove = (t: string[]) => {
    setSettings({ tricks: { remove: t } });
  };

  const reset = () => {
    setSettings({ tricks: { set: [] } });
  };

  return (
    <div>
      <GameName game={game}/>
      <DoubleList onAdd={add} onRemove={remove} onReset={reset} options={options} selected={selectedGameTricks}/>
    </div>
  );
}

export function Tricks() {
  return (
    <main>
      <h1>Tricks</h1>
      <div className="dual-panels">
        <GameTricks game="oot"/>
        <GameTricks game="mm"/>
      </div>
    </main>
  );
}
