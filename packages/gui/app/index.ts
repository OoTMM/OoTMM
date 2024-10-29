import './index.css';

import { render, createElement } from 'preact';
import AppContainer from './components/AppContainer';

const container = document.getElementById('root')!;
render(createElement(AppContainer, {}), container);
