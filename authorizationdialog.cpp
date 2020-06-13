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
  c->requestNewConnection(ui->lineEdit->text());
}
