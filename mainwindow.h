#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pic.h"

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
private slots:
    void loadFile();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    //QList<int> m_CmdList;
    QList<QString> m_FileText;
};

#endif // MAINWINDOW_H
