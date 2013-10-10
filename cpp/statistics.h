#ifndef STATISTICS_H
#define STATISTICS_H

#include <QObject>
#include "player.h"

class Statistics : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int rounds READ rounds WRITE setRounds NOTIFY roundsChanged)
    Q_PROPERTY(int firstPlayer READ firstPlayer WRITE setFirstPlayer NOTIFY firstPlayerChanged)

    int m_rounds;
    int m_firstPlayer;

public:
    explicit Statistics(QObject *parent = 0);


    int rounds() const
    {
        return m_rounds;
    }

    int firstPlayer() const
    {
        return m_firstPlayer;
    }

signals:
    
    void roundsChanged(int arg);

    void firstPlayerChanged(int arg);

public slots:

void setRounds(int arg)
{
    if (m_rounds != arg) {
        m_rounds = arg;
        emit roundsChanged(arg);
    }
}

void setFirstPlayer(int arg)
{
    if (m_firstPlayer != arg) {
        m_firstPlayer = arg;
        emit firstPlayerChanged(arg);
    }
}

void reset();
};

#endif // STATISTICS_H
