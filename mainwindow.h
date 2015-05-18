#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "pic.h"
#include <QThread>
#include <QTimer>
#include "regmodel.h"
#include "regmodeldlgt.h"
#include "codemodel.h"
#include "portmodel.h"
#include "hdwcom.h"


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

    PortModel *portA;
    PortModel *portB;

    CodeModel *codeModel;

    QTimer * UpdateRegTimer;
    void getComPorts();

    HDWCom * Com=0;
public slots:
    void setView();
    void stop();
    void updateReg();
    void start();
    void singleStep();
    void setPortA(const QModelIndex &index);
    void setPortB(const QModelIndex &index);
    void openHelp();
    void startHDWCom();
    void stopHDWCom();
    void setHDWData();
    void setPorts();
private slots:
    void loadFile();
    void on_pushButton_clicked();

    void on_inputtest_returnPressed();

    void on_Quarzfrequenz_returnPressed();

    void on_taktstate_pressed();

    void on_taktbank_returnPressed();

    void on_taktregister_returnPressed();

    void on_taktbit_returnPressed();

    void update_laufzeit();

    void update_stack0();
    void update_stack1();
    void update_stack2();
    void update_stack3();
    void update_stack4();
    void update_stack5();
    void update_stack6();
    void update_stack7();
    void on_resetLaufzeit_clicked();

    void on_resetLZ_clicked();

signals:
    void aboutToQuit();

private:
    Ui::MainWindow *ui;
    //QList<int> m_CmdList;
    //QList<QString> m_FileText;
    void closeEvent(QCloseEvent *bar);
};

#endif // MAINWINDOW_H
