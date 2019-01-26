# lightswitch-ds

An Arduino sketch for an simple low-power MQTT lightswich baased on ESP8266-01

## Features

- Publish an MQTT message on a simple button press
- Low power consumtion thanks to deep sleep
- OTA Updates

## Setup

First you need to clone this repo by:
```
git clone https://github.com/Robinhuett/lightswitch-ds.git
cd lightswitch-ds
```

After that you can rename `yoursecrets.h` to `secrets.h`.

The only thing left to do is to fill in your WiFi settings and you're good to go.

### Customize

Everything you should need to customize is directly in the top of `lightswitch-ds.ino`.

Mainly you should need to alter the following variables:
- `common_name` Hostname for your ESP.
- `wifi_ssid` Your WiFi (can be set in `secrets.h`).
- `wifi_password` Your WiFi password (can be set in `secrets.h`).
- `mqtt_server` The hostname or IP adress of your MQTT broker.
- `mqtt_topic` The MQTT topic you want to publish in.
- `mqtt_payload` The MQTT payload you want to publish.
- `ota_password` The password to secure OTA updates (can be set in `secrets.h`).

### First Flash

The first flash has to be done via USB. There are many good resources online on how to do this.

### Hardware changes

If you want an even lower power consumption you can simply desolder the red alsway on LED from your ESP.

## Usage

The usage is quiet simple. Just wire a button to the `RST` pin.
When you push the button the ESP starts, pubishes your message und goes back to deep sleep afterwards.
Thats it.

### Flash

If you want to change something on the firmware at this point, you can simply updated it via ArduinoOTA updates.

Simply push your button so the ESP wakes up. If you are fast you can update it directly. But to make things a bit easier you can now pull down `GPIO2`. Now the ESP wont go to sleep and you have all the time in the world to do your updates.

## Authors

* **Robin Tripp** - *Initial work* - [Robinhuett](https://github.com/Robinhuett)

See also the list of [contributors](https://github.com/Robinhuett/lightswitch-ds/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details