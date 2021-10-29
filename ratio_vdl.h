#ifndef RATIO_VDL_H
#define RATIO_VDL_H

#include <QWidget>
#include <QUdpSocket>


namespace Ui {
class ratio_vdl;
}

class ratio_vdl : public QWidget
{
    Q_OBJECT

public:
    explicit ratio_vdl(QWidget *parent = nullptr);
    ~ratio_vdl();

private:
    Ui::ratio_vdl *ui;
    QHostAddress sender;
    quint16 senderPort;
    QUdpSocket *mSocketH;

};

#endif // RATIO_VDL_H
