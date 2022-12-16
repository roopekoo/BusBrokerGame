#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QTime>

namespace Ui {
class Settings;
}
/**
 * @brief The Settings class
 *
 * Dialog window where the user can select gamemode, start the game or quit.
 */
class Settings : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Settings constructor
     * @param parent base class pointer
     *
     * Sets up buttons and connections
     */
    explicit Settings(QWidget *parent = nullptr);

    /**
     * @brief Settings destructor
     *
     * Removes window.
     */
    ~Settings();

private slots:
    /**
     * @brief on_startButton_clicked
     * Opens the gameWindow with a correct gamemode
     */
    void on_startButton_clicked();

    void on_timeCheckBox_stateChanged(int arg1);

private:
    Ui::Settings *ui;
    const QTime DEFAULTTIME = QTime( 7, 20 );
    bool isCustomeTimeUsed_ = false;
};

#endif // SETTINGS_H
