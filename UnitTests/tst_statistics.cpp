#include "../Game/statistics.hh"
#include <QtTest>


class StatisticsTest : public QObject
{
    Q_OBJECT

public:
    StatisticsTest();
    ~StatisticsTest();

private Q_SLOTS:
    void testNysseRemoved();
    void testPassengerCaptured();

};

StatisticsTest::StatisticsTest()
{

}

StatisticsTest::~StatisticsTest()
{

}

void StatisticsTest::testNysseRemoved()
{
    Game::Statistics stats;
    int oldScore = stats.getScore();
    int oldBusesDestroyed = stats.getDestroyedBuses();

    stats.nysseRemoved();

    QVERIFY( oldScore != stats.getScore() );
    QVERIFY( stats.getDestroyedBuses() == oldScore + 1 );
}

void StatisticsTest::testPassengerCaptured()
{
    Game::Statistics stats;
    int oldScore = stats.getScore();
    int oldPassengersCaptured = stats.getPassengersCaptured();

    stats.PassengerCaptured();

    QVERIFY2( oldScore != stats.getScore(), "Score has not changed");
    QVERIFY2( oldPassengersCaptured == ( stats.getPassengersCaptured() - 1 ),
              "Captured passenger amount has not changed" );

}

QTEST_APPLESS_MAIN(StatisticsTest)

#include "tst_statistics.moc"
