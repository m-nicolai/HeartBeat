/***************************************************************************
**
** This file is part of the heartBeat app for bluetooth heartrate measuring
** Copyright (C) 2016 Matthijs Nicolai
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>

#include "devicemanager.h"
#include "heartratemonitor.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickView* view = new QQuickView();

    DeviceManager deviceManager;
    HeartRateMonitor heartRateMonitor;
    deviceManager.setParent(view);
    view->rootContext()->setContextProperty("deviceManager", &deviceManager);
    view->rootContext()->setContextProperty("heartRateMonitor", &heartRateMonitor);
    view->setSource(QUrl(QStringLiteral("qrc:///Main.qml")));
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->show();
    return app.exec();
}

