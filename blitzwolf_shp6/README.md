# Blitzwolf SHP-6
https://www.blitzwolf.com/BlitzWolf-BW-SHP6-3450W-WIFI-Smart-Socket-EU-Plug-Works-with-Alexa-Remote-Control-Time-Switch-Electricity-Monitoring-p-412.html


# Hardware
Connect 3.3V, GND, TX and RX.  
GPIO0 has to be grounded in order to enter flashing mode and to work with esptool.  
**Disassembly and pin descriptions**:
https://tasmota.github.io/docs/devices/BlitzWolf-SHP6/

# Software

## Backup the original firmware
  - [high level how to](https://community.blynk.cc/t/how-to-backup-restore-official-firmware-on-any-espressif-esp8266-esp32/34309)
  - get chip info to know how many mibibytes to backup  
  ```
  $ python -m esptool --port COM15 --baud 115200 flash_id
  esptool.py v2.8
  Serial port COM15
  Connecting....
  Detecting chip type... ESP8266
  Chip is ESP8285
  Features: WiFi, Embedded Flash
  Crystal is 26MHz
  MAC: 50:02:91:26:69:eb
  Uploading stub...
  Running stub...
  Stub running...
  Manufacturer: a1
  Device: 4015
  Detected flash size: 2MB
  Hard resetting via RTS pin...```

 - Flash size is 2MB
 - Perform backup  
 `$ python -m esptool -b 115200 --port COM15 read_flash 0x00000 0x200000 original_firmware.bin`


## Prepare `.yaml` config
  - https://community.home-assistant.io/t/esphome-blitzwolf-bw-shp6-configuration/113938/11

## Flash using esphome docker image
  - make sure the config compiles and fix any issues  
    `sudo docker run --rm -v "${PWD}":/config --device=/dev/ttyUSB0 -it esphome/esphome shp6.yaml compile`
  
  - in case of `ESPAsyncTCP.h: No such file or directory` error
      - delete the directory associated with the compilation (same name as the yaml file)
      - compile again using `esphome/esphome:dev` container

  - if the compilation is successful, flash the firmware  
  `sudo docker run --rm -v "${PWD}":/config --device=/dev/ttyUSB0 -it esphome/esphome shp6.yaml run`

## Calibrate the sensor
  - [esphome:calibrate_linear](https://esphome.io/components/sensor/index.html#calibrate-linear)
  - [Calibration guide](https://frenck.dev/calibrating-an-esphome-flashed-power-plug/)
  
  
Calibration data: