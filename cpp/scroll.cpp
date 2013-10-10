#include "scroll.h"
#include <QDebug>

Scroll::Scroll(QObject *parent) :
    Creature(parent)
{
}

void Scroll::load(QDomNode *node)
{
    this->sourceCode = node;
    qDebug() << "loading imagefilepattern";
    m_DistanceImageFilePattern = node->attributes().namedItem("file").nodeValue();
    m_ImageFilePattern = node->attributes().namedItem("file").nodeValue();
    qDebug() << "loading jumps";
    m_jumps = node->attributes().namedItem("jumps").nodeValue().toInt();
    qDebug() << "loading manacost";
    m_ManaCost = node->attributes().namedItem("manacost").nodeValue().toInt();
    qDebug() << "loading maxdamage";
    m_maxDamage = node->attributes().namedItem("maxdam").nodeValue().toInt();
    m_minDamage = node->attributes().namedItem("mindam").nodeValue().toInt();
    m_Species = node->attributes().namedItem("name").nodeValue();
    m_range = node->attributes().namedItem("range").nodeValue().toInt();
    m_type = node->attributes().namedItem("type").nodeValue();
    m_Immune = node->attributes().namedItem("type").nodeValue();
    m_hasDistanceAttack = true;
}
