#include "start.h"
#include "ui_start.h"

start::start(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::start)
{
    ui->setupUi(this);
    this->setWindowTitle("SL");
    this->setStyleSheet("QMainWindow { background-color : #ffffff}");


    ui->label->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    ui->label->setStyleSheet("QLabel { background-color : white; color : #0078d7; }");
    ui-> label->setLineWidth(3);
    ui-> label->setMidLineWidth(3);



    ui->label_3->setFrameStyle(QFrame::Box | QFrame::Sunken);
    ui-> label_3->setLineWidth(3);
    ui-> label_3->setMidLineWidth(2);


}

start::~start()
{
    delete ui;
}

void start::on_pushButton_clicked()
{

    if ((ui -> acars -> isChecked() ))
     {
         start::close();
         acceuil.show();
     }
     if (ui -> vdl2 -> isChecked() )
     {
         start::close();
         vdl2.show();
     }


}

