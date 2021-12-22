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

SwiftyWorker::SwiftyWorker(QObject *parent) : QObject(parent)
{
    Engine *engine = new Engine;
    engine->moveToThread(&engineThread);
    connect(&engineThread, &QThread::finished, engine, &QObject::deleteLater);

    connect(this, &SwiftyWorker::message, engine, &Engine::messageReceived);
    connect(this, &SwiftyWorker::textChanged, engine, &Engine::textChanged);
    connect(this, &SwiftyWorker::addBaseProp, engine, &Engine::addBaseProp);
    connect(this, &SwiftyWorker::getAllPlugin, engine, &Engine::getAllPlugin);
    connect(this, &SwiftyWorker::signalSendMessageToPlugin, engine, &Engine::sendMessageToPlugin);
    connect(this, &SwiftyWorker::signalRemovePlugin, engine, &Engine::removePlugin);
    connect(this, &SwiftyWorker::signalActuPlugins, engine, &Engine::scanPlugin);
    connect(this, &SwiftyWorker::executeAction, engine, &Engine::executeAction);
    connect(engine, &Engine::reponseSended, this, &SwiftyWorker::reponseReceived);
    connect(engine, &Engine::addProp, this, &SwiftyWorker::addProp);
    connect(engine, &Engine::removeProp, this, &SwiftyWorker::removeProp);
    connect(engine, &Engine::removeAllProp, this, &SwiftyWorker::removeAllProp);
    connect(engine, &Engine::showQmlFile, this, &SwiftyWorker::showQmlFile);
    connect(engine, &Engine::pluginTrouved, this, &SwiftyWorker::pluginTrouved);
    connect(engine, &Engine::pluginToQml, this, &SwiftyWorker::messageToQml);
    connect(engine, &Engine::hideWindow, this, &SwiftyWorker::hide);
    connect(engine, &Engine::showWindow, this, &SwiftyWorker::open);
    connect(engine, &Engine::showHomeScreen, this, &SwiftyWorker::showHomeScreen);
    connect(engine, &Engine::previousPage, this, &SwiftyWorker::previousPage);
    connect(engine, &Engine::sendNotify, this, &SwiftyWorker::sendNotify);

    engineThread.start();

    // Displays the main proposition on the home screen main proposition on the home screen of Swifty Assistant
    emit addBaseProp();

    createActions();
    createTrayIcon();
    setIcon(":/Icon/assistantIcon.png");
    trayIcon->show();

    connect(trayIcon, &QSystemTrayIcon::activated, this, &SwiftyWorker::trayIconActivated);
    connect(trayIcon, &QSystemTrayIcon::messageClicked, this, &SwiftyWorker::notifyClicked);
}

SwiftyWorker::~SwiftyWorker()
{
    engineThread.quit();
    engineThread.wait();
}

void SwiftyWorker::declareQML()
{
    qmlRegisterType<SwiftyWorker>("SwiftyWorker", 1, 0, "Swifty");
}

//===================================================
//============== Q_INVOKABLE function ===============
//===================================================


/**
 * Send user input to the understanding engine
 *
 * @param _message the user input
 */
void SwiftyWorker::messageSended(QString _message)
{
    emit message(_message);
}

/**
 * When user input changed send new text to engine for completion
 *
 * @param text the new user input
 */
void SwiftyWorker::newText(QString text)
{
    if (text != "") emit textChanged(text);
    else emit addBaseProp();
}

/**
 * Sending a message to the engine so that it resends a list of plugins
 */
void SwiftyWorker::getPluginList()
{
    emit getAllPlugin();
}

/**
 * When a plugin interface is displayed it can use this function to communicate with the plugin
 *
 * @param message the messsage
 */
void SwiftyWorker::sendMessageToPlugin(QString message)
{
    emit signalSendMessageToPlugin(message);
}

/**
 * Delete the file of a plugin installed in the ~/SwiftyPlugins folder
 *
 * @param id the plugin identifier has been deleted
 */
void SwiftyWorker::removePlugin(QString id)
{
    emit signalRemovePlugin(id);
}

/**
 * Update the list of plugins in the ~/SwiftyPlugins folder
 */
void SwiftyWorker::actuPlugins()
{
    emit signalActuPlugins();
}

/**
 * Used to execute an action command in the engine
 *
 * @param action the command to execute
 */
void SwiftyWorker::execAction(QString action)
{
    emit executeAction(action);
}

/**
 * Return os name (ex: For Windows this function return "windows")
 *
 * @return os name
 */
QString SwiftyWorker::getOs()
{
    return QSysInfo::productType();
}

void SwiftyWorker::setWindowVisibility(bool visible)
{
    isWindowShow = visible;
}

//===================================================
//================ Private function =================
//===================================================

/**
 * Set icon and toolTip of QTrayIcon
 *
 * @param path the icon to set
 */
void SwiftyWorker::setIcon(QString path)
{
    QIcon icon(path);
    trayIcon->setIcon(icon);

    trayIcon->setToolTip("Swifty Assistant is running !");
}

/**
 * Add menu with action to QTrayIcon
 */
void SwiftyWorker::createActions()
{
    restoreAction = new QAction(tr("&Ouvrir"), this);
    connect(restoreAction, &QAction::triggered, this, &SwiftyWorker::open);

    openPluginFolder = new QAction(tr("Ouvrir le dossier des &plugins"), this);
    connect(openPluginFolder, &QAction::triggered, this, &SwiftyWorker::openPluginsFolder);

    quitAction = new QAction(tr("&Quitter"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void SwiftyWorker::createTrayIcon()
{
    trayIconMenu = new QMenu();
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addAction(openPluginFolder);
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
void SwiftyWorker::reponseReceived(
        QString _reponse, bool isFin, QString typeMessage, QList<QString> url, QList<QString> textUrl
        )
{
    emit reponse(_reponse, isFin, typeMessage, url, textUrl);
}

/**
 * Show the Swifty Assistant window
 */
void SwiftyWorker::open()
{
    QPoint pos = QCursor::pos();
    emit showWindow(pos.x(), pos.y());
}

/**
 * Hide the Swifty Assistant window
 */
void SwiftyWorker::hide()
{
    emit hideWindow();
}

/**
 * Add proposition when engine call this slot
 *
 * @param prop the proposition
 */
void SwiftyWorker::addProp(QString prop)
{
    emit addProposition(prop);
}

/**
 * Remove all proposition of the home screen
 */
void SwiftyWorker::removeAllProp()
{
    emit removeAllProposition();
}

/**
 * Remove one proposition with a index
 *
 * @param index index of removed proposition
 */
void SwiftyWorker::removeProp(int index)
{
    emit removeProposition(index);
}

/**
 * Display a qml file on the Swifty Assistant window
 *
 * @param qmlUrl url of qml file
 */
void SwiftyWorker::showQmlFile(QString qmlUrl)
{
    emit showQml(qmlUrl);
}

/**
 * When engine find a plugin
 *
 * @param name the plugin id
 */
void SwiftyWorker::pluginTrouved(QString name)
{
    emit pluginName(name);
}

/**
 * If the plugin whant to send a message to qml file actually showed
 *
 * @param message
 * @param pluginId
 */
void SwiftyWorker::messageToQml(QString message, QString pluginId)
{
    emit pluginSendedMessageToQml(message, pluginId);
}

void SwiftyWorker::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason != QSystemTrayIcon::Context) {
        if (!isWindowShow) open();
        else hide();
    }
}

void SwiftyWorker::showHomeScreen() {
    emit homeScreen();
}

void SwiftyWorker::previousPage() {
    emit showPreviousPage();
}

void SwiftyWorker::sendNotify(QString title, QString text, QString action)
{
    QSystemTrayIcon::MessageIcon messageIcon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
    trayIcon->showMessage(title, text, messageIcon, 8000);
    actionNotify = action;
}

void SwiftyWorker::notifyClicked()
{
    emit executeAction(actionNotify);
    actionNotify.clear();
}

void SwiftyWorker::openPluginsFolder()
{
    QDir pluginsDir(QDir::homePath());
    pluginsDir.cd("SwiftyPlugins");
    QDesktopServices::openUrl(QUrl("file:///"+pluginsDir.path()));
}
