#include "ratio_vdl.h"
#include "ui_ratio_vdl.h"

int counter11 =0;
int counter21 =0;
int counter31 =0;

constexpr size_t sizer1 = 16000;
int bff11[sizer1] {0};
int bff21[sizer1] {0};
bool isBff1En1d {false};
bool isBff2End1 {false};

ratio_vdl::ratio_vdl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ratio_vdl)
{
    ui->setupUi(this);

        this->setWindowTitle("Bit Error Rate Test");
        this->setStyleSheet("QWidget { background-color : #ffffff}");

        mSocketH= new QUdpSocket(this);
        mSocketH->bind(QHostAddress::LocalHost,1994);

        ui->label_4->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
        ui->label_4->setStyleSheet("QLabel { background-color : white; color : #0078d7; }");
        ui-> label_4->setLineWidth(3);
        ui-> label_4->setMidLineWidth(3);

        ui->label_3v1_2->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
        ui-> label_3v1_2->setLineWidth(3);
        ui-> label_3v1_2->setMidLineWidth(3);

        ui->label_5v1_2->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
        ui-> label_5v1_2->setLineWidth(3);
        ui-> label_5v1_2->setMidLineWidth(3);

        ui->label_7v1_2->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
        ui-> label_7v1_2->setLineWidth(3);
        ui-> label_7v1_2->setMidLineWidth(3);

        ui->label_4v1_2->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
        ui-> label_4v1_2->setLineWidth(3);
        ui-> label_4v1_2->setMidLineWidth(3);

        ui->label_8v1_2->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
        ui-> label_8v1_2->setLineWidth(3);
        ui-> label_8v1_2->setMidLineWidth(3);

        ui->label_6v1_2->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
        ui-> label_6v1_2->setLineWidth(3);
        ui-> label_6v1_2->setMidLineWidth(3);


        ui->label_4v1_2->setNum(0);
        ui->label_8v1_2->setNum(0);
        ui->label_6v1_2->setNum(0);
        int val_datalength;

        connect(mSocketH,&QUdpSocket::readyRead,[&](){
            while (mSocketH ->hasPendingDatagrams())
            {
                QByteArray data;
                QHostAddress sender;
                quint16 senderPort;
                data.resize(mSocketH->pendingDatagramSize());
                mSocketH->readDatagram(data.data(),data.size(),&sender, &senderPort);

                if(data.length() == 1){
                    val_datalength = (int) data[0];

                }




                else if (data[5] == 33) {
                    counter21++ ;
                    ui->label_6v1_2->setNum(counter21);

                    int longueur = data.size()- 2;
                    for(int i = 9; i< longueur ; i++)
                        {
                            int bff = (int) data[i];
                            QString result = QString::number( bff, 16 );
                            ui->  plainTextEditv1->appendPlainText(QString("[%1] : ").arg(i-9) + result);
                            bff11[i-9] = (int) data[i];

                        }
                        isBff1En1d = true;
                        ui->  plainTextEditv1->appendPlainText(QString("----------End Bloc-----------"));


                }
                else{
                    counter11++ ;
                    ui->label_8v1_2->setNum(counter11);


                    if(data.length() == val_datalength +39 && data[5] == 81 ){
                            for(int i = 10; i< data.length() -29 ; i++)
                                {
                                    int bff = (int) data[i];
                                    QString result = QString::number( bff, 16 );
                                    ui->  plainTextEdit_2v1->appendPlainText(QString("[%1] : ").arg(i-10) + result);
                                    bff21[i-10] = (int) data[i];

                                }
                            isBff2End1 = true;
                            ui->  plainTextEdit_2v1->appendPlainText(QString("----------End Bloc-----------"));

                    }
                    else if(data.length() == val_datalength +29 && data[5] == 81 ) {
                            for(int i = 10; i< data.length() -19 ; i++)
                                {
                                    int bff = (int) data[i];
                                    QString result = QString::number( bff, 16 );
                                    ui->  plainTextEdit_2v1->appendPlainText(QString("[%1] : ").arg(i-10) + result);
                                    bff21[i-10] = (int) data[i];

                                }
                            isBff2End1 = true;
                            ui->  plainTextEdit_2v1->appendPlainText(QString("----------End Bloc-----------"));

                    }
                }
                if (isBff1En1d && isBff2End1)
                    {
                     int longueur = data.size();
                     for(int n=0; n< longueur; n++) {

                        if((int) bff21[n+8] == (int) bff11[n]){
                            ui->label_4v1_2->setNum(0);

                        }
                        else{
                            counter31++;
                            ui->label_4v1_2->setNum(counter31);
                            //ui->  plainTextEdit_3v1->appendPlainText(QString("[%1]  ").arg(n) );

                        }
                    }
                    }


            }});

}

ratio_vdl::~ratio_vdl()
{
    delete ui;
}
