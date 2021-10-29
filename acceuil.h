#ifndef ACCEUIL_H
#define ACCEUIL_H


#include "sqpacars.h"
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include "vesion.h"
#include <QUdpSocket>
#include "ratio_tran.h"

QT_BEGIN_NAMESPACE
namespace Ui { class acceuil; }
QT_END_NAMESPACE

class acceuil : public QMainWindow
{
    Q_OBJECT

public:
    acceuil(QWidget *parent = nullptr);
    ~acceuil();

private slots:

    void on_actionPARAM_REQ_triggered();

//    void on_pushButton_clicked();

    void on_actionPWR_SET_REQ_triggered();

    void on_actionMODE_SET_REQ_2_triggered();

    void on_actionPARAM_REQ_2_triggered();

    void on_actionMODE_SET_REQ_triggered();

    void on_actionUNIT_DATA_REQ_triggered();

    void on_actionPURGE_REQ_triggered();

    void on_actionClose_triggered();

    void on_actioncLEAR_triggered();

    void on_actionVersion_triggered();

    void on_actionSQP_triggered();

    void on_actionTransmission_Ratio_triggered();



private:
    Ui::acceuil *ui;
    QSerialPort *serial;
    QSerialPort *serialReceiver;
    sqpAcars sqpacars;
    QUdpSocket *sockUdp;
    QUdpSocket *ratio;
    vesion vesion;
    ratio_tran ratio_tran;
    QByteArray buffersqp;
    QByteArray buffersqppwr;
    QByteArray buffersqpmode;
    QByteArray bufferdata_req;
    QByteArray bufferparam_reqR;
    QByteArray buffermode_Req;
    QByteArray buffer;






};
#endif // ACCEUIL_H
