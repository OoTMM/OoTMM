export function Home() {
  return (
    <div className="flex flex-col items-center mt-10">
      <p>
        The dev version of the randomizer is updated in real time.<br/>
        Things may be broken, seeds may be unbeatable.<br/>
        You may encounter frustrating game breaking bugs.<br/>
        <br/>
        Please report any bug you find in our <a className="link" target="_blank" href="https://discord.gg/4QdtPBP6wf">Discord</a>.
      </p>
      <div className="flex gap-20 m-4">
        <a className="block p-3  rounded bg-green-500 text-white hover:bg-green-400" href="/gen/stable">Generate a Seed</a>
        <a className="block p-3 border rounded hover:bg-gray-100 dark:hover:bg-gray-800" href="/gen/dev">Generate a Seed (Dev)</a>
      </div>
      <a className="link" href="/gen/old">Older versions</a>
    </div>
  )
}
