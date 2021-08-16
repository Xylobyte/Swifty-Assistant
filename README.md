# Swift Assistant

[![License](https://img.shields.io/badge/license-GPLv3.0-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.html) [![GitHub issues](https://img.shields.io/github/issues/Swiftapp-hub/Swift-Assistant.svg)](https://github.com/Swiftapp-hub/Swift-Assistant/issues) [![Maintained](https://img.shields.io/maintenance/yes/2021.svg)](https://github.com/Swiftapp-hub/Swift-Assistant/commits/develop)

Swift Assistant is a simple, user-friendly assistant based on an extension system.

Swift is a personal assistant created to be useful to both beginners and computer experts!
It works offline and does not collect any personal data!
If you like Swift and are useful to you, please feel free to contribute to the project, thank you.
I'm French so the English version of Swift isn't fully up to date, but if you want to improve don't hesitate!

Screenshoot:

<img src="screenshot/swift.png" alt="Capture Swift Assistant" style="zoom:70%;" /> 

## Download

Swift Assistant is available in appimage format!

Follow the instructions below to download and install Swift Assistant:

1. [Download](https://github.com/Swiftapp-hub/Swift-Assistant/releases/) Swift Assistant
2. Right click on the download file and go to the properties
3. Go to the permissions tab and check the box "Allow file execution as a program"
4. Now double click on the appimage file to launch Swift Assistant

## Manual installation

Follow these steps if you want to compile Swift Assistant on your own

### Dependencies

To compile Swift Assistant you need to have installed:

Qt >= 5.15.2 with at least the following modules:

* [qtbase](http://code.qt.io/cgit/qt/qtbase.git)
* [qtwebengine](https://code.qt.io/cgit/qt/qtwebengine.git)
* [qtquickcontrols2](https://code.qt.io/cgit/qt/qtquickcontrols2.git)

and you must have the build-essential package installed.

I recommend you to install Qt with the official installer from the Qt site

### Build

You need to run the following commands to compile Swift Assistant:

```bash
cd "FOLDER_OF_GIT_CLONE"/Swift\ Assistant
```

```bash
lrelease SwiftAssistant.pro
```

```bash
mkdir build && cd build
```

```bash
qmake ../
```

```bash
make
```

## Plugins

List of available plugins for Swift Assistant:

* HelloWord
* [ControlSettings](https://github.com/Swiftapp-hub/ControlSettings-Plugin-Swift-Assistant)

If you have created a plugin, consider adding it to this list!

## Contribution

Here's what you can do to contribute to the project:

* If you want to improve this project, clone this repository
* If you want to create new plugins for Swift or improve an existing plugin read the documentation

Thank you in advance for your help.

## License

This project is licensed under the GNU General Public License version 3

You will find a copy of this license in the file [LICENSE.md](https://github.com/Swiftapp-hub/Swift-Assistant/blob/master/LICENSE.md)
