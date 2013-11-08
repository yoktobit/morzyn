#ifndef TUTORIALPLAYERAI_H
#define TUTORIALPLAYERAI_H

#include "iai.h"

class TutorialPlayerAI : public IAI
{
    Q_OBJECT
public:
    explicit TutorialPlayerAI(Player *parent = 0, GameService *g = 0);
    virtual Creature* calculateSpellSelection();
    virtual void calculateCast(QPoint &point);
    virtual MovementAction* calculateMovement();
    virtual bool isInputPossible(int, int);
    QString state;
signals:

public slots:

};

#endif // TUTORIALPLAYERAI_H
