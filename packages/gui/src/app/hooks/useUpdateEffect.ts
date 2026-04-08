import type { EffectCallback } from 'react';
import { useEffect, useRef } from 'react';

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
