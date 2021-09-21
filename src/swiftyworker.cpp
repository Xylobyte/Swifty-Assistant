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

#include "swiftyworker.h"
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
    connect(this, &SwiftWorker::executeAction, engine, &Engine::executeAction);
    connect(engine, &Engine::reponseSended, this, &SwiftWorker::reponseReceived);
    connect(engine, &Engine::addProp, this, &SwiftWorker::addProp);
    connect(engine, &Engine::removeProp, this, &SwiftWorker::removeProp);
    connect(engine, &Engine::removeAllProp, this, &SwiftWorker::removeAllProp);
    connect(engine, &Engine::showQmlFile, this, &SwiftWorker::showQmlFile);
    connect(engine, &Engine::pluginTrouved, this, &SwiftWorker::pluginTrouved);
    connect(engine, &Engine::pluginToQml, this, &SwiftWorker::messageToQml);
    connect(engine, &Engine::hideWindow, this, &SwiftWorker::hide);

    engineThread.start();

    // Displays the main proposition on the home screen main proposition on the home screen of Swifty Assistant
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

void SwiftWorker::declareQML()
{
    qmlRegisterType<SwiftWorker>("SwiftyWorker", 1, 0, "Swifty");
}

//===================================================
//============== Q_INVOKABLE function ===============
//===================================================


/**
 * Send user input to the understanding engine
 *
 * @param _message the user input
 */
void SwiftWorker::messageSended(QString _message)
{
    emit message(_message);
}

void SwiftWorker::openLinkInBrowser(QString url)
{
    QDesktopServices::openUrl(QUrl(url));
}

/**
 * When user input changed send new text to engine for completion
 *
 * @param text the new user input
 */
void SwiftWorker::newText(QString text)
{
    if (text != "") emit textChanged(text);
    else emit addBaseProp();
}

/**
 * Sending a message to the engine so that it resends a list of plugins
 */
void SwiftWorker::getPluginList()
{
    emit getAllPlugin();
}

/**
 * When a plugin interface is displayed it can use this function to communicate with the plugin
 *
 * @param message the messsage
 * @param pluginIid the iid of plugin
 */
void SwiftWorker::sendMessageToPlugin(QString message, QString pluginIid)
{
    emit signalSendMessageToPlugin(message, pluginIid);
}

/**
 * Delete the file of a plugin installed in the ~/SwiftyPlugins folder
 *
 * @param iid the plugin identifier has been deleted
 */
void SwiftWorker::removePlugin(QString iid)
{
    emit signalRemovePlugin(iid);
}

/**
 * Update the list of plugins in the ~/SwiftyPlugins folder
 */
void SwiftWorker::actuPlugins()
{
    emit signalActuPlugins();
}

/**
 * Used to execute an action command in the engine
 *
 * @param action the command to execute
 */
void SwiftWorker::execAction(QString action)
{
    emit executeAction(action);
}

//===================================================
//================ Private function =================
//===================================================

/**
 * Set icon and toolTip of QTrayIcon
 *
 * @param path the icon to set
 */
void SwiftWorker::setIcon(QString path)
{
    QIcon icon(path);
    trayIcon->setIcon(icon);

    trayIcon->setToolTip("Swifty Assistant is running !");
}

/**
 * Add menu with action to QTrayIcon
 */
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

//===================================================
//====================== Slots ======================
//===================================================

/**
 * When engine send a reponse, display this on the home screen
 *
 * @param _reponse the reponse
 * @param isFin is the last message for the question
 * @param typeMessage if the reponse is a message or an action
 * @param url if the reponse is an web action specify the url of web page
 * @param textUrl if the reponse is an message with action button specify text for button
 */
void SwiftWorker::reponseReceived(
        QString _reponse, bool isFin, QString typeMessage, QList<QString> url, QList<QString> textUrl
        )
{
    emit reponse(_reponse, isFin, typeMessage, url, textUrl);
}

/**
 * Show the Swifty Assistant window
 */
void SwiftWorker::open()
{
    QPoint pos = QCursor::pos();
    emit showWindow(pos.x(), pos.y());
}

/**
 * Hide the Swifty Assistant window
 */
void SwiftWorker::hide()
{
    emit hideWindow();
}

/**
 * Add proposition when engine call this slot
 *
 * @param prop the proposition
 */
void SwiftWorker::addProp(QString prop)
{
    emit addProposition(prop);
}

/**
 * Remove all proposition of the home screen
 */
void SwiftWorker::removeAllProp()
{
    emit removeAllProposition();
}

/**
 * Remove one proposition with a index
 *
 * @param index index of removed proposition
 */
void SwiftWorker::removeProp(int index)
{
    emit removeProposition(index);
}

/**
 * Display a qml file on the Swifty Assistant window
 *
 * @param qmlUrl url of qml file
 */
void SwiftWorker::showQmlFile(QString qmlUrl)
{
    emit showQml(qmlUrl);
}

/**
 * When engine find a plugin
 *
 * @param name the plugin iid
 */
void SwiftWorker::pluginTrouved(QString name)
{
    emit pluginName(name);
}

/**
 * If the plugin whant to send a message to qml file actually showed
 * @param message
 * @param pluginIid
 */
void SwiftWorker::messageToQml(QString message, QString pluginIid)
{
    emit pluginSendedMessageToQml(message, pluginIid);
}

void SwiftWorker::trayIconActivated(QSystemTrayIcon::ActivationReason)
{
    open();
}
