#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "city.hh"
#include "player.h"
#include "police.h"
#include "statistics.hh"
#include "core/location.hh"
#include "core/logic.hh"
#include "coordinates.h"

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QSet>
#include <QTimer>

enum GameMode { TIME_GOAL, PASSENGER_GOAL };

namespace Ui {
class GameWindow;
}

/**
 * @brief The GameWindow class, the window where the whole game happens.
 *
 * This class handles player and police movements, player controlling and
 * changing playertype.
 * This class also handles changes in the GUI such as changing the text or
 * point values and everything with the graphics you can see in the gameWindow.
 */
class GameWindow : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief GameWindow constructor
     * @param parent base class pointer
     *
     * Sets the background, sets default location for the player and th police.
     */
    explicit GameWindow(QWidget *parent = nullptr);

    /**
     * @brief GameWindow destructor
     *
     * Deletes dynamic pointers
     */
    ~GameWindow();

    /**
     * @brief setGameMode function
     * @param gameMode
     * @post gameMode_ has been changed to gameMode
     *
     * This class also creates connections to end timers if time_goal gamemode
     * is selected
     */
    void setGameMode( GameMode gameMode );

    /**
     * @brief createGameLogic
     * @param gameTime is the starting time in game
     *
     * Creates and initiates a logic objec that handles the game logic.
     */
    void createGameLogic( QTime gameTime );

    /**
     * @brief playerChange function
     * @param type int value of the playertype
     * @post playertype_ and playerIcon_ are changed to correct types
     *
     * Updates actiontext and playericon seen on the scene_
     */
    void playerChange(int type);

    /**
     * @brief movePlayerIcon function
     *
     * Moves playericon to correct place according to location data and is
     * centered to correct place
     */
    void movePlayerIcon();

    /**
     * @brief rotateIcon function
     *
     * Rotates rotateIcon pointing to direction the player will be moving.
     */
    void rotateIcon();

    /**
     * @brief changeActionText funktion
     *
     * Changes actionText box info-text according to the current playertype.
     */
    void changeActionText();

    /**
     * @brief movePoliceIcon funtion
     *
     * Updates policeIcon to new location on the scene from police location
     * data.
     */
    void movePoliceIcon();

    /**
     * @brief movePolice function
     *
     * Updates police rotation pointing to the player and moves to that
     * direction
     */
    void movePolice();

private slots:
    /**
     * @brief keyPressEvent
     * @param event that registers a keyboard keypress
     *
     * Runs when key is pressed in keyboard
     * calls forward or backward moving functions if key W or S is pressed
     */
    void keyPressEvent(QKeyEvent *event);

    /**
     * @brief keyReleaseEvent
     * @param event that registers a keyboard keypress
     *
     * Runs when key is released
     * Clears released keys from keysPressed if they are NOT A or D
     */
    void keyReleaseEvent(QKeyEvent *event);

    /**
     * @brief moveQueue
     *
     * Reads set (keysPressed_) and empties keysPressed on A or D
     * Causes a continuous movement on the map and must be done this way.
     * Othervice the movement couldn't handle multiple keypresses at once.
     * This will be run on every 100 ms with gameSpeed timeout-signal
     */
    void moveQueue();

    /**
     * @brief on_walkerButton_clicked
     *
     * Changes playertype to 0 (walker) and changes actionText
     */
    void on_walkerButton_clicked();

    /**
     * @brief on_bikerButton_clicked
     *
     * Changes playertype to 1 (biker) and changes actionText
     */
    void on_bikerButton_clicked();

    /**
     * @brief on_tramButton_clicked
     *
     * Changes playertype to 2 (tram) and changes actionText
     * Also teleports player to tram starting position.
     */
    void on_tramButton_clicked();

    /**
     * @brief on_forwardButton_clicked
     *
     * Player moving forward
     */
    void on_forwardButton_clicked();

    /**
     * @brief on_leftRotateButton_clicked
     *
     * Player rotating counter-clockwice
     */
    void on_leftRotateButton_clicked();

    /**
     * @brief on_backwardButton_clicked
     *
     * Player moving backwards
     */
    void on_backwardButton_clicked();

    /**
     * @brief on_rightRotateButton_clicked
     *
     * Player rotating clockwice
     */
    void on_rightRotateButton_clicked();

    /**
     * @brief on_actionButton_clicked
     *
     * Does an action depending on the playertype
     */
    void on_actionButton_clicked();
    /**
     * @brief addStopToScene
     * @param stop data pointer
     *
     * Adds stop on screen the coordinates are withing the game borders
     */
    void addStopToScene( std::shared_ptr< Interface::IStop > stop );

    /**
     * @brief addTramStopToScene
     * @param tramStop tram stop pointer
     *
     * Adds tram stop on screen
     */
    void addTramStopToScene( std::shared_ptr< Interface::IStop > tramStop );

    /**
     * @brief addActorToScene
     * @param newactor data pointer
     * @param newactorGraphics graphics for the actor
     *
     * Adds actor (bus or passenger) to screen. These actors can be outside of
     * the game borders so the game runs properly (buses coming from outside
     * have passengers)
     */
    void addActorToScene( const std::shared_ptr< Interface::IActor >& newactor,
                          QGraphicsPixmapItem*& newactorGraphics );

    /**
     * @brief setMap
     * @param basicbackground
     * @param bigbackground (Not used in this game)
     *
     * Inserts scaled map to the screen
     */
    void setMap( QImage &basicbackground, QImage &bigbackground );

    /**
     * @brief moveActorOnScene
     * @param actor data pointer
     * @param actorGraphics graphics for the actor
     *
     * Move actorgraphics to new location
     */
    void moveActorOnScene( const std::shared_ptr< Interface::IActor >& actor,
                           QGraphicsPixmapItem*& actorGraphics  );

    /**
     * @brief isTramNearStops
     *
     * Is called whenever player is tram and moves. Action button is enabled
     * when player is near tram stops, otherwise disabled.
     */
    void isTramNearStops();

    /**
     * @brief changePlayingTime
     *
     * Changes the time seen on the gamewindow
     */
    void changePlayingTime();
    /**
     * @brief showGameEnded
     *
     * shuts down game components if time is up or police chaches the player or
     * passengergoal have been achieved
     */
    void showGameEnded();

    /**
     * @brief on_quitButton_clicked
     *
     * Stops timers and closes the window.
     */
    void on_quitButton_clicked();

private:
    Ui::GameWindow *ui;
    QGraphicsScene *scene_;
    Game::Player* player_;
    Game::Police* police_;
    Interface::Location targetLocation_;
    QGraphicsPixmapItem *playerIcon_;
    QGraphicsPixmapItem *policeIcon_;
    QGraphicsPixmapItem *directionIcon_;
    QSet<int> keysPressed_;
    QTimer* gameSpeed;

    const std::vector<QString> ACTIONTEXT=
    {
        "As a walker, you can break stationary bus and all the passengers have to exit the bus.",
        "As a biker, you can manipulate escorted passengers to use the tram.",
        "As a tram driver, you can take nearby passengers in and leave them on the next stop.",
    };
    GameMode gameMode_;
    QTimer* gameEndingTimer_;
    const int PLAYING_TIME = 300000; // 5 min
    int sec_;
    int min_;
    QTimer* playingTimer_;

    std::shared_ptr< Game::City > gameCity_;
    Game::Statistics* statistics_;
    CourseSide::Logic* logic_;

    const QPixmap BUS_PICTURE = QPixmap( "images/bus.png" );
    const QPixmap PASSENGER_PICTURE = QPixmap( "images/passenger.png" );

};

#endif // GAMEWINDOW_H
