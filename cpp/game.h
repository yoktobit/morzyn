#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QMap>
#include "player.h"
#include <QQmlListProperty>
#include <QSemaphore>
#include <QTimer>
#include <limits.h>
#include <QDebug>
#include <QStringList>

#define MAX_ANIMATION 100

class IAI;

class Game : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Player> players READ players NOTIFY playersChanged)
    Q_PROPERTY(QQmlListProperty<Creature> creatures READ creatures NOTIFY creaturesChanged)
    Q_PROPERTY(QQmlListProperty<Scroll> scrolls READ scrolls NOTIFY scrollsChanged)
    Q_PROPERTY(Player* currentPlayer READ currentPlayer WRITE setCurrentPlayer NOTIFY currentPlayerChanged)
    Q_PROPERTY(Creature* selectedCreature READ selectedCreature WRITE setSelectedCreature NOTIFY selectedCreatureChanged)
    Q_PROPERTY(QString state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(bool isCreatureDying READ isCreatureDying WRITE setIsCreatureDying NOTIFY isCreatureDyingChanged)
    Q_PROPERTY(bool isLocked READ isLocked NOTIFY lockedChanged)
    Q_PROPERTY(int lastDamage READ lastDamage WRITE setLastDamage NOTIFY lastDamageChanged)
    Q_PROPERTY(bool hasBegun READ hasBegun WRITE setHasBegun NOTIFY hasBegunChanged)
    Q_PROPERTY(bool manaMode READ manaMode WRITE setManaMode NOTIFY manaModeChanged)
    Q_PROPERTY(bool fullScreen READ fullScreen WRITE setFullScreen NOTIFY fullScreenChanged)

public:
    explicit Game(QObject *parent = 0);
    void reset();
    QSemaphore *m_semaphore;
    QList<Player*> m_players;
    QList<Creature*> m_creatures;
    QList<Scroll*> m_scrolls;
    int totalPlayers;
    int humanPlayers;
    Creature *m_selectedCreature;
    QMap<Player*,IAI*> m_AI;
    QSet<int> arrUsedPlayerColors;
    bool isOver;
    bool m_hasBegun;
    QString mode;
    QString strCurrentCheat;
    QStringList arrLastPlayerNames;
    QQmlListProperty<Player> players()
    {
        return QQmlListProperty<Player>(this, 0, &Game::append_player, &Game::count_player, &Game::at_player, &Game::clear_player);
    }
    QQmlListProperty<Creature> creatures()
    {
        return QQmlListProperty<Creature>(this, 0, &Game::append_creature, &Game::count_creature, &Game::at_creature, &Game::clear_creature);
    }
    QQmlListProperty<Scroll> scrolls()
    {
        return QQmlListProperty<Scroll>(this, m_scrolls);
    }
    int playerCount() const {
        return m_players.count();
    }
    Player *player(int ii) const {
        return m_players.at(ii);
    }
    void emitPropertyChanged() { emit propertyChanged(); }
    void emitCreaturesChanged() { emit creaturesChanged(); }

    Player* currentPlayer() const
    {
        return m_currentPlayer;
    }

    Creature* selectedCreature() const
    {
        return m_selectedCreature;
    }

    QString state() const
    {
        return m_state;
    }

    QString previousState() const
    {
        return m_previousState;
    }

    bool isCreatureDying() const
    {
        return m_isCreatureDying;
    }

    bool isLocked()
    {
        qDebug() << "Available: " << available() << "max: " << MAX_ANIMATION;
        return available() < MAX_ANIMATION;
    }

    int lastDamage() const
    {
        return m_lastDamage;
    }

    bool hasBegun() const
    {
        return m_hasBegun;
    }

    bool manaMode() const
    {
        return m_manaMode;
    }

    bool fullScreen() const
    {
        return m_fullScreen;
    }

signals:
    void propertyChanged();
    void currentPlayerChanged(Player* arg);

    void selectedCreatureChanged(Creature* arg);

    void stateChanged(QString arg);

    //void creaturesChanged(QQmlListProperty<Creature> arg);

    void isCreatureDyingChanged(bool arg);

    void creaturesChanged();
    void playersChanged();
    void scrollsChanged();

    void lockedChanged(bool arg);

    void lastDamageChanged(int arg);

public slots:

void setCurrentPlayer(Player* arg)
{
    if (m_currentPlayer != arg) {
        m_currentPlayer = arg;
        emit currentPlayerChanged(arg);
    }
}

void setSelectedCreature(Creature* arg)
{
    if (m_selectedCreature != arg) {
        m_selectedCreature = arg;
        emit selectedCreatureChanged(arg);
    }
}

void setState(QString arg)
{
    if (m_state != arg) {
        m_previousState = m_state;
        m_state = arg;
        emit stateChanged(arg);
    }
}

Q_INVOKABLE void setIsCreatureDying(bool arg)
{
    if (m_isCreatureDying != arg) {
        m_isCreatureDying = arg;
        emit isCreatureDyingChanged(arg);
    }
}


void setIsLocked(bool arg)
{
    if (m_isLocked != arg) {
        m_isLocked = arg;
        emit lockedChanged(arg);
    }
}

void setLastDamage(int arg)
{
    if (m_lastDamage != arg) {
        m_lastDamage = arg;
        emit lastDamageChanged(arg);
    }
}

Q_INVOKABLE void aquire()
{
    if (m_semaphore->tryAcquire(1, 1000))
        emit lockedChanged(true);
}

Q_INVOKABLE int available()
{
    return m_semaphore->available();
}

void throwAvailable() {
    emit onAvailable();
}

Q_INVOKABLE void release()
{
    m_semaphore->release();
    emit lockedChanged(false);
    if (m_semaphore->available() == MAX_ANIMATION)
    {
        QTimer::singleShot(50, this, SLOT(throwAvailable()));
    }
}

void setHasBegun(bool arg)
{
    if (m_hasBegun != arg) {
        m_hasBegun = arg;
        emit hasBegunChanged(arg);
    }
}

void setManaMode(bool arg)
{
    if (m_manaMode != arg) {
        m_manaMode = arg;
        emit manaModeChanged(arg);
    }
}

void setFullScreen(bool arg)
{
    if (m_fullScreen != arg) {
        m_fullScreen = arg;
        emit fullScreenChanged(arg);
    }
}

private:

Player* m_currentPlayer;
QString m_state;
QString m_previousState;
bool m_isCreatureDying;
QTimer* m_castTimer;

static void append_creature(QQmlListProperty<Creature> *list, Creature *creature);
static int count_creature(QQmlListProperty<Creature> *list);
static Creature *at_creature(QQmlListProperty<Creature> *list, int index);
static void clear_creature(QQmlListProperty<Creature> *list);

static void append_player(QQmlListProperty<Player> *list, Player *creature);
static int count_player(QQmlListProperty<Player> *list);
static Player *at_player(QQmlListProperty<Player> *list, int index);
static void clear_player(QQmlListProperty<Player> *list);

bool m_isLocked;
int m_lastDamage;

bool m_manaMode;

bool m_fullScreen;

signals:
void onAvailable();
void hasBegunChanged(bool arg);
void manaModeChanged(bool arg);
void fullScreenChanged(bool arg);
};

#endif // GAME_H
