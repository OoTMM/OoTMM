import React from 'react';
import { TRICKS } from '@ootmm/core';
import { useSettings } from '../contexts/GeneratorContext';
import { Text } from './Text';
import { TrickKey } from '@ootmm/core/lib/combo/settings';
import SelectorCard, { SelectedList } from './SelectorCard';
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
  const options = tricks.map((trickKey) => ({ key: trickKey as keyof SelectedList, value: TRICKS[trickKey] }));

  const add = async (trickKeysToAdd: SelectedList, setAdditions: Function) => {
    setSettings({ tricks: { add: Object.keys(trickKeysToAdd) } });
    setAdditions({});
  };

  const remove = (trickKeysToRemove: SelectedList, setRemovals: Function) => {
    setSettings({ tricks: { remove: Object.keys(trickKeysToRemove) } });
    setRemovals({});
  };

  return (
    <>
      <Row className="mb-2">
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

      <Row className="mt-4">
        <Col xl={12} xxl={6}>
          <GameTricks game="OOT" img={TriforceImg} />
        </Col>
        <Col>
          <GameTricks game="MM" img={MaskImg} />
        </Col>
      </Row>
    </>
  );
}
