import { createContext, useState, useRef, useEffect, useContext, useMemo } from 'react';

type ThemeContext = {
  theme: 'light' | 'dark';
  setTheme: (theme: 'light' | 'dark') => void;
};

const ThemeContext = createContext(null as ThemeContext);

export function ThemeProvider({ children }: { children: React.ReactNode }) {
  const defaultTheme = document.documentElement.style.colorScheme as 'light' | 'dark';
  const [theme, setTheme] = useState<'light' | 'dark'>(defaultTheme);
  const isMounted = useRef(false);

  useEffect(() => {
    if (!isMounted.current) {
      isMounted.current = true;
      return;
    }
    const root = document.documentElement;
    root.style.colorScheme = theme;
    root.classList.remove('light', 'dark');
    root.classList.add(theme);
    localStorage.setItem('theme', theme);
  }, [theme]);

  return (
    <ThemeContext.Provider value={{ theme, setTheme }}>
      {children}
    </ThemeContext.Provider>
  );
}

export function useTheme() {
  const ctx = useContext(ThemeContext);
  const result = useMemo(() => [ctx.theme, ctx.setTheme] as const, [ctx]);
  return result;
}
