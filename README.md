# vol2com
Simple open source LED strip management software.
The main advantage of the program is the ability to control the color depended on the music being played. Also, the advantages of the program include support for various types of connections, a large number of operating modes (with the settings for each of them), the presence of an equalizer for fine-tuning the values and a lot of other useful functions.
![ConnectionPage](assets/Connection.png)
![OperatingModesPage](assets/ModeSelector.png)
![EqualizerPage](assets/Equalizer.png)

- [Features](#features)
- [Operating modes](#modes)
- [How to build](#build)
- [Planned features](#future)

<a name="features"></a>
## Features
* Serial connection (Bluetooth via COM)
* Various work modes
* "Equalizer" for fine-tuning
* Multi-language (at the moment English and Russian)
* Notifications support (Windows 10+)

<a name="modes"></a>
## Operating modes
1. General - Analyses sound and send color corresponding color.
![GeneralMode](assets/General.png)
2. Fade - Blinks selected color to the beat of the music.
![FadeMode](assets/Fade.png)
4. Random - Smoothly changes current color in random direction.
5. Manual - Sends selected color. HSV color selector available and a few predefined colors.
![ManualMode](assets/Manual.png)
You can suggest new operating modes via [issues](https://github.com/Mixxxxa/vol2com/issues).

<a name="build"></a>
## How to build and run
Tested with Qt 5.15.2
1. Clone the repository
2. Download and place dependencies in `/src/libs`
3. Open vol2com.pro file and build
4. Place `bass.dll` and `bass_wasapi.dll` (don't forget about bitness 32/64).
5. Run builded exe

<a name="future"></a>
## Planned features
* Start at system startup
* Ability to minimize to system tray and corresponding menu for the most frequency used functions.
* Remote Control module - program will can perform various actions by request from the LED controller.
* Support connection via WebSocket's