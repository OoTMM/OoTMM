import { useEffect, useRef, useState } from 'react';
import { LuChevronDown } from 'react-icons/lu';
import { FaXmark } from 'react-icons/fa6';
import clsx from 'clsx';

export type SelectOption<T> = {
  value: T;
  label: string;
};

type SelectProps<T> = {
  id?: string;
  options: SelectOption<T>[];
  value: T | T[] | null;
  placeholder?: string;
  clearable?: boolean;
  creatable?: boolean;
  searcheable?: boolean;
  multi?: boolean;
  onSelect: (value: T) => void;
  onUnselect?: (value: T) => void;
  onClear?: () => void;
  onCreate?: (label: string) => void;
}
export function Select<T>({ id, options, value, placeholder, clearable, creatable, searcheable, multi, onSelect, onUnselect, onClear, onCreate }: SelectProps<T>) {
  const containerRef = useRef<HTMLDivElement>(null);
  const inputRef = useRef<HTMLInputElement>(null);
  const [open, setOpen] = useState(false);
  const [inputValue, setInputValue] = useState('');
  placeholder = placeholder ?? '-----';
  const currentLabel = options.find(opt => opt.value === value)?.label ?? '';

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

  let filterFunc: (opt: SelectOption<T>) => boolean;
  if (searcheable) {
    filterFunc = (opt) => opt.label.toLowerCase().includes(inputValue.toLowerCase());
  } else {
    filterFunc = () => true;
  }

  const onFocus = () => {
    setOpen(true);
    setInputValue('');
  };

  /* Detect when we click somewhere else */
  useEffect(() => {
    if (!open) return;
    const onClickOutside = (e: MouseEvent) => {
      if (containerRef.current && !containerRef.current.contains(e.target as Node)) {
        setOpen(false);
      }
    };
    document.addEventListener('mousedown', onClickOutside);
    return () => {
      document.removeEventListener('mousedown', onClickOutside);
    };
  }, [open]);

  let filteredOptions = options.filter(filterFunc);
  if (multi && Array.isArray(value)) {
    filteredOptions = filteredOptions.filter(opt => !value.includes(opt.value));
  }

  return (
    <div ref={containerRef}>
      <div className="ux-bg ux-border ux-hover flex items-center p-2" onClick={() => inputRef.current?.focus()}>
        <div className="flex-1">
          <div className="flex flex-wrap gap-1">
            {multi && Array.isArray(value) && value.map((x, i) => (
              <span key={i} className="text-xs bg-gray-500 p-1 rounded select-none whitespace-nowrap inline-flex items-center gap-1">
                <span>{options.find(opt => opt.value === x)?.label}</span>
                <span onClick={(e) => { e.stopPropagation(); onUnselect?.(x); }}><FaXmark/></span>
              </span>
            ))}
          </div>
          <input
            ref={inputRef}
            type="text"
            autoComplete="off"
            autoCorrect="off"
            id={id}
            readOnly={!creatable && !searcheable}
            placeholder={placeholder}
            className={clsx("outline-none text-left select-none", (multi && Array.isArray(value) && value.length > 0) ? 'sr-only' : 'w-full')}
            value={(open && (creatable || searcheable)) ? inputValue : currentLabel}
            onFocus={onFocus}
            onInput={(e) => { setInputValue((e.target as HTMLInputElement).value); }}
            onKeyDown={onInputKeyDown}
          />
        </div>
        {clearable && value && (!multi || (Array.isArray(value) && value.length > 0)) && <div className="mr-1 dark:hover:text-gray-400 cursor-pointer" onClick={onClear}><FaXmark/></div>}
        <div className="border-l dark:border-gray-500 pl-1">
          <LuChevronDown size={20}/>
        </div>
      </div>
      <div className="relative select-none">
        {open && (searcheable || !inputValue) && <div className="ux-bg ux-border z-1000 absolute top-1 dark:bg-gray-600 rounded border dark:border-gray-500 w-full max-h-[300px] overflow-y-auto">
          {filteredOptions.length > 0 && filteredOptions.map((opt, i) =>
            <div key={i} className="ux-hover p-2" onClick={(e) => { setOpen(false); onSelect(opt.value); e.preventDefault(); e.stopPropagation(); setInputValue(''); }}>
              {opt.label}
            </div>
          )}
          {filteredOptions.length === 0 && <div className="p-8 text-gray-500">Nothing</div>}
        </div>}
      </div>
    </div>
  );
}
