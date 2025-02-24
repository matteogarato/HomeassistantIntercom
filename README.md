# Home Assistant Integrated Gate Opener

This project allows you to control gate relays via MQTT using an ESP32, integrating with Home Assistant for automation. The system controls two gates, a regular gate and a "big gate" (for cars) via relays, and uses a WiFi connection to communicate with an MQTT broker.

## Features

- WiFi connectivity with an ESP32.
- Control of two gates using GPIO pins and relays.
- MQTT integration for remote control (via Home Assistant or any MQTT client).
- LED indicator to show the status of the gate relays.
- Basic callback handling to open gates via MQTT messages.

## Components

- **ESP32 board**
- **2 Relays** (one for the regular gate and one for the big gate)
- **LED** (for visual status) - use the one integrated on the board
- **WiFi network** (for connecting to the ESP32)
- **MQTT Broker** (for communication between devices)

## Requirements

- ESP32 microcontroller.
- Arduino IDE or PlatformIO for development.
- Libraries:
  - `WiFi.h` (for WiFi connection)
  - `PubSubClient.h` (for MQTT client functionality)

## Setup

1. **Install the ESP32 Board in Arduino IDE:**
   - Open the Arduino IDE.
   - Go to `File` -> `Preferences` and add the following URL to the `Additional Boards Manager URLs` field:
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Go to `Tools` -> `Board` -> `Boards Manager`, search for `ESP32`, and install it.

2. **Install Required Libraries:**
   - Open the Arduino IDE and go to `Sketch` -> `Include Library` -> `Manage Libraries`.
   - Search for and install the following libraries:
     - **WiFi**
     - **PubSubClient**

3. **Configure WiFi and MQTT Broker:**
   - In the code, replace the `ssid` and `password` with your WiFi credentials.
   - Update `mqttServer`, `mqttUser`, and `mqttPassword` with your MQTT broker’s IP address, username, and password.

4. **Wire the Circuit:**
   - Connect the regular gate relay to GPIO pin 15 (`GATE_RELAY_PIN`).
   - Connect the big gate relay to GPIO pin 16 (`BIG_GATE_RELAY_PIN`).

5. **Upload the Code to the ESP32:**
   - Select the correct board and port in the Arduino IDE.
   - Click `Upload`.

## Usage

Once the system is set up and running, you can control the gates by sending MQTT messages to the following topics:

- **Regular Gate:** `home/electrical/gaterelay/set`
- **Big Gate:** `home/electrical/biggaterelay/set`

You can integrate this with Home Assistant or use an MQTT client to publish messages to these topics to trigger the opening of the gates.

### Example MQTT Payload

To open the regular gate:
- Topic: `home/electrical/gaterelay/set`
- Payload: `1` (or any non-zero value)

To open the big gate:
- Topic: `home/electrical/biggaterelay/set`
- Payload: `1` (or any non-zero value)

## Code Explanation

- **WiFi Setup:**
  The ESP32 connects to the WiFi network specified in the code using `WiFi.begin(ssid, password)`. It will attempt to connect until it succeeds.
  
- **MQTT Setup:**
  The `PubSubClient` library is used to connect the ESP32 to an MQTT broker. The MQTT server's IP, username, and password must be provided in the code.

- **Relay Control:**
  - `openGate()` controls the regular gate relay. It turns on the relay (via GPIO 15), waits for a set time (`ON_TIME`), and then turns it off.
  - `openBigGate()` works similarly but controls the big gate relay (via GPIO 16).

- **MQTT Callback:**
  The `callback()` function is triggered whenever a message is received on the subscribed topics (`home/electrical/gaterelay/set` and `home/electrical/biggaterelay/set`). It checks the topic and calls the corresponding function (`openGate()` or `openBigGate()`).

## Example

### 1. MQTT Control via Home Assistant

In your `configuration.yaml` for Home Assistant, you can add the following automation to control the gates:

```yaml
  button:
    - name: "Cancelletto" # Choose an easy-to-recognize name
      command_topic: "home/electrical/gaterelay/set" # Topic to publish commands
    - name: "Cancello" # Choose an easy-to-recognize name
      command_topic: "home/electrical/biggaterelay/set" # Topic to publish commands
```

## Troubleshooting

- Cannot connect to WiFi: Ensure your WiFi credentials are correct, and that the ESP32 is within range of the WiFi router.
- MQTT connection fails: Double-check the MQTT broker address, username, and password. Ensure the broker is running and accessible.
- Relay not working: Verify the wiring and the correct GPIO pins are being used for the relays.

## Notes

- Make sure to provide adequate power to the relay module, especially if controlling high-power devices.
- Consider implementing security measures such as TLS encryption and authentication for MQTT communication in production environments.

## Contributing

Contributions are welcome! If you find any issues or have suggestions for improvements, please open an issue or submit a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.