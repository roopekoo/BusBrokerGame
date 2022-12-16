#include "creategame.hh"
#include "gamewindow.h"
#include "ui_gamewindow.h"

#include <QtDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <typeinfo>

const int STOP_SIZE = 7;
const QBrush STOP_COLOR( Qt::yellow );
const QBrush TRAM_STOP_COLOR( Qt::red );

GameWindow::GameWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameWindow), sec_(0), min_(0)
{
    ui->setupUi(this);
    scene_ = new QGraphicsScene(this);
    const int left_margin = 1;
    const int top_margin = 1;

    Game::Coordinates c;
    ui->graphicsView->setGeometry(left_margin, top_margin,
                                  c.BORDER_RIGHT+2, c.BORDER_DOWN+2);
    ui->graphicsView->setScene(scene_);
    scene_->setSceneRect(0, 0, c.BORDER_RIGHT, c.BORDER_DOWN);

    ui->actionButton->setEnabled( true );
    ui->PointsPlaceholder->setNum(0);

    gameCity_ = std::dynamic_pointer_cast< Game::City >(
                Interface::createGame() );

    connect( gameCity_.get(), &Game::City::newStopNeededInScene, this,
             &GameWindow::addStopToScene );
    connect( gameCity_.get(), &Game::City::tramStopNeededInScene, this,
             &GameWindow::addTramStopToScene );
    connect( gameCity_.get(), &Game::City::newActorNeededInScene, this,
             &GameWindow::addActorToScene );
    connect( gameCity_.get(), &Game::City::givenMapsAreValid, this,
             &GameWindow::setMap );
    connect( gameCity_.get(), &Game::City::actorMovedInCity, this,
             &GameWindow::moveActorOnScene );


    QImage basicMap( "images/map_detailed.png" );
    QImage largeMap( "images/large_map.png" );
    gameCity_->setBackground( basicMap, largeMap );
    gameCity_->addTramStops();


    player_ = new Game::Player();
    playerIcon_ = scene_->addPixmap(QPixmap(player_->getPix()));
    movePlayerIcon();
    changeActionText();
    connect( player_, &Game::Player::tramMoved, this,
             &GameWindow::isTramNearStops );

    QPixmap dir =QPixmap("images/direction.png");
    directionIcon_ = scene_->addPixmap(dir);
    directionIcon_->setPos(998,556);
    rotateIcon();

    gameSpeed = new QTimer(this);
    gameSpeed->start(100);
    connect(gameSpeed, SIGNAL(timeout()), this, SLOT(moveQueue()));

    police_ = new Game::Police();
    policeIcon_ = scene_->addPixmap(QPixmap("images/police.png"));
    connect( police_, &Game::Police::playerCaught, gameCity_.get(),
             &Game::City::gameIsOver );
    connect( police_, &Game::Police::playerCaught, this,
             &GameWindow::showGameEnded);
    movePoliceIcon();
    movePolice();

    statistics_ = new Game::Statistics;

    playingTimer_ = new QTimer(this);
    playingTimer_->start(1000);
    connect(playingTimer_, &QTimer::timeout, this,
             &GameWindow::changePlayingTime);
}

GameWindow::~GameWindow()
{
    delete ui;
    delete gameSpeed;
    delete player_;
    delete police_;
    delete statistics_;
    statistics_ = nullptr;
    delete logic_;
    logic_ = nullptr;
}

void GameWindow::setGameMode( GameMode gameMode )
{
    gameMode_ = gameMode;

    if( gameMode == TIME_GOAL )
    {
        min_ = PLAYING_TIME/60000;
        sec_ = ((PLAYING_TIME)/1000) % 60;
        ui->lcdMinutes->display(min_);
        ui->lcdSeconds->display(sec_);

        gameEndingTimer_ = new QTimer( this );
        gameEndingTimer_->start( PLAYING_TIME );
        connect( gameEndingTimer_, &QTimer::timeout, gameCity_.get(),
                 &Game::City::gameIsOver );
        connect( gameEndingTimer_, &QTimer::timeout, this,
                 &GameWindow::showGameEnded );
    }
}

void GameWindow::createGameLogic(QTime gameTime)
{
    logic_ = new CourseSide::Logic();
    logic_->takeCity( gameCity_ );
    logic_->fileConfig();
    logic_->setTime( gameTime.hour(), gameTime.minute() );
    logic_->finalizeGameStart();
}

void GameWindow::setMap(QImage &basicbackground, QImage &bigbackground)
{
    int w = scene_->width();
    int h = scene_->height();
    scene_->addPixmap( QPixmap::fromImage( basicbackground ).scaled(
                           w,h,Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void GameWindow::on_walkerButton_clicked()
{
    playerChange(0);
    player_->changeRotation(0,false);
    rotateIcon();
    ui->rightRotateButton->setEnabled(true);
    ui->leftRotateButton->setEnabled(true);
    ui->actionButton->setEnabled(true);
}

void GameWindow::on_bikerButton_clicked()
{
    playerChange(1);
    player_->changeRotation(0,false);
    rotateIcon();
    ui->rightRotateButton->setEnabled(true);
    ui->leftRotateButton->setEnabled(true);
    ui->actionButton->setEnabled(true);
}

void GameWindow::on_tramButton_clicked()
{
    if(player_->getType() != 2)
    {
        playerChange(2);
        player_->setXY(0,468);
        player_->changeRotation(6,false);
        rotateIcon();
        movePlayerIcon();
        ui->rightRotateButton->setDisabled(true);
        ui->leftRotateButton->setDisabled(true);
        ui->actionButton->setDisabled(true);
    }
}
void GameWindow::playerChange(int type)
{
    player_->setType(type);
    playerIcon_->setPixmap(QPixmap(player_->getPix()));
    changeActionText();
    movePlayerIcon();
}
void GameWindow::changeActionText()
{
    ui->textBrowser->setFontPointSize(13);
    ui->textBrowser->setText(ACTIONTEXT.at(player_->getType()));
}
void GameWindow::movePlayerIcon()
{
    int dx = playerIcon_->boundingRect().width()/2;
    int dy = playerIcon_->boundingRect().height();
    int h = scene_->height();
    playerIcon_->setPos(player_->getX()-dx,h-player_->getY()-dy);
    //qDebug() << "X:" << player_->getX() << " Y:" << player_->getY() << " rot:" << player_->getRotation();
}

void GameWindow::rotateIcon()
{
    directionIcon_->setTransformOriginPoint(6, 6);
    int rot = player_->getRotation();
    directionIcon_->setRotation(-rot);
}

void GameWindow::keyPressEvent(QKeyEvent* event)
{
    keysPressed_ += event->key();
}

void GameWindow::keyReleaseEvent(QKeyEvent* event)
{
    keysPressed_ -= event->key();
}

void GameWindow::moveQueue()
{
    foreach(int key, keysPressed_)
    {
        if(ui->forwardButton->isEnabled() and key == Qt::Key_W)
        {
            on_forwardButton_clicked();
        }
        else if(ui->leftRotateButton->isEnabled() and key == Qt::Key_A)
        {
            on_leftRotateButton_clicked();
        }
        else if(ui->backwardButton->isEnabled() and key == Qt::Key_S)
        {
            on_backwardButton_clicked();
        }
        else if(ui->rightRotateButton->isEnabled() and key == Qt::Key_D)
        {
            on_rightRotateButton_clicked();
        }
    }
}

void GameWindow::on_forwardButton_clicked()
{
    if(!gameCity_->isGameOver())
    {
        player_->movePlayer(1);
        movePlayerIcon();
        movePolice();
    }
}

void GameWindow::on_leftRotateButton_clicked()
{
    if(!gameCity_->isGameOver())
    {
        player_->changeRotation(15, true);
        rotateIcon();
    }
}

void GameWindow::on_backwardButton_clicked()
{
    if(!gameCity_->isGameOver())
    {
        player_->movePlayer(-1);
        movePlayerIcon();
        movePolice();
    }
}

void GameWindow::on_rightRotateButton_clicked()
{
    if(!gameCity_->isGameOver())
    {
        player_->changeRotation(-15,true);
        rotateIcon();
    }
}

void GameWindow::on_actionButton_clicked()
{
    if(player_->getType() == 0)
    {
        int destroyedBuses = player_->destroyBus(gameCity_);
        for(int i = 1; i <= destroyedBuses; ++i)
        {
            statistics_->nysseRemoved();
        }
        ui->PointsPlaceholder->setNum(statistics_->getScore());
        ui->BusDestroyedScore->setText(QString::number(
                                           statistics_->getDestroyedBuses()));
    }
    else if(player_->getType() == 1)
    {
          player_->persuadePassengers( gameCity_ );
    }
    else if(player_->getType() == 2)
    {
          player_->enterOrExitPassengers( gameCity_ );
    }
}

void GameWindow::addStopToScene(std::shared_ptr<Interface::IStop> stop)
{
    Game::Coordinates c;
    double east = stop->getLocation().giveEasternCoord();
    double north = stop->getLocation().giveNorthernCoord();
    int xCoord = c.xFromEast(east);
    int yCoord = c.yFromNorth(north);
    int deltaX = STOP_SIZE/2;
    int deltaY = STOP_SIZE/2;

    if( xCoord < c.BORDER_LEFT || xCoord > c.BORDER_RIGHT ||
            yCoord < c.BORDER_UP || yCoord > c.BORDER_DOWN )
    {
        return;
    }
    scene_->addRect( xCoord - deltaX, c.BORDER_DOWN-yCoord - deltaY,
                     STOP_SIZE, STOP_SIZE, QPen(Qt::black),STOP_COLOR);
}

void GameWindow::addTramStopToScene(std::shared_ptr<Interface::IStop> tramStop)
{
    Game::Coordinates c;
    double east = tramStop->getLocation().giveEasternCoord();
    double north = tramStop->getLocation().giveNorthernCoord();
    int xCoord = c.xFromEast(east);
    int yCoord = c.yFromNorth(north);
    int deltaX = STOP_SIZE/2;
    int deltaY = STOP_SIZE/2;

    scene_->addRect(xCoord-deltaX, c.BORDER_DOWN-yCoord-deltaY,
                     STOP_SIZE, STOP_SIZE, QPen(Qt::black), TRAM_STOP_COLOR );
}

void GameWindow::addActorToScene(const std::shared_ptr<Interface::IActor> &newactor,
                                 QGraphicsPixmapItem *&newactorGraphics)
{
    Game::Coordinates c;
    double east = newactor->giveLocation().giveEasternCoord();
    double north = newactor->giveLocation().giveNorthernCoord();
    int xCoord = c.xFromEast(east);
    int yCoord = c.yFromNorth(north);
    int deltaX = 0;
    int deltaY = 0;

    if( typeid ( *newactor) == typeid ( CourseSide::Nysse) )
    {
        deltaX = BUS_PICTURE.size().width()/2;
        deltaY = BUS_PICTURE.size().height()/2;
        newactorGraphics = scene_->addPixmap( BUS_PICTURE );
    }
    else if( typeid ( *newactor ) == typeid ( CourseSide::Passenger ) )
    {
        deltaX = PASSENGER_PICTURE.size().width()/2;
        deltaY = PASSENGER_PICTURE.size().height()/2;
        newactorGraphics = scene_->addPixmap( PASSENGER_PICTURE );
    }
    newactorGraphics->setPos(xCoord-deltaX, c.BORDER_DOWN-yCoord-deltaY);
}

void GameWindow::moveActorOnScene(const std::shared_ptr<Interface::IActor>&
                                  actor, QGraphicsPixmapItem *&actorGraphics)
{
    Game::Coordinates c;
    double east = actor->giveLocation().giveEasternCoord();
    double north = actor->giveLocation().giveNorthernCoord();
    int newXCoord = c.xFromEast(east);
    int newYCoord = c.yFromNorth(north);
    int deltaX = 0;
    int deltaY = 0;

    if( typeid ( *actor) == typeid ( CourseSide::Nysse) )
    {
        deltaX = BUS_PICTURE.size().width()/2;
        deltaY = BUS_PICTURE.size().height()/2;
    }
    else if( typeid ( *actor ) == typeid ( CourseSide::Passenger ) )
    {
        deltaX = PASSENGER_PICTURE.size().width()/2;
        deltaY = PASSENGER_PICTURE.size().height()/2;
    }
    actorGraphics->setPos(newXCoord-deltaX, c.BORDER_DOWN-newYCoord-deltaY);
}

void GameWindow::isTramNearStops()
{
    ui->actionButton->setDisabled( true );

    if( player_->giveLocation().isClose(
                gameCity_->giveTramStop1()->getLocation() ) )
    {
        if( player_->getPassengers().empty() )
        {
            ui->actionButton->setEnabled( true );
        }
    }
    else if( player_->giveLocation().isClose(
                 gameCity_->giveTramStop2()->getLocation() ) )
    {
        if( !( player_->getPassengers().empty() ) )
        {
            ui->actionButton->setEnabled( true );
        }
    }
}

void GameWindow::changePlayingTime()
{
    if( gameMode_ == TIME_GOAL )
    {
        if( sec_ == 0 )
        {
            sec_ = 59;
            --min_;
            ui->lcdMinutes->display(min_);
        }
        else
        {
            --sec_;
        }
        ui->lcdSeconds->display(sec_);
    }
    else if ( gameMode_ == PASSENGER_GOAL )
    {
        if(sec_ == 59)
        {
            sec_ = 0;
            min_++;
            ui->lcdMinutes->display(min_);
        }
        else
        {
            sec_++;
        }
        ui->lcdSeconds->display(sec_);
    }
}

void GameWindow::movePoliceIcon()
{
    int h = scene_->height();
    int dy = policeIcon_->boundingRect().height()/2;
    int dx = policeIcon_->boundingRect().width()/2;
    policeIcon_->setPos(police_->getX()-dx, h-police_->getY()-dy);
}
void GameWindow::movePolice()
{
    police_->updateRotation(player_->getX(), player_->getY());
    police_->approachPlayer();
    movePoliceIcon();
    //qDebug() << "Police location: X:" << police_->getX() << " Y:" << police_->getY() << " Rotation:" << police_->getRotation();
}

void GameWindow::showGameEnded()
{
    if(gameMode_ == TIME_GOAL)
    {
         ui->lcdSeconds->display(0);
         gameEndingTimer_->stop();
    }
    gameSpeed->stop();
    playingTimer_->stop();

    ui->forwardButton->setDisabled(true);
    ui->backwardButton->setDisabled(true);
    ui->leftRotateButton->setDisabled(true);
    ui->rightRotateButton->setDisabled(true);

    ui->walkerButton->setDisabled(true);
    ui->bikerButton->setDisabled(true);
    ui->tramButton->setDisabled(true);

    ui->actionButton->setDisabled(true);
    qDebug() << "GG WP";
}


void GameWindow::on_quitButton_clicked()
{
    gameCity_->gameIsOver();
    showGameEnded();
}
