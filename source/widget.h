#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "chessboard.h"
#include "promotiondialog.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    ChessBoard *board;

    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
signals:
    void promotion(bool);
    void promotion(int);

private:
    PromotionDialog promDialog;
    Ui::Widget *ui;
};

#endif // WIDGET_H
