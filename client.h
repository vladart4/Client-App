#ifndef CLIENT_H
#define CLIENT_H

#include "authorizationdialog.h"
#include "mainwindow.h"
#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>
#include "../ServerTestNew/newclient.h"

typedef NewClient::Action Action;

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
    void requestNewConnection(QString name);

};

#endif // CLIENT_H
