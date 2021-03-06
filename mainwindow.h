#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

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

    void setClient(Client* cl);
    void setName(QString name);
    void displayMessage(QString message, QString sender);
    void displayPrivateMessage(QString message, QString sender);
    void onChatEnter(QString name, bool newOne);
    void onChatExit(QString name, bool atWill);
    void keyPressEvent(QKeyEvent *event);
    void callRequestDisplay(QString name);
    bool bCall = false;
    void setVoiceName(QString name);
    void callStopped(QString name);

    QStandardItemModel *model;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_Call_Button_clicked();

private:
    Ui::MainWindow *ui;
    Client* c;
    QString WhisperRec;
    QString VoiceBuddy;

};
#endif // MAINWINDOW_H
