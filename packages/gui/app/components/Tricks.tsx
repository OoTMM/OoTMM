import React from 'react';
import { TRICKS } from '@ootmm/core';
import { useSettings } from '../contexts/GeneratorContext';
import { Text } from './Text';
import { TrickKey } from '@ootmm/core/lib/combo/settings';
import SelectorCard from './SelectorCard';
import TriforceImg from '../assets/blacktriforce.png';
import MaskImg from '../assets/blackmask.png';
import { Col, Row } from 'react-bootstrap';
import Button from './Button';

const NAMES = {
  OOT: 'Ocarina of Time',
  MM: "Majora's Mask",
};

export function GameTricks({ game, img }: { game: keyof typeof NAMES; img: typeof TriforceImg }) {
  const [settings, setSettings] = useSettings();
  const tricks: TrickKey[] = Object.keys(TRICKS).filter((x) => x.startsWith(game));
  const options = tricks.map((trick) => ({ value: trick as TrickKey, label: TRICKS[trick].name }));

  const add = (trick: string) => {
    setSettings({ tricks: { add: [trick as TrickKey] } });
  };

  const remove = (trick: string) => {
    setSettings({ tricks: { remove: [trick as TrickKey] } });
  };

  return (
    <>
      <Row>
        <Col>
          <Text size="jb">{NAMES[game]}</Text>
        </Col>
        <Col lg="auto">
          <img src={img} height={100} width={100} />
        </Col>
      </Row>
      <SelectorCard
        options={options}
        selected={settings.tricks.filter((x) => tricks.includes(x))}
        add={add}
        remove={remove}
        label="Trick"
      />
    </>
  );
}

export function Tricks() {
  const [_, setSettings] = useSettings();
  const clear = () => {
    setSettings({ tricks: { set: [] } });
  };

  return (
    <>
      <Text size="mg">Tricks</Text>

      <Button label="Reset All" variant="danger" className="btn-auto" handleClick={clear}></Button>
      <Row>
        <Col lg={12} xl={6}>
          <GameTricks game="OOT" img={TriforceImg} />
        </Col>
        <Col>
          <GameTricks game="MM" img={MaskImg} />
        </Col>
      </Row>
    </>
  );
}
