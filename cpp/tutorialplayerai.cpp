#include "tutorialplayerai.h"

TutorialPlayerAI::TutorialPlayerAI(Player *parent, GameService* gameService) :
    IAI(parent, gameService)
{
    passive = true;
    state = "";
}

Creature *TutorialPlayerAI::calculateSpellSelection()
{
    return NULL;
}

void TutorialPlayerAI::calculateCast(QPoint &point)
{
    point = QPoint(-1, -1);
}

IAI::MovementAction *TutorialPlayerAI::calculateMovement()
{
    return NULL;
}

bool TutorialPlayerAI::isInputPossible(int x, int y)
{
    return true;
}
