#ifndef RATIO_TRAN_H
#define RATIO_TRAN_H

#include <QWidget>
#include <QUdpSocket>

namespace Ui {
class ratio_tran;
}

class ratio_tran : public QWidget
{
    Q_OBJECT

public:
    explicit ratio_tran(QWidget *parent = nullptr);
    ~ratio_tran();

private:
    Ui::ratio_tran *ui;
    QHostAddress sender;
    quint16 senderPort;
    QUdpSocket *mSocketW;
};

#endif // RATIO_TRAN_H
