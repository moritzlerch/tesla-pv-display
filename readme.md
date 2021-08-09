<h2 align="center">"PV-Display" Project - Monitor the power flows of your Tesla Powerwall</h2>


<p align="center">
    <img src="./assets/logo/pv_display.svg" width=60%><br><br>
    <a href="https://github.com/MoritzLerch/pv_display/releases"><img src="https://img.shields.io/github/v/release/MoritzLerch/pv_display.svg?color=blue&style=flat-square"></a>
    <a href="https://github.com/MoritzLerch/pv_display/releases"><img src="https://img.shields.io/github/workflow/status/MoritzLerch/pv_display/PlatformIO%20CI?color=blue&style=flat-square"></a>
    <a href="https://raw.githubusercontent.com/MoritzLerch/pv_display/master/LICENSE"><img src="https://img.shields.io/github/license/MoritzLerch/pv_display?color=blue&style=flat-square"></a>
    <a href="https://github.com/MoritzLerch/pv_display/commits/master"><img src="https://img.shields.io/github/commit-activity/m/MoritzLerch/pv_display?color=blue&style=flat-square"></a>
    <a href="https://github.com/MoritzLerch/pv_display"><img src="https://img.shields.io/github/repo-size/MoritzLerch/pv_display?color=blue&style=flat-square"></a>
</p>

## 💡 Short description
This project uses an ESP8266 with attached LCD-Display to connect to the local API of the Tesla Powerwall (V2). It is displaying all the current power flows (grid, home, solar, battery) and the SOC (state of charge).

## 🎈 Showcase
**<p align="center">A picture says more than a thousand words!</p>**
<div align="center"><img src="./assets/pictures/showcase01.jpg" width=75% ></div>
<p align="center">(This is the main screen.)</p>


## ⚙ Features
- [Main screen](#-showcase) includes:
    - SOC (state of charge)
    - Four different power flows (grid, home, solar, battery)
- Power flows are formatted in an organized manner. No overlapping will happen, units of values are converted to properly fit and look nice. :-)
- Indicator (dot in upper right corner) for the current API request status

For more information on all the different screens have a look at [this wiki-page](https://github.com/MoritzLerch/pv_display/wiki/Different-screens).

## 👾 Installation and usage

- You can find a **quick start guide and documentation** in the [wiki](https://github.com/MoritzLerch/pv_display/wiki). Have fun there!
- If you want to know, how the display shows the current data from the Powerwall check out this [Wiki-Page](https://github.com/MoritzLerch/pv_display/wiki/Different-screens). Enjoy exploring!

## 🤝 License and credits

### License

Project code is licensed under the GPL-3.0-License, see `LICENSE` for more information.

### Logo, graphics and image license
The logos, graphics and images of PV-Display (everything in `/assets`) are subject to copyright (c) Moritz Lerch, all rights reserved, and may not be used without permission. If you would like to use the logos, graphics or images, please contact the author in order to get a permission.

### Credits
The API requests are heavily based on the [powerwall2](https://github.com/vloschiavo/powerwall2) API documentation (Apache-2.0 License).

---
**Disclaimer**: *In accordance with the GNU General Public License v3.0, I assume no liability for any damage to you or any other person or equipment. There is no warranty on this open source project.*
