import { ReactNode } from 'react';

type RadioCardProps = {
  selected?: boolean;
  onSelect?: () => void;
  children?: ReactNode;
};

export function RadioCard({ selected, onSelect, children }: RadioCardProps) {
  return (
    <div onClick={onSelect} className="cursor-pointer flex items-center ux-bg ux-hover ux-outline p-2 first:rounded-t-lg last:rounded-b-lg border not-first:border-t-0 dark:border-gray-600 border-gray-400">
      <div className="p-1 mr-2">
        <div className="w-[20px] h-[20px] rounded-full border border-gray-400">
          <div className={`transition-all w-full h-full rounded-full bg-black dark:bg-white ${selected ? 'scale-70 ' : 'scale-0'}`} onClick={onSelect}></div>
        </div>
      </div>
      <div className="flex flex-col w-full">
        {children}
      </div>
    </div>
  );
}
