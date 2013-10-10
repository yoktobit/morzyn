#include "statistics.h"

Statistics::Statistics(QObject *parent) :
    QObject(parent)
{
    reset();
}


void Statistics::reset()
{
    m_firstPlayer = -1;
    m_rounds = 1;
}
