import { useCallback } from 'react';
import { LuSun, LuMoon } from 'react-icons/lu';

import { useTheme } from './ThemeProvider';

export function ThemeSwitch() {
  const [theme, setTheme] = useTheme();

  const toggle = useCallback(() => {
    setTheme(theme === 'light' ? 'dark' : 'light');
  }, [theme]);

  const selectorStyle = {
    left: theme === 'light' ? '0' : 'calc(100% - 16px)',
  };

  return (
    <button onClick={toggle} className="p-2 rounded-full bg-gray-700">
      <div className="relative flex gap-4">
        <div className="absolute w-[20px] h-[20px] ml-[-2px] mt-[-2px] rounded-full bg-gray-400 transition-all" style={selectorStyle}/>
        <LuSun size="16"/>
        <LuMoon size="16"/>
      </div>
    </button>
  );
}
