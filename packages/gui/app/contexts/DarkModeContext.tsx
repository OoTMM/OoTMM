import { useEffect, useMemo, useState } from 'react';
import { useMediaQuery } from 'react-responsive';

export function DarkModeContext() {
  const darkMode: boolean = JSON.parse(localStorage.getItem('darkMode') || '{}');
  const systemPrefersDark = useMediaQuery(
    {
      query: '(prefers-color-scheme: dark)',
    },
    undefined
  );

  const [isDark, setIsDark] = useState(darkMode);
  const value = useMemo(() => (isDark === undefined ? !!systemPrefersDark : isDark), [isDark, systemPrefersDark]);

  useEffect(() => {
    if (value) {
      document.body.classList.add('dark');
      localStorage.setItem('darkMode', JSON.stringify('true'));
    } else {
      document.body.classList.remove('dark');
      localStorage.setItem('darkMode', JSON.stringify('false'));
    }
  }, [value]);

  return {
    isDark: value,
    setIsDark,
  };
}
