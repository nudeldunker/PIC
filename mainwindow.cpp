#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>



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

    //BlablaBlub
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
                    //int k_long= m_CmdList[m_CmdList.size()-1] & 0b0000011111111111;


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

//MainWindow::getParameters(int idx)
//{
//    int k_long= m_CmdList[idx] & 0b0000011111111111;

//}

MainWindow::decodeCmd()
{


    for( int i=0; i<m_CmdList.size(); i++)
    {
     int k_long=m_CmdList[i] & 0x7FF;
     int k=m_CmdList[i] & 0xFF;
     int f=m_CmdList[i] & 0x7F;
     int d=m_CmdList[i] & 0x80;
     int l=d;
     int b=m_CmdList[i] & 0x380;

     int ByteCmd=m_CmdList[i] & 0x3F00;

     if(ByteCmd == 0x0700 )
        qDebug() << "ADDWF" <<i; //ADDWF(d,f);
     else if(ByteCmd == 0x0500)
         qDebug() << "ANDWF(d,f)" << i;


    if((m_CmdList[i] & 0x3800 ) == 0x2800)
        qDebug()<< "GOTO " << i;
    }
}

void MainWindow::on_pushButton_clicked()
{
    decodeCmd();
}
