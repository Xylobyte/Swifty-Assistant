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

#include "swiftworker.h"
#include "engine.h"

#include <QtQml>
#include <QMenu>
#include <QAction>
#include <QtWidgets>
#include <QSettings>
#include <QMessageBox>
#include <QDesktopServices>

SwiftWorker::SwiftWorker(QObject *parent) : QObject(parent)
{
    Engine *engine = new Engine;
    engine->moveToThread(&engineThread);
    connect(&engineThread, &QThread::finished, engine, &QObject::deleteLater);
    connect(this, &SwiftWorker::message, engine, &Engine::messageReceived);
    connect(this, &SwiftWorker::textChanged, engine, &Engine::textChanged);
    connect(this, &SwiftWorker::addBaseProp, engine, &Engine::addBaseProp);
    connect(this, &SwiftWorker::getAllPlugin, engine, &Engine::getAllPlugin);
    connect(this, &SwiftWorker::signalSendMessageToPlugin, engine, &Engine::sendMessageToPlugin);
    connect(this, &SwiftWorker::signalRemovePlugin, engine, &Engine::removePlugin);
    connect(this, &SwiftWorker::signalActuPlugins, engine, &Engine::scanPlugin);
    connect(engine, &Engine::reponseSended, this, &SwiftWorker::reponseReceived);
    connect(engine, &Engine::addProp, this, &SwiftWorker::addProp);
    connect(engine, &Engine::removeProp, this, &SwiftWorker::removeProp);
    connect(engine, &Engine::removeAllProp, this, &SwiftWorker::removeAllProp);
    connect(engine, &Engine::showQmlFile, this, &SwiftWorker::showQmlFile);
    connect(engine, &Engine::pluginTrouved, this, &SwiftWorker::pluginTrouved);
    connect(engine, &Engine::pluginToQml, this, &SwiftWorker::messageToQml);
    connect(engine, &Engine::hideWindow, this, &SwiftWorker::hide);
    engineThread.start();

    emit addBaseProp();

    createActions();
    createTrayIcon();
    setIcon(":/Icon/assistantIcon.png");
    trayIcon->show();

    connect(trayIcon, &QSystemTrayIcon::activated, this, &SwiftWorker::trayIconActivated);
}

SwiftWorker::~SwiftWorker()
{
    engineThread.quit();
    engineThread.wait();
}

void SwiftWorker::actuPlugins()
{
    emit signalActuPlugins();
}

void SwiftWorker::removePlugin(QString iid)
{
    emit signalRemovePlugin(iid);
}

void SwiftWorker::declareQML()
{
    qmlRegisterType<SwiftWorker>("SwiftWorker", 1, 0, "Swift");
}

void SwiftWorker::reponseReceived(QString _reponse, bool isFin, QString typeMessage, QList<QString> url, QList<QString> textUrl)
{
    emit reponse(_reponse, isFin, typeMessage, url, textUrl);
}

void SwiftWorker::trayIconActivated(QSystemTrayIcon::ActivationReason)
{
    open();
}

void SwiftWorker::messageToQml(QString message, QString pluginIid)
{
    emit pluginSendedMessageToQml(message, pluginIid);
}

void SwiftWorker::pluginTrouved(QString name)
{
    emit pluginName(name);
}

void SwiftWorker::getPluginList()
{
    emit getAllPlugin();
}

void SwiftWorker::sendMessageToPlugin(QString message, QString pluginIid)
{
    emit signalSendMessageToPlugin(message, pluginIid);
}

void SwiftWorker::showQmlFile(QString qmlUrl)
{
    emit showQml(qmlUrl);
}

void SwiftWorker::newText(QString text)
{
    if (text != "") emit textChanged(text);
    else emit addBaseProp();
}

void SwiftWorker::open()
{
    QPoint pos = QCursor::pos();
    emit showWindow(pos.x(), pos.y());
}

void SwiftWorker::hide()
{
    emit hideWindow();
}

void SwiftWorker::openLinkInBrowser(QString url)
{
    QDesktopServices::openUrl(QUrl(url));
}

void SwiftWorker::messageSended(QString _message)
{
    emit message(_message);
}

void SwiftWorker::addProp(QString prop)
{
    emit addProposition(prop);
}

void SwiftWorker::removeAllProp()
{
    emit removeAllProposition();
}

void SwiftWorker::removeProp(int index)
{
    emit removeProposition(index);
}

void SwiftWorker::setIcon(QString path)
{
    QIcon icon(path);
    trayIcon->setIcon(icon);

    trayIcon->setToolTip("Swift Assistant is running !");
}

void SwiftWorker::createActions()
{
    restoreAction = new QAction(tr("&Ouvrir"), this);
    connect(restoreAction, &QAction::triggered, this, &SwiftWorker::open);

    quitAction = new QAction(tr("&Quitter"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void SwiftWorker::createTrayIcon()
{
    trayIconMenu = new QMenu();
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}
