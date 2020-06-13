#include "authorizationdialog.h"
#include "ui_authorizationdialog.h"
#include "client.h"
#include <QCryptographicHash>

AuthorizationDialog::AuthorizationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthorizationDialog)
{
    ui->setupUi(this);
}

AuthorizationDialog::~AuthorizationDialog()
{
    delete ui;
}

void AuthorizationDialog::SetClient(Client* cl)
{
    c = cl;
}

void AuthorizationDialog::on_pushButton_2_clicked()
{
    QApplication::quit();
}

void AuthorizationDialog::on_pushButton_clicked()
{
  QByteArray arr;
  QDataStream stream (&arr, QIODevice::WriteOnly);
  stream << ui->lineEdit->text();
  QString pass = QString(QCryptographicHash::hash(ui->lineEdit_2->text().toUtf8(),QCryptographicHash::Md5).toHex());
  stream << pass ;
  stream.device()->seek(0);
  c->WriteD(arr);
}
