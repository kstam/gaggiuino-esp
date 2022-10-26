import React, { useState, useEffect } from 'react';
import useWebSocket from 'react-use-websocket';
import { Button } from '@mui/material';
import logo from './logo.svg';
import './App.css';
import WifiPage from './components/wifi/WifiPage';
import Chart from './components/chart/ShotChart';
import GaugeChart from './components/chart/GaugeChart';

function App() {
  const [socketUrl] = useState('ws://192.168.18.44/ws');
  const { lastJsonMessage, readyState } = useWebSocket(socketUrl, {
    share: true,
    onError: (e) => console.log('Error!', e),
    onClose: (e) => console.log('Connection closed!', e),
  });
  const [sensorData, setSensorData] = useState([]);
  const [lastSensorData, setLastSensorData] = useState({
    temp: 0, pressure: 0, timeInShot: 0, flow: 0, weight: 0,
  });

  useEffect(() => {
    if (lastJsonMessage !== null && lastJsonMessage.action === 'sensor_data_update') {
      setSensorData((prev) => {
        if (prev.length >= 400) {
          prev.shift();
        }
        return prev.concat(lastJsonMessage.data);
      });
      setLastSensorData(lastJsonMessage.data);
    }
  }, [lastJsonMessage]);

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

      <div className="gauges-container">
        <div><GaugeChart value={lastSensorData.temp} primaryColor="#ef4040" title="Temperature" unit="°C" /></div>
        <div><GaugeChart value={lastSensorData.pressure} primaryColor="#6296C5" title="Pressure" unit="bar" /></div>
        <div><GaugeChart value={lastSensorData.flow} primaryColor="#844B48" title="Flow" unit="ml/s" /></div>
        <div><GaugeChart value={lastSensorData.weight} primaryColor="#ffb53e" title="Weight" unit="g" /></div>
      </div>

      <div className="graph-container">
        <div><Chart data={sensorData} /></div>
      </div>

      <Button>Clear</Button>
      <div>
        <h4>Messages</h4>
        Connection state:
        {readyState}
      </div>
    </div>
  );
}

export default App;
