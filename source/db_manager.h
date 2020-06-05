#ifndef GAMESTORER_H
#define GAMESTORER_H

#include <QObject>
#include <QVector>
#include <QSqlDatabase>

class DBManager : public QObject
{
    Q_OBJECT
public:
    struct Move{
        int32_t cell_from;
        int32_t cell_to;
        int32_t cell_add1;
        int32_t cell_add2;
        int32_t status;
    };
public:
    DBManager(QObject *parent = nullptr);

signals:
    void stored();
    void storeError(QString);
public slots:
    void getLastMove(const std::pair<QString, Move>& move);
    void clear();
    void store(QString name);


signals:
    void sendGames(QStringList);
    void sendMoves(QVector<Move>);
    void sendMoves(QStringList);
public slots:
    void loadGame(QString);
    void sendGames();


private:
    QVector<std::pair<QString, Move>> moves;
    QSqlDatabase db;
};

#endif // GAMESTORER_H
