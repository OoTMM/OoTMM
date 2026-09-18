import type { ReactNode } from 'react';
import { cn } from '@/app/util';

type CardProps = {
  children: ReactNode;
  className?: string;
}
export function Card({ className, children }: CardProps) {
  return (
    <div className={cn(
      'bg-gray-200 dark:bg-gray-800 p-4 rounded border border-gray-400 dark:border-slate-700',
      'flex flex-col gap-2',
      className,
    )}>
      {children}
    </div>
  );
}
