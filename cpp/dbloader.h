#ifndef DBLOADER_H
#define DBLOADER_H

#include <QObject>
#include <QDomDocument>
#include "playercolor.h"
#include "creature.h"
#include "scroll.h"
#include "enemyname.h"
#include <QVariantList>
#include <QtQml>
#include <QQmlListProperty>

class Library : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Creature> creatures READ creatures)

public:
    explicit Library(QObject *parent = 0);
    QDomDocument library;
    QList<PlayerColor*> lstPlayerColors;
    QList<PlayerColor*> lstPlayerColorsSorcerer;
    QList<PlayerColor*> lstPlayerColorsRogue;
    QList<PlayerColor*> lstPlayerColorsWarrior;
    QList<Creature*> lstCategory0;
    QList<Creature*> lstCategory1;
    QList<Creature*> lstCategory2;
    QList<Creature*> lstCategory3;
    QList<Creature*> lstCategory4;
    QList<Creature*> lstCheatCreatures;
    QList<Scroll*> lstScrolls;
    QList<Scroll*> lstCheatScrolls;
    QList<EnemyName*> lstEnemyNames;
    QList<Creature*> lstAllCreatures;
    void loadCreatures();
    void loadScrolls();
    void loadEnemyNames();
    void loadPlayerColors();
    Q_INVOKABLE void getAllCreatures();
    Q_INVOKABLE void loadAll();
    QQmlListProperty<Creature> creatures();
    static int creatureCount(QQmlListProperty<Creature> *list);
    static Creature* creature(QQmlListProperty<Creature> *list, int index);
signals:

public slots:

};

#endif // DBLOADER_H
