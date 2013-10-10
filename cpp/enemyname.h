#ifndef ENEMYNAME_H
#define ENEMYNAME_H

#include <QObject>
#include <QDomNode>

class EnemyName : public QObject
{
    Q_OBJECT
public:
    explicit EnemyName(QObject *parent = 0);
    QString name;
    QString enemyClass;
    QString set;
    void load(QDomNode *node);
    QDomNode *sourceCode;
signals:

public slots:

};

#endif // ENEMYNAME_H
