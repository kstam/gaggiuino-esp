## ESP component for Gaggiuino
An experimental repo for creating an ESP32 companion to the main STM32 mcu for the Gaggiunio project to manage:
- WiFi connection 
- Webapp UI
- Bluetooh devices (i.e. bluetooth Scales)

### Structure

- src -> ESP32 code 
- web-interface -> React webapp that consumes the ESP32 API

### Flushing to ESP32
To upload this to your ESP32 device you will need:
* npm
* platformio

#### 1. Build the web app
```
cd web-interface
npm install
npm run build
```

#### 2. Upload files to ESP32 files system (SPIFFS)
On plaformio run the "Platform > Upload Filesystem Image" task

#### 3. Uplaoding the ESP32 code
On platformio run the "General > Upload" task

#### 4. Go to the webpage
1. Connect to the Gaggiuino AP WiFi
2. Head to `192.168.4.1` on your browser (default AP IP)

If you connect to the WiFi you can also access the App via the IP of the ESP32 (you can see it on the UI after connecting)