#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "pic.h"
#include <QThread>
#include <QTimer>
#include "regmodel.h"
#include "regmodeldlgt.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

   void reset();
   // void decodeCmd();

    PIC *pic;
    QThread *thread;

    RegModel *regModel;
    RegModelDlgt *regModelDlgt;

    QTimer * UpdateRegTimer;
public slots:
    void setView();
    void stop();
    void updateReg();
    void start();
    void singleStep();
private slots:
    void loadFile();
    void on_pushButton_clicked();

signals:
    void aboutToQuit();

private:
    Ui::MainWindow *ui;
    //QList<int> m_CmdList;
    QList<QString> m_FileText;
    void closeEvent(QCloseEvent *bar);
};

#endif // MAINWINDOW_H
