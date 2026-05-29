import { useEffect, useRef, useState } from 'react';
import { LuChevronDown } from 'react-icons/lu';
import { FaXmark } from 'react-icons/fa6';
import clsx from 'clsx';

export type MultiSelectOption<T> = {
  value: T;
  label: string;
};

type MultiSelectProps<T> = {
  id?: string;
  options: MultiSelectOption<T>[];
  value: T[];
  placeholder?: string;
  clearable?: boolean;
  creatable?: boolean;
  searcheable?: boolean;
  onSelect: (value: T) => void;
  onUnselect?: (value: T) => void;
  onClear?: () => void;
  onCreate?: (label: string) => void;
};

export function MultiSelect<T>({
                                 id,
                                 options,
                                 value,
                                 placeholder = '-----',
                                 clearable,
                                 creatable,
                                 searcheable,
                                 onSelect,
                                 onUnselect,
                                 onClear,
                                 onCreate,
                               }: MultiSelectProps<T>) {
  const containerRef = useRef<HTMLDivElement>(null);
  const inputRef = useRef<HTMLInputElement>(null);

  const [open, setOpen] = useState(false);
  const [inputValue, setInputValue] = useState('');

  const selectedOptions = options.filter((opt) => value.includes(opt.value));

  const displayLabel =
      selectedOptions.length === 1
          ? selectedOptions[0].label
          : selectedOptions.length > 1
              ? `${selectedOptions.length} Events Selected`
              : '';

  const isSelected = (optionValue: T) => value.includes(optionValue);

  const handleToggleOption = (optionValue: T) => {
    if (isSelected(optionValue)) {
      onUnselect?.(optionValue);
    } else {
      onSelect(optionValue);
    }

    setInputValue('');
    setOpen(true);
  };

  const onInputKeyDown = (e: React.KeyboardEvent<HTMLInputElement>) => {
    if (e.key === 'Enter' && inputValue.trim() && creatable) {
      e.preventDefault();

      onCreate?.(inputValue.trim());

      setInputValue('');
      setOpen(true);
    }
  };

  const filterFunc = (opt: MultiSelectOption<T>) => {
    if (!searcheable) return true;

    return opt.label.toLowerCase().includes(inputValue.toLowerCase());
  };

  const onFocus = () => {
    setOpen(true);
  };

  useEffect(() => {
    if (!open) return;

    const onClickOutside = (e: MouseEvent) => {
      if (containerRef.current && !containerRef.current.contains(e.target as Node)) {
        setOpen(false);
        setInputValue('');
      }
    };

    document.addEventListener('mousedown', onClickOutside);

    return () => {
      document.removeEventListener('mousedown', onClickOutside);
    };
  }, [open]);

  const filteredOptions = options.filter(filterFunc);

  return (
      <div ref={containerRef}>
        <div
            className="ux-bg ux-border ux-hover flex items-center p-2"
            onClick={() => {
              setOpen(true);
              inputRef.current?.focus();
            }}
        >
          <div className="flex-1">
            {open && (creatable || searcheable) ? (
                <input
                    ref={inputRef}
                    type="text"
                    autoComplete="off"
                    autoCorrect="off"
                    id={id}
                    placeholder={displayLabel || placeholder}
                    className="w-full outline-none text-left select-none bg-transparent"
                    value={inputValue}
                    onFocus={onFocus}
                    onInput={(e) => {
                      setInputValue((e.target as HTMLInputElement).value);
                    }}
                    onKeyDown={onInputKeyDown}
                />
            ) : (
                <div
                    className={clsx(
                        'outline-none text-left select-none',
                        displayLabel ? '' : 'text-gray-500',
                    )}
                >
                  {displayLabel || placeholder}
                </div>
            )}
          </div>

          {clearable && value.length > 0 && (
              <div
                  className="mr-1 dark:hover:text-gray-400 cursor-pointer"
                  onClick={(e) => {
                    e.stopPropagation();
                    onClear?.();
                    setInputValue('');
                  }}
              >
                <FaXmark />
              </div>
          )}

          <div className="border-l dark:border-gray-500 pl-1">
            <LuChevronDown size={20} />
          </div>
        </div>

        <div className="relative select-none">
          {open && (
              <div className="ux-bg ux-border z-50 absolute top-1 dark:bg-gray-600 rounded border dark:border-gray-500 w-full max-h-[300px] overflow-y-auto">
                {filteredOptions.length > 0 &&
                    filteredOptions.map((opt, i) => {
                      const checked = isSelected(opt.value);

                      return (
                          <div
                              key={i}
                              className="ux-hover p-2 flex items-center gap-2 cursor-pointer"
                              onClick={(e) => {
                                e.preventDefault();
                                e.stopPropagation();
                                handleToggleOption(opt.value);
                              }}
                          >
                            <input
                                type="checkbox"
                                checked={checked}
                                readOnly
                                className="cursor-pointer"
                            />

                            <span>{opt.label}</span>
                          </div>
                      );
                    })}

                {filteredOptions.length === 0 && (
                    <div className="p-8 text-gray-500">Nothing</div>
                )}
              </div>
          )}
        </div>
      </div>
  );
}