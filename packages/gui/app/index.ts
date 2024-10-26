import './index.css';

import { Buffer } from 'buffer';
globalThis.Buffer ||= Buffer;

import React from 'react';
import { createRoot } from 'react-dom/client';
import AppContainer from './components/AppContainer';

const container = document.getElementById('root')!;
const root = createRoot(container);
root.render(React.createElement(AppContainer));
