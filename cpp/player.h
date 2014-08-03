#ifndef PLAYER_H
#define PLAYER_H

#include "creature.h"
#include "scroll.h"
#include "playercolor.h"
#include <QQmlListProperty>

class Player : public Creature
{
    Q_OBJECT
    //Q_PROPERTY(PlayerColor* color READ color WRITE setColor NOTIFY propertyChanged)
    Q_PROPERTY(int spellPoints READ spellPoints WRITE setSpellPoints NOTIFY propertyChanged)
    Q_PROPERTY(bool isNPC READ isNPC WRITE setIsNPC NOTIFY propertyChanged)
    Q_PROPERTY(QString type READ type NOTIFY propertyChanged)
    Q_PROPERTY(QString race READ race WRITE setRace NOTIFY propertyChanged)
    Q_PROPERTY(QQmlListProperty<Creature> possibleCreatures READ possibleCreatures NOTIFY possibleCreaturesChanged)

    static int count_possibleCreature(QQmlListProperty<Creature> *list);
    static Creature *at_possibleCreature(QQmlListProperty<Creature> *list, int index);

public:
    explicit Player(QObject *parent = 0);
    inline int spellPoints() const { return m_SpellPoints; }
    void setSpellPoints(int spellPoints);
    bool isNPC() { return m_isNPC; }
    void setIsNPC(bool isNPC) { m_isNPC = isNPC; emit propertyChanged(); }
    QList<Scroll*> m_PossibleScrolls;
    QList<Creature*> m_PossibleCreatures;
    QList<Creature*> m_Creatures;
    QString type()
    {
        return isNPC() ? tr("AI ", "AI shortcut as player category") : tr("Human ", "human as player category");
    }
    QQmlListProperty<Creature> possibleCreatures();

    void emitPossibleCreaturesChanged() { emit possibleCreaturesChanged(); }

    QString race() const
    {
        return m_race;
    }

protected:
    int m_SpellPoints;
    bool m_isNPC;
    QString m_type;
    QString m_race;
signals:
    void propertyChanged();
    void possibleCreaturesChanged();
public slots:
    void initializeScrolls();
    void initialize();
    void setRace(QString arg)
    {
        if (m_race != arg) {
            m_race = arg;
            emit propertyChanged();
        }
    }
};

#endif // PLAYER_H
