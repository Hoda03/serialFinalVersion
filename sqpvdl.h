#ifndef SQPVDL_H
#define SQPVDL_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtCharts>
#include <QHostAddress>
#include <QUdpSocket>

constexpr size_t sizen =100;

namespace Ui {
class sqpvdl;
}

class sqpvdl : public QMainWindow
{
    Q_OBJECT

public:
    explicit sqpvdl(QWidget *parent = nullptr);
    ~sqpvdl();

private:
    Ui::sqpvdl *ui;
    QUdpSocket *mSocketh;
    QUdpSocket *Sockvaldata;

    QHostAddress sender;
    quint16 senderPort;
    QChartView *graphique; // un widget pour afficher un graphe
    QChart *graphe; // la représentation d'un graphe
    QLineSeries *courbe; // les données

private:
    int numbers[sizen] {0};
    int counter=0;

};

#endif // SQPVDL_H
