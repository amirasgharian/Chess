#ifndef GLOBALS_H
#define GLOBALS_H

#include "chessman.h"
#include "cell.h"

extern QGraphicsScene *chessScene;
extern QVector<QString> movesSeries;
extern QList<chessman *> blackDeaths;
extern QList<chessman *> whiteDeaths;
extern bool startGame;
extern bool resetGame;
extern Cell *chessBoard[8][8];
extern chessman *movingPiece;
extern QString turn;
extern void changeTurn();
extern QList<chessman *> alives;
extern int positiveSuser1 ;
extern int negativeSuser1 ;
extern int positiveSuser2 ;
extern int negativeSuser2 ; 
#endif // GLOBALS_H
