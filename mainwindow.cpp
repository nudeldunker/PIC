#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QTextStream>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionLaden, SIGNAL(triggered()), this, SLOT(loadFile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow::loadFile()
{
    static bool geladen=false;

    if(geladen)
    {
        reset();
    }

    QString LoadFile=QFileDialog::getOpenFileName(this,"Open existing file", QDir::currentPath(), "LST-Dateien (*.lst)" );

    if(!LoadFile.isEmpty())
    {
        QFile file(LoadFile);
        if(file.open( QFile::ReadOnly)) //mehr Flags
        {
            QTextStream StreamIn (&file);
            while (!StreamIn.atEnd())
            {
                QString line= StreamIn.readLine();
                QString Cmd=line.mid(5,4);
                Cmd.remove(" ");
                if(!Cmd.isEmpty())
                {
                    m_CmdList.append(Cmd.toInt(0, 16)); //base=16
                }

                m_FileText.append(line);
            }
        }

        geladen=true;
    }


}

MainWindow::reset()
{
    m_CmdList.clear();
    m_FileText.clear();
}
