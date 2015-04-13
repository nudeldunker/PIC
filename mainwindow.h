#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    reset();
private slots:
    loadFile();
private:
    Ui::MainWindow *ui;
    QList<int> m_CmdList;
    QList<QString> m_FileText;
};

#endif // MAINWINDOW_H
