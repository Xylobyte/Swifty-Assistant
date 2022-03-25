/* Swift Assistant is a simple, user-friendly assistant based on an extension system.

   Copyright (C) <2022>  <SwiftApp>

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

#ifndef SWIFTWORKER_H
#define SWIFTWORKER_H

#ifndef QT_NO_SYSTEMTRAYICON

#include <QObject>
#include <QThread>
#include <QDialog>
#include <QString>
#include <QSystemTrayIcon>

#include "plugininterface.h"

class SwiftyWorker : public QObject
{
    Q_OBJECT

public:
    SwiftyWorker(QObject *parent = nullptr);
    ~SwiftyWorker();

    static void declareQML();

    Q_INVOKABLE void messageSended(QString message);
    Q_INVOKABLE void newText(QString text);
    Q_INVOKABLE void getPluginList();
    Q_INVOKABLE void sendMessageToPlugin(QString message);
    Q_INVOKABLE void removePlugin(QString id);
    Q_INVOKABLE void execAction(QString action);
    Q_INVOKABLE QString getOs();
    Q_INVOKABLE void setWindowVisibility(bool visible);

public slots:
    void reponseReceived(
            QString reponse, bool isFin, QString typeMessage, QList<QString> url = QList<QString>(),
            QList<QString> textUrl = QList<QString>()
            );
    void open();
    void hide();
    void addProp(QString prop);
    void removeAllProp();
    void removeProp(int index);
    void showQmlFile(QString qmlUrl);
    void pluginTrouved(QString name);
    void messageToQml(QString message, QString pluginId);
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void showHomeScreen();
    void previousPage();
    void sendNotify(QString title, QString text, QString action);
    void notifyClicked();
    void openPluginsFolder();

signals:
    void reponse(QString text, bool isFin, QString typeMessage, QList<QString> url, QList<QString> textUrl);
    void message(QString message);
    void textChanged(QString text);
    void showWindow(int x, int y);
    void hideWindow();
    void addProposition(QString prop);
    void removeAllProposition();
    void removeProposition(int index);
    void addBaseProp();
    void showQml(QString qmlUrl);
    void getAllPlugin();
    void pluginName(QString name);
    void signalSendMessageToPlugin(QString message);
    void pluginSendedMessageToQml(QString message, QString pluginId);
    void signalRemovePlugin(QString id);
    void executeAction(QString action);
    void homeScreen();
    void showPreviousPage();

private:
    void setIcon(QString path);
    void createActions();
    void createTrayIcon();

    QThread engineThread;

    QAction *restoreAction;
    QAction *openPluginFolder;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    bool isWindowShow = false;
    QString actionNotify;
};

#endif

#endif
