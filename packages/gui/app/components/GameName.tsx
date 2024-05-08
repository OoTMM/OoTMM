import React from 'react';
import imgTriforce from '../assets/blacktriforce.png';
import imgMajora from '../assets/blackmask.png';

const GAME_PROPERTIES = {
  oot: { name: "Ocarina of Time", img: imgTriforce },
  mm: { name: "Majora's Mask", img: imgMajora },
}

type GameNameProps = {
  game: 'oot' | 'mm';
};
export function GameName({ game }: GameNameProps) {
  const p = GAME_PROPERTIES[game];
  return (
    <div className="game">
      <img src={p.img} alt={p.name} />
      <h2>{p.name}</h2>
    </div>
  );
}
