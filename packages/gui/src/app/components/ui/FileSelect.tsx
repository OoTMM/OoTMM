import { ComponentProps } from 'react';
import clsx from 'clsx';
import { FaXmark } from 'react-icons/fa6';

type FileSelectProps = {
  imageSrc: string;
  accept: string;
  file: File | null;
  onInput: (file: File | null) => void;
} & Omit<ComponentProps<'input'>, 'onInput'>;

export const FileSelect = ({ imageSrc, accept, onInput, file, ...props }: FileSelectProps) => {
  const handleInput = (e: Event) => {
    const input = e.currentTarget as HTMLInputElement;
    if (input.files && input.files[0]) {
      onInput(input.files[0]);
    }
    input.value = '';
  };

  const handleReset = (e: Event) => {
    e.preventDefault();
    onInput(null);
  };

  return (
    <div className="ux-bg ux-border w-[200px] h-[200px] min-w-[200px] min-h-[200px] relative">
      <input type="file" accept={accept} className="opacity-0 w-full h-full cursor-pointer" onInput={handleInput} {...props}/>
      <img src={imageSrc} className={clsx("absolute w-full h-full object-contain scale-75 inset-0 pointer-events-none", file ? '' : 'grayscale' )}/>
      <span className="absolute bottom-1 left-0 w-full text-center select-none pointer-events-none overflow-hidden overflow-ellipsis">{file ? file.name : "No file selected"}</span>
      {file && <span className="absolute top-1 right-1 cursor-pointer hover:text-gray-500" onClick={handleReset}><FaXmark/></span>}
    </div>
  );
};
