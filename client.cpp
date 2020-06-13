#include "client.h"
#include "authorizationdialog.h"
#include <QtWidgets>
#include <QtNetwork>
#include <QMessageBox>
#include <QTranslator>
#include <QTimer>
#include "D:\Users\vlada\Documents\ServerTestNew\newclient.h"



Client::Client(QWidget *parent) :
     QDialog(parent)
    , tcpSocket(new QTcpSocket(this))
{
    //QObject::connect(this, &Client::ShowAut, this, &Client::requestNewConnection);

   connect(tcpSocket, &QIODevice::readyRead, this, &Client::ReadyRead);

   connect(tcpSocket, &QAbstractSocket::errorOccurred,

            this, &Client::displayError);
   blockSize = 0;

}

void Client::ShowAut() //Вызываем окно аутентификации
{

 ad.show();
 ad.SetClient(this);

}




void Client::requestNewConnection(QString name) //запрашиваем коннект
{
    tcpSocket->abort();
    tcpSocket->connectToHost("127.0.0.1",
                             1234);

    Username = name;
    QTimer::singleShot(100,this,&Client::sendName);

}

void Client::ReadyRead()
{



    QDataStream in(tcpSocket);

    Action command;

    in >> command;

    switch(command)
    {
        case Action::Connect :
    {
        QMessageBox msg;

        bool bAccess;
        in >> bAccess;
        if (bAccess)
        {
            ad.close();
            mw.show();
            mw.SetClient(this);
            Auth = true;
            QList<QString> NamesList;
            in >> NamesList;
            mw.UpdateNames(NamesList);
            mw.SetName(Username);
            SendCurrentMessage("Has entered the chat");
        }
        else
        {
            msg.setText("This username is already taken");
            msg.exec();
        }
        tcpSocket->readAll();
        break;

    }
    case Action::NameUpdate :
    {

       if (Auth)
       {
           QList<QString> NamesList;
           in >> NamesList;
        mw.UpdateNames(NamesList);


       }
        tcpSocket->readAll();
        break;
    }

    case Action::Message :
    {
        QString msg;
        QString name;
        in >> msg;
        in >> name;
        mw.DisplayMessage(msg, name);
        break;

    }

    case Action::Private :
    {
        QString msg;
        QString name;
        in >> msg;
        in >> name;

        mw.DisplayPrivateMessage(msg, name);
        break;

    }



    }



}

void Client::displayError(QAbstractSocket::SocketError socketError)
{

   QMessageBox msg;
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
       msg.setText("The host was not found. Please check the "
                   "host name and port settings.");
       msg.exec();
        break;
    case QAbstractSocket::ConnectionRefusedError:
        msg.setText("The connection was refused by the peer. "
                    "Make sure the fortune server is running, "
                    "and check that the host name and port "
                    "settings are correct.");
        msg.exec();
        break;
    default:
        msg.setText("The following error occurred: %1.");
        msg.exec();
    }


}


bool Client::sendName()
{
    if(tcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        //tcpSocket->write(IntToArray(data.size()));
        Action ac = Action::Connect;
        QByteArray l;
        //l.append(QByteArray::number(ac));
       QDataStream stream (&l, QIODevice::WriteOnly);
       stream << quint16(0);
       stream << ac;
       stream << Username;
       stream.device()->seek(0);
       stream << quint16(l.size() - sizeof(quint16));
       tcpSocket->write(l);
        //tcpSocket->write(data);
        return tcpSocket->waitForBytesWritten();
    }
    else
        return false;

}



bool Client::SendCurrentMessage(QString Message)
{
    if(tcpSocket->state() == QAbstractSocket::ConnectedState)
    {

       Action ac = Action::Message;
       QByteArray l;;
       QDataStream stream (&l, QIODevice::WriteOnly);
       //QString aa = "test";
       stream << quint16(0);
       stream << ac;
       stream << Username;
       stream << Message;
       stream.device()->seek(0);
       stream << quint16(l.size() - sizeof(quint16));
       tcpSocket->write(l);

        return tcpSocket->waitForBytesWritten();

    }

    else return false;
}

bool Client::SendPrivateMessage(QString Message, QString Reciever)
{
    if(tcpSocket->state() == QAbstractSocket::ConnectedState)
    {

        Action ac = Action::Private;
        QByteArray l;
       QDataStream stream (&l, QIODevice::WriteOnly);
       stream << quint16(0);
       stream << ac;
       stream << Username;
       stream << Message;
       stream << Reciever;
       stream.device()->seek(0);
       stream << quint16(l.size() - sizeof(quint16));
       tcpSocket->write(l);

       return tcpSocket->waitForBytesWritten();

    }

    else
        return false;
}





