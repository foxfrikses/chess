#include "logic.h"


Logic::Logic(QObject *parent) : QObject(parent){
    // connect with db
    connect(&db , SIGNAL(sendGames(QStringList)), &mw.rlgw, SLOT(getGames(QStringList)));
    connect(&db , SIGNAL(sendMoves(QStringList)), &mw.rlgw.rgw, SLOT(getMoves(QStringList)));
    connect(&db , SIGNAL(sendMoves(QVector<Move>)),
            &br, SLOT(initBoard(QVector<Move>)));
    connect(&bg , SIGNAL(sendLastMove(const std::pair<QString, Move>&)),
            &db, SLOT(getLastMove(const std::pair<QString, Move>&)));
    connect(&mw.gw , SIGNAL(newGame()), &db, SLOT(clear()));
    connect(&mw.gw , SIGNAL(saveGame(QString)), &db, SLOT(store(QString)));
    connect(&mw.rlgw , SIGNAL(loadGame(QString)), &db, SLOT(loadGame(QString)));
    connect(&mw.rlgw , SIGNAL(loadGames()), &db, SLOT(sendGames()));

    // connect business logic with its representation (game)
    connect(&bg , SIGNAL(moved(int*const*const, int, bool)),
            mw.gw.board, SLOT(setBoard(int*const*const, int, bool)));
    connect(&bg , SIGNAL(promotion(bool)), &mw.gw, SIGNAL(promotion(bool)));
    connect(mw.gw.board , SIGNAL(sendMove(const QPoint, const QPoint)),
            &bg, SLOT(move(const QPoint, const QPoint)));
    connect(&mw.gw , SIGNAL(promotion(int)),
            &bg, SLOT(promotion(int)));
    connect(&mw.gw , SIGNAL(newGame()),
            &bg, SLOT(initBoard()));

    // connect business logic with its representation (replay)
    connect(&br, SIGNAL(moved(int*const*const, int, bool)),
            mw.rlgw.rgw.board, SLOT(setBoard(int*const*const, int, bool)));
    connect(&mw.rlgw.rgw, SIGNAL(selectedMove(int)), &br, SLOT(move(int)));

    bg.initBoard();
}
