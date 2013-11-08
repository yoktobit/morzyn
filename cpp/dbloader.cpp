#include "dbloader.h"
#include <QFile>
#include <QList>
#include <QtXml>
#include "playercolor.h"

#ifndef DEPLOYMENT_PATH
#define DEPLOYMENT_PATH ""
#endif

Library::Library(QObject *parent) :
    QObject(parent)
{
    library = QDomDocument("mydocument");
#ifdef Q_OS_ANDROID
    QFile file("assets:/qml/morzyn/database/database.xml");
#else
    QFile file(QString(DEPLOYMENT_PATH) + QString("qml/morzyn/database/database.xml"));
#endif

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << file.fileName() << "does not exist";
        return;
    }
    if (!library.setContent(&file))
    {
        qDebug() << file.fileName() << "is not well formed";
        file.close();
        return;
    }
    file.close();
    loadPlayerColors();
    loadCreatures();
    loadScrolls();
    loadEnemyNames();
}

void Library::loadPlayerColors()
{
    QDomNodeList pcList = library.elementsByTagName("PlayerColor");
    for (int ii = 0; ii < pcList.length(); ii++)
    {
        QDomNode xmlpc = pcList.item(ii);
        PlayerColor* pc = new PlayerColor();
        pc->file = xmlpc.attributes().namedItem("file").nodeValue();
        pc->name = xmlpc.attributes().namedItem("name").nodeValue();
        pc->colorClass = xmlpc.attributes().namedItem("class").nodeValue();
        if (pc->colorClass == "dark priest")
            lstPlayerColors.append(pc);
        else if (pc->colorClass == "sorcerer")
            lstPlayerColorsSorcerer.append(pc);
        else if (pc->colorClass == "rogue")
            lstPlayerColorsRogue.append(pc);
        else if (pc->colorClass == "warrior")
            lstPlayerColorsWarrior.append(pc);
    }
}

int Library::creatureCount(QQmlListProperty<Creature> *list)
{
    Library *library = qobject_cast<Library*>(list->object);
    return library->lstAllCreatures.count();
}

Creature *Library::creature(QQmlListProperty<Creature> *list, int index)
{
    Library *library = qobject_cast<Library*>(list->object);
    return library->lstAllCreatures.at(index);
}

QQmlListProperty<Creature> Library::creatures()
{
    if (lstAllCreatures.length() == 0)
    {
        getAllCreatures();
    }
    return QQmlListProperty<Creature>(this, 0, &Library::creatureCount, &Library::creature);
}

void Library::getAllCreatures()
{
    lstAllCreatures.clear();
    foreach(Creature* c, lstCategory0)
    {
        lstAllCreatures << c;
    }
    foreach(Creature* c, lstCategory1)
    {
        lstAllCreatures << c;
    }
    foreach(Creature* c, lstCategory2)
    {
        lstAllCreatures << c;
    }
    foreach(Creature* c, lstCategory3)
    {
        lstAllCreatures << c;
    }
}

void Library::loadCreatures()
{
    QDomNodeList cList = library.elementsByTagName("Creature");
    for (int ii = 0; ii < cList.count(); ii++)
    {
        QDomNode xmlc = cList.item(ii);
        QDomNode* domNode = new QDomNode(xmlc);
        Creature* creature = new Creature(this);
        creature->load(domNode);
        creature->setUnitClass("Creature");
        if (creature->cheat().length() == 0)
        {
            switch (creature->category())
            {
                case 0:
                    lstCategory0.append(creature);
                    break;
                case 1:
                    lstCategory1.append(creature);
                    break;
                case 2:
                    lstCategory2.append(creature);
                    break;
                case 3:
                    lstCategory3.append(creature);
                    break;
                case 4:
                    lstCategory4.append(creature);
                    break;
                default:
                    break;
            }
        }
        else
        {
            lstCheatCreatures.append(creature);
        }
    }
}

void Library::loadScrolls()
{
    QDomNodeList sList = library.elementsByTagName("Scroll");
    for (int ii = 0; ii < sList.count(); ii++)
    {
        QDomNode xmls = sList.item(ii);
        QDomNode* domNode = new QDomNode(xmls);
        Scroll* scroll = new Scroll(this);
        scroll->load(domNode);
        scroll->setUnitClass("Scroll");
        lstScrolls.append(scroll);
    }
}

void Library::loadEnemyNames()
{
    QDomNodeList eList = library.elementsByTagName("EnemyName");
    for (int ii = 0; ii < eList.count(); ii++)
    {
        QDomNode xmls = eList.item(ii);
        EnemyName* en = new EnemyName(this);
        en->load(&xmls);
        lstEnemyNames.append(en);
    }
}
