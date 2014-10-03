#ifndef SCROLL_H
#define SCROLL_H

#include <QObject>
#include <QDomNode>
#include "creature.h"

class Scroll : public Creature
{
    Q_OBJECT
    Q_PROPERTY(int minDamage READ minDamage WRITE setMinDamage NOTIFY propertyChanged)
    Q_PROPERTY(int maxDamage READ maxDamage WRITE setMaxDamage NOTIFY propertyChanged)
    Q_PROPERTY(int range READ range WRITE setRange NOTIFY propertyChanged)
    Q_PROPERTY(int jumps READ jumps WRITE setJumps NOTIFY propertyChanged)

public:
    explicit Scroll(QObject *parent = 0);
    int     m_minDamage;
    int     m_maxDamage;
    int     m_range;
    int     m_jumps;
    int minDamage() const
    {
        return m_minDamage;
    }

    int maxDamage() const
    {
        return m_maxDamage;
    }

    int range() const
    {
        return m_range;
    }

    int jumps() const
    {
        return m_jumps;
    }

    virtual void load(QDomNode* node);

    void loadHealing();
signals:
    void propertyChanged();
public slots:
    void setMinDamage(int arg)
    {
        if (m_minDamage != arg) {
            m_minDamage = arg;
            emit propertyChanged();
        }
    }
    void setMaxDamage(int arg)
    {
        if (m_maxDamage != arg) {
            m_maxDamage = arg;
            emit propertyChanged();
        }
    }
    void setRange(int arg)
    {
        if (m_range != arg) {
            m_range = arg;
            emit propertyChanged();
        }
    }
    void setJumps(int arg)
    {
        if (m_jumps != arg) {
            m_jumps = arg;
            emit propertyChanged();
        }
    }
};

#endif // SCROLL_H
