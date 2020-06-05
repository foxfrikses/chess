#include "db_manager.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlRecord>
#include <QDir>
//#include <time.h>
#include <QDebug>

DBManager::DBManager(QObject *parent): QObject (parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
//    QDir databasePath;
//    QString path = databasePath.currentPath()+"moves.db";
//    db.setDatabaseName(path);
    db.setDatabaseName(QString("/home/artsiom/qtproj/g/chess/source/") + "moves.db");

    if (!db.open()) return;

    QSqlQuery query(db);

    query.exec("CREATE TABLE IF NOT EXISTS game("
               "ID INTEGER NOT NULL PRIMARY KEY,"
               "name TEXT NOT NULL UNIQUE);");
    query.exec("CREATE TABLE IF NOT EXISTS move("
               "ID INTEGER NOT NULL PRIMARY KEY,"
               "gameID INTEGER NOT NULL,"
               "text TEXT,"
               "num_move INTEGER NOT NULL,"
               "cell_from INTEGER NOT NULL,"
               "cell_to INTEGER NOT NULL,"
               "cell_add1 INTEGER,"
               "cell_add2 INTEGER,"
               "status INTEGER);");
}

void DBManager::store(QString name)
{
    if (!db.open()) return;
    QSqlQuery query(db);

    query.prepare("SELECT ID FROM game WHERE name = ?" );
    query.bindValue( 0, name);
    query.exec();
    int n = 0;
    while (query.next())
        ++n;
    if (n != 0){
        emit storeError("This name is already used");
        return;
    }

    query.prepare("INSERT INTO game (name) "
                  "VALUES (:name)");
    query.bindValue(":name", name);
//    query.bindValue(":date", qint64(time(nullptr)));
    qDebug()<<query.exec()<<endl;

    auto gameID = query.lastInsertId();
    qDebug()<<"index: "<<gameID<<endl;
    if (gameID.isValid()){
        for (int i = 0; i < moves.count(); ++i){
            query.prepare("INSERT INTO move (gameID, text, num_move, cell_from, cell_to, cell_add1, cell_add2, status) "
                          "VALUES (:gameID, :text, :num_move, :cell_from, :cell_to, :cell_add1, :cell_add2, :status)");
            query.bindValue(":gameID", gameID);
            query.bindValue(":text", QString::number(int((i+2)/2)) + ((i%2)?"... ":".   ") + moves[i].first);
            query.bindValue(":num_move", qint32(i + 1));
            query.bindValue(":cell_from", moves[i].second.cell_from);
            query.bindValue(":cell_to",   moves[i].second.cell_to);
            query.bindValue(":cell_add1", moves[i].second.cell_add1);
            query.bindValue(":cell_add2", moves[i].second.cell_add2);
            query.bindValue(":status",    moves[i].second.status);
            qDebug()<<query.exec()<<endl;
        }
    }
}

void DBManager::getLastMove(const std::pair<QString, Move>& move){
    moves.append(move);
}

void DBManager::clear(){
    moves.clear();
}

void DBManager::loadGame(QString name)
{
    if (!db.open()) return;

    QSqlQuery query(db);
    query.prepare("SELECT ID FROM game WHERE name = ?" );
    query.bindValue( 0, name);
    query.exec();
    int n = 0;
    while (query.next())
        ++n;
    if (n != 1) return;
    query.first();
    qint64 id = query.value(0).toInt();

    QStringList list;
    QVector<Move> moves;
    query.prepare("SELECT num_move, text, cell_from, cell_to, cell_add1, cell_add2, status "
                  "FROM move "
                  "WHERE gameID = :id "
                  "ORDER BY num_move ASC");
    query.bindValue(":id", id);
    qDebug()<< query.exec()<<endl;
    int st_index  = query.record().indexOf("status");
    int txt_index = query.record().indexOf("text");
    int cf_index  = query.record().indexOf("cell_from");
    int ct_index  = query.record().indexOf("cell_to");
    int ca1_index = query.record().indexOf("cell_add1");
    int ca2_index = query.record().indexOf("cell_add2");

    while (query.next()){
        Move m;
        list << query.value(txt_index).toString();
        m.cell_from = query.value(cf_index).toInt();
        m.cell_to   = query.value(ct_index).toInt();
        m.cell_add1 = query.value(ca1_index).toInt();
        m.cell_add2 = query.value(ca2_index).toInt();
        m.status    = query.value(st_index).toInt();
        moves.append(m);
    }
    emit sendMoves(list);
    emit sendMoves(moves);
}

void DBManager::sendGames()
{
    if (!db.open()) return;

    QStringList list;
    QSqlQuery query("SELECT name FROM game", db);
    while (query.next())
        list << query.value(0).toString();
    emit sendGames(list);
}

