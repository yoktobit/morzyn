#ifndef EASYAI_H
#define EASYAI_H

#include "iai.h"
#include "player.h"
#include "game.h"
#include <QMap>
#include <QList>
#include <QPoint>

class EasyAI : public IAI
{
    Q_OBJECT
public:
    explicit EasyAI(Player *parent = 0, GameService *g = 0);
    virtual Creature* calculateSpellSelection();
    virtual void calculateCast(QPoint &point);
    virtual MovementAction* calculateMovement();
    virtual void calculateAttack();
    virtual void calculateDistanceAttack();
    int nCreatureIndex;
    QMap<Creature*, QList<QPoint> > lstWayPoints;
private:
signals:

public slots:
    void initializeForNextRound(Player *);
};

#endif // EASYAI_H
