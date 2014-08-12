#include "player.h"
#include <QDebug>

Player::Player(QObject *parent) :
    Creature(parent)
{
    this->m_Speed = 1;
    int nStrDef = rand() % 3 + 6; // 6 bis 8
    this->m_Strength = rand() % (nStrDef - 4) + 2; // 2 bis nStrDef-3
    this->m_Defense = nStrDef - this->m_Strength;
    this->m_HP = rand() % 5 + 15; // 15 bis 19
    if (nStrDef == 6) this->m_HP += 4;
    if (nStrDef == 7) this->m_HP += 2;
    this->m_SpellPoints = 20;
    this->m_Player = this;
    this->m_isNPC = false;
    this->m_Species = "wizard";
    m_alive = true;
    m_hasDistanceAttack = false;
    m_RemainingMovePoints = this->m_Speed;
}

void Player::setSpellPoints(int spellPoints)
{
    m_SpellPoints = spellPoints;
    spellPointsChanged(spellPoints);
}

void Player::initializeScrolls()
{
    m_PossibleScrolls.clear();
}

void Player::initialize()
{
    qDebug() << this->m_Name << "is a" << race();
    if (race() == "rogue")
    {
        this->m_Speed = 2;
        this->m_Strength = 4;//rand() % 2 + 3; // 3 bis 4
        this->m_Defense = rand() % 2 + 6; // 6 bis 7
        this->m_HP = rand() % 3 + 15; // 15 bis 17
        this->m_SpellPoints = 20;
        this->m_Player = this;
        this->m_Species = "wizard";
        m_alive = true;
        m_hasDistanceAttack = false;
        m_RemainingMovePoints = this->m_Speed;
    }
    else if (race() == "sorcerer")
    {
        this->m_Speed = 1;
        this->m_Strength = 3; // 2
        this->m_Defense = rand() % 2 + 2; // 2 bis 3
        this->m_HP = rand() % 3 + 13; // 13 bis 15
        this->m_SpellPoints = 20;
        this->m_Player = this;
        this->m_Species = "wizard";
        m_Immune = "ALL";
        m_alive = true;
        m_hasDistanceAttack = false;
        m_RemainingMovePoints = this->m_Speed;
    }
    else if (race() == "warrior")
    {
        this->m_Speed = 1;
        this->m_Strength = 5; // 5
        this->m_Defense = rand() % 2 + 4; // 4 bis 5
        this->m_HP = rand() % 3 + 18; // 18 bis 20
        this->m_SpellPoints = 20;
        this->m_Player = this;
        this->m_Species = "wizard";
        m_alive = true;
        m_hasDistanceAttack = false;
        m_RemainingMovePoints = this->m_Speed;
    }
    else
    {
        this->m_Speed = 1;
        int nStrDef = rand() % 3 + 6; // 6 bis 8
        this->m_Strength = rand() % (nStrDef - 4) + 2; // 2 bis nStrDef-3
        this->m_Defense = nStrDef - this->m_Strength;
        this->m_HP = rand() % 5 + 15; // 15 bis 19
        if (nStrDef == 6) this->m_HP += 4;
        if (nStrDef == 7) this->m_HP += 2;
        this->m_SpellPoints = 20;
        this->m_Player = this;
        this->m_Species = "wizard";
        m_alive = true;
        m_hasDistanceAttack = false;
        m_RemainingMovePoints = this->m_Speed;
    }
    m_originalHp = this->m_HP;
}

int Player::count_possibleCreature(QQmlListProperty<Creature> *list)
{
    Player *player = qobject_cast<Player *>(list->object);
    if (player)
        return player->m_PossibleCreatures.count();
    return 0;
}

Creature* Player::at_possibleCreature(QQmlListProperty<Creature> *list, int index)
{
    Player *player = qobject_cast<Player *>(list->object);
    if (player && index < player->m_PossibleCreatures.size() && index >= 0)
        return player->m_PossibleCreatures.at(index);
    return NULL;
}

QQmlListProperty<Creature> Player::possibleCreatures()
{
    return QQmlListProperty<Creature>(this, 0, &Player::count_possibleCreature, &Player::at_possibleCreature);
}
