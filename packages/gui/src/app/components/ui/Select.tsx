import { useEffect, useRef, useState } from 'react';
import { LuChevronDown } from 'react-icons/lu';
import { FaXmark } from 'react-icons/fa6';

export type SelectOption<T> = {
  value: T;
  label: string;
};

type SelectProps<T> = {
  options: SelectOption<T>[];
  value: T | null;
  placeholder?: string;
  clearable?: boolean;
  creatable?: boolean;
  onSelect: (value: T | null) => void;
  onClear?: () => void;
  onCreate?: (label: string) => void;
}
export function Select<T>({ options, value, placeholder, clearable, creatable, onSelect, onClear, onCreate }: SelectProps<T>) {
  const containerRef = useRef<HTMLDivElement>(null);
  const [open, setOpen] = useState(false);
  const [inputValue, setInputValue] = useState('');
  placeholder = placeholder ?? '-----';
  const currentLabel = options.find(opt => opt.value === value)?.label ?? '';

  useEffect(() => {
    const handleClickOutside = (event: MouseEvent) => {
      if (containerRef.current && !containerRef.current.contains(event.target as Node)) {
        setInputValue('');
        setOpen(false);
      }
    };
    document.addEventListener('click', handleClickOutside);
    return () => document.removeEventListener('click', handleClickOutside);
  }, []);

  const onInputKeyDown = (e: KeyboardEvent) => {
    if (e.key === 'Enter' && inputValue.trim() && creatable) {
      e.preventDefault();
      if (onCreate) {
        onCreate(inputValue.trim());
      }
      setInputValue('');
      setOpen(false);
    }
  };

  return (
    <div ref={containerRef}>
      <div className="ux-bg ux-border ux-hover flex items-center p-2" onClick={() => setOpen(!open)}>
        <input
          readOnly={!creatable}
          placeholder={placeholder}
          className="outline-none flex-1 text-left"
          value={(open && creatable) ? inputValue : currentLabel}
          onInput={(e) => { setInputValue((e.target as HTMLInputElement).value); setOpen(true); }}
          onKeyDown={onInputKeyDown}
        />
        {clearable && value && <div className="mr-1 dark:hover:text-gray-400 cursor-pointer" onClick={onClear}><FaXmark/></div>}
        <div className="border-l dark:border-gray-500 pl-1">
          <LuChevronDown size={20}/>
        </div>
      </div>
      <div className="relative">
        {open && !inputValue && <div className="ux-bg ux-border z-10 absolute top-1 dark:bg-gray-600 rounded border dark:border-gray-500 w-full max-h-[300px] overflow-y-auto">
          {options.map((opt, i) =>
            <div key={i} className="ux-hover p-2" onClick={(e) => { e.preventDefault(); e.stopPropagation(); setOpen(false); setInputValue(''); onSelect(opt.value); }}>
              {opt.label}
            </div>
          )}
        </div>}
      </div>
    </div>
  );
}
