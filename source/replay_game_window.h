#ifndef REPLAY_GAME_WINDOW_H
#define REPLAY_GAME_WINDOW_H

#include <QDialog>
#include "board_replay_widget.h"

namespace Ui {
class ReplayGameWindow;
}

class ReplayGameWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ReplayGameWindow(QWidget *parent = nullptr);
    ~ReplayGameWindow();

    BoardReplayWidget *board;
signals:
    void selectedMove(int);

private slots:
    void on_exit_clicked();
    void getMoves(QStringList);
    void setStatus(int status);
    void setTurn(bool turn);

    void on_listView_clicked(const QModelIndex &index);

private:
    Ui::ReplayGameWindow *ui;
};

#endif // REPLAY_GAME_WINDOW_H
