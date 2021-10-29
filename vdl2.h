#ifndef VDL2_H
#define VDL2_H

#include <QMainWindow>
#include <QSerialPort>
#include <QUdpSocket>
#include "sqpvdl.h"
#include "vesion.h"
#include "ratio_vdl.h"


namespace Ui {
class vdl2;
}

class vdl2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit vdl2(QWidget *parent = nullptr);
    ~vdl2();

private slots:
    void on_actionClose_triggered();

    void on_actionClear_triggered();

    void on_actionPARAM_REQ_triggered();

    void on_actionPWR_SET_REQ_triggered();

    void on_actionMODE_SET_REQ_triggered();

    void on_actionPURGE_REQ_triggered();

    void on_actionUNIT_DATA_REQ_triggered();

    void on_actionPARAM_REQ_2_triggered();

    void on_actionMODE_SET_REQ_2_triggered();

    void on_actionSQP_triggered();

    void on_pushButton_clicked();




    void on_actionVersion_triggered();

    void on_actionTransmission_ratio_triggered();

private:
    Ui::vdl2 *ui;
    QSerialPort *serialVDL;
    QSerialPort *serialVDLmode;
    QSerialPort *serialVDLfreq;


    QUdpSocket *sockUdp;
    QUdpSocket *sockUdpval;
    QUdpSocket *sendunitdataind;




    QPlainTextEdit receivehexa;
    QByteArray  receivebuffer ;
    QTimer *m_timer;


    QByteArray bufferparam;
    QByteArray bufferpwrvdl;
    QByteArray buffermodeE;
    QByteArray bufferdataE;

    QByteArray bufferparamR;
    QByteArray buffermodeR;

    QByteArray buffer1;

    sqpvdl sqpvdl;
    vesion vesion;
    ratio_vdl  ratio_vdl;



};

#endif // VDL2_H
