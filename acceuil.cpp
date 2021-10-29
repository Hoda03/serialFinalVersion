#include "acceuil.h"
#include "ui_acceuil.h"
#include <QSerialPort>
#include <QDebug>
#include <QMessageBox>
#include <QObject>
#include <QComboBox>
#include <QList>
#include <QSerialPortInfo>
#include "sqpacars.h"


QSerialPort *serial;
#define SIZE_SYNCHRO_SL     4
const uint8_t synchrovdl2[SIZE_SYNCHRO_SL] = {0xff,0x00,0xf0,0xca};


int counter =0;
int counter_unit_data_conf =0;
int counterstream =0;
int counterstreampwr =0;
int counterstreammodeemetteur =0;
int counterunitdatareq =0;
int counterparamreq_R =0;
int countermode_reqR  =0;
int countersqp =0;
int ct =1;

bool isBoolPARAM_REQ {false};
bool isBoolPWR_SET_REQ {false};
bool isBoolMode_SET {false};
bool isBoolUnit_data_req {false};
bool isBoolPARAM_REQ_Re {false};
bool isBoolMode_req_R {false};



struct tsPid20 {

    uint8_t synchro[SIZE_SYNCHRO_SL];
    uint8_t protocoleCode;
    uint8_t pid;
    uint8_t lengthMsb;
    uint8_t lengthLsb;
    uint8_t freq_lsb;
    uint8_t freq_msb;
    uint8_t mod;
    uint8_t prekey;
    uint16_t crc;
};


acceuil::acceuil(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::acceuil)
{
   ui->setupUi(this);
   this->setWindowTitle("ACARS");
   this->setStyleSheet("QMainWindow { background-color : #ffffff}");
   ui->logo->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
   ui->logo->setStyleSheet("QLabel { background-color : white; color : #0078d7; }");
   ui-> logo->setLineWidth(3);
   ui-> logo->setMidLineWidth(3);

   serial = new QSerialPort(this);
   serial->setPortName("COM7");

   sockUdp = new QUdpSocket(this);
   sockUdp->bind(QHostAddress::LocalHost,1111);

   ratio = new QUdpSocket(this);
   ratio->bind(QHostAddress::LocalHost,2222);

 if (serial->open(QIODevice::ReadWrite))
   {

       qDebug() << "Connected";
   }
   else
   {
       qDebug() << "Error";

   }

   serialReceiver = new QSerialPort(this);
   serialReceiver->setPortName("COM5");

   if (serialReceiver->open(QIODevice::ReadWrite))
   {

       qDebug() << "Connected";
   }
   else
   {
       qDebug() << "Error";

   }




   QList<QSerialPortInfo> list;
   list = QSerialPortInfo::availablePorts();

   ui->transmitter->addItem(QStringLiteral("COM7"));
   ui ->receiver->addItem(QStringLiteral("COM5"));

   //Set baud rate
   ui->baudrate->addItem(QStringLiteral("BaudRate"));
   ui->baudrate->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
   ui->baudrate->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
   ui->baudrate->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
   ui->baudrate->addItem(QStringLiteral("57600"),QSerialPort::Baud57600);
   ui->baudrate->addItem(QStringLiteral("115200"),QSerialPort::Baud115200);

   ui->baudratte_R->addItem(QStringLiteral("BaudRate"));
   ui->baudratte_R->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
   ui->baudratte_R->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
   ui->baudratte_R->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
   ui->baudratte_R->addItem(QStringLiteral("57600"),QSerialPort::Baud57600);
   ui->baudratte_R->addItem(QStringLiteral("115200"),QSerialPort::Baud115200);

   // set data bits
   ui->comboBox_dataBits->addItem(QStringLiteral("5"), QSerialPort::Data5);
   ui->comboBox_dataBits->addItem(QStringLiteral("6"), QSerialPort::Data6);
   ui->comboBox_dataBits->addItem(QStringLiteral("7"), QSerialPort::Data7);
   ui->comboBox_dataBits->addItem(QStringLiteral("8"), QSerialPort::Data8);
   ui->comboBox_dataBits->setCurrentIndex(3);


   //Set stop bit
   ui->comboBox_stop->addItem(QStringLiteral("1"), QSerialPort::OneStop);
   ui->comboBox_stop->addItem(QStringLiteral("2"), QSerialPort::TwoStop);


/*   unsigned char allstrings[sizeof(myData1)+sizeof(myData3)];
   memcpy(allstrings,myData1,sizeof(myData1));
   memcpy(allstrings+sizeof(myData1),myData3,sizeof(myData3));*/

   connect(serial,&QSerialPort::readyRead,[&](){

       qDebug() << "New data available: " << serial->bytesAvailable();
       QByteArray datas = serial->readAll();
       QByteArray ba_as_hex_string = datas.toHex();
       qDebug() << datas;
       while(serial->waitForReadyRead(100)) {

           if(isBoolPARAM_REQ){

               if(datas.length() == 14 && (datas[5] == 80) && (datas[8] >= 18)){

                   int PARAM_REQ = ui-> frequence -> value();
                   ui-> plainTextEdit_2-> appendPlainText(QString("PARAM_CONF_TX = %1").arg(PARAM_REQ));
                   ui->plainTextEdit_4-> appendPlainText(ba_as_hex_string);


               }
               else if(datas.length() != 14 ){
                          counterstream ++;
                          if( counterstream == 1 ){

                              buffersqp = datas;
                              qDebug() << " data : " << buffersqp;
                          }
                        else   if( counterstream > 1 )
                          {

                            buffersqp = buffersqp.append(datas);
                            qDebug() << "new data : " << buffersqp;


                            if(buffersqp.length() == 14&& (buffersqp[5] == 80) && (buffersqp[8] >= 18)){
                             int PARAM_REQ = ui-> frequence -> value();
                             ui-> plainTextEdit_2-> appendPlainText(QString("PARAM_CONF_TX = %1").arg(PARAM_REQ));
                             QByteArray ba_as_hex_string = buffersqp.toHex();
                             ui->plainTextEdit_4-> appendPlainText(ba_as_hex_string);
                         }
                       }


                }
           }
           if(isBoolPWR_SET_REQ){

               if(datas.length() == 12 && ((datas[8] == 47) || (datas[8] == 44) || (datas[8] == 40)) && ( datas[9] == 1)){

                   int PWR_SET_REQ = ui -> puissance -> value();
                   ui->plainTextEdit_2->appendPlainText(QString("PWR_SET_CONF = %1").arg(PWR_SET_REQ));
                   QByteArray ba_as_hex_string = datas.toHex();
                   ui->plainTextEdit_4-> appendPlainText(ba_as_hex_string);


                }
              else  if(datas.length() != 12 ){
                          counterstreampwr ++;
                          if( counterstreampwr == 1 ){

                              buffersqppwr = datas;
                              qDebug() << " data : " << buffersqppwr;
                          }
                          else if( counterstreampwr > 1 )
                          {

                            buffersqppwr = buffersqppwr.append(datas);
                            qDebug() << "new data : " << buffersqppwr;

                          if(buffersqppwr.length() == 12 && ((buffersqppwr[8] == 47) || (buffersqppwr[8] == 44) || (buffersqppwr[8] == 40)) && ( buffersqppwr[9] == 1)){

                              int PWR_SET_REQ = ui -> puissance -> value();
                              ui->plainTextEdit_2->appendPlainText(QString("PWR_SET_CONF = %1").arg(PWR_SET_REQ));
                              QByteArray ba_as_hex_string = buffersqppwr.toHex();
                              ui->plainTextEdit_4-> appendPlainText(ba_as_hex_string);


                           }}


                }


           }
           if(isBoolMode_SET){

               if(datas.length() == 12 && datas[9] == 1 && datas[11] == 93){

                   ui->plainTextEdit_2->appendPlainText(QString("MODE_SET_CONF_TX"));
                   ui->  plainTextEdit_4->appendPlainText(ba_as_hex_string); //Affichage des trames en héxadécimal

                }

               else if(datas.length() != 12 ){
                   counterstreammodeemetteur++;
                          if( counterstreammodeemetteur == 1 ){

                              buffersqpmode = datas;
                              qDebug() << " data : " << buffersqpmode;
                          }
                          else if( counterstreammodeemetteur > 1 )
                          {

                            buffersqpmode = buffersqpmode.append(datas);
                            qDebug() << "new data : " << buffersqpmode;

                          if(buffersqpmode.length() == 12 && buffersqpmode[9] == 1 && buffersqpmode[11] == 93){

                              ui->plainTextEdit_2->appendPlainText(QString("MODE_SET_CONF_TX"));
                              ui->  plainTextEdit_4->appendPlainText(ba_as_hex_string); //Affichage des trames en héxadécimal

                           }}


                }


           }
           if(isBoolUnit_data_req){

               if(datas.length() == 11 && datas[5] == 85 ){

                   ui->plainTextEdit_2->appendPlainText(QString("UNIT_DATA_CONF"));
                   ui->  plainTextEdit_4->appendPlainText(ba_as_hex_string); //Affichage des trames en héxadécimal

                }
               else if(datas.length() != 11 ){
                   counterunitdatareq++;
                          if( counterunitdatareq == 1 ){

                              bufferdata_req = datas;
                              qDebug() << " data : " << bufferdata_req;
                          }
                          if( counterunitdatareq > 1 )
                          {

                            bufferdata_req = bufferdata_req.append(datas);
                            qDebug() << "new data : " << bufferdata_req;

                            if(bufferdata_req.length() == 11 && bufferdata_req[9] == 1 && bufferdata_req[11] == 93){

                              ui->plainTextEdit_2->appendPlainText(QString("MODE_SET_CONF_TX"));
                              ui->  plainTextEdit_4->appendPlainText(ba_as_hex_string); //Affichage des trames en héxadécimal

                           }}



                }

           }
     }

           isBoolPWR_SET_REQ = false;
           isBoolPARAM_REQ = false;
           isBoolMode_SET = false;
           isBoolUnit_data_req = false;
           isBoolPARAM_REQ_Re = false;
           isBoolMode_req_R = false;

           bufferdata_req.clear();
           buffersqppwr.clear();
           buffersqp.clear();
           buffersqpmode.clear();


           counterunitdatareq=0;
           counterstreammodeemetteur=0;
           counterstream=0;
           counterstreampwr =0;

        });


   connect(serialReceiver,&QSerialPort::readyRead,[&](){

       qDebug() << "New data available: " << serialReceiver->bytesAvailable();
       QByteArray datas = serialReceiver->readAll();
       qDebug() << datas;
       //Affichage des trames en héxadécimal
       QByteArray ba_as_hex_string = datas.toHex();
       while(serialReceiver->waitForReadyRead(100)) {




       if(isBoolPARAM_REQ_Re) {

           if(datas.length() == 14 && (datas[5] == 80) && (datas[8] >= 18)){

               int PARAM_REQ = ui-> frequence -> value();
               ui-> plainTextEdit_2-> appendPlainText(QString("PARAM_CONF_RX = %1").arg(PARAM_REQ));
               ui->plainTextEdit_4-> appendPlainText(ba_as_hex_string);

           }

           else if(datas.length() != 14 ){
                      counterparamreq_R ++;
                      if( counterparamreq_R == 1 ){

                          bufferparam_reqR = datas;
                          qDebug() << " data param : " << bufferparam_reqR;
                      }
                     else if( counterparamreq_R > 1 )
                      {

                        bufferparam_reqR = bufferparam_reqR.append(datas);
                        qDebug() << "new data : " << bufferparam_reqR;


                             if(bufferparam_reqR.length() == 14&& (bufferparam_reqR[5] == 80) && (bufferparam_reqR[8] >= 18)){
                                 int PARAM_REQ = ui-> frequence -> value();
                                 ui-> plainTextEdit_2-> appendPlainText(QString("PARAM_CONF_TX = %1").arg(PARAM_REQ));
                                 QByteArray ba_as_hex_string = buffersqp.toHex();
                                 ui->plainTextEdit_4-> appendPlainText(ba_as_hex_string);
                             }

                      }


            }

       }
       else if(isBoolMode_req_R == true)
       {

           if(datas.length() == 12 && datas[9] == 1 && datas[11] == 93){
               ui->plainTextEdit_2->appendPlainText(QString("MODE_SET_CONF_RX"));
               ui->  plainTextEdit_4->appendPlainText(ba_as_hex_string); //Affichage des trames en héxadécimal

            }
           else{
               countermode_reqR++;
                      if( countermode_reqR == 1 ){

                          buffermode_Req = datas;
                          qDebug() << " data : " << buffermode_Req;
                      }
                      if( countermode_reqR > 1 )
                      {

                        buffermode_Req = buffermode_Req.append(datas);
                        qDebug() << "new data : " << buffermode_Req;

                     }
                      if(buffermode_Req.length() == 12 && buffermode_Req[9] == 1 && buffermode_Req[11] == 93){
                          ui->plainTextEdit_2->appendPlainText(QString("MODE_SET_CONF_RX"));
                          ui->  plainTextEdit_4->appendPlainText(ba_as_hex_string); //Affichage des trames en héxadécimal

                       }

            }



       }


       else if( datas.length() ==  ui->data->value() + 40 && datas[5] == 81 ){

             ui->plainTextEdit_2->appendPlainText(QString("UNIT DATA IND"));
             ui-> plainTextEdit_4->appendPlainText (datas.toHex());
             ratio->writeDatagram(datas,QHostAddress::LocalHost, 5555);


        }
       else if( datas.length() ==  10  && datas[4] == 95 ){

             ui->plainTextEdit_2->appendPlainText(QString("SQP_IND"));
             ui-> plainTextEdit_4->appendPlainText (datas.toHex());
             sockUdp->writeDatagram(datas,QHostAddress::LocalHost, 2021);


       }


      else {

           if(ct ==1){

               ui-> plainTextEdit_4->appendPlainText ("");
               buffer = datas;
               qDebug() << "data : "<<buffer;



           }

           if(ct > 1 ) {
               buffer = buffer.append(datas);
               qDebug() << "counter : "<<ct;
               qDebug() << "data1 : "<<buffer;



               if(buffer[4] == 95 && buffer.length() == ui->data->value() + 50){


                           ui->plainTextEdit_2->appendPlainText(QString("SQP_IND"));
                           ui-> plainTextEdit_4->appendPlainText (buffer.toHex());
                           sockUdp->writeDatagram(buffer,QHostAddress::LocalHost, 2021);

                           ui->plainTextEdit_2->appendPlainText(QString("UNIT DATA IND"));
                           ui-> plainTextEdit_4->appendPlainText (buffer.toHex());
                           ratio->writeDatagram(buffer,QHostAddress::LocalHost, 5555);

               }

               else if(buffer[5] == 81 &&  buffer.length() == ui->data->value() + 50) {


                        ui->plainTextEdit_2->appendPlainText(QString("UNIT DATA IND"));
                        ui-> plainTextEdit_4->appendPlainText (buffer.toHex());
                        ratio->writeDatagram(buffer,QHostAddress::LocalHost, 5555);

                        ui->plainTextEdit_2->appendPlainText(QString("SQP_IND"));
                        ui-> plainTextEdit_4->appendPlainText (buffer.toHex());
                        sockUdp->writeDatagram(buffer,QHostAddress::LocalHost, 2021);

                }
               else if(buffer[5] == 81 &&  buffer.length() == ui->data->value() + 40)
                   {

                        ui->plainTextEdit_2->appendPlainText(QString("UNIT DATA IND"));
                        ui-> plainTextEdit_4->appendPlainText (buffer.toHex());
                        ratio->writeDatagram(buffer,QHostAddress::LocalHost, 5555);

               }

           }
           ct++;


       }
     }
       isBoolPARAM_REQ_Re = false;
       isBoolMode_req_R = false;
       buffer.clear();
       buffermode_Req.clear();
       bufferparam_reqR.clear();
       ct =1;
       countermode_reqR=0;
       counterparamreq_R =0;


   });
}



uint16_t CRCV2(uint8_t *x, uint16_t len) {
    uint8_t *data = x;
    uint16_t crc=0;

    while (len--) {
        crc ^= *data++;
        for (uint8_t k = 0; k < 8; k++)
            if (crc & 1) crc =(crc >> 1) ^ 0x8408;
            else crc = crc >> 1;
    }

    return crc;
}

acceuil::~acceuil()
{
    delete ui;
}


void acceuil::on_actionPARAM_REQ_triggered()
{  
    serial->setBaudRate(QSerialPort::Baud57600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

        /*-----------------------------------------------------------------------------------*/
            // Algo de traitement de la fréquence
            //Il faut créer une liste qui prend la taille de la valeur de fréquence désiré
            //mettre le nombre saisie dans le tableu sous forme de chiffre

            /*-----------------------------------------------------------------------------------------*/


            int T[6];
            int c = 0;
            int msb[2] , lsb[2] ;
            int monnombre = ui->frequence->value();
            int mynumber = 0;
            int p =1 , e = 1 , o = 1;

            /*------------------mettre le nmbre saisie dans un tableau en forme de chiffre---------------------------------*/

            while(monnombre!=0) {
                int mod = monnombre%10;
                T[c] = mod;
                monnombre-=mod;
                monnombre/=10;
                c++;
            }

            /*------------------------Reconversion du tableau en chiffre-----------------------------------------------------*/
           for(int i = 1; i<5 ; i++){
                mynumber =  mynumber + T[i]*p;
                p=p*10;
            }

            msb[0] = T[4] ;
            msb[1] = T[3];

            lsb[0] = T[2];
            lsb[1] = T[1];

            uint8_t var = 0 ;
            for(int k=1;k>=0;k--) {
                    var = var +msb[k]*e ;
                    e = e*10 ;
            }
            uint8_t var2 = 0 ;
            for(int k=1;k>=0;k--) {
                    var2 = var2 +lsb[k]*o ;
                    o = o*10 ;
            }


            uint8_t var4;
            if (ui -> prekey -> value() >= 0 && ui -> prekey -> value() <= 85 ){
                var4 = ui -> prekey -> value();
            }
            else{

                var4 = 37;
            }
            /*-------------------------------Calcul du Crc16 Kermit-------------------------------------*/

            unsigned char myData[] = {0xff, 0x00, 0xf0, 0xca, 0xf2, 0x20, 0x00, 0x04,var,var2,0x00,var4};
            unsigned int myDataLen;
            unsigned short myCrc;
            myDataLen = sizeof(myData);
            myCrc = CRCV2(myData, myDataLen);
            myCrc = ((myCrc << 8) | (myCrc >> 8));

            /*-------------Définition des champs de la trame et son envoie dans un fil------------- */

            if  ( ui -> frequence -> value() >= 118000  &  ui -> frequence -> value() <= 136000 )
                    {
                        tsPid20 Pid20;
                        Pid20.synchro [0] = synchrovdl2[0];
                        Pid20.synchro [1] = synchrovdl2[1];
                        Pid20.synchro [2] = synchrovdl2[2];
                        Pid20.synchro [3] = synchrovdl2[3];
                        Pid20.protocoleCode = 0xF2;
                        Pid20.pid = 0x20;
                        Pid20.lengthMsb =0x00;
                        Pid20.lengthLsb = 0x04;
                        Pid20.freq_msb = var2;
                        Pid20.freq_lsb = var;
                        Pid20.mod = 0x00;
                        Pid20.prekey=var4;
                        Pid20.crc=myCrc;

                        QByteArray buffer;


                        //  ecrire les données dans le flux d'envoie
                        QDataStream s(&buffer, QIODevice::WriteOnly);
                        if (false){

                            s.setByteOrder(QDataStream::LittleEndian);
                        }

                        s << (uint8_t)Pid20.synchro [0]<< (uint8_t)Pid20.synchro [1]<<(uint8_t)Pid20.synchro [2]<<(uint8_t)Pid20.synchro [3]
                        << (uint8_t)Pid20.protocoleCode << (uint8_t)Pid20.pid << (uint8_t)Pid20.lengthMsb <<(uint8_t)Pid20.lengthLsb
                       <<(uint8_t)Pid20.freq_lsb<<(uint8_t)Pid20.freq_msb<<(uint8_t)Pid20.mod<<(uint8_t)Pid20.prekey<<(uint16_t)Pid20.crc;


               serial->write(buffer);
               serial->flush();
               serial->waitForBytesWritten(1000);

               int PARAM_REQ = ui -> frequence -> value();
               ui-> plainTextEdit-> appendPlainText(QString("PARAM_REQ_TX = %1").arg(PARAM_REQ));

               QByteArray ba_as_hex_string = buffer.toHex();
               ui->  plainTextEdit_3->appendPlainText(ba_as_hex_string);
      }
            isBoolPARAM_REQ = true;




}



void acceuil::on_actionPWR_SET_REQ_triggered()
{
    serial->setBaudRate(QSerialPort::Baud57600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);



        /*-----------------------------structure de la PidF1----------------------------------------------------*/

           struct tsPidF1{
               uint8_t synchro[SIZE_SYNCHRO_SL];
               uint8_t protocoleCode;
               uint8_t pid;
               uint8_t lengthMsb;
               uint8_t lengthLsb;
               uint8_t pwr;
               uint8_t req;
               uint16_t crc;

       };
           /*----------------instruction conditionnelle pour l'envoie de la puissance à la Radio------------------------*/


           int pwr = ui -> puissance -> value();
           uint8_t var3;

          if(  pwr == 50 ) {
                var3 = 47;
           }
          if ( pwr == 25){
                var3 = 44;
           }
          if ( pwr == 10) {

                var3 = 40;
           }


          /*-------------Définition des champs de la trame ------------- */



           tsPidF1 PidF1;

           PidF1.synchro [0] = synchrovdl2[0];
           PidF1.synchro [1] = synchrovdl2[1];
           PidF1.synchro [2] = synchrovdl2[2];
           PidF1.synchro [3] = synchrovdl2[3];
           PidF1.protocoleCode = 0xF2;
           PidF1.pid = 0xF1;
           PidF1.lengthMsb =0x00;
           PidF1.lengthLsb = 0x02;
           PidF1.pwr = var3;
           PidF1.req = 0x00;

           /*-------------------------------Calcul du Crc16 Kermit-------------------------------------*/

           unsigned char myData[] = {0xff, 0x00, 0xf0, 0xca, 0xf2, 0xf1, 0x00, 0x02,var3,0x00};
           unsigned int myDataLen;
           unsigned short myCrc;
           myDataLen = sizeof(myData);
           myCrc = CRCV2(myData, myDataLen);
           myCrc = ((myCrc << 8) | (myCrc >> 8));
           PidF1.crc = myCrc;


           /*-------------------Intégration des champs du buffer dans un fil d'envoie--------------------------*/

           QByteArray buffer;

           //  ecrire les données dans le flux d'envoie
           QDataStream s(&buffer, QIODevice::WriteOnly);

           if (false){

               s.setByteOrder(QDataStream::LittleEndian);
           }
            s << (uint8_t)PidF1.synchro [0]<< (uint8_t)PidF1.synchro [1]<<(uint8_t)PidF1.synchro [2]
            <<(uint8_t)PidF1.synchro [3]  << (uint8_t)PidF1.protocoleCode << (uint8_t)PidF1.pid
            << (uint8_t)PidF1.lengthMsb <<(uint8_t)PidF1.lengthLsb<<(uint8_t)PidF1.pwr<<(uint8_t)PidF1.req<<(uint16_t)PidF1.crc;

            serial->write(buffer);
            serial->flush();
            serial->waitForBytesWritten(1000);

            int PWR_SET_REQ = ui -> puissance-> value();
            ui-> plainTextEdit -> appendPlainText(QString("PWR_SET_REQ= %1").arg(PWR_SET_REQ));

            QByteArray ba_as_hex_string = buffer.toHex();
            ui->  plainTextEdit_3->appendPlainText(ba_as_hex_string);
            isBoolPWR_SET_REQ = true;


}


void acceuil::on_actionMODE_SET_REQ_2_triggered()
{

    serial->setBaudRate(QSerialPort::Baud57600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    struct tsPidF0{
         uint8_t synchro[SIZE_SYNCHRO_SL];
         uint8_t protocoleCode;
         uint8_t pid;
         uint8_t lengthMsb;
         uint8_t lengthLsb;
         uint8_t mode;
         uint8_t req;
         uint16_t crc;

     };


     /*-------------Définition des champs de la trame ------------- */

     tsPidF0 PidF0;
     PidF0.synchro [0] = synchrovdl2[0];
     PidF0.synchro [1] = synchrovdl2[1];
     PidF0.synchro [2] = synchrovdl2[2];
     PidF0.synchro [3] = synchrovdl2[3];
     PidF0.protocoleCode = 0xF2;
     PidF0.pid = 0xF0;
     PidF0.lengthMsb =0x00;
     PidF0.lengthLsb = 0x02;
     PidF0.mode = 0xF1;
     PidF0.req = 0x00;


     /*-------------------------------Calcul du Crc16 Kermit-------------------------------------*/

     unsigned char myData[] = {0xff, 0x00, 0xf0, 0xca, 0xf2, 0xf0, 0x00, 0x02,0xf1, 0x00};

     unsigned int myDataLen;
     unsigned short myCrc;
     myDataLen = sizeof(myData);
     myCrc = CRCV2(myData, myDataLen);
     myCrc = ((myCrc << 8) | (myCrc >> 8));
     PidF0.crc=myCrc;


     /*-------------------Intégration des champs du buffer dans un flux d'envoie--------------------------*/

     QByteArray buffer;
     //  ecrire les données dans le flux d'envoie
     QDataStream s(&buffer, QIODevice::WriteOnly);

     if (false){

         s.setByteOrder(QDataStream::LittleEndian);
     }
     s << (uint8_t)PidF0.synchro [0]<< (uint8_t)PidF0.synchro [1]<<(uint8_t)PidF0.synchro [2]<<(uint8_t)PidF0.synchro [3]  << (uint8_t)PidF0.protocoleCode << (uint8_t)PidF0.pid << (uint8_t)PidF0.lengthMsb <<(uint8_t)PidF0.lengthLsb<<(uint8_t)PidF0.mode<<(uint8_t)PidF0.req<<(uint16_t)PidF0.crc;


     /*--------------------------------Envoie des données--------------------------------------*/
     serial->write(buffer);
     serial->flush();
     serial->waitForBytesWritten(1000);

     ui-> plainTextEdit -> appendPlainText(QString("MODE_SET_REQ_TX"));

     QByteArray ba_as_hex_string = buffer.toHex();
     ui->  plainTextEdit_3->appendPlainText(ba_as_hex_string);
     isBoolMode_SET = true;


}


void acceuil::on_actionPARAM_REQ_2_triggered()
{

    serialReceiver->setBaudRate(QSerialPort::Baud57600);
    serialReceiver->setDataBits(QSerialPort::Data8);
    serialReceiver->setParity(QSerialPort::NoParity);
    serialReceiver->setStopBits(QSerialPort::OneStop);
    serialReceiver->setFlowControl(QSerialPort::NoFlowControl);

     /*-----------------------------------------------------------------------------------*/
            // Algo de traitement de la fréquence
            //Il faut créer une liste qui prend la taille de la valeur de fréquence désiré
            //mettre le nombre saisie dans le tableu sous forme de chiffre

            /*-----------------------------------------------------------------------------------------*/


            int T[6];
            int c = 0;
            int msb[2] , lsb[2] ;
            int monnombre = ui->frequence->value();
            int mynumber = 0;
            int p =1 , e = 1 , o = 1;

            /*------------------mettre le nmbre saisie dans un tableau en forme de chiffre---------------------------------*/

            while(monnombre!=0) {
                int mod = monnombre%10;
                T[c] = mod;
                monnombre-=mod;
                monnombre/=10;
                c++;
            }

            /*------------------------Reconversion du tableau en chiffre-----------------------------------------------------*/
           for(int i = 1; i<5 ; i++){
                mynumber =  mynumber + T[i]*p;
                p=p*10;
            }

            msb[0] = T[4] ;
            msb[1] = T[3];

            lsb[0] = T[2];
            lsb[1] = T[1];

            uint8_t var = 0 ;
            for(int k=1;k>=0;k--) {
                    var = var +msb[k]*e ;
                    e = e*10 ;
            }
            uint8_t var2 = 0 ;
            for(int k=1;k>=0;k--) {
                    var2 = var2 +lsb[k]*o ;
                    o = o*10 ;
            }


            uint8_t var4;
            if (ui -> prekey -> value() >= 0 && ui -> prekey -> value() <= 85 ){
                var4 = ui -> prekey -> value();
            }
            else{

                var4 = 37;
            }
            /*-------------------------------Calcul du Crc16 Kermit-------------------------------------*/

            unsigned char myData[] = {0xff, 0x00, 0xf0, 0xca, 0xf2, 0x20, 0x00, 0x04,var,var2,0x00,var4};
            unsigned int myDataLen;
            unsigned short myCrc;
            myDataLen = sizeof(myData);
            myCrc = CRCV2(myData, myDataLen);
            myCrc = ((myCrc << 8) | (myCrc >> 8));

            /*-------------Définition des champs de la trame et son envoie dans un fil------------- */

            if  ( ui -> frequence -> value() >= 118000  &  ui -> frequence -> value() <= 136000 )
                    {
                        tsPid20 Pid20;
                        Pid20.synchro [0] = synchrovdl2[0];
                        Pid20.synchro [1] = synchrovdl2[1];
                        Pid20.synchro [2] = synchrovdl2[2];
                        Pid20.synchro [3] = synchrovdl2[3];
                        Pid20.protocoleCode = 0xF2;
                        Pid20.pid = 0x20;
                        Pid20.lengthMsb =0x00;
                        Pid20.lengthLsb = 0x04;
                        Pid20.freq_msb = var2;
                        Pid20.freq_lsb = var;
                        Pid20.mod = 0x00;
                        Pid20.prekey=var4;
                        Pid20.crc=myCrc;

                        QByteArray buffer;


                        //  ecrire les données dans le flux d'envoie
                        QDataStream s(&buffer, QIODevice::WriteOnly);
                        if (false){

                            s.setByteOrder(QDataStream::LittleEndian);
                        }

                        s << (uint8_t)Pid20.synchro [0]<< (uint8_t)Pid20.synchro [1]<<(uint8_t)Pid20.synchro [2]<<(uint8_t)Pid20.synchro [3]
                        << (uint8_t)Pid20.protocoleCode << (uint8_t)Pid20.pid << (uint8_t)Pid20.lengthMsb <<(uint8_t)Pid20.lengthLsb
                       <<(uint8_t)Pid20.freq_lsb<<(uint8_t)Pid20.freq_msb<<(uint8_t)Pid20.mod<<(uint8_t)Pid20.prekey<<(uint16_t)Pid20.crc;


               serialReceiver->write(buffer);
               serialReceiver->flush();
               serialReceiver->waitForBytesWritten(1000);

               int PARAM_REQ = ui -> frequence -> value();
               ui-> plainTextEdit -> appendPlainText(QString("PARAM_REQ_RX = %1").arg(PARAM_REQ));

               QByteArray ba_as_hex_string = buffer.toHex();
               ui->  plainTextEdit_3->appendPlainText(ba_as_hex_string);
      }
            isBoolPARAM_REQ_Re = true;

}


void acceuil::on_actionMODE_SET_REQ_triggered()
{

    serialReceiver->setBaudRate(QSerialPort::Baud57600);
    serialReceiver->setDataBits(QSerialPort::Data8);
    serialReceiver->setParity(QSerialPort::NoParity);
    serialReceiver->setStopBits(QSerialPort::OneStop);
    serialReceiver->setFlowControl(QSerialPort::NoFlowControl);

    if (serialReceiver->open(QIODevice::ReadWrite))
    {

        qDebug() << "Connected";
    }
    else
    {
        qDebug() << "Error";

    }
    struct tsPidF0{
         uint8_t synchro[SIZE_SYNCHRO_SL];
         uint8_t protocoleCode;
         uint8_t pid;
         uint8_t lengthMsb;
         uint8_t lengthLsb;
         uint8_t mode;
         uint8_t req;
         uint16_t crc;

     };


     /*-------------Définition des champs de la trame ------------- */

     tsPidF0 PidF0;
     PidF0.synchro [0] = synchrovdl2[0];
     PidF0.synchro [1] = synchrovdl2[1];
     PidF0.synchro [2] = synchrovdl2[2];
     PidF0.synchro [3] = synchrovdl2[3];
     PidF0.protocoleCode = 0xF2;
     PidF0.pid = 0xF0;
     PidF0.lengthMsb =0x00;
     PidF0.lengthLsb = 0x02;
     PidF0.mode = 0xF1;
     PidF0.req = 0x00;


     /*-------------------------------Calcul du Crc16 Kermit-------------------------------------*/

     unsigned char myData[] = {0xff, 0x00, 0xf0, 0xca, 0xf2, 0xf0, 0x00, 0x02,0xf1, 0x00};

     unsigned int myDataLen;
     unsigned short myCrc;
     myDataLen = sizeof(myData);
     myCrc = CRCV2(myData, myDataLen);
     myCrc = ((myCrc << 8) | (myCrc >> 8));
     PidF0.crc=myCrc;


     /*-------------------Intégration des champs du buffer dans un flux d'envoie--------------------------*/

     QByteArray buffer;
     //  ecrire les données dans le flux d'envoie
     QDataStream s(&buffer, QIODevice::WriteOnly);

     if (false){

         s.setByteOrder(QDataStream::LittleEndian);
     }
     s << (uint8_t)PidF0.synchro [0]<< (uint8_t)PidF0.synchro [1]<<(uint8_t)PidF0.synchro [2]<<(uint8_t)PidF0.synchro [3]  << (uint8_t)PidF0.protocoleCode << (uint8_t)PidF0.pid << (uint8_t)PidF0.lengthMsb <<(uint8_t)PidF0.lengthLsb<<(uint8_t)PidF0.mode<<(uint8_t)PidF0.req<<(uint16_t)PidF0.crc;


     /*--------------------------------Envoie des données--------------------------------------*/
     serialReceiver->write(buffer);
     serialReceiver->flush();
     serialReceiver->waitForBytesWritten(1000);

     ui-> plainTextEdit -> appendPlainText(QString("MODE_SET_REQ_RX"));

     QByteArray ba_as_hex_string = buffer.toHex();
     ui->  plainTextEdit_3->appendPlainText(ba_as_hex_string);

     isBoolMode_req_R =true;



}


void acceuil::on_actionUNIT_DATA_REQ_triggered()
{
    serial->setBaudRate(QSerialPort::Baud57600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    /*-----------------------------structure de la Pid21----------------------------------------------------*/
       struct tsPid21{
           uint8_t synchro[SIZE_SYNCHRO_SL];
           uint8_t protocoleCode;
           uint8_t pid;
           uint8_t lengthMsb;
           uint8_t lengthLsb;
           uint8_t donnees;
           uint8_t sufBcs;
           uint8_t soh;
           uint8_t mode;
           uint8_t address1;
           uint8_t address2;
           uint8_t address3;
           uint8_t address4;
           uint8_t address5;
           uint8_t address6;
           uint8_t address7;
           uint8_t ack;
           uint8_t label1;
           uint8_t label2;
           uint8_t bi;
           uint8_t stx;
           char str1[220];
           uint8_t suff;
           uint8_t bcs;
           uint8_t DE;
           uint8_t bcsSuff;
           uint16_t crc;
           uint8_t msn;
           uint8_t flightid;


       };
      // uint8_t varh = ui-> spinBox -> value() + 27;

       uint8_t var6 = ((ui-> data -> value())) +28;
       unsigned char str[] ={
       0x4c, 0x27, 0x20, 0x41, 0x43, 0x41, 0x52, 0x53 , 0x20, 0x41,
       0x69, 0x72, 0x63, 0x72, 0x61, 0x66, 0x74, 0x20, 0x43, 0x6f,
       0x6d, 0x6d, 0x75,0x6e,  0x69, 0x63, 0x61, 0x74 ,0x69, 0x6f,
       0x6e, 0x20, 0x41 ,0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69,
       0x6e, 0x67, 0x20 ,0x61, 0x6e, 0x64, 0x20 ,0x52, 0x65, 0x70,
       0x6f, 0x72, 0x74, 0x69 ,0x6e, 0x67,  0x20, 0x53 ,0x79, 0x73,
       0x74, 0x65, 0x6d, 0x29, 0x20, 0x65 ,0x73 ,0x74 ,0x20,0x75,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6c, 0x69, 0x74 ,0x65, 0x20, 0x68,0x7f,0x64,0x61,0x61};
       unsigned char str2[ui-> data->value()];
       memcpy(str2, str,sizeof(str2));
       QByteArray databuf = QByteArray((char*)str2, sizeof(str2));

       /*--------------------------- initialisation des valeurs de chaque paramètre de la structure du PARAM_DATA_REQ -------------------------------*/

       tsPid21 Pid21;
       Pid21.synchro [0] = synchrovdl2[0];
       Pid21.synchro [1] = synchrovdl2[1];
       Pid21.synchro [2] = synchrovdl2[2];
       Pid21.synchro [3] = synchrovdl2[3];
       Pid21.protocoleCode = 0xF2;
       Pid21.pid = 0x21;
       Pid21.lengthMsb=0x00;
       Pid21.lengthLsb=var6;
       Pid21.soh = 0x01;
       Pid21.mode = 0x02;

       Pid21.address1=0x2e;
       Pid21.address2=0x2e;
       Pid21.address3=0x6e;
       Pid21.address4=0x33;
       Pid21.address5=0x38;
       Pid21.address6=0x37;
       Pid21.address7=0x32;
       Pid21.ack = 0x01;
       Pid21.label1 = 0x01;
       Pid21.label2 = 0x00;
       Pid21.bi = 0x01;
       Pid21.stx = 0x01;
       Pid21.msn=0x31;
       Pid21.flightid=0x58;
       //strncpy ((char *)Pid21.str1,str, longeur );
       Pid21.suff = 0x03;


       /*---------------------------- Cette Fonction Permettre d'écrire les données dans un flus d'envoie ---------------------------------*/

       QByteArray buffer;
       QDataStream s(&buffer, QIODevice::WriteOnly);

       if (false){

           s.setByteOrder(QDataStream::LittleEndian);
       }
       s << (uint8_t)Pid21.synchro [0]<< (uint8_t)Pid21.synchro [1]<<(uint8_t)Pid21.synchro [2]
         <<(uint8_t)Pid21.synchro [3]  << (uint8_t)Pid21.protocoleCode << (uint8_t)Pid21.pid
         << (uint8_t)Pid21.lengthMsb<< (uint8_t)Pid21.lengthLsb<<(uint8_t)Pid21.soh
         <<(uint8_t)Pid21.mode<<(uint8_t)Pid21.address1<<(uint8_t)Pid21.address2<<(uint8_t)Pid21.address3
         <<(uint8_t)Pid21.address4<<(uint8_t)Pid21.address5<<(uint8_t)Pid21.address6
         <<(uint8_t)Pid21.address7<<(uint8_t)Pid21.ack<<(uint8_t)Pid21.label1<<(uint8_t)Pid21.label2
         <<(uint8_t)Pid21.bi<<(uint8_t)Pid21.stx<<(uint8_t)Pid21.msn<<(uint8_t)Pid21.msn<<(uint8_t)Pid21.msn<<(uint8_t)Pid21.msn
         <<(uint8_t)Pid21.flightid<<(uint8_t)Pid21.flightid<<(uint8_t)Pid21.flightid<<(uint8_t)Pid21.flightid<<(uint8_t)Pid21.flightid
         <<(uint8_t)Pid21.flightid;

       /*-------------------------------Calcul du Crc16 Kermit-------------------------------------*/


       unsigned char myData[] ={
       0xff ,0x00, 0xf0, 0xca, 0xf2, 0x21, 0x00 ,var6 , 0x01,0x02,
       0x2e , 0x2e , 0x6e , 0x33 , 0x38 , 0x37,0x32 , 0x01 , 0x01,
       0x00 ,0x01, 0x01,0x31,0x31,0x31,0x31,0x58,0x58,0x58,0x58,
       0x58,0x58,    0x4c, 0x27, 0x20, 0x41, 0x43, 0x41, 0x52, 0x53 , 0x20, 0x41,
       0x69, 0x72, 0x63, 0x72, 0x61, 0x66, 0x74, 0x20, 0x43, 0x6f,
       0x6d, 0x6d, 0x75,0x6e,  0x69, 0x63, 0x61, 0x74 ,0x69, 0x6f,
       0x6e, 0x20, 0x41 ,0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x69,
       0x6e, 0x67, 0x20 ,0x61, 0x6e, 0x64, 0x20 ,0x52, 0x65, 0x70,
       0x6f, 0x72, 0x74, 0x69 ,0x6e, 0x67,  0x20, 0x53 ,0x79, 0x73,
       0x74, 0x65, 0x6d, 0x29, 0x20, 0x65 ,0x73 ,0x74 ,0x20,0x75,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6e, 0x20,  0x73, 0x79, 0x73, 0x74, 0x74, 0x74, 0x74 ,0x74,
       0x6c, 0x69, 0x74 ,0x65, 0x20, 0x68,0x7f,0x64,0x61,0x61};


         //nouveau atbleau ou on ibnsère le nombre saisie dans l'interface graphique de caractère
      unsigned char myData1[ui-> data->value()+32];

         // on copie les valeurs dans le nouveau tableau
      memcpy(myData1, myData,sizeof(myData1));

        // un tableau qui contien la valeur 0x03
      unsigned char myData3[] ={0x03,0x02,0x02,0x7f};
      QByteArray databuf3 = QByteArray((char*)myData3, sizeof(myData3));



        // on conbine entre les deux tableau pour l'intégrer deans la fonction qui calcule le crc

      unsigned char allstrings[sizeof(myData1)+sizeof(myData3)];
      memcpy(allstrings,myData1,sizeof(myData1));
      memcpy(allstrings+sizeof(myData1),myData3,sizeof(myData3));


      unsigned short myCrc;
      int myDataLen = sizeof(myData1)+ sizeof(myData3);
      myCrc = CRCV2(allstrings, myDataLen);
      myCrc = ((myCrc << 8) | (myCrc >> 8));

      Pid21.crc=myCrc;

      QByteArray bufferCrc;
      QDataStream g(&bufferCrc, QIODevice::WriteOnly);

      if (false){

          g.setByteOrder(QDataStream::LittleEndian);
      }
      g<< (uint16_t)Pid21.crc;

      /*--------------------------------Envoie des données--------------------------------------*/



      QByteArray buffer2 ;

      buffer2 = buffer;
      buffer2 = buffer2.append(databuf);
      buffer2 = buffer2.append(databuf3);
      buffer2 = buffer2.append(bufferCrc);

      serial->write(buffer2);
      serial->flush();
      serial->waitForBytesWritten(1000);


      counter++;
      ui-> plainTextEdit -> appendPlainText(QString("[%1] : UNIT_DATA_REQ").arg(counter));
      QByteArray ba_as_hex_string = buffer2.toHex();
      ui->  plainTextEdit_3->appendPlainText(ba_as_hex_string);
      ratio->writeDatagram(buffer2,QHostAddress::LocalHost, 5555);

      isBoolUnit_data_req = true ;

}


void acceuil::on_actionPURGE_REQ_triggered()
{

    serial->setBaudRate(QSerialPort::Baud57600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);



    struct tsPid25
      {
        uint8_t synchro[SIZE_SYNCHRO_SL];
        uint8_t protocoleCode;
        uint8_t pid;
        uint8_t lengthMsb;
        uint8_t lengthLsb;
        uint16_t crc;

      };

    tsPid25 Pid25;
    Pid25.synchro [0] = synchrovdl2[0];
    Pid25.synchro [1] = synchrovdl2[1];
    Pid25.synchro [2] = synchrovdl2[2];
    Pid25.synchro [3] = synchrovdl2[3];
    Pid25.protocoleCode = 0xF2;
    Pid25.pid = 0x25;
    Pid25.lengthLsb =0x00;
    Pid25.lengthMsb = 0x00;

    /*-------------------------------Calcul du Crc16 Kermit-------------------------------------*/


    unsigned char myData[] = {0xff, 0x00, 0xf0, 0xca, 0xf2, 0x25, 0x00, 0x00};
    unsigned int myDataLen;
    unsigned short myCrc;
    myDataLen = sizeof(myData);
    myCrc = CRCV2(myData, myDataLen);
    myCrc = ((myCrc << 8) | (myCrc >> 8));
    Pid25.crc = myCrc;

    /*-------------------Intégration des champs du buffer dans un flux d'envoie--------------------------*/


    QByteArray buffer;
    QDataStream s(&buffer, QIODevice::WriteOnly);

    if (false){

        s.setByteOrder(QDataStream::LittleEndian);
    }
    s << (uint8_t)Pid25.synchro [0]<< (uint8_t)Pid25.synchro [1]<<(uint8_t)Pid25.synchro [2]<<(uint8_t)Pid25.synchro [3]
      << (uint8_t)Pid25.protocoleCode << (uint8_t)Pid25.pid << (uint8_t)Pid25.lengthMsb <<(uint8_t)Pid25.lengthLsb<<(uint16_t)Pid25.crc;

    /*--------------------------------Envoie des données--------------------------------------*/
    serial->write(buffer);
    serial->flush();
    serial->waitForBytesWritten(1000);

    ui-> plainTextEdit -> appendPlainText(QString("PURGE_REQ"));
    QByteArray ba_as_hex_string = buffer.toHex();
    ui->  plainTextEdit_3->appendPlainText(ba_as_hex_string);


}


void acceuil::on_actionClose_triggered()
{
   acceuil::close();
}


void acceuil::on_actioncLEAR_triggered()
{
    ui->  plainTextEdit_4->clear();
    ui->  plainTextEdit_3->clear();
    ui->  plainTextEdit_2->clear();
    ui->  plainTextEdit->clear();
}


void acceuil::on_actionVersion_triggered()
{
      vesion.show();
}


void acceuil::on_actionSQP_triggered()
{
    sqpacars.show();
}


void acceuil::on_actionTransmission_Ratio_triggered()
{
     ratio_tran.show();
}

