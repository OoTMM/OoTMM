import { useState, useCallback } from 'react';

export type Router<T> = {
  routes: T[];
  currentIndex: number;
  currentRoute: T;
  navigate: (index: number) => void;
};

export function useRouter<T>(routes: T[]): Router<T> {
  const [currentIndex, setCurrentIndex] = useState(0);
  const currentRoute = routes[currentIndex];

  const navigate = useCallback((index: number) => {
    setCurrentIndex(index);
  }, []);

  return {
    routes,
    currentIndex,
    currentRoute,
    navigate,
  };
}
