#ifndef CLIENT_H
#define CLIENT_H
//#define MIPCONFIG_SUPPORT_PORTAUDIO
#include "authorizationdialog.h"
#include "mainwindow.h"
#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>
#include <QUdpSocket>
#include "emiplib/mipaudiosession.h"
#include "emiplib/mippainputoutput.h"
#include "emiplib/mipconfig.h"
#include "jrtplib3/rtpipv4address.h"
#include <iostream>

class Client : public QDialog
{
    Q_OBJECT

private:
    AuthorizationDialog ad;
    MainWindow mw;

    QTcpSocket *socket = nullptr;
    QString currentData;
    quint16 blockSize;
    QString Username;
    QTimer *timer;
    MIPAudioSessionParams AudioParameters;
    MIPAudioSession session;
    QUdpSocket *udpSocket;

private slots:
    void ReadyRead();
    void displayError(QAbstractSocket::SocketError socketError);
    bool sendName();

public:
    explicit Client(QWidget *parent = nullptr);
    void ShowAut();
    bool Auth = false;
    bool sendCurrentMessage(QString Message);
    bool sendPrivateMessage(QString Message, QString Reciever);
    void requestNewConnection(QString name, QString ipAddress="127.0.0.1");
    void makeCall(QString address);
    void stopCall();
    bool requestCall(QString Reciever);
    bool callAccepted(QString Sender);
    bool callRejected(QString Sender);

};

#endif // CLIENT_H
