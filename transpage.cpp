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
#include "transpage.h"
#include <QVBoxLayout>
#include <QKeyEvent>

TransPage::TransPage(QWidget *parent)
    : QWidget(parent),
      m_orginEdit(new TextEdit),
      m_transEdit(new TextEdit),
      m_typeBox(new QComboBox),
      m_transBtn(new QPushButton("翻译")),
      m_api(YoudaoAPI::instance())
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *transLayout = new QHBoxLayout;

    //m_typeBox->addItem("自动检测语言");
    for (QString i: m_translateName) {
        m_typeBox->addItem("→ " + i);
    }

    transLayout->addWidget(m_typeBox);
    transLayout->addWidget(m_transBtn);

    layout->setContentsMargins(15, 10, 15, 15);
    layout->addWidget(m_orginEdit);
    layout->addSpacing(5);
    layout->addLayout(transLayout);
    layout->addSpacing(5);
    layout->addWidget(m_transEdit);

    m_transBtn->setObjectName("QueryBtn");
    m_transBtn->setStyleSheet(m_transBtn->styleSheet() + "border-radius: 4px");
    m_transBtn->setFixedSize(130, 35);

    m_orginEdit->setPlaceholderText("请输入您要翻译的文字");
    m_transEdit->setReadOnly(true);

    connect(m_transBtn, &QPushButton::clicked, this, &TransPage::translate);
    //connect(m_api, &YoudaoAPI::translateStreamDataReceived, this, &TransPage::handleTranslateStreamDataReceived);
    connect(m_api, &YoudaoAPI::translateFinished, this, &TransPage::handleTranslateFinished);
    connect(m_typeBox, &QComboBox::currentTextChanged, [=] { translate(); });

    connect(m_orginEdit, &TextEdit::textChanged, [=] {
                                                     if (m_orginEdit->toPlainText().isEmpty()) {
                                                         m_transEdit->clear();
                                                     }
                                                 });

    connect(m_orginEdit, &TextEdit::focusIn, [=] { m_transEdit->clearSelection(); });
    connect(m_orginEdit, &TextEdit::focusOut, [=] { m_orginEdit->clearSelection(); });
}

TransPage::~TransPage()
{
}

void TransPage::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Return && (e->modifiers() & Qt::ControlModifier)) {
        translate();
    }

    // request window keypress.
    (qobject_cast<MainWindow *>(this->window()))->requestKeyPressEvent(e);
}

void TransPage::translate()
{
    QString text = m_orginEdit->toPlainText();

    if (text.isEmpty())
        return;

    int currentType = m_typeBox->currentIndex();
    m_api->translate(text, m_translateToList[currentType]);
}

void TransPage::handleTranslateStreamDataReceived(const QString &result)
{
    QString r = result;
    r.replace("<think>", "");
    r.replace("</think>", "");
    r.replace("\n\n", "\n");
    m_transEdit->setPlainText(m_transEdit->toPlainText() + r);
}

void TransPage::handleTranslateFinished(const QString &result)
{
    m_transEdit->setPlainText(result);
}
