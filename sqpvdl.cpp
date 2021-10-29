#include "sqpvdl.h"
#include "ui_sqpvdl.h"

sqpvdl::sqpvdl(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::sqpvdl)
{
    ui->setupUi(this);
    this->setWindowTitle("Signal Quality");

    int valeur_DATA;

    mSocketh = new QUdpSocket(this);
    mSocketh->bind(QHostAddress::LocalHost,2022);


    Sockvaldata = new QUdpSocket(this);
    Sockvaldata->bind(QHostAddress::LocalHost,2684);

    connect(Sockvaldata,&QUdpSocket::readyRead,[&](){
        while (Sockvaldata ->hasPendingDatagrams())
        {
            QByteArray data;
            QHostAddress sender;
            quint16 senderPort;
            data.resize(Sockvaldata->pendingDatagramSize());
            Sockvaldata->readDatagram(data.data(),data.size(),&sender, &senderPort);

            valeur_DATA = (int) data[0];


         }});


    connect(mSocketh,&QUdpSocket::readyRead,[&](){
        while (mSocketh ->hasPendingDatagrams())
        {
            QByteArray data;
            QHostAddress sender;
            quint16 senderPort;
            data.resize(mSocketh->pendingDatagramSize());
            mSocketh->readDatagram(data.data(),data.size(),&sender, &senderPort);

             if(data[5] == 84){
                int hd = (int)data[8];
                if (counter > 10 ){
                    for (int i=0; i<9; i++){
                        numbers[i]= numbers[i+1];
                    }
                    numbers[9] = hd;
                }
                else {
                    numbers[counter] = hd;
                }

             }
             else if(data[5] == 81 && data.length()  ==  valeur_DATA + 39){
                int hd = (int)data[data.length() - 9];
                if (counter > 10 ){
                    for (int i=0; i<9; i++){
                        numbers[i]= numbers[i+1];
                    }
                    numbers[9] = hd;
                }
                else {
                    numbers[counter] = hd;
                }

             }

             else if(data[5] == 81 && data.length()  ==  valeur_DATA + 29){
                int hd = (int)data[data.length() - 9];
                if (counter > 10 ){
                    for (int i=0; i<9; i++){
                        numbers[i]= numbers[i+1];
                    }
                    numbers[9] = hd;
                }
                else {
                    numbers[counter] = hd;
                }

             }
                counter++;
                QBarSet *set0 = new QBarSet("Signal Quality");
                *set0 << numbers[0]<< numbers[1]<< numbers[2]<< numbers[3]<< numbers[4]<< numbers[5]<< numbers[6]<< numbers[7]<< numbers[8]<< numbers[9] ;
                QBarSeries *series = new QBarSeries();
                series->append(set0);
                graphe = new QChart();
                graphe->setTitle("Signal Quality graph");
                graphe->addSeries(series);
                QBarCategoryAxis *axisX = new QBarCategoryAxis();
                graphe->addAxis(axisX, Qt::AlignBottom);
                series->attachAxis(axisX);
                QValueAxis *axisY = new QValueAxis();
                axisY->setRange(0,15);
                graphe->addAxis(axisY, Qt::AlignLeft);
                series->attachAxis(axisY);
                graphique = new QChartView;
                graphique->setRenderHint(QPainter::Antialiasing);
                graphique->setChart(graphe);
                setCentralWidget(graphique);
                resize(640, 480);

       }

});
}

sqpvdl::~sqpvdl()
{
    delete ui;
}
