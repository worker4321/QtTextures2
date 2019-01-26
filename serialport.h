 #ifndef SERIALPORT_H
#define SERIALPORT_H

#include <queue>
#include <QThread>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QQuaternion>
#include <QSemaphore>
#include <QMutex>

//head cmd  x                             CRC
//0x55 0x51 AxL AxH AyL AyH AzL AzH TL TH SUM
//0x55 0x54 HxL HxH HyL HyH HzL HzH TL TH SUM
#define  FRAME_HEAD          0x55
#define  FRAME_CMD_ACC       0x51
#define  FRAME_CMD_ANGLE     0x53
#define  FRAME_CMD_MAG       0x54
#define  FRAME_CMD_QUATER    0x59
#define  FRAME_LENGTH        11
#define  FRAME_OFFSET_CMD    1
#define  FRAME_OFFSET_CRC    10


class SerialPort : public QThread
{
public:
    explicit SerialPort(QString port);
    ~SerialPort();

    //dataParseFrame pFrame;
    //QByteArray RcvBuf;
    QQuaternion Q4;
    float EulerAngles[3];
    int openSerialPort();
    void closeSerialPort();
//    void setBusy(bool val);
 //   bool getBusy(void);
    void parseFrame(void);
 //   void getQuternion(void);
   void write(const QByteArray &writeData);
private:

 //   void readData();

    void handleError(QSerialPort::SerialPortError error);
    QSerialPort *m_serialPort = nullptr;
    QByteArray m_readData;
    QByteArray m_writeData;
    QByteArray m_dataBuf;
    qint64 m_bytesWritten = 0;

private slots:
    void handleReadyRead();
    void handleBytesWritten(qint64 bytes);

protected:
 //   void run();
    QString m_comName;
 //   bool busy;
};


#endif // SERIALPORT_H
