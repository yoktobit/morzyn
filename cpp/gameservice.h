#ifndef GAMESERVICE_H
#define GAMESERVICE_H

#include <QList>
#include <QPoint>
#include <QMap>
//#include "statemanager.h"
#include "game.h"
#include "player.h"
#include "constants.h"
#include "dbloader.h"
#include "statistics.h"
#include <QSettings>
#include <QtQuick/QQuickView>
#include <QMediaPlayer>

class GameService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)

public:
    explicit GameService(QObject *parent = 0);
    Q_INVOKABLE QList<int> generateDistinctNumberList(int count, int min, int max);
    Q_INVOKABLE void addPlayer(Player* player);
    Q_INVOKABLE void addPlayers(int nTotalPlayerCount, int nHumanPlayerCount);
    Q_INVOKABLE void placePlayers();
    Q_INVOKABLE void loadPlayerSpells(Player* player);
    Q_INVOKABLE void setCreatureImage(Creature* creature);
    Q_INVOKABLE bool canAfford(Player *p, Creature *c);
    Q_INVOKABLE void selectSpell(Creature *creature);
    Q_INVOKABLE void trySelectCreature(Creature* creature);
    Q_INVOKABLE void selectCreature(Creature* creature);
    Q_INVOKABLE bool isSelectable(Creature* creature);
    Q_INVOKABLE void resetMovementPoints(Creature *creature);
    Q_INVOKABLE double getDistance(int sourceX, int sourceY, int targetX, int targetY);
    Q_INVOKABLE double getDistance(Creature *creature, int x, int y);
    Q_INVOKABLE bool isMovementPossible(Creature *creature, int x, int y, bool message);
    Q_INVOKABLE double getRealMovementDistance(Creature *creature, int x, int y);
    Q_INVOKABLE void tryMoveCreature(Creature *creature, int x, int y);
    Q_INVOKABLE void moveCreature(Creature *creature, int x, int y);
    Q_INVOKABLE bool isCreatureNearEnemy(Creature *creature);
    Q_INVOKABLE bool isAttackable(Creature *creature);
    Q_INVOKABLE bool isAttackable(Creature *attacker, Creature *attacked);
    Q_INVOKABLE void tryAttacks(Creature *creature);
    Q_INVOKABLE bool tryAttackCreature(Creature *creature);
    Q_INVOKABLE void attackCreature(Creature *creature);
    Q_INVOKABLE bool isDistanceAttackable(Creature *creature);
    Q_INVOKABLE bool isDistanceAttackable(Creature *attacker, Creature* attacked);
    Q_INVOKABLE void tryDistanceAttackCreature(Creature *creature);
    Q_INVOKABLE void distanceAttackCreature(Creature *creature);
    Q_INVOKABLE void scrollAttackCreature(Scroll* scroll, Creature *creature);
    Q_INVOKABLE void tryCast(int x, int y, Creature* creature);
    Q_INVOKABLE void tryBuy(Creature* creature);
    Q_INVOKABLE void buy(Creature* creature);
    Q_INVOKABLE bool isCastable(int x, int y, Creature* creature);
    Q_INVOKABLE void castCreature(int x, int y, Creature* creature);
    Q_INVOKABLE void deselectAll();
    Q_INVOKABLE void tryKillCreature(Creature* c, bool moveToWhenDead);
    Q_INVOKABLE bool isKillable(Creature* c);
    Q_INVOKABLE void killCreature(Creature* c);
    Q_INVOKABLE void tryGameOver();
    Q_INVOKABLE bool isGameOver();
    Q_INVOKABLE void goGameOver();
    Q_INVOKABLE int randomInteger(int min, int max);
    Q_INVOKABLE double randomDouble();
    Q_INVOKABLE int calculateDamage(Creature* attacker, Creature *attacked);
    Q_INVOKABLE  int calculateSpellPointBonus(int nDamage);
    Q_INVOKABLE int calculateDistanceDamage(Creature* attacker, Creature *attacked);
    Q_INVOKABLE int calculateDistanceSpellPointBonus(int nDamage);
    Q_INVOKABLE void applyDamage(Creature *attacked, int nDamage, bool moveToWhenDead);
    Q_INVOKABLE void applySpellPointBonus(Player *player, int nSpellPointBonus);
    Q_INVOKABLE Creature *getCreatureAt(int x, int y);
    Q_INVOKABLE void getFreeFieldsAround(Creature *creature, QList<QPoint> &lstFree);
    Q_INVOKABLE Creature* getNearestEnemy(Creature *creature);
    Q_INVOKABLE void getOrderedFieldsByEnemy(const QList<QPoint> &lstFree, Creature *creature, QMap<double, QPoint> &map);
    Q_INVOKABLE bool isEnemyAdjacent(Creature *creature, Creature *enemy);
    Q_INVOKABLE bool isEnemy(Creature *creature);
    Q_INVOKABLE QString getRandomCreatureName(Player* player);
    Q_INVOKABLE void checkCheat(QString strNewCheatCharacter);
    Q_INVOKABLE void setCreatureImage(int index, int colorIndex, QString category);
    Q_INVOKABLE void getEnemiesAround(Creature* creature, QList<Creature*> &lstEnemies);
    Q_INVOKABLE void playTitleSong(bool startstop);
    Q_INVOKABLE void getRandomPlayerColor(Player *player);
    Q_INVOKABLE bool checkDuplicates();
    Q_INVOKABLE int getColorIndex(QString imageFilename);
    Q_INVOKABLE void setFullScreen(bool fullscreen);
    Q_INVOKABLE void simulateFight(Creature* c1, Creature* c2);
    Q_INVOKABLE QStringList getCreatureImages(QString filenamePattern);
    Q_INVOKABLE IAI* getAI(Player* player);
    Q_INVOKABLE void saveSettings(bool bMusic, bool bSound, bool bFullscreen);
    Q_INVOKABLE bool getBoolSetting(QString name);
    Q_INVOKABLE void setBoolSetting(QString name, bool bValue);
    Q_INVOKABLE void setGameMode(QString mode);
    Q_INVOKABLE void initTutorialGame();
    Q_INVOKABLE void addTutorialPlayers();
    Q_INVOKABLE void addTutorialPlayer(Player* player);
    Q_INVOKABLE void placeTutorialPlayers();
    Q_INVOKABLE void quit();
    Q_INVOKABLE bool getFullScreen();
    Q_INVOKABLE QColor getColorOfEmptyField(int index, bool isLocked, int x, int y, Creature *selectedCreature, Player *currentPlayer, QString state);

    void emitCreatureMoved(Creature* creature);
    void emitPlayerSwitched(Player* player);
    void emitSpellSelected(Creature* creature);
    void emitCreatureCasted(Creature* creature);
    void emitCreatureHovered(QObject* creature);
    void emitCreatureUnhovered();
    void emitCreatureSelected(Creature *creature);
    void emitDeselectAll();
    void emitGameOver();
    void emitCreatureAttacked(Creature *attackingCreature, Creature *attackedCreature, int nDamage);
    void emitCreatureDied(Creature *dyingcreature);
    void emitPlayerDied(Creature *creature);
    void gotoMoveState();
    void emitCreatureDistanceAttacked(Creature *attackingCreature, Creature *attackedCreature, int nDamage);

    QMediaPlayer* titleSound;
    bool previousPlayedTitleSound;

    //void setStateManager(StateManager* sm);
    void setGame(Game* g);
    void setConstants(Constants* c);
    void setLibrary(Library* l);
    void setStatistics(Statistics* s);
    void setSettings(QSettings* s);
    //StateManager* statemanager;
    Game* game;
    Constants* config;
    Library* library;
    Statistics* statistics;
    QSettings* settings;
    QString message() const
    {
        return m_message;
    }

    Q_INVOKABLE bool isTitleSongPlaying();
private:

    QString m_message;

signals:
    void creatureMoved(Creature* creature);
    void playerSwitched(Player* player);
    void spellSelected(Creature*);
    void creatureCasted(Creature* creature);
    void scrollCasted(Scroll*);
    void creatureHovered(QObject*);
    void creatureUnhovered();
    void moveStateGo();
    void creatureSelected(Creature* creature);
    void creatureAttacked(Creature *attackingCreature, Creature *attackedCreature, int nDamage);
    void creatureDistanceAttacked(Creature *attackingCreature, Creature *attackedCreature, int nDamage);
    void creatureDied(Creature *dyingcreature);
    void playerDied(Creature *creature);
    void gameOver();
    void stateChanged(QString arg);
    void gameReset();
    void playersCreaturesDie(Player* player);
    void messageChanged(QString arg);

public slots:
    void setTotalPlayers(int number);
    void setHumanPlayers(int number);
    void addCreature(Creature* c, int x, int y);
    void castScroll(Scroll* s, int x, int y);
    void initPlayerForNewRound(Player *player);
    void switchNextPlayer();
    void resetGame();
    void gameStateChanged(QString state);
    Q_INVOKABLE void abort(Creature* creature = NULL);
    Q_INVOKABLE void start();
    Q_INVOKABLE void emitPlayersCreaturesDie(Player* player);
    void setMessage(QString arg)
    {
        if (m_message != arg) {
            m_message = arg;
            emit messageChanged(arg);
        }
    }
};

#endif // GAMESERVICE_H
