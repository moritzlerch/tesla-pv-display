# "PV-Display" Project - Watch power flows on your Tesla Powerwall


<p align="center">
    <img src="/assets/pv_display.svg" width=50%><br><br>
    <a href="https://github.com/MoritzLerch/pv_display/releases"><img src="https://img.shields.io/github/v/release/MoritzLerch/pv_display.svg?color=blue&style=flat-square"></a>
    <a href="https://github.com/MoritzLerch/pv_display/releases"><img src="https://img.shields.io/github/workflow/status/MoritzLerch/pv_display/PlatformIO%20CI?color=blue&style=flat-square"></a>
    <a href="https://raw.githubusercontent.com/MoritzLerch/pv_display/master/LICENSE"><img src="https://img.shields.io/github/license/MoritzLerch/pv_display?color=blue&style=flat-square"></a>
    <a href="https://github.com/MoritzLerch/pv_display/commits/master"><img src="https://img.shields.io/github/commit-activity/m/MoritzLerch/pv_display?color=blue&style=flat-square"></a>
    <a href="https://github.com/MoritzLerch/pv_display"><img src="https://img.shields.io/github/repo-size/MoritzLerch/pv_display?color=blue&style=flat-square"></a>
</p>

## Short description
This project uses an ESP8266 with attached LCD-Display to connect to the local API of the Tesla Powerwall (V2). It is displaying all the current power flows (grid, home, solar, battery) and the SOC (state of charge), so it's perfect for creating an information screen.
(If you wonder about the name: PV --> Photovoltaic; Display (german anglicism 🙂) --> Screen)

<!-- ## Installation and Usage -->

## License and credits

Project code is licensed under GPL-3.0 License, see `LICENSE` for more information.

The API requests are heavily based on the [powerwall2](https://github.com/vloschiavo/powerwall2) API documentation (Apache-2.0 License).
