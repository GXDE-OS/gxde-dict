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

#include "popupcontent.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMouseEvent>
#include <QLabel>
#include "dimagebutton.h"
#include <QDBusMessage>
#include <QtConcurrent>
#include <QDBusConnection>

PopupContent::PopupContent(QWidget *parent)
    : DAbstractDialog(parent),
      m_queryLabel(new QLabel),
      m_transLabel(new QLabel)
{
    DBlurEffectWidget *bgWidget = new DBlurEffectWidget(this);
    bgWidget->setBlendMode(DBlurEffectWidget::BehindWindowBlend);
    bgWidget->setMaskColor(DBlurEffectWidget::LightColor);

    QScrollArea *contentFrame = new QScrollArea;
    contentFrame->setWidgetResizable(true);
    contentFrame->setStyleSheet(contentFrame->styleSheet()
                                + "QScrollArea { background: transparent; }" 
                                + "QScrollArea > QWidget > QWidget { background: transparent; }");

    m_transLabel->setStyleSheet("color: black;");
    m_queryLabel->setStyleSheet("color: black;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(contentFrame);
    layout->setContentsMargins(0, 0, 0, 0);

    m_querySpeakBtn.setNormalPic(":/images/audio-light-normal.svg");
    m_querySpeakBtn.setHoverPic(":/images/audio-light-hover.svg");
    m_querySpeakBtn.setPressPic(":/images/audio-light-press.svg");
    layout->addWidget(&m_querySpeakBtn);
    connect(&m_querySpeakBtn, &DImageButton::clicked, this, [this](){
        speakText(m_queryLabel->text());
    });

    m_transSpeakBtn.setNormalPic(":/images/audio-light-normal.svg");
    m_transSpeakBtn.setHoverPic(":/images/audio-light-hover.svg");
    m_transSpeakBtn.setPressPic(":/images/audio-light-press.svg");
    layout->addWidget(&m_transSpeakBtn);
    connect(&m_transSpeakBtn, &DImageButton::clicked, this, [this](){
        speakText(m_transLabel->text());
    });


    QWidget *mainWidget = new QWidget;
    QGridLayout *mainLayout = new QGridLayout(mainWidget);
    contentFrame->setWidget(mainWidget);

    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->addWidget(m_queryLabel, 0, 0);
    mainLayout->addWidget(&m_querySpeakBtn, 0, 1);
    mainLayout->addWidget(m_transLabel, 1, 0);
    mainLayout->addWidget(&m_transSpeakBtn, 1, 1);
    //mainLayout->addStretch();

    m_queryLabel->setWordWrap(true);
    m_transLabel->setWordWrap(true);

    setFixedSize(300, 200);
    bgWidget->resize(size());
}

PopupContent::~PopupContent()
{
}

void PopupContent::mouseMoveEvent(QMouseEvent *e)
{
    // disable move window.
    e->ignore();
}

void PopupContent::speakText(QString text)
{
    QDBusMessage dbus = QDBusMessage::createMethodCall("com.gxde.daemon.ai.speaker",
                                                       "/com/gxde/daemon/ai/speaker",
                                                       "com.gxde.daemon.ai.speaker",
                                                       "TextToSpeech");
    dbus << text;
    auto future = QtConcurrent::run([=]() {
        QDBusConnection::sessionBus().call(dbus);
    });
    Q_UNUSED(future)
}

void PopupContent::clear()
{
    m_queryLabel->setText("");
    m_transLabel->setText("");
}

void PopupContent::updateContent(std::tuple<QString, QString, QString, QString, QString> data)
{
    m_queryLabel->show();
    m_querySpeakBtn.show();
    m_queryLabel->setText(std::get<0>(data));
    m_transLabel->setText(std::get<3>(data));
}

void PopupContent::updateTranslate(QString text)
{
    m_queryLabel->hide();
    m_querySpeakBtn.hide();
    m_queryLabel->setText("");
    m_transLabel->setText(text);
}
