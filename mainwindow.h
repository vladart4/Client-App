#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Client;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void UpdateNames(QList<QString> names);
    void SetClient(Client* cl);
    void SetName(QString name);
    void DisplayMessage(QString Message, QString Sender);
    void keyPressEvent(QKeyEvent *event);


private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Client* c;
};
#endif // MAINWINDOW_H
