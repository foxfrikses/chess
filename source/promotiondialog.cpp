#include "promotiondialog.h"
#include "ui_promotiondialog.h"

PromotionDialog::PromotionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PromotionDialog)
{
    ui->setupUi(this);
    ui->Rook->resize(50, 50);
    ui->Knight->resize(50, 50);
    ui->Bishop->resize(50, 50);
    ui->Queen->resize(50, 50);
}

PromotionDialog::~PromotionDialog()
{
    delete ui;
}

void PromotionDialog::execute(bool isWhite)
{
    if (isWhite){
        ui->Rook->setStyleSheet("border-image:url(:/img/img/rook-white.png);");
        ui->Bishop->setStyleSheet("border-image:url(:/img/img/bishop-white.png);");
        ui->Knight->setStyleSheet("border-image:url(:/img/img/knight-white.png);");
        ui->Queen->setStyleSheet("border-image:url(:/img/img/queen-white.png);");
    }else{
        ui->Rook->setStyleSheet("border-image:url(:/img/img/rook-black.png);");
        ui->Bishop->setStyleSheet("border-image:url(:/img/img/bishop-black.png);");
        ui->Knight->setStyleSheet("border-image:url(:/img/img/knight-black.png);");
        ui->Queen->setStyleSheet("border-image:url(:/img/img/queen-black.png);");
    }
    turn = isWhite;
    exec();
}

void PromotionDialog::on_Knight_clicked()
{
    emit sendResult((turn)? 2: -2);
    close();
}

void PromotionDialog::on_Bishop_clicked()
{
    emit sendResult((turn)? 3: -3);
    close();
}

void PromotionDialog::on_Rook_clicked()
{
    emit sendResult((turn)? 4: -4);
    close();
}

void PromotionDialog::on_Queen_clicked()
{
    emit sendResult((turn)? 5: -5);
    close();
}
