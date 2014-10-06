#include "scroll.h"
#include <QDebug>

Scroll::Scroll(QObject *parent) :
    Creature(parent)
{
}

void Scroll::load(QDomNode *node)
{
    this->sourceCode = node;
    if (node == NULL)
    {
        if (m_Species == "Healing")
        {
            loadHealing();
            return;
        }
        if (m_Species == "Mana Transfer")
        {
            loadManaTransfer();
            return;
        }
    }
    qDebug() << "loading imagefilepattern";
    m_DistanceImageFilePattern = node->attributes().namedItem("file").nodeValue();
    m_ImageFilePattern = node->attributes().namedItem("file").nodeValue();
    qDebug() << "loading jumps";
    m_jumps = node->attributes().namedItem("jumps").nodeValue().toInt();
    qDebug() << "loading manacost";
    m_ManaCost = node->attributes().namedItem("manacost").nodeValue().toInt();
    qDebug() << "loading maxdamage";
    m_maxDamage = node->attributes().namedItem("maxdam").nodeValue().toInt();
    m_DistanceStrength = m_maxDamage;
    m_minDamage = node->attributes().namedItem("mindam").nodeValue().toInt();
    m_Species = node->attributes().namedItem("name").nodeValue();
    m_range = node->attributes().namedItem("range").nodeValue().toInt();
    m_type = node->attributes().namedItem("type").nodeValue();
    m_Immune = node->attributes().namedItem("type").nodeValue();
    m_hasDistanceAttack = true;
    m_Category = -1;
}

void Scroll::loadHealing()
{
    m_DistanceImageFilename = "heart.png";
    m_DistanceImageFilePattern = "heart.png";
    m_ImageFilename = "heart.png";
    m_ImageFilePattern = "heart.png";
    m_jumps = 0;
    m_ManaCost = 15;
    m_maxDamage = 0;
    m_DistanceStrength = 0;
    m_minDamage = 0;
    m_Species = "Healing";
    m_range = 4;
    m_type = "friendly";
    m_Immune = "Healing";
    m_hasDistanceAttack = true;
    m_Category = -1;
}

void Scroll::loadManaTransfer()
{
    m_DistanceImageFilename = "heart.png";
    m_DistanceImageFilePattern = "heart.png";
    m_ImageFilename = "heart.png";
    m_ImageFilePattern = "heart.png";
    m_jumps = 0;
    m_ManaCost = 0;
    m_maxDamage = 0;
    m_DistanceStrength = 0;
    m_minDamage = 0;
    m_Species = "Mana Transfer";
    m_range = 5;
    m_type = "friendly";
    m_Immune = "Mana";
    m_hasDistanceAttack = true;
    m_Category = -1;
}
