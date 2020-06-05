#include "savegamedialog.h"
#include "ui_savegamedialog.h"

SaveGameDialog::SaveGameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveGameDialog)
{
    ui->setupUi(this);
}

SaveGameDialog::~SaveGameDialog()
{
    delete ui;
}

void SaveGameDialog::on_cancel_clicked()
{
    close();
}

void SaveGameDialog::on_save_clicked()
{
    emit save(ui->name->text());
    close();
}
