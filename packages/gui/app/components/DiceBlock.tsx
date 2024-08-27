import { faDice, faDiceD6 } from '@fortawesome/free-solid-svg-icons';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import React from 'react';

type DiceBlockProps = {
  checked: boolean;
  onChange: (checked: boolean) => void;
}

export const DiceBlock = ({ checked, onChange }: DiceBlockProps) => (
  <label style={{marginTop: "0", marginBottom: "0"}}>
    <>
    <input
        type="checkbox"
        checked={checked}
        onChange={(e) => onChange(e.target.checked)}
        style={{display: "none"}}
        />
        {checked && <FontAwesomeIcon icon={faDice} />}
        {!checked && <FontAwesomeIcon icon={faDiceD6} />}
    </>
  </label>
);
