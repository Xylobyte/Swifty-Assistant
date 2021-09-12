#   Swifty Assistant is a simple, user-friendly assistant based on an extension system.
#	
#   Copyright (C) <2021>  <SwiftApp>
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <https://www.gnu.org/licenses/>.

requires(qtConfig(accessibility))

QT += quick quickcontrols2 xml qml webengine core gui

TARGET     = swifty
QMAKE_PROJECT_NAME = swiftyassistant

qtHaveModule(widgets) {
    QT += widgets # QApplication is required to get native styling with QtQuickControls
}

# install
INSTALLS += target

CONFIG += install_ok  # Do not cargo-cult this!

TRANSLATIONS += ts/swiftyassistant_en.ts

RESOURCES += \
    src/res/res.qrc \
    trad.qrc

HEADERS += \
    src/engine.h \
    src/plugininterface.h \
    src/swiftyworker.h

SOURCES += \
    src/engine.cpp \
    src/main.cpp \
    src/swiftyworker.cpp
