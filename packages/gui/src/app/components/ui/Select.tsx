import { useEffect, useRef, useState } from 'react';
import { LuChevronDown } from 'react-icons/lu';
import { FaXmark } from 'react-icons/fa6';

export type SelectOption<T> = {
  value: T;
  label: string;
};

type SelectProps<T> = {
  id?: string;
  options: SelectOption<T>[];
  value: T | null;
  placeholder?: string;
  clearable?: boolean;
  creatable?: boolean;
  searcheable?: boolean;
  onSelect: (value: T | null) => void;
  onClear?: () => void;
  onCreate?: (label: string) => void;
}
export function Select<T>({ id, options, value, placeholder, clearable, creatable, searcheable, onSelect, onClear, onCreate }: SelectProps<T>) {
  const containerRef = useRef<HTMLDivElement>(null);
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
    const onClickOutside = (e: MouseEvent) => {
      if (containerRef.current && !containerRef.current.contains(e.target as Node)) {
        setOpen(false);
      }
    };
    document.addEventListener('mousedown', onClickOutside);
    return () => {
      document.removeEventListener('mousedown', onClickOutside);
    };
  }, []);

  const filteredOptions = options.filter(filterFunc);

  return (
    <div ref={containerRef}>
      <div className="ux-bg ux-border ux-hover flex items-center p-2">
        <input
          type="text"
          autoComplete="off"
          autoCorrect="off"
          id={id}
          readOnly={!creatable && !searcheable}
          placeholder={placeholder}
          className="outline-none flex-1 text-left"
          value={(open && (creatable || searcheable)) ? inputValue : currentLabel}
          onFocus={onFocus}
          onInput={(e) => { setInputValue((e.target as HTMLInputElement).value); }}
          onKeyDown={onInputKeyDown}
        />
        {clearable && value && <div className="mr-1 dark:hover:text-gray-400 cursor-pointer" onClick={onClear}><FaXmark/></div>}
        <div className="border-l dark:border-gray-500 pl-1">
          <LuChevronDown size={20}/>
        </div>
      </div>
      <div className="relative select-none">
        {open && (searcheable || !inputValue) && <div className="ux-bg ux-border z-10 absolute top-1 dark:bg-gray-600 rounded border dark:border-gray-500 w-full max-h-[300px] overflow-y-auto">
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
