#ifndef PLAYERCOLOR_H
#define PLAYERCOLOR_H

#include <QObject>

class PlayerColor : public QObject
{
    Q_OBJECT
public:
    explicit PlayerColor(QObject *parent = 0);
    QString name;
    QString file;
    QString colorClass;
signals:

public slots:

};

#endif // PLAYERCOLOR_H
