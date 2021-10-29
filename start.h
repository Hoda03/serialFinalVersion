#ifndef START_H
#define START_H

#include <QMainWindow>
#include "acceuil.h"
#include "vdl2.h"

namespace Ui {
class start;
}

class start : public QMainWindow
{
    Q_OBJECT

public:
    explicit start(QWidget *parent = nullptr);
    ~start();

private slots:
    void on_pushButton_clicked();

private:
    Ui::start *ui;
    acceuil acceuil;
    vdl2 vdl2;
};

#endif // START_H
