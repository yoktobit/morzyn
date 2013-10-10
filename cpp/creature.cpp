#include "creature.h"

Creature::Creature(QObject *parent) :
    QObject(parent)
{
    this->m_hasMoved = false;
}

void Creature::load(QDomNode *node)
{
    this->sourceCode = node;
    m_Species = node->attributes().namedItem("name").nodeValue();
    m_Category = node->attributes().namedItem("category").nodeValue().toInt();
    m_ManaCost = node->attributes().namedItem("manacost").nodeValue().toInt();
    m_HP = node->attributes().namedItem("life").nodeValue().toInt();
    m_originalHp = m_HP;
    m_Strength = node->attributes().namedItem("attack").nodeValue().toInt();
    m_Defense = node->attributes().namedItem("defense").nodeValue().toInt();
    m_Speed = node->attributes().namedItem("speed").nodeValue().toInt();
    m_hasDistanceAttack = node->attributes().namedItem("distattack").nodeValue().toLower() == "true";
    m_DistanceRange = node->attributes().namedItem("distrange").nodeValue().toInt();
    m_DistanceStrength = node->attributes().namedItem("distdam").nodeValue().toInt();
    m_Immune = node->attributes().namedItem("immune").nodeValue();
    m_ImageFilePattern = node->attributes().namedItem("file").nodeValue();
    m_Cheat = node->attributes().namedItem("cheat").nodeValue();
    m_DistanceImageFilePattern = node->attributes().namedItem("distfile").nodeValue();
    m_Player = NULL;
    m_RemainingMovePoints = m_Speed;
    m_hasMoved = false;
    m_hasAttacked = false;
    m_hasDistanceAttacked = false;
    m_alive = true;
    m_distanceAttackMode = false;
    emit propertyChanged();
}
