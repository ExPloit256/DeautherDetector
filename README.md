# ESP32 Deauther Detector

A lightweight and customizable ESP32-based tool designed to detect deauthentication packets in WiFi networks.

The **ESP32 Deauther Detector** monitors nearby WiFi traffic and identifies deauthentication frames. It offers two modes: **Solo Mode**, where the ESP32 creates its own access point (AP), and **Leech Mode**, where it connects to an existing WiFi network. The onboard LED provides a simple visual alert whenever deauth packets are detected, making it intuitive and easy to use.

## Features

- **Solo Mode (AP Mode)**: Creates an access point with a customizable SSID, allowing the ESP32 to monitor for deauthentication frames attacks on the AP(can be useful for automated Deauth attack such as the one used in [DeautherINO](https://github.com/ExPloit256/DeautherINO).
- **Leech Mode (Station Mode)**: Connects to an existing WiFi network, enabling deauthentication packet monitoring on the network selected by the user.
- **LED Notification**: The onboard LED blinks for 1 second each time a deauthentication packet is detected.
- **Real-Time Monitoring**: Logs deauthentication frames, displaying the BSSID and source MAC via serial output.
- **Configurable Settings**: Easily modify SSID, network credentials, and mode settings.

## How It Works

The ESP32 operates in promiscuous mode, listening for specific WiFi frames. When a deauthentication frame is detected, the detector logs the attacker's source MAC address and the network's BSSID. Simultaneously, the LED blinks, giving you immediate visual feedback that deauthentication activity is occurring.

## Modes

- **Solo Mode**: The ESP32 creates an open access point (SSID: `DeautherDetector-AP` by default) and monitors deauthentication frames without being connected to any other network.
- **Leech Mode**: The ESP32 connects to an existing WiFi network using credentials defined in the code. This mode is ideal for monitoring deauth packets while being part of the target network.

## Default Configuration

- **Solo Mode**: Enabled by default (`useSoloMode = true`)
- **Access Point SSID**: `DeautherDetector-AP`
- **AP Password**: None (open network)
- **Leech Mode SSID and Password**: Configurable via `network_SSID` and `network_password` in the code.

## LED Feedback

The onboard LED (default GPIO 2) blinks for 1 second every time a deauthentication frame is detected, offering instant visual feedback so you can quickly identify attacks in real time.

## Usage

1. **Install the Arduino IDE** and make sure the **ESP32 Arduino Core** (version 2.0.0) is installed.
2. **Upload the code** to your ESP32 using the Arduino IDE.
3. **Select your mode** by setting the `useSoloMode` variable in the code:
   - `true`: Solo Mode (AP Mode)
   - `false`: Leech Mode (Station Mode)
4. **Monitor the serial output** to see when deauthentication packets are detected, along with the source MAC and BSSID of the network under attack.
5. **Watch the onboard LED** for visual feedback when deauth frames are detected.

## Legal and Ethical Notice

This tool is intended solely for ethical hacking, educational purposes, and testing your own networks. **Do not use this on any network without explicit permission**, as doing so is illegal and unethical.

## Installation Instructions:

You will need to use **ESP32 for Arduino version 2.0.0** for DeautherINO to work properly.

1. Install the Arduino IDE and add to the boards manager ESP32 version 2.0.0.
2. Open `platform.txt`:
   - **Windows**: It should be at `C:\\Users\\<USERNAME>\\AppData\\Local\\Arduino15\\packages\\esp32\\hardware\\esp32\\2.0.0\\platform.txt`
   - **Linux**: It should be at `~/.arduino15/packages/esp32/hardware/esp32/2.0.0/platform.txt`
3. Open the file using any text editor.
4. Add `-w` to the following compiler settings:
   ```txt
   build.extra_flags.esp32
   build.extra_flags.esp32s2
   build.extra_flags.esp32s3
   build.extra_flags.esp32c3
5. Add `-zmuldefs` to the following compiler settings.
   ```txt
    compiler.c.elf.libs.esp32
    compiler.c.elf.libs.esp32s2
    compiler.c.elf.libs.esp32s3
    compiler.c.elf.libs.esp32c3
6. Open the Serial Monitor at a baud rate of `115200`.
