/* eslint-disable jsx-a11y/interactive-supports-focus */
/* eslint-disable jsx-a11y/anchor-is-valid */
/* eslint-disable jsx-a11y/control-has-associated-label */
/* eslint-disable jsx-a11y/click-events-have-key-events */
import React, { useState, useEffect } from 'react';
import { Replay } from '@mui/icons-material';
import WifiStatus from './WifiStatus';
import { getWifiStatus } from '../client/WifiClient';
import AvailableNetworks from './available-networks/AvailableNetworks';
import Loader from '../loader/Loader';

export default function WifiPage() {
  const [wifiStatus, setWifiStatus] = useState(null);
  const [wifiStatusLoading, setWifiStatusLoading] = useState(true);

  async function loadWiFiStatus() {
    try {
      setWifiStatusLoading(true);
      const status = await getWifiStatus();
      setWifiStatus(status);
    } catch (e) {
      setWifiStatus(null);
    } finally {
      setWifiStatusLoading(false);
    }
  }

  useEffect(() => {
    loadWiFiStatus();
  }, []);

  return (
    <div className="WifiPage">
      <h2 className="Inline-Icon">
        WiFi status
        {' '}
        <a href="#" onClick={() => loadWiFiStatus()}><Replay /></a>
      </h2>
      {wifiStatusLoading && <Loader /> }
      {!wifiStatusLoading && (
      <WifiStatus
        status={wifiStatus}
        onDisconnected={() => loadWiFiStatus()}
      />
      )}
      <h2>Available networks</h2>
      <AvailableNetworks onConnected={() => loadWiFiStatus()} />
    </div>
  );
}
