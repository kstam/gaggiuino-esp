import React from 'react';
import logo from './logo.svg';
import './App.css';
import WifiPage from './components/wifi/WifiPage';

function App() {
  return (
    <div className="App">
      <header className="App-header">
        <img src={logo} className="App-logo" alt="logo" />
        <h1>Gaggiuino</h1>
        <a
          className="App-link"
          href="https://github.com/Zer0-bit/gaggiuino"
          target="_blank"
          rel="noopener noreferrer"
        >
          About
        </a>
      </header>
      <WifiPage />
    </div>
  );
}

export default App;
