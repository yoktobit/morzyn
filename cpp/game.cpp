#include "game.h"
#include <limits.h>

Game::Game(QObject *parent) :
    QObject(parent)
{
    totalPlayers = 0;
    humanPlayers = 0;
    m_selectedCreature = NULL;
    m_currentPlayer = NULL;
    isOver = false;
    m_hasBegun = false;
    m_state = "titleScreenState";
    m_isCreatureDying = false;
    m_isLocked = false;
    m_semaphore = new QSemaphore(MAX_ANIMATION);
    m_manaMode = false;
}

void Game::append_creature(QQmlListProperty<Creature> *list, Creature *creature)
{
    Game *game = qobject_cast<Game *>(list->object);
    if (game) {
        creature->setParent(game);
        game->m_creatures.append(creature);
        game->creaturesChanged();
    }
}

int Game::count_creature(QQmlListProperty<Creature> *list)
{
    Game *game = qobject_cast<Game *>(list->object);
    if (game)
        return game->m_creatures.count();
    return 0;
}

Creature* Game::at_creature(QQmlListProperty<Creature> *list, int index)
{
    Game *game = qobject_cast<Game *>(list->object);
    if (game)
        return game->m_creatures.at(index);
    return NULL;
}

void Game::clear_creature(QQmlListProperty<Creature> *list)
{
    Game *game = qobject_cast<Game *>(list->object);
    if (game)
        game->m_creatures.clear();
}

void Game::append_player(QQmlListProperty<Player> *list, Player *creature)
{
    Game *game = qobject_cast<Game *>(list->object);
    if (game) {
        creature->setParent(game);
        game->m_players.append(creature);
        game->playersChanged();
    }
}

int Game::count_player(QQmlListProperty<Player> *list)
{
    Game *game = qobject_cast<Game *>(list->object);
    if (game)
        return game->m_players.count();
    return 0;
}

Player* Game::at_player(QQmlListProperty<Player> *list, int index)
{
    Game *game = qobject_cast<Game *>(list->object);
    if (game)
        return game->m_players.at(index);
    return NULL;
}

void Game::clear_player(QQmlListProperty<Player> *list)
{
    Game *game = qobject_cast<Game *>(list->object);
    if (game)
        game->m_players.clear();
}
