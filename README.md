# Medieval-Climate-Forecast-Monitor
A medieval themed 3D design where it uses TRIOE board as microcontroller along with the BME280 sensor for temperature, humidity, and air pressure. Forecast prediction was also implemented using the said parameters and sensor

<p align="center">
  <img src="https://github.com/user-attachments/assets/064888bd-3849-440a-8005-90c39d811a12"
alt="Project cover" width="100%">
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Microcontroller-ESP32--S3-blue">
  <img src="https://img.shields.io/badge/Sensor-BME280-green">
  <img src="https://img.shields.io/badge/Display-1.5%22%20SPI%20LCD-orange">
  <img src="https://img.shields.io/badge/Enclosure-3D%20Printed-lightgrey">
</p>

---

## Features

- **Environmental Sensing:** Measures temperature, humidity, and atmospheric pressure with the BME280.
- **Forecast Prediction:** Uses temperature, humidity, and atmospheric pressure when predicting rain probability.
- **On-Device Display:** A 1.5" SPI LCD shows live readings, sensor status, and alerts.
- **Web Dashboard:** View your readings from any device on your network.
- **Fully 3D-Printed Enclosure:** Base, wall display, sensor holder, and sliding lock are all printable.
- **Expandable Platform:** A starting point for future IoT and embedded-system projects.

## Demo

![demo](assets/demo.gif)

---

## Getting Started

### 1. Gather Parts

| Component | Notes |
|---|---|
| TRIOE ESP32-S3 microcontroller | Main controller |
| BME280 sensor | Temperature, humidity, pressure |
| 1.5" LCD display (SPI) | Shows readings |
| 3D-printed TRIOE base | Holds the microcontroller on 3 posts |
| 3D-printed wall display | Holds the LCD and sensor holder |
| 3D-printed BME280 sensor holder | Torch-style holder with flame design |
| Male pin headers and connecting wires | For soldering and wiring |

### 2. Print Parts

Print the base, wall display, sensor holder, and sliding lock from the `hardware/` folder.

### 3. Build the Hardware

<details>
<summary><b>Step-by-step assembly (click to expand)</b></summary>

1. **Prepare the components.** Lay out everything from the parts list above.
2. **Solder the pin headers.** Solder male pin headers onto the TRIOE ESP32-S3 and the BME280. Make sure the joints are secure and there are no solder bridges between adjacent pins.
3. **Install the BME280 sensor.** Insert the sensor into the space in the BME280 sensor holder (the torch with the flame design). Make sure the sensor is not covered, because it needs to be exposed to gather data. Then mount the holder into the hole in the 3D-printed wall display.
4. **Install the 1.5" LCD display.** Insert the LCD into its slot in the wall display and secure it with the 3D-printed sliding lock. The lock should be tight enough that the display cannot move.
5. **Mount the TRIOE ESP32-S3.** Place the board on the three posts of the 3D-printed base so the posts go through the board's holes. The board's port must face the back, so the wires can run from the base to the wall display.
6. **Mount the wall display.** Attach the wall display (with the sensor holder and LCD) to the base using the grooves behind the wall display.
7. **Connect the components.** Wire the BME280 and LCD to the TRIOE ESP32-S3 using the [pinout tables](#wiring--pinout) below. Check each connection before powering on.
8. **Check the assembly.** See the checklist below.
9. **Power and test.** Connect the TRIOE ESP32-S3 to a power source and turn it on. Verify that the LCD shows information and that the BME280 gives sensor readings.

</details>

**Assembly checklist**

- [ ] Solder pins are straight and aligned at 90 degrees
- [ ] BME280 is positioned and secured in the sensor holder
- [ ] LCD is firmly secured to the wall display
- [ ] TRIOE ESP32-S3 is securely mounted on the three posts
- [ ] Wall display is secured to the base
- [ ] Wires are routed properly and connected to the assigned pins

### 4. Wiring &amp; Pinout

**BME280 sensor**

| BME280 Pin | TRIOE ESP32-S3 Pin | Purpose |
|---|---|---|
| VCC | 3.3V | Power |
| GND | GND | Ground |
| SCL | D1 | Serial Clock |
| SDA | D2 | Serial Data |

**1.5" LCD display (SPI)**

| LCD Pin | TRIOE ESP32-S3 Pin | Purpose |
|---|---|---|
| GND | GND | Ground |
| VCC | 3.3V | Display Power |
| SCL | D4 | SPI Clock |
| SDA | D5 | SPI Data (MOSI) |
| RES | D6 | Reset |
| DC | D7 | Data/Command Select |
| CS | D8 | Chip Select |

### 5. Flash Firmware & Open the Dashboard

**Requirements**

- [Arduino IDE](https://www.arduino.cc/en/software)
- ESP32 board support (Boards Manager → search **esp32** by Espressif Systems)
- A USB data cable
- A 2.4 GHz WiFi network with internet access (the dashboards run on the TRIOE Hub cloud)
- A TRIOE Hub account with this device registered

**Libraries**

Install these from **Sketch → Include Library → Manage Libraries**:

| Library | Used for |
|---|---|
| Adafruit BME280 Library | Reading the BME280 sensor |
| Adafruit Unified Sensor | Required by the BME280 library |
| Adafruit GFX Library | Drawing graphics and text |
| Adafruit SH110X | Driving the 128x128 SH1107 display |
| TRIOE | Sending data to and receiving commands from the TRIOE Hub (includes `Trioe.h` and `TRIOE_ROOT_CA.h`) |

> `WiFi`, `Wire`, and `SPI` come with the ESP32 board package.

**Set up your credentials**

1. In the `firmware/` folder, copy `secrets.example.h` and rename the copy to `secrets.h`.
2. Fill in your own details:
   ```cpp
   const char* WIFI_SSID     = "YOUR_WIFI_NAME";
   const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
   const char* API_KEY       = "YOUR_TRIOE_API_KEY";
   ```
3. In the main sketch, update `TELEMETRY_URL` and `STATE_URL` with **your own device ID** from the TRIOE Hub.

> `secrets.h` is listed in `.gitignore`, so your password and key are never uploaded to GitHub.

**Flash the firmware**

1. Open the sketch in the Arduino IDE.
2. Under **Tools → Board**, select **ESP32S3 Dev Module**, and select the correct **Port**.
3. Click **Upload**. If it can't connect, hold the **BOOT** button while the upload starts.
4. Open **Tools → Serial Monitor** at **115200** baud to see status messages.

The pin assignments in the code match the [wiring tables](#4-wiring--pinout):

| Part | Pins in code |
|---|---|
| BME280 (I2C) | `SDA = 2`, `SCL = 1` (address `0x76`) |
| Display (SPI) | `SCL = 4`, `SDA = 5`, `RES = 6`, `DC = 7`, `CS = 8` |

**What the device does**

- Shows four pages on the display, rotating every 3 seconds: **Temperature, Humidity, Pressure, Forecast**.
- Plots a live graph on each sensor page.
- Predicts the weather from pressure changes and humidity. For the first hour after startup, the forecast improves as the device collects more readings.
- Sends readings to the TRIOE Hub every 10 seconds (or when a value changes by 0.2).
- Checks the Hub for new commands every 2 seconds.

---

## Live Dashboards

Once the device is online, you can view its live readings here:

| Dashboard | Link |
|---|---|
| 💻 **Web Dashboard** | [Open on desktop](https://hub.trioe.dev/shared/bcfd1d9f6ff7fe5e4085ee3fba18743dda829b3331ec693ade3cfee0efe1d3ff) |
| 📱 **Mobile Dashboard** | [Open on mobile](https://hub.trioe.dev/shared/8cefc63fa434dba523678482e63e48b7b5f891fa942d0f653468addb9f76945e) |

The dashboards show `temperature`, `humidity`, `pressure`, `sealevel_pressure`, `forecast_prediction`, `rain_probability`, `rain_status`, and status labels for each sensor.

> To build your own, register your device on the [TRIOE Hub](https://hub.trioe.dev), then create and share a dashboard for it.

**Control the display**

Send these as text commands from the dashboard (or type them in the Serial Monitor):

| Command | Result |
|---|---|
| `auto` | Rotate pages automatically |
| `temperature`, `humidity`, `pressure`, `forecast` | Lock the display on one page |
| `day` / `night` | Switch between light and dark theme |
| `boat`, `ball`, `fish`, `dino`, `dragon` | Choose the animated character riding the graph line |
| `off` | Hide the character |

> The animated character only shows while the device is connected to WiFi.

**Troubleshooting**

| Problem | Fix |
|---|---|
| `Could not find a valid BME280 sensor!` | Check VCC, GND, SCL (D1), and SDA (D2). The code expects I2C address `0x76`. |
| Display stays blank | Recheck the SPI wiring (D4–D8). |
| No data on the dashboard | Confirm the WiFi is 2.4 GHz with internet access, and that your API key and device ID are correct. |
| Forecast says "Analyzing" | Normal at startup. Wait a few minutes for readings to build up. |
| Readings run high | Keep the sensor away from the board's heat and make sure the holder doesn't cover it. |

---

## Customization

The kit is modular, so you can build on it for your own project.

| Area | What you can do |
|---|---|
| **Temp &amp; Humidity Monitoring** | Mount the BME280 in the smaller sensor opening to measure temperature, humidity, and pressure. |
| **LCD Display** | Show temperature, humidity, pressure, forecast prediction, sensor status, and warnings or alerts. The LCD head can be redesigned for a different display size if the dimensions stay compatible with the chassis. |
| **Microcontroller Mounting** | The three mounting points in the base hold the TRIOE ESP32-S3 in place while leaving its connections accessible. |
| **Sensor Expansion** | Add a light, gas, motion, distance, or other environmental sensor by modifying the base and wall design with new openings or mounting points. |
| **Enclosure** | Edit sensor openings, mounting-hole positions, LCD head dimensions, cable-routing openings, overall shape, and extra mounting points in CAD. |
| **Cable Management** | Add cable-routing holes or channels to the wall design to keep wires clear of the sensors, microcontroller, and LCD. |
| **Future Expansion** | Add communication modules, sensors, indicators, or control components without redesigning the main body. |

---

## Contributing

Pull requests are welcome for:

- New display layouts and screens
- Dashboard improvements
- Additional sensor support
- Enclosure design variations

---

*Created by Brenzzell.*
