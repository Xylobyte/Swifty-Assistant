/* Swifty Assistant is a simple, user-friendly assistant based on an extension system.

   Copyright (C) <2021>  <SwiftApp>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QObject>
#include <QString>

class PluginInterface
{
public:
    virtual ~PluginInterface() = default;
    virtual QString getDataXml() = 0;
    virtual QString pluginId() = 0;
    virtual void execAction(QList<QString> cmd) = 0;
    virtual QList<QString> getCommande() = 0;
    virtual QObject* getObject() = 0;

signals:
    void sendMessage(QString reply, bool isFin, QString typeMessage, QString id, QList<QString> url = QList<QString>(), QList<QString> textUrl = QList<QString>());
    void sendMessageToQml(QString message);
    void showQml(QString qml, QString id);
    void execAction(QString action);

public slots:
    virtual void messageReceived(QString message, QString pluginId) = 0;
};


QT_BEGIN_NAMESPACE

#define PluginInterface_iid "fr.swiftapp.swiftyassistant.plugin"

Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)

QT_END_NAMESPACE

#endif
