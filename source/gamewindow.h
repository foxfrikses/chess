#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QDialog>
#include "board_widget.h"
#include "promotiondialog.h"
#include "savegamedialog.h"

namespace Ui {
class GameWindow;
}

class GameWindow : public QDialog
{
    Q_OBJECT
public:
    BoardWidget *board;
public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();
signals:
    void promotion(bool);
    void promotion(int);
    void newGame();
    void saveGame(QString);
private slots:
    void setStatus(int status);
    void setTurn(bool turn);

    void on_exit_clicked();

private:
    PromotionDialog promDialog;
    SaveGameDialog saveGameDialog;
private:
    Ui::GameWindow *ui;
};

#endif // GAMEWINDOW_H
