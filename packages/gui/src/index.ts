import './index.css';

import { render, createElement } from 'preact';
import AppContainer from './app/components/AppContainer';

const container = document.getElementById('root')!;
render(createElement(AppContainer, {}), container);
