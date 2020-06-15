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

    void UpdateNames(QList<QString> names);
    void SetClient(Client* cl);
    void SetName(QString name);
    void DisplayMessage(QString message, QString sender);
    void DisplayPrivateMessage(QString message, QString sender);
    void DisplayChatEnter(QString name);
    void DisplayChatExit(QString name);
    void keyPressEvent(QKeyEvent *event);

    QStandardItemModel *model;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    Client* c;
    QString WhisperRec;
};
#endif // MAINWINDOW_H
