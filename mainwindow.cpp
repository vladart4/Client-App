#include "mainwindow.h"
#include "client.h"
#include "ui_mainwindow.h"
#include <QStringListModel>
#include <QKeyEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    ui->listWidget_2->addItem(Sender);
    //ui->listWidget_2->item(0)->setForeground(Qt::red);
    ui->listWidget_2->addItem(Message);
    for (int i=0; i< ui->listWidget_2->count(); i += 2)
    {
        ui->listWidget_2->item(i)->setForeground(Qt::red);
    }

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
