#include "settings.h"
#include "ui_settings.h"
#include "gamewindow.h"


Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    connect(ui->quitButton, &QPushButton::clicked, this, &Settings::close);
    ui->timeGoalButton->setChecked( true );
    ui->groupBox->setDisabled(true);
    ui->timeEdit->setTime( DEFAULTTIME );
    ui->timeEdit->setDisabled( true );
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_startButton_clicked()
{
    GameWindow *window = new GameWindow(this);

    if( ui->timeGoalButton->isChecked() )
    {
        window->setGameMode( TIME_GOAL );
    }
    else if( ui->passengerGoalButton->isChecked() )
    {
        window->setGameMode( PASSENGER_GOAL );
    }

    if( ui->timeCheckBox->isChecked() )
    {
        window->createGameLogic( ui->timeEdit->time() );
    }
    else
    {
        window->createGameLogic( DEFAULTTIME );
    }

    window->show();
}

void Settings::on_timeCheckBox_stateChanged(int arg1)
{
    if( !isCustomeTimeUsed_ )
    {
        ui->timeEdit->setEnabled( true ) ;
        isCustomeTimeUsed_ = true;
    }
    else
    {
        ui->timeEdit->setDisabled( true );
        isCustomeTimeUsed_ = false;
    }
}
