# vol2com
Simple open source LED strip management software.
The main advantage of the program is the ability to control the color depended on the music being played. Also, the advantages of the program include support for various types of connections, a large number of operating modes (with the settings for each of them), the presence of an equalizer for fine-tuning the values and a lot of other useful functions.

<p align="middle">
  <img src=".github/Connection.png" alt="ConnectionPage" width="33%"/>
  <img src=".github/ModeSelector.png" alt="OperatingModesPage" width="33%"/>
  <img src=".github/Equalizer.png" alt="EqualizerPage" width="33%"/>
</p>

- [Features](#features)
- [Operating modes](#modes)
- [How to build](#build)
- [SAST Tools](#sast)
- [Dependencies](#deps)

<a name="features"></a>
## Features
* Serial connection
* Various work modes
* "Equalizer" for fine-tuning
* Multi-language (at the moment English and Russian)

<a name="modes"></a>
## Operating modes
| Name | Description | Screenshot |
| ------------- |:-------------:| -----:|
| General | Analyses sound and send color corresponding color | <img src=".github/General.png" width="300"> |
| Fade | Blinks selected color to the beat of the music | <img src=".github/Fade.png" width="300"> |
| Random | Smoothly changes current color in random direction | Nothing to see here :) |
| Manual | Sends selected color. HSV color selector available and a few predefined colors | <img src=".github/Manual.png" width="300"> |

You can suggest new operating modes via [issues](https://github.com/Mixxxxa/vol2com/issues).

<a name="build"></a>
## How to build and run
Tested with Qt 6.6.1
1. Clone the repository
2. Configure CMake project (it will automatically download and configure all dependencies)
5. Build and run

<a name="sast"></a>
## SAST Tools
[PVS-Studio](https://pvs-studio.com/pvs-studio/?utm_source=website&utm_medium=github&utm_campaign=open_source) - static analyzer for C, C++, C#, and Java code.

<a name="deps"></a>
## Dependencies
Thank you to the developers of these wonderful libraries:
* [Qt](https://qt.io/)
* [Bass](https://www.un4seen.com/)
* [WinToast](https://github.com/mohabouje/WinToast/)


