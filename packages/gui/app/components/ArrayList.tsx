import React from 'react';
import Select, { createFilter, components } from 'react-select';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { solid } from '@fortawesome/fontawesome-svg-core/import.macro';
import Group from './Group';

const CustomOption = ({ children, ...props }: { children?: React.ReactNode, props: any }) => {
  const { onMouseMove, onMouseOver, ...rest } = (props as any).innerProps;
  const newProps = { ...props, innerProps: rest };
  return (
    <components.Option {...newProps as any}>
      {children}
    </components.Option>
  );
};

export type ArrayListOption = {
  value: string;
  label: string;
};
type ArrayListProps = {
  options: ArrayListOption[];
  selected: string[];
  add: (x: string) => void;
  remove: (x: string) => void;
  clear?: () => void;
}
export function ArrayList({ options, selected, add, remove, clear }: ArrayListProps) {
  const remaining = options.filter(x => !selected.includes(x.value));

  return (
    <Group direction='vertical' spacing='xl'>
      <Group direction='horizontal'>
        <Select
          className='plando-select'
          filterOption={createFilter({ignoreAccents: false})}
          components={{ Option: CustomOption } as any}
          options={remaining}
          onChange={(v) => v && add(v.value)}
        />
        {clear && <button className="btn-danger" onClick={clear}>Remove All</button>}
      </Group>
      <Group direction='vertical' spacing='xs'>
        {selected.map(v => (
          <Group direction='horizontal' spacing='xs' key={v}>
            <span className="plando-remove" onClick={() => remove(v)}><FontAwesomeIcon icon={solid('xmark')}/></span>
            <span className="plando-item">{options.find(x => x.value === v)?.label || v}</span>
          </Group>
        ))}
      </Group>
    </Group>
  );
}
