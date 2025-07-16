import { useCallback } from 'react';

import { useTheme } from './ThemeProvider';

export function ThemeSwitch() {
  const [theme, setTheme] = useTheme();

  const toggle = useCallback(() => {
    setTheme(theme === 'light' ? 'dark' : 'light');
  }, [theme]);

  return (
    <button onClick={toggle}>{theme === 'light' ? 'Dark' : 'Light'} Mode</button>
  );
}
