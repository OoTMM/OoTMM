import React, { useState } from 'react';
import { Col, Row } from 'react-bootstrap';
import Button from './Button';
import { CaretLeftFill, CaretRightFill, QuestionCircleFill, TextParagraph, Youtube } from 'react-bootstrap-icons';
import { Checkbox } from './Checkbox';
import { Trick } from '@ootmm/core/lib/combo/settings/tricks';
import { Tooltip } from './Tooltip';
import { Text } from './Text';

/*const CustomOption = ({ children, ...props }: { children?: React.ReactNode; props: any }) => {
  const { onMouseMove, onMouseOver, ...rest } = (props as any).innerProps;
  const newProps = { ...props, innerProps: rest };
  return <components.Option {...(newProps as any)}>{children}</components.Option>;
};*/

export type SelectedList = { [key: string]: boolean };

const ToolTips = ({ listOfOptions }: { listOfOptions: SelectorCardOption[] }) => {
  const body = [];
  for (const option of listOfOptions) {
    body.push(
      <Tooltip id={option.key as string} key={option.key}>
        <Text size="xl" style={{ whiteSpace: 'pre-line' }}>
          {option.value.tooltip}
        </Text>
      </Tooltip>
    );
  }
  return <>{body}</>;
};

const Options = ({
  listOfOptions,
  selectedList,
  setSelectedList,
}: {
  listOfOptions: SelectorCardOption[];
  selectedList: SelectedList;
  setSelectedList: Function;
}) => {
  const body = [];
  for (const option of listOfOptions) {
    body.push(
      <Row className="m-1" key={option.key}>
        <Col>
          <Checkbox
            label={option.value.name}
            checked={selectedList[option.key] ? true : false}
            onChange={() => {
              const value = selectedList[option.key] ? true : undefined;
              setSelectedList((previousList: SelectedList) => {
                const newList = { ...previousList };
                if (newList[option.key] === true) {
                  delete newList[option.key];
                } else {
                  newList[option.key] = true;
                }
                return newList;
              });
            }}
          />
        </Col>
        <Col lg="3">
          <div className="forceRight">
            {!['None', ''].includes(option.value.tooltip) && (
              <a className="tooltip-link" id={option.key as string} href="#">
                <QuestionCircleFill />
              </a>
            )}
            {!['None', ''].includes(option.value.youtubeLink) && (
              <a href={option.value.youtubeLink} className="icon-spacing">
                <Youtube />
              </a>
            )}
            {!['None', ''].includes(option.value.writtenDocumentationLink) && (
              <a href={option.value.writtenDocumentationLink} className="icon-spacing">
                <TextParagraph />
              </a>
            )}
          </div>
        </Col>
      </Row>
    );
  }
  return <>{body}</>;
};

// This type needs to change as you implement more features with it ( IE: Glitches, Plando, etc )  Just use ORs to chain the value type together.
export type SelectorCardOption = {
  value: Trick;
  key: keyof SelectedList;
};

type SelectorCardProps = {
  options: SelectorCardOption[];
  selected: string[];
  add: (xToAdd: SelectedList, setAdditions: Function) => void;
  remove: (xToRemove: SelectedList, setRemovals: Function) => void;
  clear?: () => void;
  label: string;
};

const SelectorCard = ({ options, selected, add, remove, clear, label }: SelectorCardProps) => {
  const remaining = options.filter((x) => !selected.includes(x.key as string));
  const added = options.filter((x) => selected.includes(x.key as string));
  const [selectedToAdd, setAdditions] = useState({});
  const [selectedToRemove, setRemovals] = useState({});

  return (
    <>
      <Row>
        <Col>
          <Button
            label={`Add ${label}`}
            variant="primary"
            iconRight={<CaretRightFill />}
            handleClick={() => {
              console.log(selectedToAdd);
              add(selectedToAdd, setAdditions);
            }}
          />
        </Col>
        <Col>
          <Button
            label="Reset"
            variant="danger"
            handleClick={() => {
              const itemsToReset: SelectedList = {};
              added.forEach((checkedItem) => {
                itemsToReset[checkedItem.key as string] = true;
              });
              remove(itemsToReset, setRemovals);
            }}
          />
        </Col>
        <Col>
          <Button
            label={`Remove ${label}`}
            variant="primary"
            iconLeft={<CaretLeftFill />}
            handleClick={() => {
              remove(selectedToRemove, setRemovals);
            }}
          />
        </Col>
      </Row>
      <Row className="mt-3">
        <Col>
          <ToolTips listOfOptions={remaining} />
          <div className="card">
            <Options listOfOptions={remaining} selectedList={selectedToAdd} setSelectedList={setAdditions} />
          </div>
        </Col>
        <Col>
          <div className="card">
            <Options listOfOptions={added} selectedList={selectedToRemove} setSelectedList={setRemovals} />
          </div>
        </Col>
      </Row>
    </>
  );
};

export default SelectorCard;
