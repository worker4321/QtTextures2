/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include "glwidget.h"
#include "window.h"
#include <serialport.h>
#include <QtSerialPort/QSerialPortInfo>

Window::Window()
{

    QHBoxLayout *mainLayout = new QHBoxLayout;
    QHBoxLayout *bomLayout = new QHBoxLayout;
    //QHBoxLayout *topLayout = new QHBoxLayout;

    //configMenu = menuBar()->addMenu(tr("&Config"));
    //configMenu->addAction(Serail);

    QTextStream out(stdout);

    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    out << "Total number of ports available: " << serialPortInfos.count() << endl;
    const QString blankString = "N/A";
    QString description;
    QString manufacturer;
    QString serialNumber;

    serialPortLabel= new QLabel(tr("port:"));
    serialPortComboBox = new QComboBox();
    statusLabel= new QLabel(tr("Status: Not running."));  statusLabel->setHidden(true);
    runButton = new QPushButton(tr("Start"));
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
        serialPortComboBox->addItem(info.portName());

    //QHBoxLayout mainLayout = new QHBoxLayout;
    mainLayout->addStretch();
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(serialPortLabel);
    mainLayout->addWidget(serialPortComboBox);
    mainLayout->addWidget(runButton);
    mainLayout->addWidget(statusLabel);
    setLayout(mainLayout);
    setWindowTitle(tr("Serial"));
    serialPortComboBox->setFocus();
    connect(runButton, &QPushButton::clicked, this, &Window::BtnRun);
    m_serialPort = NULL;
//    for (const QSerialPortInfo &serialPortInfo: serialPortInfos)
//    {
//        description = serialPortInfo.description();
//        manufacturer = serialPortInfo.manufacturer();
//        serialNumber = serialPortInfo.serialNumber();
//        out << endl
//            << "Port: " << serialPortInfo.portName() << endl
//            << "Location: " << serialPortInfo.systemLocation() << endl
//            << "Description: " << (!description.isEmpty() ? description : blankString) << endl
//            << "Manufacturer: " << (!manufacturer.isEmpty() ? manufacturer : blankString) << endl
//            << "Serial number: " << (!serialNumber.isEmpty() ? serialNumber : blankString) << endl
//            << "Vendor Identifier: " << (serialPortInfo.hasVendorIdentifier()
//                                         ? QByteArray::number(serialPortInfo.vendorIdentifier(), 16)
//                                         : blankString) << endl
//            << "Product Identifier: " << (serialPortInfo.hasProductIdentifier()
//                                          ? QByteArray::number(serialPortInfo.productIdentifier(), 16)
//                                          : blankString) << endl
//            << "Busy: " << (serialPortInfo.isBusy() ? "Yes" : "No") << endl;


//    m_serialPort = new SerialPort(serialPortInfo.portName());
//    break;
//    }
//    if(!m_serialPort)
//    {
//        qDebug()<<"new serialPort failed,exit"<<endl;
//        exit(-1);
//    }


    for (int i = 0; i < NumRows; ++i)
    {
        for (int j = 0; j < NumColumns; ++j)
        {
            QColor clearColor;
            //clearColor.setRgb(128,128,128,255);
            //clearColor.setHsv(255,255,255,0);

            glWidgets[i][j] = new GLWidget;
            glWidgets[i][j]->setClearColor(clearColor);
            glWidgets[i][j]->rotateBy(0, 0, 0);
            bomLayout->addWidget(glWidgets[i][j]);//, i, j

            connect(glWidgets[i][j], &GLWidget::clicked,this, &Window::setCurrentGlWidget);
        }
    }
   // this->setAutoFillBackground(true);    //Widget增加背景图片时，这句一定要。
 //   QPixmap pixmap(":/images/mag1.png");
//    QPixmap fitpixmap=pixmap.scaled(1200, 200).scaled(1200,1200, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    QPalette palette;
//    palette.setBrush(QPalette::Foreground, QBrush(fitpixmap));
//    this->setPalette(palette);

    // QGridLayout *topLayout = new QGridLayout;

    m_magLabel = new QLabel(this);
    m_angleLabel = new QLabel(this);
    m_magPixmap = new QPixmap(":/images/mag1.png");

    //
    //pLabel->setScaledContents(true);
    //pLabel->move(10,10);
   // pLabel->show();
   // pLabel->setAttribute(Qt::WA_DeleteOnClose);
    //pLabel->setGeometry(rect().width() - 200, rect().height(),200, 200);
    //pLabel->setParent(this);

    //bomLayout->addWidget(pLabel);

    mainLayout->addLayout(bomLayout);
   // mainLayout->addLayout(topLayout);

    setLayout(mainLayout);

    QMatrix matrix;
     matrix.rotate(0);
    m_magLabel->setPixmap(m_magPixmap->transformed(matrix,Qt::SmoothTransformation));
    m_magLabel->raise();
    m_magLabel->move(10,10);
    m_magLabel->setFixedSize(120, 120);
    m_magLabel->setScaledContents(true);

    m_angleLabel->move(130,10);
    QPalette pal;
    pal.setColor(QPalette::WindowText,Qt::white);
    m_angleLabel->setPalette(pal);

    QFont font;
    font.setPointSize(12);
    font.setBold(false);
    m_angleLabel->setFont(font);
    m_angleLabel->raise();
    //float a  = 0.9123f;
   // m_angleLabel->setText(QString::number(a));



    currentGlWidget = glWidgets[0][0];

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Window::rotateOneStep);
    timer->start(10);

    setWindowTitle(tr("Textures"));

    connect(m_magLabel, SIGNAL(setQlabel()), this, SLOT(senddata()));
}
void Window::BtnRun(void)
{
  QString status = runButton->text();
  if(  status == "running"){
       //serial->closeSerialPort();
       //m_serialPort->setBusy(true);
       m_serialPort->quit = true;
       //statusLabel->setText("closed");
       delete m_serialPort;
       //m_serialPort = NULL;
       runButton->setText("closed");
  }else{
      QString com = serialPortComboBox->currentText();
      m_serialPort = new SerialPort(com);
      if(!m_serialPort){
          qDebug()<<"new serialPort failed,exit"<<endl;
          return;
      }
      m_serialPort->start();
      runButton->setText("running");
      //statusLabel->setText("runing");
  }
}
void Window::setCurrentGlWidget()
{
    currentGlWidget = qobject_cast<GLWidget *>(sender());
}

void Window::rotateOneStep()
{
    if(runButton->text() != "running")
      return;

    if (currentGlWidget)
        currentGlWidget->rotateBy(m_serialPort->EulerAngles[0], m_serialPort->EulerAngles[1], m_serialPort->EulerAngles[2]);

    QPixmap tempPixmap(m_magPixmap->size());
    tempPixmap.fill(Qt::transparent);
    QPainter  painter(&tempPixmap);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.translate(m_magPixmap->width() / 2, m_magPixmap->height() / 2);
    painter.rotate(- (m_serialPort->EulerAngles[2]) - 15);
    painter.translate(-(m_magPixmap->width() / 2), -(m_magPixmap->height() / 2));
    painter.drawPixmap(0, 0, *m_magPixmap);
    painter.end();
    m_magLabel->setPixmap(tempPixmap);


/*    QString str0,str1,str2;
    str0.setNum(m_serialPort->EulerAngles[0],'f',2);
    str1.setNum(m_serialPort->EulerAngles[1],'f',2);
    str2.setNum(m_serialPort->EulerAngles[2],'f',2);

     m_angleLabel->setText(tr("Roll ") + str0 + tr("\n") + \
                           tr("Pitch ") + str1 + tr("\n") + \
                           tr("Yaw ") + str2 + tr("\n"));*/
}
void Window::mouseDoubleClickEvent(QMouseEvent* event)
{
//    qDebug("emit");
    emit setQlabel();

    QByteArray data;
    data.resize(5);
    data[0] = 0xFF;
    data[1] = 0xAA;
    data[2] = 0x01;
    data[3] = 0x08;
    data[4] = 0x00;
    m_serialPort->write(data);
}
void Window::senddata()
{
    qDebug("senddata");
}
