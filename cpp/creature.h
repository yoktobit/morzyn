#ifndef CREATURE_H
#define CREATURE_H

#include <QObject>
#include <QDomNode>

class Player;

class Creature : public QObject
{
    Q_OBJECT
    // Creature Properties
    Q_PROPERTY(QString species READ species WRITE setSpecies NOTIFY propertyChanged)
    Q_PROPERTY(int category READ category WRITE setCategory NOTIFY propertyChanged)
    Q_PROPERTY(int manaCost READ manaCost WRITE setManaCost NOTIFY propertyChanged)
    Q_PROPERTY(int hp READ hp WRITE setHp NOTIFY propertyChanged)
    Q_PROPERTY(int strength READ strength WRITE setStrength NOTIFY propertyChanged)
    Q_PROPERTY(int defense READ defense WRITE setDefense NOTIFY propertyChanged)
    Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY propertyChanged)
    Q_PROPERTY(bool hasDistanceAttack READ hasDistanceAttack WRITE setHasDistanceAttack NOTIFY propertyChanged)
    Q_PROPERTY(int distanceRange READ distanceRange WRITE setDistanceRange NOTIFY propertyChanged)
    Q_PROPERTY(int distanceStrength READ distanceStrength WRITE setDistanceStrength NOTIFY propertyChanged)
    Q_PROPERTY(QString immune READ immune WRITE setImmune NOTIFY propertyChanged)
    Q_PROPERTY(QString imageFilePattern READ imageFilePattern WRITE setImageFilePattern NOTIFY propertyChanged)
    Q_PROPERTY(QString cheat READ cheat WRITE setCheat NOTIFY propertyChanged)
    Q_PROPERTY(QString distanceImageFilePattern READ distanceImageFilePattern WRITE setDistanceImageFilePattern NOTIFY propertyChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY propertyChanged)

    // Runtime Properties
    Q_PROPERTY(QString imageFilename READ imageFilename WRITE setImageFilename NOTIFY imageFilenameChanged)
    Q_PROPERTY(QString distanceImageFilename READ distanceImageFilename WRITE setDistanceImageFilename NOTIFY propertyChanged)
    Q_PROPERTY(int remainingMovePoints READ remainingMovePoints WRITE setRemainingMovePoints NOTIFY propertyChanged)
    Q_PROPERTY(bool hasMoved READ hasMoved WRITE setHasMoved NOTIFY propertyChanged)
    Q_PROPERTY(Player* player READ player WRITE setPlayer NOTIFY propertyChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY propertyChanged)
    Q_PROPERTY(QString originalName READ originalName WRITE setOriginalName NOTIFY propertyChanged)
    Q_PROPERTY(int x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(int y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(int xField READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(int yField READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(bool hasAttacked READ hasAttacked WRITE setHasAttacked NOTIFY propertyChanged)
    Q_PROPERTY(bool hasDistanceAttacked READ hasDistanceAttacked WRITE setHasDistanceAttacked NOTIFY propertyChanged)
    Q_PROPERTY(QString unitClass READ unitClass WRITE setUnitClass NOTIFY propertyChanged)
    Q_PROPERTY(bool alive READ alive WRITE setAlive NOTIFY aliveChanged)
    Q_PROPERTY(int originalHp READ originalHp WRITE setOriginalHp NOTIFY originalHpChanged)
    Q_PROPERTY(bool distanceAttackMode READ distanceAttackMode WRITE setDistanceAttackMode NOTIFY distanceAttackModeChanged)

public:
    explicit Creature(QObject *parent = 0);

    // Creature Properties
    inline QString species() const { return m_Species; }
    inline void setSpecies(QString species) { m_Species = species; emit propertyChanged(); }
    inline int category() const { return m_Category; }
    inline void setCategory(int category) { m_Category = category; emit propertyChanged(); }
    inline int manaCost() const { return m_ManaCost; }
    inline void setManaCost(int manaCost) { m_ManaCost = manaCost; emit propertyChanged(); }
    inline int hp() const { return m_HP; }
    inline void setHp(int hp) { m_HP = hp; emit propertyChanged(); }
    inline int strength() const { return m_Strength; }
    inline void setStrength(int strength) { m_Strength = strength; emit propertyChanged(); }
    inline int defense() const { return m_Defense; }
    inline void setDefense(int defense) { m_Defense = defense; emit propertyChanged(); }
    inline int speed() const { return m_Speed; }
    inline void setSpeed(int speed) { m_Speed = speed; emit propertyChanged(); }
    inline bool hasDistanceAttack() const { return m_hasDistanceAttack; }
    inline void setHasDistanceAttack(bool hasDistanceAttack) { m_hasDistanceAttack = hasDistanceAttack; emit propertyChanged(); }
    inline int distanceRange() const { return m_DistanceRange; }
    inline void setDistanceRange(int distanceRange) { m_DistanceRange = distanceRange; emit propertyChanged(); }
    inline int distanceStrength() const { return m_DistanceStrength; }
    inline void setDistanceStrength(int distanceStrength) { m_DistanceStrength = distanceStrength; emit propertyChanged(); }
    inline QString immune() const { return m_Immune; }
    inline void setImmune(QString immune) { m_Immune = immune; emit propertyChanged(); }
    inline QString imageFilePattern() const { return m_ImageFilePattern; }
    inline void setImageFilePattern(QString imageFilePattern) { m_ImageFilePattern = imageFilePattern; emit propertyChanged(); }
    inline QString cheat() const { return m_Cheat; }
    inline void setCheat(QString cheat) { m_Cheat = cheat; emit propertyChanged(); }
    inline QString distanceImageFilePattern() const { return m_DistanceImageFilePattern; }
    inline void setDistanceImageFilePattern(QString distanceImageFilePattern) { m_DistanceImageFilePattern = distanceImageFilePattern; emit propertyChanged(); }

    // Runtime Properties
    inline QString name() const { return m_Name; }
    inline void setName(QString name) { m_Name = name; /*emit propertyChanged();*/ }
    inline QString imageFilename() const { return m_ImageFilename; }
    inline void setImageFilename(QString imageFilename) { m_ImageFilename = imageFilename; emit imageFilenameChanged(m_ImageFilename); }
    inline QString distanceImageFilename() const { return m_DistanceImageFilename; }
    inline void setDistanceImageFilename(QString distanceImageFilename) { m_DistanceImageFilename = distanceImageFilename; emit propertyChanged(); }
    inline double remainingMovePoints() const { return m_RemainingMovePoints; }
    inline void setRemainingMovePoints(double remainingMovePoints) { m_RemainingMovePoints = remainingMovePoints; emit propertyChanged(); }
    inline bool hasMoved() const { return m_hasMoved; }
    inline void setHasMoved(bool hasMoved) { m_hasMoved = hasMoved; emit propertyChanged(); }
    inline Player* player() const { return m_Player; }
    inline void setPlayer(Player* player) { m_Player = player; emit propertyChanged(); }

    QDomNode* sourceCode;
    virtual void load(QDomNode* node);

    int x() const
    {
        return m_x;
    }

    int y() const
    {
        return m_y;
    }

    QString originalName() const
    {
        return m_originalName;
    }

    bool hasAttacked() const
    {
        return m_hasAttacked;
    }
    bool hasDistanceAttacked() const
    {
        return m_hasDistanceAttacked;
    }

    QString type() const
    {
        return m_type;
    }

    QString unitClass() const
    {
        return m_unitClass;
    }

    bool alive() const
    {
        return m_alive;
    }

    int originalHp() const
    {
        return m_originalHp;
    }

    bool distanceAttackMode() const
    {
        return m_distanceAttackMode;
    }

    int m_x;
    int m_y;

protected:
    // Creature Properties
    QString m_Species;
    int m_Category;
    int m_ManaCost;
    int m_HP;
    int m_Strength;
    int m_Defense;
    int m_Speed;
    bool m_hasDistanceAttack;
    int m_DistanceRange;
    int m_DistanceStrength;
    QString m_Immune;
    QString m_ImageFilePattern;
    QString m_Cheat;
    QString m_DistanceImageFilePattern;

    // Runtime Properties
    QString m_Name;
    QString m_ImageFilename;
    QString m_DistanceImageFilename;
    double m_RemainingMovePoints;
    bool m_hasMoved;
    Player *m_Player;
    QString m_originalName;
    QString m_type;
    QString m_unitClass;
    bool m_hasAttacked;
    bool m_hasDistanceAttacked;
    bool m_alive;
    int m_originalHp;
    bool m_distanceAttackMode;
signals:
    void propertyChanged();
    void xChanged(int x);
    void yChanged(int y);
    void imageFilenameChanged(QString arg);
    void aliveChanged(bool alive);

    void originalHpChanged(int arg);

    void distanceAttackModeChanged(bool arg);

public slots:

    void setX(int arg)
    {
        if (m_x != arg) {
            m_x = arg;
            emit xChanged(arg);
        }
    }
    void setY(int arg)
    {
        if (m_y != arg) {
            m_y = arg;
            emit yChanged(arg);
        }
    }
    void setOriginalName(QString arg)
    {
        if (m_originalName != arg) {
            m_originalName = arg;
            emit propertyChanged();
        }
    }
    void setHasAttacked(bool arg)
    {
        if (m_hasAttacked != arg) {
            m_hasAttacked = arg;
        }
    }
    void setHasDistanceAttacked(bool arg)
    {
        if (m_hasDistanceAttacked != arg) {
            m_hasDistanceAttacked = arg;
            emit propertyChanged();
        }
    }
    void setType(QString arg)
    {
        if (m_type != arg) {
            m_type = arg;
            emit propertyChanged();
        }
    }
    void setUnitClass(QString arg)
    {
        if (m_unitClass != arg) {
            m_unitClass = arg;
            emit propertyChanged();
        }
    }
    void setAlive(bool arg)
    {
        if (m_alive != arg) {
            m_alive = arg;
            emit aliveChanged(true);
        }
    }
    void setOriginalHp(int arg)
    {
        if (m_originalHp != arg) {
            m_originalHp = arg;
            emit originalHpChanged(arg);
        }
    }
    void setDistanceAttackMode(bool arg)
    {
        if (m_distanceAttackMode != arg) {
            m_distanceAttackMode = arg;
            emit distanceAttackModeChanged(arg);
        }
    }
};

#endif // CREATURE_H
