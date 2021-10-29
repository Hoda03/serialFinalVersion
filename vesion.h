#ifndef VESION_H
#define VESION_H

#include <QMainWindow>

namespace Ui {
class vesion;
}

class vesion : public QMainWindow
{
    Q_OBJECT

public:
    explicit vesion(QWidget *parent = nullptr);
    ~vesion();

private:
    Ui::vesion *ui;
};

#endif // VESION_H
