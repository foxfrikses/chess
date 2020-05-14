#ifndef PROMOTIONDIALOG_H
#define PROMOTIONDIALOG_H

#include <QDialog>

namespace Ui {
class PromotionDialog;
}

class PromotionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PromotionDialog(QWidget *parent = nullptr);
    ~PromotionDialog();

private slots:
    void on_Knight_clicked();

    void on_Bishop_clicked();

    void on_Rook_clicked();

    void on_Queen_clicked();

    void execute(bool);

signals:
    void sendResult(int);


private:
    bool turn;
    Ui::PromotionDialog *ui;
};

#endif // PROMOTIONDIALOG_H
