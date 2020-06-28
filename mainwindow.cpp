#include "mainwindow.h"
#include "client.h"
#include "ui_mainwindow.h"
#include <QStringListModel>
#include <QKeyEvent>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QMediaPlayer>
#include <QMediaPlaylist>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new QStandardItemModel();
    ui->listView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setName(QString name)
{
    ui->label->setText(name);
}

void MainWindow::displayMessage(QString Message, QString Sender)
{
    QStandardItem *itemM = new QStandardItem(Message);
    QStandardItem *itemS = new QStandardItem(Sender);
    itemS->setForeground(Qt::red);
    model->appendRow(itemS);
    model->appendRow(itemM);
}

void MainWindow::displayPrivateMessage(QString Message, QString Sender)
{
    QStandardItem *itemM = new QStandardItem(Message);
    QStandardItem *itemS;
    if (ui->label->text() == Sender)
        itemS = new QStandardItem("You Whisper to " + WhisperRec);
    else
        itemS = new QStandardItem(Sender + " Whispers");
    itemS->setForeground(Qt::magenta);
    itemM->setForeground(Qt::magenta);
    model->appendRow(itemS);
    model->appendRow(itemM);
}

void MainWindow::onChatEnter(QString name, bool newOne)
{
    ui->listWidget->addItem(name);

    if (newOne)
    {
        QStandardItem *itemS = new QStandardItem(name + " entered the chat");
        itemS->setForeground(Qt::blue);
        model->appendRow(itemS);
    }
}

void MainWindow::onChatExit(QString name)
{
    QList<QListWidgetItem*> items = ui->listWidget->findItems(name, Qt::MatchExactly);
    foreach (QListWidgetItem* item, items)
    {
        delete ui->listWidget->takeItem(ui->listWidget->row(item));
    }

    QStandardItem *itemS = new QStandardItem(name + " exited the chat");
    itemS->setForeground(Qt::blue);
    model->appendRow(itemS);
}

void MainWindow::setClient(Client *cl)
{
    c = cl;
}

void MainWindow::on_pushButton_clicked()
{
    if(!ui->lineEdit->text().isEmpty())
    {
        c->sendCurrentMessage(ui->lineEdit->text());
        ui->lineEdit->clear();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if( (event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return))
        on_pushButton_clicked();
}

void MainWindow::callRequestDisplay(QString name)
{
    QDialog *d = new QDialog(this);
    QVBoxLayout *vbox = new QVBoxLayout();
    d->setMinimumSize(200,200);
    QLabel * labelA = new QLabel();
    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                        | QDialogButtonBox::Cancel, d);

    vbox->addWidget(labelA);
    vbox->addWidget(buttonBox);
    d->setLayout(vbox);
    labelA->setText("You've got a call from " + name);
    connect(buttonBox, &QDialogButtonBox::accepted, d, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, d, &QDialog::reject);
    QMediaPlaylist *playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl(QUrl::fromLocalFile("sound/PerfectCall.mp3")));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    QMediaPlayer *music = new QMediaPlayer();
    music->setPlaylist(playlist);
    music->play();
    int result = d->exec();
    if(result == QDialog::Accepted)
    {
     music->stop();
     c->callAccepted(name);
     d->close();
    }
    if(result == QDialog::Rejected)
    {
       c->callRejected(name);
       music->stop();
       d->close();
    }
}

void MainWindow::setVoiceName(QString name)
{
    VoiceBuddy = name;
    ui->label_2->setText("Voice chat with: " + VoiceBuddy);
    ui->Call_Button->setText("Stop");
}

void MainWindow::callStopped(QString name)
{
    QMessageBox msg;
    msg.setText("Voice session with " + name + " has ended.");
    VoiceBuddy = "";
    bCall = false;
    ui->Call_Button->setText("Call");
    ui->label_2->setText("Voice chat with: None");
    msg.exec();
}

void MainWindow::on_pushButton_2_clicked()
{
    if(!ui->lineEdit->text().isEmpty() && ui->listWidget->selectedItems().size() != 0)
    {
        QString Reciever = ui->listWidget->currentItem()->text();
        WhisperRec = Reciever;
        c->sendPrivateMessage(ui->lineEdit->text(), Reciever);
        ui->lineEdit->clear();
    }

}

void MainWindow::on_Call_Button_clicked()
{
    if (!bCall)
    {
       // c->makeCall(ui->lineEdit_2->text());
        QString Reciever = ui->listWidget->currentItem()->text();
        c->requestCall(Reciever);
    }
    else
    {
        c->callRejected(VoiceBuddy);
        VoiceBuddy = "";
        c->stopCall();
        ui->Call_Button->setText("Call");
        bCall = false;

    }

}
