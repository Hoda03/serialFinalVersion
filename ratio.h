#ifndef RATIO_H
#define RATIO_H

#include <QMainWindow>
#include <QUdpSocket>

namespace Ui {
class ratio;
}

class ratio : public QMainWindow
{
    Q_OBJECT

public:
    explicit ratio(QWidget *parent = nullptr);
    ~ratio();

private:
    Ui::ratio *ui;
    QHostAddress sender;
    quint16 senderPort;
    QUdpSocket *mSocketW;
};

#endif // RATIO_H
