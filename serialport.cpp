#include "serialport.h"
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QDebug>

SerialPort::SerialPort(QString port):m_comName(port)
{
    m_serialPort = new QSerialPort(this);
    if(m_serialPort == nullptr)
    {
        qDebug()<<"new QserialPort failed"<<endl;
    }

    connect(m_serialPort, &QSerialPort::readyRead, this, &SerialPort::handleReadyRead);
    connect(m_serialPort, &QSerialPort::bytesWritten, this, &SerialPort::handleBytesWritten);
    openSerialPort();
}
int SerialPort::openSerialPort()
{
    if(m_comName.length() < 1)
    {
        qDebug() << "port is not valid:"<<m_comName;
        return -1;
    }
    m_serialPort->setPortName(m_comName);
    m_serialPort->setBaudRate(QSerialPort::Baud9600);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    m_serialPort->setReadBufferSize(1024*20);
    if (m_serialPort->open(QIODevice::ReadWrite))
    {
        qDebug() << "Connected to" << m_comName;
    }
    else
    {
        qCritical()<<m_comName << " :Serial Port error:" << m_serialPort->errorString();
        qDebug() << tr("Open error");
    }
    return 1;
}
void SerialPort::handleReadyRead()
{
    m_readData.append(m_serialPort->readAll());
    m_dataBuf += m_readData;
    m_readData.clear();
    parseFrame();
}
void SerialPort::handleBytesWritten(qint64 bytes)
{
    m_bytesWritten += bytes;
    if (m_bytesWritten == m_writeData.size())
    {
        m_bytesWritten = 0;
//        m_standardOutput << QObject::tr("Data successfully sent to port %1")
//                            .arg(m_serialPort->portName()) << endl;
//        QCoreApplication::quit();
    }
}
void SerialPort::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError)
    {
        // QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        qCritical() << "Serial Port error:" << m_serialPort->errorString();
        closeSerialPort();
    }
}

void SerialPort::closeSerialPort()
{
    m_serialPort->close();
    qDebug() << tr("closed");
}
void SerialPort::write(const QByteArray &writeData)
{
    m_writeData = writeData;
    const qint64 bytesWritten = m_serialPort->write(writeData);
    if ((bytesWritten == -1) || (bytesWritten != m_writeData.size()))
    {
       qDebug("Failed to write.") ;
    }
}

void SerialPort::parseFrame(void)
{
//    float tmp;
    //uint8_t data[FRAME_LENGTH];
    int i=0,j = 0;
    unsigned char sum = 0;
//    char *data = new char[RcvBuf.size() + 1];
 //   static QByteArray tmp;

    while(m_dataBuf.length() >= FRAME_LENGTH)
    {
        sum = 0;
       // strcpy(data, RcvBuf.data());
       // char *data = RcvBuf.data();
        while((uint8_t)m_dataBuf.data()[0]!= FRAME_HEAD )
        {
           //qDebug("0x%02X",(uint8_t)dataBuf.data()[0]);

            if( m_dataBuf.length() )
            {
                m_dataBuf.remove(0,1);
                continue;
            }
            else
                return;
        }
        //55 53 05 00 01 00 03 00 4D 0B 09
        for(j=0; j<FRAME_LENGTH -1; j++)
        {
            sum += (uint8_t)(uint8_t)m_dataBuf.data()[j];
           // qDebug("0x%02X",(uint8_t)data[j]);
        }

        if(sum != (uint8_t)m_dataBuf.data()[FRAME_LENGTH -1])//
        {
             for(j=0; j<FRAME_LENGTH -1; j++)
             {
                qDebug("0x%02X",(uint8_t)(uint8_t)m_dataBuf.data()[j]);
             }
            qDebug("sum = 0x%02X",sum);
            qDebug("0x%02X",(uint8_t)m_dataBuf.data()[FRAME_LENGTH -1]);
            if( m_dataBuf.length() )
            {
                m_dataBuf.remove(0,1);
                continue;
            }
            else
                return;
            qDebug() << tr("sum error");
           // if( dataBuf.length() )
            //    continue;
            //else
                return;
        }
        else
        {
            sum = 0;
            //qDebug() << tr("sum ok");
            switch((uint8_t)m_dataBuf.data()[FRAME_OFFSET_CMD])
            {
                case FRAME_CMD_ANGLE:
                {
                    EulerAngles[0] = int16_t((uint8_t)m_dataBuf.data()[3]<<8 | (uint8_t)m_dataBuf.data()[2]) / 32768.0f *180.f;
                    EulerAngles[1] = -int16_t((uint8_t)m_dataBuf.data()[5]<<8 | (uint8_t)m_dataBuf.data()[4]) / 32768.0f *180.f;
                    EulerAngles[2] = int16_t((uint8_t)m_dataBuf.data()[7]<<8 | (uint8_t)m_dataBuf.data()[6]) / 32768.0f *180.f;
                    //for(j=2; j<FRAME_LENGTH -1; j++)
                    //{
                    //   qDebug("0x%02X",(uint8_t)(uint8_t)RcvBuf.data()[j]);
                    //}
                    qDebug("1 EulerAngles = %0.2f,%0.2f,%0.2f",EulerAngles[0],EulerAngles[1],EulerAngles[2]);
                }break;
                case FRAME_CMD_QUATER:
                {
                    Q4.setScalar(int16_t((uint8_t)m_dataBuf.data()[3]<<8 | (uint8_t)m_dataBuf.data()[2]) / 32768.0f);
                    Q4.setZ(int16_t((uint8_t)m_dataBuf.data()[5]<<8 | (uint8_t)m_dataBuf.data()[4]) / 32768.0f);
                    Q4.setX(int16_t((uint8_t)m_dataBuf.data()[7]<<8 | (uint8_t)m_dataBuf.data()[6]) / 32768.0f);
                    Q4.setY(int16_t((uint8_t)m_dataBuf.data()[9]<<8 | (uint8_t)m_dataBuf.data()[8]) / 32768.0f);

                   // QVector3D q3d = Q4.toEulerAngles();
                   // qDebug("2 EulerAngles = %0.2f,%0.2f,%0.2f",q3d.x(),q3d.y(),q3d.z());
                    //Q4.getEulerAngles(&EulerAngles[0],&EulerAngles[1],&EulerAngles[2]);
                   // qDebug("2 EulerAngles = %0.2f,%0.2f,%0.2f",EulerAngles[0],EulerAngles[1],EulerAngles[2]);
                }break;
                default:
                {

                }break;
            }
            if( m_dataBuf.length() >= FRAME_LENGTH)
            {
                m_dataBuf.remove(0,FRAME_LENGTH);
            }
        }
    }
 //   delete [] data;
}


SerialPort::~SerialPort()
{
    closeSerialPort();
}
