export function Home() {
  return (
    <div>
      <p className="disclaimer">
        The dev version of the randomizer is updated in real time.<br/>
        Things may be broken, seeds may be unbeatable.<br/>
        You may encounter frustrating game breaking bugs.<br/>
        <br/>
        Please report any bug you find in our <a target="_blank" href="https://discord.gg/4QdtPBP6wf">Discord</a>.
      </p>
      <div className="gen-box">
        <a className="gen gen-stable" href="/gen/stable">Generate a Seed</a>
        <a className="gen gen-dev" href="/gen/dev">Generate a Seed (Dev)</a>
      </div>
    </div>
  )
}
