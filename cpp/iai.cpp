#include <QDebug>
#include "iai.h"
//#include "statemanager.h"
#include "gameservice.h"
#include <QSemaphore>

IAI::IAI(Player *parent, GameService* g) :
    QObject(parent), gameService(g)
{
    this->m_Player = parent;
    this->tmpCreature = NULL;
    //connect(gameService->statemanager->castState, SIGNAL(entered()), SLOT(callCast()));
    //connect(gameService->statemanager->moveState, SIGNAL(entered()), SLOT(callMovement()));
    //connect(gameService->statemanager->selectSpellState, SIGNAL(entered()), SLOT(callSpellSelection()));
    connect(gameService->game, SIGNAL(stateChanged(QString)), SLOT(stateChanged(QString)));
    m_timerMovement = new QTimer(this);
    m_timerMovement->setSingleShot(true);
    m_timerMovement->setInterval(500);
    timerCast = new QTimer(this);
    timerCast->setInterval(10);
    nCreatureIndex = 0;
    connect(m_timerMovement, SIGNAL(timeout()), SLOT(onTimerElapsedMovement()));
    aAction = NULL;
}

IAI::~IAI()
{
    delete aAction;
}

void IAI::callSpellSelection()
{
    if (gameService->game->currentPlayer() != m_Player)
    {
        return;
    }
    if (gameService->game->isOver)
    {
        this->deleteLater();
        return;
    }
    qDebug() << "begin callSpellSelection() for " << gameService->game->currentPlayer()->name();
    qDebug() << "AI selecting spell for " << m_Player->name();
    Creature* selectedSpell = NULL;
    selectedSpell = calculateSpellSelection();
    tmpCreature = selectedSpell;
    if (selectedSpell)
    {
        qDebug() << "selected spell " << selectedSpell->species();
        doSpellSelection();
        //QTimer::singleShot(1, this, SLOT(doSpellSelection()));
    }
    else
    {
        qDebug() << "no spell selected/found";
        //gameService->game->setState("moveState");
        gameService->abort();
        //QTimer::singleShot(1, gameService, SLOT(abort()));
        //gameService->statemanager->gotoMoveState();
    }
    qDebug() << "end callSpellSelection() for " << gameService->game->currentPlayer()->name();
}

void IAI::callCast()
{
    if (gameService->game->currentPlayer() != m_Player)
    {
        return;
    }
    if (gameService->game->isOver)
    {
        this->deleteLater();
        return;
    }
    qDebug() << "begin callCast() for " << gameService->game->currentPlayer()->name();
    QPoint p(-1, -1);
    calculateCast(p);
    tmpPoint = p;
    if (p.x() != -1)
    {
        QTimer::singleShot(1, this, SLOT(doCast()));
    }
    else
    {
        QTimer::singleShot(1, gameService, SLOT(abort()));
        //gameService->game->setState("moveState");//gameService->statemanager->gotoMoveState();
    }
    qDebug() << "end callCast() for " << gameService->game->currentPlayer()->name();
}

void IAI::callMovement()
{
    if (gameService->game->currentPlayer() != m_Player)
    {
        return;
    }
    if (gameService->game->isOver)
    {
        this->deleteLater();
        return;
    }
    qDebug() << "begin callMovement() for " << gameService->game->currentPlayer()->name();
    startShortTimer();
    qDebug() << "end callMovement() for " << gameService->game->currentPlayer()->name();
}

void IAI::callAttack()
{
    if (gameService->game->currentPlayer() != m_Player) return;
}

void IAI::callDistanceAttack()
{
    if (gameService->game->currentPlayer() != m_Player) return;
}

void IAI::doSpellSelection()
{
    if (gameService->game->isOver)
    {
        this->deleteLater();
        return;
    }
    Creature* creature = tmpCreature;
    qDebug() << "begin doSpellSelection() for " << gameService->game->currentPlayer()->name();
    gameService->selectSpell(creature);
    qDebug() << "end doSpellSelection() for " << gameService->game->currentPlayer()->name();
}

void IAI::doCast()
{
    if (gameService->game->isOver)
    {
        this->deleteLater();
        return;
    }
    QPoint point = tmpPoint;
    qDebug() << "begin doCast() for " << gameService->game->currentPlayer()->name();
    gameService->tryCast(point.x(), point.y(), tmpCreature);
    tmpCreature = NULL;
    qDebug() << "end doCast() for " << gameService->game->currentPlayer()->name();
}

void IAI::doMovement(MovementAction* aAction)
{
    if (gameService->game->isOver)
    {
        this->deleteLater();
        return;
    }
    qDebug() << "begin doMovement() for " << gameService->game->currentPlayer()->name();
    if (!aAction)
    {
        qDebug() << "no action given for " << gameService->game->currentPlayer()->name() << "; deselecting and switching player...";
        gameService->deselectAll();
        QTimer::singleShot(5, gameService, SLOT(switchNextPlayer()));
        qDebug() << "end doMovement() for " << gameService->game->currentPlayer()->name();
        return;
    }
    switch (aAction->action)
    {
        case ACTION_MOVE:
            qDebug() << "moving " << aAction->creature->species() << " to " << aAction->point.x() << "," << aAction->point.y();
            if (gameService->game->selectedCreature() != aAction->creature)
                gameService->selectCreature(aAction->creature);
            QTimer::singleShot(1, this, SLOT(moveCreature()));
            m_PreviousAction = ACTION_MOVE;
            break;
        case ACTION_ATTACK:
            qDebug() << "attacking " << aAction->target->species() << " with " << aAction->creature->species();
            if (gameService->game->selectedCreature() != aAction->creature)
                gameService->selectCreature(aAction->creature);
            QTimer::singleShot(1, this, SLOT(attackCreature()));
            m_PreviousAction = ACTION_ATTACK;
            break;
        case ACTION_DISTANCEATTACK:
            qDebug() << "distance attacking " << aAction->target->species() << " with " << aAction->creature->species();
            if (gameService->game->selectedCreature() != aAction->creature)
                gameService->selectCreature(aAction->creature);
            QTimer::singleShot(1, this, SLOT(distanceAttackCreature()));
            m_PreviousAction = ACTION_DISTANCEATTACK;
            break;
        case ACTION_NOTHING:
            gameService->deselectAll();
            startShortTimer();
            m_PreviousAction = ACTION_NOTHING;
            break;
        default:
            qDebug() << "unknown action given for " << gameService->game->currentPlayer()->name() << "; deselecting and switching player...";
            gameService->deselectAll();
            gameService->switchNextPlayer();
            break;
    }
    qDebug() << "end doMovement() for " << gameService->game->currentPlayer()->name();
}

void IAI::moveCreature()
{
    if (gameService->game->isOver)
    {
        this->deleteLater();
        return;
    }
    gameService->moveCreature(aAction->creature, aAction->point.x(), aAction->point.y());
    startShortTimer();
}

void IAI::attackCreature()
{
    if (gameService->game->isOver)
    {
        this->deleteLater();
        return;
    }
    gameService->attackCreature(aAction->target);
    //if (gameService->game->isCreatureDying())
    //    startLongTimer();
    //else {
    if (aAction->creature->hasDistanceAttack())
    {
        start1000Timer();
    }
    else
    {
        startShortTimer();
    }
    //}
}

void IAI::distanceAttackCreature()
{
    if (gameService->game->isOver)
    {
        this->deleteLater();
        return;
    }
    gameService->distanceAttackCreature(aAction->target);
    /*if (gameService->game->isCreatureDying())
        startLongTimer();
    else*/
    startShortTimer();
}

void IAI::startLongTimer()
{
    m_timerMovement->setInterval(2000);
    m_timerMovement->start();
}

void IAI::startShortTimer()
{
    m_timerMovement->setInterval(500);
    m_timerMovement->start();
}

void IAI::start600Timer()
{
    m_timerMovement->setInterval(610);
    m_timerMovement->start();
}

void IAI::start1000Timer()
{
    m_timerMovement->setInterval(1000);
    m_timerMovement->start();
}

void IAI::start100Timer()
{
    m_timerMovement->setInterval(50);
    m_timerMovement->start();
}

void IAI::doAttack()
{

}

void IAI::doDistanceAttack()
{

}

void IAI::onTimerElapsedMovement()
{
    qDebug() << "begin onTimerElapsedMovement";
    if (gameService->game->isOver || !gameService->game->hasBegun())
    {
        qDebug() << "breaking onTimerElapsedMovement, cause game is over";
        this->deleteLater();
        return;
    }
    if (nCreatureIndex >= 0 && gameService->game->isLocked())
    {
        qDebug() << "waiting for animation completion";
        start100Timer();
    }
    else
    {
        aAction = calculateMovement();
        doMovement(aAction);
    }
    qDebug() << "end onTimerElapsedMovement";
}

void IAI::stateChanged(QString state)
{
    if (gameService->game->isOver)
    {
        this->deleteLater();
        return;
    }
    if (state == "moveState")
    {
        callMovement();
    }
    else if (state == "spellSelectState")
    {
        callSpellSelection();
    }
    else if (state == "castSpellState")
    {
        callCast();
    }
}
