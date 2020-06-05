#ifndef REPLAY_LIST_GAMES_WINDOW_H
#define REPLAY_LIST_GAMES_WINDOW_H

#include <QDialog>
#include "replay_game_window.h"

namespace Ui {
class ReplayListGamesWindow;
}

class ReplayListGamesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ReplayListGamesWindow(QWidget *parent = nullptr);
    ~ReplayListGamesWindow();

private slots:
    void on_load_games_button_clicked();

private:
    Ui::ReplayListGamesWindow *ui;
signals:
    void loadGames();
    void loadGame(QString);

private slots:
    void getGames(QStringList games);

    void on_gamesList_clicked(const QModelIndex &index);

public:
    ReplayGameWindow rgw;
};

#endif // REPLAY_LIST_GAMES_WINDOW_H
