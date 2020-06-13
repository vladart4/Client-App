#include "client.h"
#include "authorizationdialog.h"
#include <QtWidgets>
#include <QtNetwork>
#include <QMessageBox>
#include <QTranslator>
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

void Client::ShowAut()
{

 ad.show();
 ad.SetClient(this);
 requestNewConnection();

}

void Client::WriteD(QByteArray data)
{
  VerifyLogin(data);
}



void Client::requestNewConnection()
{
    tcpSocket->abort();
    tcpSocket->connectToHost("127.0.0.1",
                             1234);

}

void Client::ReadyRead()
{
//    in.startTransaction();

//    QString nextData;
//    in >> nextData;

//   if (!in.commitTransaction())
//       return;

//  if (nextData == currentData) {
//      QTimer::singleShot(0, this, &Client::requestNewConnection);
//        return;
//    }

//    currentData = nextData;


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

bool Client::VerifyLogin(QByteArray data)
{
    if(tcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        //tcpSocket->write(IntToArray(data.size()));
        Action ac = Action::Connect;
        QByteArray l;
        //l.append(QByteArray::number(ac));
       QDataStream stream (&l, QIODevice::WriteOnly);
       QDataStream newstr (&data,QIODevice::ReadWrite);
       //QString aa = "test";
       QString login;
       QString pass;
       newstr >> login;
       newstr >> pass;
       Username = login;
       stream << quint16(0);
       stream << ac;
       stream << login;
       stream << pass;
       stream.device()->seek(0);
       stream << quint16(l.size() - sizeof(quint16));
       tcpSocket->write(l);
        //tcpSocket->write(data);
        return tcpSocket->waitForBytesWritten();

    }

   else return false;


}

QByteArray Client::IntToArray(qint32 source) //Use qint32 to ensure that the number have 4 bytes
{
    //Avoid use of cast, this is the Qt way to serialize objects
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}


bool Client::SendCurrentMessage(QString Message)
{
    if(tcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        //tcpSocket->write(IntToArray(data.size()));
        Action ac = Action::Message;
        QByteArray l;
        //l.append(QByteArray::number(ac));
       QDataStream stream (&l, QIODevice::WriteOnly);
       //QString aa = "test";
       stream << quint16(0);
       stream << ac;
       stream << Username;
       stream << Message;
       stream.device()->seek(0);
       stream << quint16(l.size() - sizeof(quint16));
       tcpSocket->write(l);
        //tcpSocket->write(data);
        return tcpSocket->waitForBytesWritten();

    }

   else return false;
}

void Client::RecieveMessage(QString Message, QString Sender)
{

}



