import { ResultFile } from '../api';
import { Button, Card } from './ui';

const download = (file: ResultFile) => {
  const a = document.createElement('a');
  let blob: Blob;
  if (file.data instanceof Blob) {
    blob = file.data;
  } else {
    blob = new Blob([file.data], { type: file.mime });
  }
  a.href = window.URL.createObjectURL(blob);
  a.download = file.name;
  a.click();
};

type ResultProps = {
  archive: ResultFile | null;
  warnings: string[];
}
export const Result = ({ archive, warnings }: ResultProps) => {
  return (
    <div className="ux-bg ux-border flex flex-col gap-8 text-center mx-auto mb-8 p-8">
      <h1 className="text-2xl font-bold">Your seed has been generated</h1>
      {!!warnings.length && (
        <div className="bg-yellow-700 p-2 rounded border border-yellow-600 text-left max-h-[200px] overflow-y-auto">
          <h2 className="text-xl mb-2 text-center">Warnings</h2>
          <ul className="list-disc list-inside ml-4">
            {warnings.map((warning, i) => <li key={i}>{warning}</li>)}
          </ul>
        </div>
      )}
      {!archive && <p>Building archive...</p>}
      {archive && <>
        <p>You can now download the seed.<br/>If there are multiple files (spoiler log, etc.), they will be bundled in a zip archive.</p>
        <Button variant="submit" onClick={() => download(archive)}>Download</Button>
      </>}
    </div>
  );
};
