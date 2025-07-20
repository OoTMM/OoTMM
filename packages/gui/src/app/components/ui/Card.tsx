import { ReactNode } from 'react';
import clsx from 'clsx';
import { twMerge } from 'tailwind-merge';

type CardProps = {
  children: ReactNode;
  className?: string;
}
export function Card({ className, children }: CardProps) {
  return (
    <div className={twMerge(clsx(
      'bg-gray-100 dark:bg-gray-800 p-4 rounded border dark:border-slate-700',
      'flex flex-col gap-2',
      className,
    ))}>
      {children}
    </div>
  );
}
