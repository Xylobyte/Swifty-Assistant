/* Swift Assistant is a simple, user-friendly assistant based on an extension system.

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

#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QDomDocument>
#include <QSettings>
#include <QString>

#include "plugininterface.h"

#define key_settings_name "settings_name"
#define key_settings_sound "settings_sound"
#define key_settings_proposition "settings_proposition"

class Engine : public QObject
{
    Q_OBJECT
public:
    explicit Engine(QObject *parent = nullptr);

private:
    void format(QString text);
    void execAction(QList<QString> cmd, QList<QString> var);
    void analize(QList<QList<QString>> array_cmd);
    void updateSettingsVar();
    QString readVarInText(QString text, QList<QString> var);
    void analizeAllPlugins(QList<QList<QString>> array_cmd, QList<QString> cmd);
    bool analizePlugin(QList<QList<QString>> array_cmd, QList<QString> cmd);

    QDomDocument doc;
    QSettings settings;
    QString userName = "Inconnu";
    bool soundEnabled = true;
    bool propEnabled = true;
    QList<PluginInterface *> listPlugins;
    QList<QString> prop;
    QList<QString> main_prop;
    QList<QString> mainVolatil_prop;
    QList<QString> showedProp;
    int removePropNuber = 0;

    QString nextReplyPluginName = "";
    QString nextReplyNeedId = "";
    QString nextReplyItemId = "";

signals:
    void reponseSended(QString reponse, bool isFin, QString typeMessage, QList<QString> url, QList<QString> textUrl);
    void addProp(QString prop);
    void removeAllProp();
    void removeProp(int index);
    void showQmlFile(QString qmlUrl);
    void pluginTrouved(QString name);
    void signalSendMessageToPlugin(QString message, QString pluginIid);
    void pluginToQml(QString message, QString pluginIid);
    void hideWindow();

public slots:
    void messageReceived(QString message);
    void textChanged(QString text);
    void addBaseProp();
    void showQml(QString qml, QString iid);
    void getAllPlugin();
    void sendReply(QString reply, bool isFin, QString typeMessage, QList<QString> url, QList<QString> textUrl);
    void sendMessageToPlugin(QString message, QString pluginIid);
    void receiveMessageSendedToQml(QString message, QString pluginIid);
    void removePlugin(QString iid);
    void scanPlugin();
};

#endif // ENGINE_H
