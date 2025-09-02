import { Card } from './ui';

function ProgressBar({ value }: { value: number | null }) {
  return (
    <div className="w-full relative h-[24px] bg-neutral-300 border-neutral-400 dark:bg-neutral-700 border rounded dark:border-neutral-600">
      {(value !== null) && (
        <>
          <div className="rounded absolute top-[-2px] left-[-2px] h-[26px] bg-green-500 border-green-400 dark:bg-green-700 border dark:border-green-600" style={{width: `${value}%`}}/>
          <div className="absolute w-full text-center inset-0 z-10">{value.toFixed(1)}%</div>
        </>
      )}
    </div>
  );
}

type ProgressProps = {
  message: string;
  progress: number | null;
};
export const Progress = ({ message, progress }: ProgressProps) => {
  return (
    <Card className="w-[800px] h-[300px] justify-between p-8 flex flex-col gap-8 items-center">
      <h1 className="text-xl font-bold">Your seed is being generated</h1>
      <p>{message}</p>
      <ProgressBar value={progress}/>
    </Card>
  );
};
