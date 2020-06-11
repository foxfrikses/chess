#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gw.setModal(true);
    rlgw.setModal(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_replay_clicked()
{
    this->hide();
    rlgw.load();
    rlgw.exec();
    this->show();
}

void MainWindow::on_newgame_clicked()
{
    this->hide();
    gw.exec();
    this->show();
}
