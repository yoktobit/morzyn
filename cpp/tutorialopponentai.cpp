#include "tutorialopponentai.h"

TutorialOpponentAI::TutorialOpponentAI(Player *parent, GameService* gameService) :
    IAI(parent, gameService)
{
    passive = false;
}

Creature *TutorialOpponentAI::calculateSpellSelection()
{
    return NULL;
}

void TutorialOpponentAI::calculateCast(QPoint &point)
{
    point = QPoint(-1, -1);
}

IAI::MovementAction *TutorialOpponentAI::calculateMovement()
{
    return NULL;
}

bool TutorialOpponentAI::isInputPossible(int x, int y)
{
    return false;
}

