# Air Quality Sensor

An ESP32-based device that measures CO2, particulate matter and
temperature/humidity, and reports the readings over Wi-Fi to a self-hosted
dashboard.

**Status:** archived. In daily use by the author, not developed further.

## What it measures

- CO2 (ppm)
- Particulate matter PM1.0, PM2.5, PM10 (µg/m³)
- Temperature (°C) and relative humidity (%)

The device measures and sends all three particulate readings, but the
server only stores and charts PM2.5 (see How it works).

Source: `apps/device/src/devices/co2/co2.cpp`,
`apps/device/src/devices/pm/pm.cpp`,
`apps/device/src/devices/tempHumid/tempHumid.cpp`.

## Parts

- ESP32 dev board (PlatformIO board `nodemcu-32s`)
- A particulate-matter sensor read over UART, wired as a `PMS5003` (per the
  `PMS5003_SERIAL` macro), using the `PMS Library`
- A CO2 sensor read over UART, wired as an `MH-Z19E` (per the
  `MHZ19E_SERIAL` macro), using the `MH-Z19` library
- A DHT22 temperature/humidity sensor, using the Adafruit `DHT sensor
  library`

Source: `apps/device/platformio.ini`, `apps/device/src/_config.h`.

## Wiring

The pin column is the ESP32 side: a sensor's TX line goes to the ESP32's RX.

| Signal | ESP32 pin |
| --- | --- |
| DHT22 data | GPIO15 |
| PM sensor (`PMS5003_SERIAL`) UART | `Serial1` default pins (firmware does not set pins) |
| CO2 sensor (`MHZ19E_SERIAL`) UART | `Serial2` default pins (firmware does not set pins) |

Both sensors' `begin()` calls pass only a baud rate
(`apps/device/src/devices/pm/pm.cpp:13`,
`apps/device/src/devices/co2/co2.cpp:9`), so both ports use the Arduino
core's default pins, not the RX/TX pins named in a `_config.h` comment.
Caution: the Arduino core's default pins for `Serial1` are the module's
flash-interface pads on ESP32-WROOM boards; the repository does not
document how the author's board is actually wired for that port.

The firmware sets `pinMode` on the board's built-in LED
(`PIN_LED_WIFI_STATUS`) but the code that blinks it is commented out, so it
needs no wiring.

Source: `apps/device/src/_config.h`. Power wiring is not documented in the
repository.

## How it works

On boot, the device waits through a warm-up delay
(`SENSORS_AFTER_SLEEP_WARM_UP_DELAY_MS`, `apps/device/src/_config.h:35`),
then reads the three sensors, connects to Wi-Fi, and POSTs a JSON payload to
a server. It then deep-sleeps for `REPORT_INTERVAL_MS`
(`apps/device/src/_config.h:36`) before repeating, so the warm-up adds to
that interval and the real cadence is a little longer than the sleep
duration alone (`apps/device/src/main.cpp:20-50`,
`apps/device/src/devices/report/report.cpp`).

The server is a SvelteKit app (`apps/web`) whose `/api/aqs` route validates
the payload and stores it in Postgres (`packages/database`); the app's home
page fetches the stored measurements from the same route and renders them
as charts. An earlier Firebase-based backend and dashboard (`apps/backend`,
`apps/dashboard`, `firebase.json`) were replaced by `apps/web` and are no
longer used by the device.

Repository layout:

- `apps/device` — the firmware described above (PlatformIO, Arduino
  framework)
- `apps/web` — the SvelteKit app: API route, Postgres access and dashboard
- `packages/database` — Postgres client and schema migrations
- `packages/shared-types` — the payload schema shared by the API route and
  database layer

## Flashing

1. Install [PlatformIO](https://platformio.org/) (CLI or the VS Code
   extension).
2. In `apps/device/src/_config.h`, set `WIFI_SSID`, `WIFI_PASSWORD` and
   `API_URL` to your own values. This file is tracked by git and not
   covered by `.gitignore`, so a plain edit risks committing your Wi-Fi
   password; first run
   `git update-index --skip-worktree apps/device/src/_config.h`
   so git stops tracking local changes to it.
3. From `apps/device`, build and upload: `pio run -e nodemcu-32s -t upload`.
4. Optional: set `SHOULD_LOG_TO_SERIAL` to `true` and run
   `pio device monitor` at 115200 baud to see logs.

## Licence

MIT, see [LICENSE](LICENSE).
