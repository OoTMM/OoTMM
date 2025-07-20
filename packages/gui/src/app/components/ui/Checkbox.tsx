import { ComponentProps } from 'react';
import { ImCheckmark } from 'react-icons/im';
import clsx from 'clsx';

type CheckboxProps = {
  checked: boolean;
  onChange: (checked: boolean) => void;
  className?: string;
} & Omit<ComponentProps<'input'>, 'onChange' | 'checked'>;

export function Checkbox({ checked, onChange, className, ...props }: CheckboxProps) {
  return (
    <div className="relative w-6 h-6 min-w-6 min-h-6">
      <input
        type="checkbox"
        checked={checked}
        onChange={() => onChange(!checked)}
        className={clsx(
          'ux-bg ux-border ux-hover ux-outline',
          'w-full h-full appearance-none cursor-pointer',
          'hover:bg-gray-100 dark:hover:bg-gray-700',
          'focus:outline-none',
          'focus:border-blue-500 dark:focus:border-blue-400',
          'disabled:opacity-50 disabled:cursor-not-allowed',
          className
        )}
        {...props}
      />
      <div className={clsx(
        "absolute top-0 left-0 w-full h-full grid place-items-center pointer-events-none transition-all",
        checked ? 'scale-100' : 'scale-0'
      )}>
        <ImCheckmark/>
      </div>
    </div>
  );
}
