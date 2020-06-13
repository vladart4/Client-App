#include "mainwindow.h"
#include "client.h"
#include "ui_mainwindow.h"
#include <QStringListModel>
#include <QKeyEvent>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QStandardItem>

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

void MainWindow::UpdateNames(QList<QString> names)
{
    ui->listWidget->clear();
    foreach (QString c, names)
    {
        ui->listWidget->addItem(c);

    }
}

void MainWindow::SetName(QString name)
{
    ui->label->setText(name);
}

void MainWindow::DisplayMessage(QString Message, QString Sender)
{

    QStandardItem *itemM = new QStandardItem(Message);
    QStandardItem *itemS = new QStandardItem(Sender);
    itemS->setForeground(Qt::red);
    model->appendRow(itemS);
    model->appendRow(itemM);

}

void MainWindow::DisplayPrivateMessage(QString Message, QString Sender)
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

void MainWindow::SetClient(Client *cl)
{
    c = cl;
}

void MainWindow::on_pushButton_clicked()
{
 if(!ui->lineEdit->text().isEmpty())
 {
    c->SendCurrentMessage(ui->lineEdit->text());
    ui->lineEdit->clear();
 }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if( (event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return))
            on_pushButton_clicked();

}

void MainWindow::on_pushButton_2_clicked()
{
    if(!ui->lineEdit->text().isEmpty() && ui->listWidget->selectedItems().size() != 0)
    {
        QString Reciever = ui->listWidget->currentItem()->text();
        WhisperRec = Reciever;
        c->SendPrivateMessage(ui->lineEdit->text(), Reciever);
        ui->lineEdit->clear();
    }

}
