#ifndef IAI_H
#define IAI_H

#include <QObject>
#include <QTimer>
#include <QPoint>
#include "gameservice.h"
#include "player.h"
//#include "statemanager.h"

class IAI : public QObject
{
    Q_OBJECT
public:
    enum Action {
       ACTION_ATTACK, ACTION_MOVE, ACTION_DISTANCEATTACK, ACTION_NOTHING
    };
    struct MovementAction {
        Action action;
        Creature* target;
        Creature* creature;
        QPoint point;
    };
    explicit IAI(Player *parent = 0, GameService *g = 0);
    ~IAI();
    QTimer *m_timerMovement;
    QTimer *timerCast;
    bool timerCastInitialized;
    QTimer *timerAttack;
    QTimer *timerDistanceAttack;
    Player *m_Player;
    Action m_PreviousAction;
    Creature *tmpCreature;
    QPoint tmpPoint;
    int nCreatureIndex;
    bool passive;
    MovementAction* aAction;
    //QMap<Creature*, QList<QPoint> > m_lstWayPoints;
    QList<Creature*> m_lstDoneDistanceAttacks;
    virtual Creature* calculateSpellSelection() { return NULL; }
    virtual void calculateCast(QPoint&) { }
    virtual MovementAction* calculateMovement() { return NULL; }
    virtual void calculateAttack() {}
    virtual void calculateDistanceAttack() {}
    void doMovement(MovementAction* aAction);
    void doAttack();
    void doDistanceAttack();
    GameService *gameService;
    virtual bool isInputPossible(int, int) { return false; };
private:

public slots:
    void callSpellSelection();
    void callCast();
    void callMovement();
    void callAttack();
    void callDistanceAttack();
    void onTimerElapsedMovement();
    void stateChanged(QString state);
    void moveCreature();
    void attackCreature();
    void distanceAttackCreature();
    void startLongTimer();
    void startShortTimer();
    void start600Timer();
    void start1000Timer();
    void start100Timer();
    void doSpellSelection();
    void doCast();
};

#endif // IAI_H
