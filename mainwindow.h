#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pic.h"
#include <QThread>
#include <QTimer>

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

    QTimer * UpdateRegTimer;
public slots:
    void setView(RegModel *regModel, RegModelDlgt *regModelDlgt);
private slots:
    void loadFile();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    //QList<int> m_CmdList;
    QList<QString> m_FileText;
};

#endif // MAINWINDOW_H
