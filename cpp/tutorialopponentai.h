#ifndef TUTORIALOPPONENTAI_H
#define TUTORIALOPPONENTAI_H

#include "iai.h"

class TutorialOpponentAI : public IAI
{
    Q_OBJECT
public:
    explicit TutorialOpponentAI(Player *parent = 0, GameService* gameService = 0);

    virtual Creature* calculateSpellSelection();
    virtual void calculateCast(QPoint &point);
    virtual MovementAction* calculateMovement();
    virtual bool isInputPossible(int, int);
    QString state;

signals:

public slots:

};

#endif // TUTORIALOPPONENTAI_H
