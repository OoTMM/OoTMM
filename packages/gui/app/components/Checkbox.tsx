import React from 'react';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { solid } from '@fortawesome/fontawesome-svg-core/import.macro';
import { Group } from './Group';
import { Text } from './Text';
type CheckboxProps = {
  label?: string;
  checked: boolean;
  tooltip?: string;
  onChange: (checked: boolean) => void;
}

export const Checkbox = ({ label, checked, tooltip, onChange }: CheckboxProps) => (
  <label>
    <Group direction='horizontal'>
      <input
        type="checkbox"
        checked={checked}
        onChange={(e) => onChange(e.target.checked)}
        />
        <Group direction='horizontal' spacing='xs' style={{paddingTop: '4px'}}>
          <Text size='xl'>{label}</Text>
          {tooltip && <a className="tooltip-link" id={tooltip} href="#"><FontAwesomeIcon icon={solid('question-circle')}/></a>}
        </Group>
    </Group>
  </label>
);
