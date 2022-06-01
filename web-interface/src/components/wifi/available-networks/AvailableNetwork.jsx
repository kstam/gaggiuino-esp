import React, { useState } from 'react';
import SignalWifi3BarIcon from '@mui/icons-material/SignalWifi3Bar';
import { Alert, Button, TextField } from '@mui/material';
import PropTypes from 'prop-types';
import { Network } from '../NetworkPropTypes';
import { connectToWifi } from '../../client/WifiClient';
import Loader from '../../loader/Loader';

export default function AvailableNetwork({
  network,
  onClick,
  expanded,
  onConnected,
}) {
  const [password, setPassword] = useState('');
  const [connecting, setConnecting] = useState(false);
  const [connectionError, setConnectionError] = useState(false);

  async function handleSubmit() {
    setConnecting(true);
    setConnectionError(false);
    try {
      await connectToWifi({
        ssid: network.ssid,
        pass: password,
      });
      onConnected(network);
    } catch (e) {
      setConnectionError(true);
    } finally {
      setConnecting(false);
    }
  }

  return (
    <div
      className="NetworkItem"
      key={`${network.ssid}${network.rssi}`}
    >
      <header
        onClick={() => onClick()}
        onKeyUp={() => {}}
        tabIndex="0"
        role="link"
      >
        <SignalWifi3BarIcon />
        <div>{network.ssid}</div>
      </header>
      <div className={`Content${expanded ? ' expanded' : ' hidden'}`}>
        {connectionError && <Alert severity="error">Failed to connect to WiFi</Alert>}
        <TextField
          id="outlined-basic"
          size="small"
          type="password"
          label="Password"
          variant="outlined"
          value={password}
          onChange={(event) => setPassword(event.target.value)}
        />
        <Button type="submit" size="large" variant="outlined" onClick={() => handleSubmit()} disabled={connecting}>
          {connecting && <Loader />}
          Connect
        </Button>
      </div>
    </div>
  );
}

AvailableNetwork.propTypes = {
  network: Network.isRequired,
  onClick: PropTypes.func,
  onConnected: PropTypes.func,
  expanded: PropTypes.bool,
};

AvailableNetwork.defaultProps = {
  onClick: () => {},
  onConnected: () => {},
  expanded: false,
};
