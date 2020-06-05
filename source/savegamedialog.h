#ifndef SAVEGAMEDIALOG_H
#define SAVEGAMEDIALOG_H

#include <QDialog>

namespace Ui {
class SaveGameDialog;
}

class SaveGameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveGameDialog(QWidget *parent = nullptr);
    ~SaveGameDialog();

signals:
    void save(QString);
    void cancel();
private slots:
    void on_cancel_clicked();
    void on_save_clicked();

private:
    Ui::SaveGameDialog *ui;
};

#endif // SAVEGAMEDIALOG_H
