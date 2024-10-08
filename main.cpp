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

#include "mainwindow.h"
#include <DApplication>
#include <DWidgetUtil>
#include "utils.h"

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    app.loadTranslator();
    app.setOrganizationName("GXDE");
    app.setApplicationVersion(DApplication::buildVersion("0.1"));
    app.setApplicationAcknowledgementPage("https://gitee.com/GXDE-OS");
    app.setProductIcon(QIcon(":/images/gxde-dict.svg"));
    app.setProductName(DApplication::translate("Main", "GXDE Dictionary"));
    app.setApplicationDescription(DApplication::translate("Main", "GXDE词典，基于 Qt / DTK 开发，源于Redict，占用内存小，使用有道词典 API，内容版权归网易有道公司所有。"));

    if (!app.setSingleInstance("gxde-dict")) {
        return -1;
    }

    MainWindow w;
    w.show();

    Dtk::Widget::moveToCenter(&w);

    return app.exec();
}
