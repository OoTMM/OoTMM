import React from 'react';
import Select, { createFilter, components } from 'react-select';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faXmark } from '@fortawesome/free-solid-svg-icons';

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
    <div>
      <Select
        className='select'
        filterOption={createFilter({ignoreAccents: false})}
        components={{ Option: CustomOption } as any}
        options={remaining}
        onChange={(v) => v && add(v.value)}
      />
      {clear && <button className="btn-danger" onClick={clear}>Remove All</button>}
      <ul>
        {selected.map(v => (
          <li key={v}>
            <span className="list-remove" onClick={() => remove(v)}><FontAwesomeIcon icon={faXmark}/></span>
            <span className="list-item">{options.find(x => x.value === v)?.label || v}</span>
          </li>
        ))}
      </ul>
    </div>
  );
}
