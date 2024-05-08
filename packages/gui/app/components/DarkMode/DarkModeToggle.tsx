import React, { Dispatch } from 'react';
import Form from 'react-bootstrap/Form';
import { MoonFill } from 'react-bootstrap-icons';

const DarkModeToggle = (props: { isDark: boolean; setIsDark: Dispatch<boolean> }) => {
  const { isDark, setIsDark } = props;
  return (
    <Form>
      <Form.Check // prettier-ignore
        className="darkModeToggle"
        type="switch"
        id="dark-mode-toggle"
        checked={isDark}
        label={
          <div className="flex mt-1">
            <MoonFill />
            <span className="mx-2">Dark Mode</span>
          </div>
        }
        onChange={({ target }) => setIsDark(target.checked)}
        aria-label="Dark mode toggle"
      />
    </Form>
  );
};

export default DarkModeToggle;
