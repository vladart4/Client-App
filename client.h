#ifndef CLIENT_H
#define CLIENT_H

#include "authorizationdialog.h"
#include "mainwindow.h"
#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>
#include "D:\Users\vlada\Documents\ServerTestNew\newclient.h"

typedef NewClient::Action Action;

class Client : public QDialog
{
    Q_OBJECT

private:
    AuthorizationDialog ad;
    MainWindow mw;
    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    QString currentData;
    quint16 blockSize;
    QString Username;
    QTimer *timer;

private slots:
    void ReadyRead();
    void displayError(QAbstractSocket::SocketError socketError);
    bool sendName();

public:
    explicit Client(QWidget *parent = nullptr);
    void ShowAut();
    QByteArray IntToArray(qint32 source);
    bool Auth = false;
    bool SendCurrentMessage(QString Message);
    bool SendPrivateMessage(QString Message, QString Reciever);
    void RecieveMessage(QString Message, QString Sender);
    void requestNewConnection(QString name);

};

#endif // CLIENT_H
