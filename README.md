# Air Quality Sensor

An ESP32-based device that measures CO2, particulate matter and
temperature/humidity, and reports the readings over Wi-Fi to a self-hosted
dashboard.

**Status:** archived. In daily use by the author, not developed further.

## What it measures

- CO2 (ppm)
- Particulate matter PM1.0, PM2.5, PM10 (µg/m³)
- Temperature (°C) and relative humidity (%)

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

| Signal | ESP32 pin |
| --- | --- |
| DHT22 data | GPIO15 |
| PMS5003 UART (`Serial1`) | RX on SD2, TX on SD3 |
| MH-Z19E UART (`Serial2`) | RX on GPIO16, TX on GPIO17 |
| Wi-Fi status LED | `LED_BUILTIN` |

Source: `apps/device/src/_config.h`. Power wiring is not documented in the
repository.

## How it works

The device wakes up, reads the three sensors, connects to Wi-Fi, and POSTs a
JSON payload to a server, then goes into deep sleep for 10 minutes before
repeating (`apps/device/src/main.cpp`,
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
   `API_URL` to your own values.
3. From `apps/device`, build and upload: `pio run -e nodemcu-32s -t upload`.
4. Optional: set `SHOULD_LOG_TO_SERIAL` to `true` and run
   `pio device monitor` at 115200 baud to see logs.

## Licence

MIT, see [LICENSE](LICENSE).
