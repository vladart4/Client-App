#include <QtWidgets>
#include <QtNetwork>
#include <QMessageBox>
#include <QTranslator>
#include <QAbstractSocket>
#include <QTimer>
#include "client.h"
#include "authorizationdialog.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

QString cutStringWithLength(QString& src)
{
    int index = src.indexOf("_");
    if (index == -1) return "";

    int msgSize = src.left(index).toInt();
    if (msgSize == 0) return "";

    src.remove(0, index+1);
    if (msgSize > src.size()) return "";

    QString answer = src.left(msgSize);
    src.remove(0, msgSize+1);

    return answer;
}

QByteArray prepareMessage(QString msg) {
    QByteArray l;
    QDataStream stream (&l, QIODevice::WriteOnly);
    stream << quint16(0);
    stream << msg;
    stream.device()->seek(0);
    stream << quint16(l.size() - sizeof(quint16));
    return l;
}


Client::Client(QWidget *parent) :
     QDialog(parent)
    , socket(new QTcpSocket(this))
{
    connect(socket, &QIODevice::readyRead, this, &Client::ReadyRead);
    connect(socket, &QAbstractSocket::errorOccurred, this, &Client::displayError);

    blockSize = 0;


}


//Вызываем окно аутентификации
void Client::ShowAut()
{
    ad.show();
    ad.SetClient(this);
}


// Запрашиваем коннект
void Client::requestNewConnection(QString name, QString ipAddress)
{
    socket->abort();
    socket->connectToHost(ipAddress, 14000);
    // socket->connectToHost("192.168.0.4", 14000);
    // socket->connectToHost("134.19.130.129", 14000);

    Username = name;
    QTimer::singleShot(100,this,&Client::sendName);
}


void checkRet(bool ret,const MIPErrorBase &obj)
{
    if (!ret)
    {
        std::cerr << obj.getErrorString() << std::endl;
        exit(-1);
    }
}



void Client::makeCall(QString address)
{
    WSADATA dat;
    #ifdef WIN64
    WSAStartup(MAKEWORD(2,2),&dat);
    #endif // WIN64


    AudioParameters.setPortbase(14002);
    auto ret = session.init(&AudioParameters);
    checkRet(ret, session);
    QHostAddress haddress = QHostAddress(address);
    uint32_t ipaddress = haddress.toIPv4Address();
    session.addDestination(jrtplib::RTPIPv4Address(ipaddress, 14002));

}

void Client::stopCall()
{
    session.destroy();

    #ifdef WIN64
    WSACleanup();
    #endif // WIN64

}




void Client::ReadyRead()
{
   QDataStream in(socket);
   while (socket->bytesAvailable())
   {


    if (blockSize == 0) {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> blockSize;
    }

    if (socket->bytesAvailable() < blockSize)
        return;
    else
        blockSize = 0;

    QString command;
    in >> command;

    if (!command.startsWith("EVMp"))
    {
        // qDebug() << socketDescriptor << "Bad header!";
        return;
    }
    command.remove(0,5); //удаляем EVMp_

    QString cmdType = command.left(command.indexOf("_"));
    command.remove(0, cmdType.length()+1);

    if (cmdType == "WELCOME")
    {
        ad.close();
        mw.show();
        mw.setClient(this);
        Auth = true;
        mw.setName(Username);
        // TODO: внедрить реакцию на отказ имени
        /*
            msg.setText("This username is already taken");
            msg.exec();
        */
    }
    else if (!Auth) {
        return;
    }
    else if (cmdType == "CONNECT")
    {
        bool newOne = command[0] == "1";
        command.remove(0, 2); // удаляем флаг
        QString name = cutStringWithLength(command);
        if (name == "") return;

        mw.onChatEnter(name, newOne);
    }
    else if (cmdType == "DISCONNECT")
    {
        QString name = cutStringWithLength(command);
        if (name == "") return;

        mw.onChatExit(name, false);
    }
    else if (cmdType == "USERLEFT")
    {
        QString name = cutStringWithLength(command);
        if (name == "") return;

        mw.onChatExit(name, true);
    }
    else if (cmdType == "SENDMSG")
    {
        QString name = cutStringWithLength(command);
        if (name == "") return;

        QString msg = command;

        mw.displayMessage(msg, name);
    }
    else if (cmdType == "PRIVATEMSG")
    {
        QString name = cutStringWithLength(command);
        if (name == "") return;

        QString msg = command;

        mw.displayPrivateMessage(msg, name);
    }
    else if (cmdType == "CALL")
    {

        QString name = command;

        mw.callRequestDisplay(name);

    }
    else if (cmdType == "CALLACCEPT")
    {

        QStringList list = command.split(QLatin1Char('_'));
        QString address = list[0];
        QString name = list[1];
        mw.bCall = true;
        makeCall(address);
        mw.setVoiceName(name);

    }
    else if (cmdType == "CALLREJECT")
    {
        QString name = command;
        if (mw.bCall == true)
        {
            stopCall();
            mw.callStopped(name);
        }
        else
        {
            QMessageBox msg;
            msg.setText(name + " has rejected your call");
            msg.exec();
        }

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
            msg.setText("The following error occurred (" + QString::number(socketError) +
                        "): " + socket->errorString());
            msg.exec();
    }

}


bool Client::requestCall(QString Reciever)
{

    if(socket->state() != QAbstractSocket::ConnectedState)
        return false;

    QByteArray carr = prepareMessage("EVMp_CALL_" + Reciever);
    socket->write(carr);

    return socket->waitForBytesWritten();

}

bool Client::callAccepted(QString Sender)
{
    if(socket->state() != QAbstractSocket::ConnectedState)
        return false;

    QByteArray acarr = prepareMessage(
        "EVMp_CALLACCEPT_" + Sender);
    socket->write(acarr);
    return socket->waitForBytesWritten();
}

bool Client::callRejected(QString Sender)
{
    if(socket->state() != QAbstractSocket::ConnectedState)
        return false;

    QByteArray acarr = prepareMessage(
        "EVMp_CALLREJECT_" + Sender);
    socket->write(acarr);
    return socket->waitForBytesWritten();
}

bool Client::sendName()
{
    if(socket->state() != QAbstractSocket::ConnectedState)
        return false;

    QByteArray parr = prepareMessage(
        "EVMp_CONNECT_" + QString::number(Username.length()) +
        "_" + Username);
    socket->write(parr);

    return socket->waitForBytesWritten();
}


bool Client::sendCurrentMessage(QString message)
{
    if(socket->state() != QAbstractSocket::ConnectedState)
        return false;

    QByteArray parr = prepareMessage("EVMp_SENDMSG_" + message);
    socket->write(parr);

    return socket->waitForBytesWritten();
}

bool Client::sendPrivateMessage(QString message, QString receiver)
{
    if(socket->state() != QAbstractSocket::ConnectedState)
        return false;

    QByteArray parr = prepareMessage(
        "EVMp_PRIVATEMSG_" + QString::number(receiver.length()) +
        "_" + receiver + "_" + message);
    socket->write(parr);

    return socket->waitForBytesWritten();
}







