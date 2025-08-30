import logo from '../../assets/logo.png';
import { ThemeSwitch } from '../theme/ThemeSwitch';

export function Header() {
	return (
		<header className="flex items-center h-16 bg-gradient-to-r from-blue-900 to-purple-900 text-white">
      <a href="/" className="flex items-center nav-link">
        <img className="rounded-2xl h-16 object-contain p-1 ml-3 mr-1" alt="OoTMM Logo" src={logo}/>
        <h1 className="text-4xl font-bold">OoTMM</h1>
      </a>
      <nav className="flex text-xl items-center gap-8 ml-10">
        <a className="nav-link" href="/faq">FAQ</a>
        <a className="nav-link" href="/multiplayer">Multiplayer Guide</a>
        <a className="nav-link" target="_blank" href="https://discord.gg/4QdtPBP6wf">Discord</a>
        <a className="nav-link" target="_blank" href="https://ootmm.wiki.gg">Wiki</a>
      </nav>
      <div className="flex-grow"></div>
      <div className="mr-8">
        <ThemeSwitch/>
      </div>
    </header>
	);
}

