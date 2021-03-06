#include "game.h"
#include "qdebug.h"
#include "globals.h"

Game::Game(QString t, QString w, QString b, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Game)
{
    ui->setupUi(this);
    ui->title->setText(t);
    ui->player_1->setText(w);
    ui->player_2->setText(b);
    chessScene = new QGraphicsScene;
    turnDisplay = new QGraphicsTextItem;
    player1Point = new QGraphicsTextItem;
    player2Point = new QGraphicsTextItem;
    checkDisplay = new QGraphicsTextItem;
    QFont font1;
    font1.setBold(true);
    font1.setPixelSize(16);
    turnDisplay = chessScene->addText("White" , font1);
    checkDisplay = chessScene->addText("Check!" , font1);
    player1Point = chessScene->addText("0");
    player2Point = chessScene->addText("0");
    checkDisplay->setDefaultTextColor(Qt::red);
    checkDisplay->setPos(75, 900);
    player1Point->setPos(143 , 732);
    player2Point->setPos(143 , 790);
    checkDisplay->setVisible(false);
    turnDisplay->setPos(143 , 842);
    chessScene->setSceneRect(0, 0, 1861, 960);
    ui->chessboardscene->setScene(chessScene);
    ui->chessboardscene->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->chessboardscene->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //chessScene->addItem(turnDisplay);
    setBoard();
}

void Game::setWhiteMans()
{

    chessman *piece;
    for (int i = 0; i < 8; i++)
    {
        piece = new Pawn("White");
        whites.append(piece);
    }
    piece = new Rook("White");
    whites.append(piece);
    piece = new Knight("White");
    whites.append(piece);
    piece = new Bishop("White");
    whites.append(piece);
    piece = new Queen("White");
    whites.append(piece);
    piece = new King("White");
    whites.append(piece);
    piece = new Bishop("White");
    whites.append(piece);
    piece = new Knight("White");
    whites.append(piece);
    piece = new Rook("White");
    whites.append(piece);
}

void Game::setBlackMans()
{
    chessman *piece;
    piece = new Rook("Black");
    blacks.append(piece);
    piece = new Knight("Black");
    blacks.append(piece);
    piece = new Bishop("Black");
    blacks.append(piece);
    piece = new Queen("Black");
    blacks.append(piece);
    piece = new King("Black");
    blacks.append(piece);
    piece = new Bishop("Black");
    blacks.append(piece);
    piece = new Knight("Black");
    blacks.append(piece);
    piece = new Rook("Black");
    blacks.append(piece);
    for (int i = 0; i < 8; i++)
    {
        piece = new Pawn("Black");
        blacks.append(piece);
    }
}

void Game::addPiece()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {

            Cell *c = chessBoard[i][j];
            if (i < 2)
            {
                static int m;
                if (m == 16)
                    m = 0;
                c->setPiece(blacks[m]);
                alives.append(blacks[m]);
                QGraphicsItem *p = blacks[m];
                m++;
                chessScene->addItem(p);
            }
            if (i > 5)
            {
                static int n;
                if (n == 16)
                    n = 0;
                c->setPiece(whites[n]);
                alives.append(whites[n]);
                QGraphicsItem *p = whites[n];
                n++;
                chessScene->addItem(p);
            }
        }
    }
}

void Game::endGame() //exit
{
}

void Game::openReplacePawn()
{

    ReplacePawn *rp;
    rp = new ReplacePawn(this);
    rp->setModal(true);
    rp->show();
}

Game::~Game()
{
    delete ui;
}

void Game::setBoard()
{
    cb = new ChessBoard();
    board.resize(64);
    cb->drawBoard(board, 565, 0);
    for (size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            QGraphicsItem *c = board[i][j];
            chessScene->addItem(c);
            chessBoard[i][j] = board[i][j];
        }
    }
    setBlackMans();
    setWhiteMans();
}

void Game::on_undoPushButton_clicked() //undo
{
    if (movesSeries.size() == 0)
    {
        return;
    }
    QString lastMove = movesSeries.last();
    movesSeries.pop_back();
    QString piece;
    QString destination;
    QString origin;
    QString deathPiece;
    for (int i = 0; i < lastMove.size(); i++)
    {
        if (i < 2)
            piece.append(lastMove.at(i));
        else if (i > 1 && i < 6)
            destination.append(lastMove.at(i));
        else if (i == 6)
            deathPiece.append(lastMove.at(i));
        else
            origin.append(lastMove.at(i));
    }
    int i = static_cast<QString>(origin[1]).toInt();
    int j = static_cast<QString>(origin[3]).toInt();
    int m = static_cast<QString>(destination[1]).toInt();
    int n = static_cast<QString>(destination[3]).toInt();
    chessBoard[i][j]->setOccupied(false);
    if(chessBoard[i][j]->piece->getSymbol() == "P")
    {
        if(chessBoard[i][j]->piece->getColor() == "White" && (i == 4 || i == 5))
        {
            chessBoard[i][j]->piece->firstmove = true;
        }
        else if (chessBoard[i][j]->piece->getColor() == "Black" && (i == 2 || i == 3))
            chessBoard[i][j]->piece->firstmove = true;

    }
    chessBoard[m][n]->setPiece(chessBoard[i][j]->piece);
    chessBoard[i][j]->piece = nullptr;
    changeTurn();
    if (deathPiece == "N")
        return;
    else
    {
        if (piece[0] == "W")
        {
            chessBoard[i][j]->setPiece(blackDeaths.last());
            blackDeaths.pop_back();
        }
        else
        {
            chessBoard[i][j]->setPiece(whiteDeaths.last());
            whiteDeaths.pop_back();
        }
        chessBoard[i][j]->setOccupied(true);
    }
}

void Game::on_startPushButton_clicked() //start
{
    if (!startGame)
    {
        startGame = true;
        addPiece();
    }
}

void Game::on_resetPushButton_clicked()
{
    if (startGame)
    {
        resetGame = true;
        movesSeries.clear();
        alives.clear();
        turn = "White";
        whites.clear();
        blacks.clear();
        board.clear();
        chessScene->clear();
        setBoard();
        addPiece();
    }
}

void Game::on_replacePawnPushButton_clicked()
{
    if (turn == "White")
    {
        if (movingPiece && movingPiece->getSymbol() == "P" && movingPiece->getCell()->row == 0)
        {
            openReplacePawn();
        }
        else
            return;
    }
    else
    {
        if (movingPiece && movingPiece->getSymbol() == "P" && movingPiece->getCell()->row == 7)
        {
            openReplacePawn();
        }
        else
            return;
    }
}
