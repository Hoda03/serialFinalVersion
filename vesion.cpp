#include "vesion.h"
#include "ui_vesion.h"

vesion::vesion(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::vesion)
{
    ui->setupUi(this);
    this->setWindowTitle("version");
    this->setStyleSheet("QMainWindow { background-color : #ffffff}");


    ui->logo->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    ui->logo->setStyleSheet("QLabel { background-color : white; color : #0078d7; }");
    ui-> logo->setLineWidth(3);
    ui-> logo->setMidLineWidth(3);

    ui->label_3->setFrameStyle(QFrame::Box | QFrame::Sunken);
    ui-> label_3->setLineWidth(3);
    ui-> label_3->setMidLineWidth(2);
}

vesion::~vesion()
{
    delete ui;
}
