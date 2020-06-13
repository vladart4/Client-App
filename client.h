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

private slots:
    void requestNewConnection();
    void ReadyRead();
    void displayError(QAbstractSocket::SocketError socketError);
    bool VerifyLogin(QByteArray data);

public:
    explicit Client(QWidget *parent = nullptr);
    void ShowAut();
    void WriteD(QByteArray data);
    QByteArray IntToArray(qint32 source);
    bool Auth = false;
    bool SendCurrentMessage(QString Message);
    void RecieveMessage(QString Message, QString Sender);

};

#endif // CLIENT_H
