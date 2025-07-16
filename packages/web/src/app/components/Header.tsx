import logo from '../../assets/logo.png';
import { ThemeSwitch } from '../theme/ThemeSwitch';

export function Header() {
	return (
		<header class="header">
      <a class="header-title" href="/">
        <img class="logo" alt="OoTMM Logo" src={logo}/>
        <h1 class="title">OoTMM</h1>
      </a>
      <nav class="header-nav">
        <a href="/faq">FAQ</a>
        <a target="_blank" href="https://discord.gg/4QdtPBP6wf">Discord</a>
        <a href="/multiplayer">Multiplayer Guide</a>
        <ThemeSwitch/>
      </nav>
    </header>
	);
}

