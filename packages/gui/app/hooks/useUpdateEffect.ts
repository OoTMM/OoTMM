import { useEffect, useRef, EffectCallback } from 'react';

export function useUpdateEffect(effect: EffectCallback, deps: any[]) {
  const isFirst = useRef(true);

  useEffect(() => {
    if (isFirst.current) {
      isFirst.current = false;
      return;
    }
    return effect();
  }, deps);
}
