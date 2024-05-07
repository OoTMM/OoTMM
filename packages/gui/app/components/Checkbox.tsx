import React from 'react';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faQuestionCircle } from '@fortawesome/free-solid-svg-icons';

import { Group } from './Group';
import { Text } from './Text';
import { Tooltip } from './Tooltip';

type CheckboxProps = {
  label?: string;
  checked: boolean;
  tooltip?: React.ReactNode;
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
          {tooltip && <Tooltip>{tooltip}</Tooltip>}
        </Group>
    </Group>
  </label>
);
