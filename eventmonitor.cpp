/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     rekols <rekols@foxmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "eventmonitor.h"

#include <QGuiApplication>

#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <X11/extensions/record.h>

static void xrecordCallback(XPointer ptr, XRecordInterceptData *data);

EventMonitor::EventMonitor(QObject *parent)
    : QThread(parent)
{
}

EventMonitor::~EventMonitor()
{
    requestInterruption();
    quit();
    wait();
}

void EventMonitor::run()
{
    if (isInterruptionRequested())
        return;

    auto *x11App = qApp->nativeInterface<QNativeInterface::QX11Application>();
    if (!x11App)
        return;
    auto *display = x11App->display();

    if (display == nullptr) {
        return;
    }

    XRecordClientSpec clients = XRecordAllClients;
    XRecordRange *range = XRecordAllocRange();
    if (range == nullptr) {
        return;
    }

    memset(range, 0, sizeof(XRecordRange));
    range->device_events.first = KeyPress;
    range->device_events.last = MotionNotify;

    XRecordContext context = XRecordCreateContext(display, 0, &clients, 1, &range, 1);
    if (context == 0) {
        return;
    }
    XFree(range);

    XSync(display, True);

    Display *display_datalink = XOpenDisplay(nullptr);
    if (display_datalink == nullptr) {
        return;
    }

    XRecordEnableContext(display_datalink, context, xrecordCallback, (XPointer) this);
}

static void xrecordCallback(XPointer ptr, XRecordInterceptData *data)
{
    EventMonitor *monitor = static_cast<EventMonitor *>(static_cast<void *>(ptr));

    if (data->category == XRecordFromServer) {
        xEvent *event = (xEvent *)data->data;

        switch (event->u.u.type) {
        case ButtonPress:
            QMetaObject::invokeMethod(monitor, "buttonPress", Qt::QueuedConnection,
                                      Q_ARG(int, event->u.keyButtonPointer.rootX),
                                      Q_ARG(int, event->u.keyButtonPointer.rootY));
            break;

        case ButtonRelease:
            QMetaObject::invokeMethod(monitor, "buttonRelease", Qt::QueuedConnection,
                                      Q_ARG(int, event->u.keyButtonPointer.rootX),
                                      Q_ARG(int, event->u.keyButtonPointer.rootY));
            break;

        default:
            break;
        }
    }

    fflush(stdout);
    XRecordFreeData(data);
}
