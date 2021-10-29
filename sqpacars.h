#ifndef SQPACARS_H
#define SQPACARS_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtCharts>
#include <QHostAddress>
#include <QUdpSocket>

constexpr size_t sizeT =100;

namespace Ui {
class sqpAcars;
}

class sqpAcars : public QMainWindow
{
    Q_OBJECT

public:
    explicit sqpAcars(QWidget *parent = nullptr);
    ~sqpAcars();

private:
    Ui::sqpAcars *ui;
    QUdpSocket *mSocketaCARS;
    QHostAddress sender;
    quint16 senderPort;
    QChartView *graphique; // un widget pour afficher un graphe
    QChart *graphe; // la représentation d'un graphe
    QLineSeries *courbe; // les données

private:
    int numbers[sizeT] {0};
    int counter=0;
};

#endif // SQPACARS_H
