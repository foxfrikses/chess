#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "gamewindow.h"
#include "replay_list_games_window.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    GameWindow gw;
    ReplayListGamesWindow rlgw;
private slots:
    void on_replay_clicked();

    void on_newgame_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
