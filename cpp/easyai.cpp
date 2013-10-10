#include "easyai.h"
//#include "statemanager.h"
#include "gameservice.h"
#include "game.h"
#include <QDebug>

EasyAI::EasyAI(Player *parent, GameService *g) :
    IAI(parent, g), nCreatureIndex(0)
{
    connect(gameService, SIGNAL(playerSwitched(Player*)), SLOT(initializeForNextRound(Player*)));
    nCreatureIndex = 0;
}

Creature* EasyAI::calculateSpellSelection()
{
    qDebug() << "begin calculateSpellSelection() for " << gameService->game->currentPlayer()->name();
    Creature* creature = NULL;
    QList<Creature*> affordableCreatures;
    foreach(Creature* c, m_Player->m_PossibleCreatures)
    {
        if (gameService->canAfford(m_Player, c))
        {
            if (QString(c->metaObject()->className()).startsWith("Creature"))
            {
                // wenn nix drumherum frei ist, dann braucht er auch nix zaubern
                QList<QPoint> lstFree;
                gameService->getFreeFieldsAround(gameService->game->currentPlayer(), lstFree);
                if (lstFree.length() > 0)
                    affordableCreatures.append(c);
            }
            else
            {
                Creature *nearestEnemy = gameService->getNearestEnemy(gameService->game->currentPlayer());//gameService->is
                if (nearestEnemy && gameService->isCastable(nearestEnemy->x(), nearestEnemy->y(), c))
                {
                    affordableCreatures.append(c);
                }
            }
        }
    }
    if (affordableCreatures.length() > 0)
    {
        int rnd = gameService->randomInteger(0, affordableCreatures.length());
        creature = affordableCreatures[rnd];
    }

    qDebug() << "end calculateSpellSelection() for " << gameService->game->currentPlayer()->name();
    return creature;
}

void EasyAI::calculateCast(QPoint &point)
{
    qDebug() << "begin calculateCast() for " << gameService->game->currentPlayer()->name();
    QList<QPoint> lstPoints;
    if (QString(tmpCreature->metaObject()->className()).startsWith("Creature"))
    {
        gameService->getFreeFieldsAround(m_Player, lstPoints);
        if (lstPoints.count() == 0)
        {
            point = QPoint(-1, -1);
            qDebug() << "break calculateCast() with -1 point" << gameService->game->currentPlayer()->name();
            return;
        }
        int rndField = gameService->randomInteger(0, lstPoints.count());
        point = lstPoints[rndField];
    }
    else
    {
        Creature* nearestEnemy = gameService->getNearestEnemy(m_Player);
        if (nearestEnemy && gameService->isCastable(nearestEnemy->x(), nearestEnemy->y(), tmpCreature))
        {
            point = QPoint(nearestEnemy->x(), nearestEnemy->y());
        }
    }
    qDebug() << "end calculateCast() for " << gameService->game->currentPlayer()->name();
}

IAI::MovementAction* EasyAI::calculateMovement()
{
    qDebug() << "begin calculateMovement() for " << gameService->game->currentPlayer()->name();
    Creature* u = NULL;
    if (aAction)
        delete aAction;
    if (nCreatureIndex == -1)
        nCreatureIndex = 0;
    MovementAction *movementAction = new MovementAction();
    // Find Creature
    do {
        if (nCreatureIndex > m_Player->m_Creatures.length())
        {
            qDebug() << "No more creatures for " << m_Player->name();
            delete movementAction;
            movementAction = NULL;
            return movementAction;
        }
        if (nCreatureIndex == m_Player->m_Creatures.length())
        {
            u = m_Player;
        }
        else
        {
            u = m_Player->m_Creatures[nCreatureIndex];
        }
        if (!u->alive())
        {
            nCreatureIndex++;
            continue;
        }
        break;
    } while(1);

    Creature *nearestEnemy = gameService->getNearestEnemy(u);
    if (gameService->isEnemyAdjacent(u, nearestEnemy) && (m_PreviousAction == ACTION_MOVE || m_PreviousAction == ACTION_NOTHING))
    {
        if (u->remainingMovePoints() >= 0.5)
        {
            //u->setRemainingMovePoints(0);
            movementAction->action = ACTION_ATTACK;
            movementAction->target = nearestEnemy;
            movementAction->creature = u;
            qDebug() << "finish calculateMovement() with attack of " << gameService->game->currentPlayer()->name();
            return movementAction;
        }
    }
    QList<QPoint> lstFree;
    gameService->getFreeFieldsAround(u, lstFree);
    foreach(QPoint p, lstFree)
    {
        foreach(QPoint p2, lstWayPoints[u])
        {
            if (p == p2)
            {
                lstFree.removeAll(p);
            }
        }
    }
    if (lstFree.count() == 0)
    {
        u->setRemainingMovePoints(0);
        m_PreviousAction = ACTION_NOTHING;
        nCreatureIndex++;
        qDebug() << "break calculateMovement() without action" << gameService->game->currentPlayer()->name();
        delete movementAction;
        return NULL;
    }
    QMap<double, QPoint> lstOrderedFree;
    gameService->getOrderedFieldsByEnemy(lstFree, nearestEnemy, lstOrderedFree);
    // wenn die Einheit nicht der Spieler selbst ist, dann einfach zum ersten Feld ziehen
    if (u->player() != u || u->player()->race() != "sorcerer")
    {
        // das Feld nutzen, was am nächsten zum Gegner ist
        if (lstOrderedFree.count() > 0)
        {
            if (u->remainingMovePoints() > 0 && m_PreviousAction != ACTION_ATTACK && m_PreviousAction != ACTION_DISTANCEATTACK)
            {
                movementAction->action = ACTION_MOVE;
                movementAction->creature = u;
                movementAction->point = lstOrderedFree.begin().value();
                qDebug() << "finish calculateMovement() with movement " << gameService->game->currentPlayer()->name();
                return movementAction;
            }
        }
    }
    else // Einheit ist Sorcerer, der soll folgendes beachten #36
    {
        // 1. nächster Gegner < 3 entfernt: abhauen
        // 2. nächster Gegner = 3 entfernt: stehen bleiben
        // 3. nächster Gegner > 3 entfernt: hin ziehen
        double distance = gameService->getRealMovementDistance(u, nearestEnemy->x(), nearestEnemy->y());
        if (distance < 3 && u->player()->spellPoints() >= 5)
        {
            // das Feld nutzen, was am weitesten weg vom Gegner ist
            if (lstOrderedFree.count() > 0)
            {
                if (u->remainingMovePoints() > 0 && m_PreviousAction != ACTION_ATTACK && m_PreviousAction != ACTION_DISTANCEATTACK)
                {
                    movementAction->action = ACTION_MOVE;
                    movementAction->creature = u;
                    int nRandom = gameService->randomInteger(0, 100);
                    // 70:30-Chance, dass sie abhaut #36
                    if (nRandom <= 70)
                    {
                        movementAction->point = lstOrderedFree.values().last();
                    }
                    else
                    {
                        movementAction->point = lstOrderedFree.values().first();
                    }
                    qDebug() << "finish calculateMovement() with Abhau-movement " << gameService->game->currentPlayer()->name();
                    return movementAction;
                }
            }
        }
        else if (distance >= 3 && distance < 4 && u->player()->spellPoints() >= 5)
        {
            // vorerst nix
            qDebug() << "finish calculateMovement() nothing, cause distance is >=3 and <4";
        }
        else // distance >= 4 oder spellPoints < 5
        {
            if (u->remainingMovePoints() > 0 && m_PreviousAction != ACTION_ATTACK && m_PreviousAction != ACTION_DISTANCEATTACK)
            {
                movementAction->action = ACTION_MOVE;
                movementAction->creature = u;
                movementAction->point = lstOrderedFree.values().first();
                qDebug() << "finish calculateMovement() with Angriff-movement " << gameService->game->currentPlayer()->name();
                return movementAction;
            }
        }
    }
    // #43
    if (int(gameService->getRealMovementDistance(u, nearestEnemy->x(), nearestEnemy->y())) <= u->distanceRange())
    {
        if (m_PreviousAction != ACTION_DISTANCEATTACK && !u->hasDistanceAttacked() && gameService->isDistanceAttackable(u, nearestEnemy))
        {
            m_PreviousAction = ACTION_DISTANCEATTACK;
            movementAction->action = ACTION_DISTANCEATTACK;
            movementAction->target = nearestEnemy;
            movementAction->creature = u;
            return movementAction;
        }
    }
    m_PreviousAction = ACTION_NOTHING;
    movementAction->action = ACTION_NOTHING;
    nCreatureIndex++;
    qDebug() << "end calculateMovement() without action" << gameService->game->currentPlayer()->name();
    return movementAction;
}

void EasyAI::calculateAttack()
{

}

void EasyAI::calculateDistanceAttack()
{

}

void EasyAI::initializeForNextRound(Player *player)
{
    qDebug() << "begin initializeForNextRound() for " << player->name();
    if (gameService->game->currentPlayer() != m_Player)
    {
        qDebug() << "break initializeForNextRound() for " << player->name();
        return;
    }
    lstWayPoints.clear();
    nCreatureIndex = -1;
    m_PreviousAction = ACTION_NOTHING;
    qDebug() << "end initializeForNextRound() for " << player->name();
}
