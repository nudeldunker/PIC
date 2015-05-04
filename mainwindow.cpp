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

    UpdateRegTimer=new QTimer(this);

    qRegisterMetaType<regs>("regs");

    pic=new PIC();
    thread=new QThread();
    pic->moveToThread(thread);


    connect(thread, SIGNAL(started()), pic, SLOT(init()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(start()));
    connect(ui->pushButton, SIGNAL(clicked()), pic, SLOT(runCode()));
    connect(pic, SIGNAL(pointer()), this, SLOT(setView()));
    connect(UpdateRegTimer, SIGNAL(timeout()), this, SLOT(updateReg()));
    connect(this, SIGNAL(aboutToQuit()), pic, SLOT(finish()));
    connect(pic, SIGNAL(finished()), thread, SLOT(quit()));
    connect(pic, SIGNAL(finished()), pic, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    connect(ui->pushButtonStop, SIGNAL(clicked(bool)), this, SLOT(stop()));

    connect(ui->pushButtonSingleStep, SIGNAL(clicked()), this, SLOT(singleStep()));

    connect(thread, SIGNAL(finished()), pic, SLOT(deleteLater()));

    thread->start();

    codeModel=new CodeModel(this);
}



void MainWindow::stop()
{
    pic->stop=true;

    ui->pushButton->setEnabled(true);
    ui->pushButtonStop->setEnabled(false);
}

void MainWindow::start()
{
    pic->stop=false;

    ui->pushButton->setEnabled(false);
    ui->pushButtonStop->setEnabled(true);
}

void MainWindow::singleStep()
{
    pic->singleStep=true;
    pic->runCode();
}

void MainWindow::updateReg()
{
    static int i=0;
    pic->regModel->dataChanged(pic->regModel->index(0,0,QModelIndex()),pic->regModel->index(pic->regModel->rowCount()-1, pic->regModel->columnCount()-1,QModelIndex()));
    qDebug()<< "Update" << i;

    ui->codeView->selectRow(codeModel->findPCIdx(pic->getPC()));
    i++;
}

void MainWindow::setView()
{
//    regModel=pic->regModel;
//    regModelDlgt= pic->regModelDlgt;
    ui->regView->setModel(pic->regModel);
    ui->regView->setItemDelegate(pic->regModelDlgt);
    ui->regView->resizeColumnsToContents();
    UpdateRegTimer->start(1000); //Einmal die Sekunde updaten

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadFile()
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
                QString PCounter= line.mid(0,4);
                QString Cmd=line.mid(5,4);
                QString Rest=line.mid(5, 500);
                Cmd.remove(" ");
                if(!Cmd.isEmpty())
                {
                    pic->m_CmdList.append(Cmd.toInt(0, 16)); //base=16
                    //int k_long= m_CmdList[m_CmdList.size()-1] & 0b0000011111111111;
                }

                codeModel->code.push_back(QStringList()<< "" <<PCounter << Rest);

                //m_FileText.append(line);
            }
        }

        geladen=true;
        ui->codeView->setModel(codeModel);
        ui->codeView->resizeColumnsToContents();
    }


}

void MainWindow::reset()
{
    pic->m_CmdList.clear();
   // m_FileText.clear();
}

//MainWindow::getParameters(int idx)
//{
//    int k_long= m_CmdList[idx] & 0b0000011111111111;

//}

//void PIC::decodeCmd()
//{


//    for( int i=0; i<m_CmdList.size(); i++)
//    {
//     int k_long=m_CmdList[i] & 0x7FF;
//     int k=m_CmdList[i] & 0xFF;
//     int f=m_CmdList[i] & 0x7F;
//     int d=m_CmdList[i] & 0x80;
//     int l=d;
//     int b=m_CmdList[i] & 0x380;

//     int ByteCmd=m_CmdList[i] & 0x3F00;
//     int BitCmd=m_CmdList[i] & 0x3C00;
//     int ShrtCmd=m_CmdList[i] & 0x3800;

//     if(ByteCmd == 0x0700 )
//        qDebug() << "ADDWF" <<i; //ADDWF(f,d);
//     else if(ByteCmd == 0x0500)
//         qDebug() << "ANDWF(f,d)" << i;
//     else if(ByteCmd == 0x0100)
//         CLRF(f);
//     else if((m_CmdList[i] & 0x3F80) == 0x0100)
//         CLRW();
//     else if(ByteCmd == 0x0900)
//         COMF(f,d);
//     else if(ByteCmd == 0x0300)
//         DECF(f,d);
//     else if(ByteCmd == 0x0B00)
//         DECFSZ(f,d);
//     else if(ByteCmd == 0x0A00)
//         INCF(f,d);
//     else if(ByteCmd == 0x0F00)
//         INCFSZ(f,d);
//     else if(ByteCmd == 0x0400)
//         IORWF(f,d);
//     else if(ByteCmd == 0x0800)
//         MOVF(f,d);
//     else if(ByteCmd == 0x0000)
//         MOVWF(f);
//     else if((m_CmdList[i] & 0x3F9F) == 0x0000)
//         NOP();
//     else if(ByteCmd == 0x0D00)
//         RLF(f,d);
//     else if(ByteCmd == 0x0C00)
//         RRF(f,d);
//     else if(ByteCmd == 0x0200)
//         SUBWF(f,d);
//     else if(ByteCmd == 0x0E00)
//         SWAPF(f,d);
//     else if(ByteCmd == 0x0600)
//         XORWF(f,d);

//     else if(BitCmd == 0x1000)
//         BCF(f,b);
//     else if(BitCmd == 0x1400)
//         BSF(f,b);
//     else if(BitCmd == 0x1800)
//         BTFSC(f,b);
//     else if(BitCmd == 0x1C00)
//         BTFSS(f,b);

//     else if((m_CmdList[i] & 0x3E00 ) == 0x3E00)
//        ADDLW(k);
//     else if(ByteCmd == 0x3900)
//         ANDLW(k);
//     else if(ShrtCmd == 0x2000)
//         CALL(k_long);
//     else if((m_CmdList[i]& 0XFFFF) == 0x0064)
//         CLRWDT();
//     else if(ShrtCmd == 0x2800)
//         GOTO(k_long);

//    if((m_CmdList[i] & 0x3F00 ) == 0x3A00)
//        qDebug()<< "XORLW" << i;

//    if((m_CmdList[i] & 0x3E00 ) == 0x3C00)
//        qDebug()<< "SUBLW" << i;

//    if((m_CmdList[i] & 0x3F00 ) == 0x3C00)
//        qDebug()<< "SUBLW" << i;

//    if((m_CmdList[i] & 0xFFFF ) == 0x0063)
//        qDebug()<< "SLEEP" << i;

//    if((m_CmdList[i] & 0xFFFF ) == 0x0008)
//        qDebug()<< "RETURN" << i;

//    if((m_CmdList[i] & 0x3C00 ) == 0x3400)
//        qDebug()<< "RETURNLW" << i;

//    if((m_CmdList[i] & 0xFFFF ) == 0x0009)
//        qDebug()<< "RETURNFIE" << i;

//    if((m_CmdList[i] & 0x3F00 ) == 0x3000)
//        qDebug()<< "MOVLW" << i;

//    if((m_CmdList[i] & 0x3F00 ) == 0x3100)
//        qDebug()<< "MOVLW" << i;

//    if((m_CmdList[i] & 0x3F00 ) == 0x3200)
//        qDebug()<< "MOVLW" << i;

//    if((m_CmdList[i] & 0x3F00 ) == 0x3300)
//        qDebug()<< "MOVLW" << i;


//    if((m_CmdList[i] & 0x3800 ) == 0x3800)
//        qDebug()<< "IORLW" << i;
//}
//}

void MainWindow::on_pushButton_clicked()
{

    // pic->decodeCmd();
}

void MainWindow::closeEvent(QCloseEvent *bar)
{
  emit aboutToQuit();
    bar->accept();
}
