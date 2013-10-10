#include "enemyname.h"

EnemyName::EnemyName(QObject *parent) :
    QObject(parent)
{
    name = "";
    enemyClass = "";
    set = "";
}

void EnemyName::load(QDomNode *node)
{
    sourceCode = node;
    this->name = node->attributes().namedItem("name").nodeValue();
    this->enemyClass = node->attributes().namedItem("class").nodeValue();
}
