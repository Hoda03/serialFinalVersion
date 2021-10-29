#include "ratio.h"
#include "ui_ratio.h"

ratio::ratio(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ratio)
{
    ui->setupUi(this);
}

ratio::~ratio()
{
    delete ui;
}
