#ifndef CREATURE_H
#define CREATURE_H

#include <QObject>
#include <QDomNode>

class Player;

class Creature : public QObject
{
    Q_OBJECT
    // Creature Properties
    Q_PROPERTY(QString species READ species WRITE setSpecies NOTIFY speciesChanged)
    Q_PROPERTY(int category READ category WRITE setCategory NOTIFY categoryChanged)
    Q_PROPERTY(int manaCost READ manaCost WRITE setManaCost NOTIFY manaCostChanged)
    Q_PROPERTY(int hp READ hp WRITE setHp NOTIFY hpChanged)
    Q_PROPERTY(int strength READ strength WRITE setStrength NOTIFY strengthChanged)
    Q_PROPERTY(int defense READ defense WRITE setDefense NOTIFY defenseChanged)
    Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(bool hasDistanceAttack READ hasDistanceAttack WRITE setHasDistanceAttack NOTIFY hasDistanceAttackChanged)
    Q_PROPERTY(int distanceRange READ distanceRange WRITE setDistanceRange NOTIFY distanceRangeChanged)
    Q_PROPERTY(int distanceStrength READ distanceStrength WRITE setDistanceStrength NOTIFY distanceStrengthChanged)
    Q_PROPERTY(QString immune READ immune WRITE setImmune NOTIFY immuneChanged)
    Q_PROPERTY(QString imageFilePattern READ imageFilePattern WRITE setImageFilePattern NOTIFY imageFilePatternChanged)
    Q_PROPERTY(QString cheat READ cheat WRITE setCheat NOTIFY cheatChanged)
    Q_PROPERTY(QString distanceImageFilePattern READ distanceImageFilePattern WRITE setDistanceImageFilePattern NOTIFY distanceImageFilePatternChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)

    // Runtime Properties
    Q_PROPERTY(QString imageFilename READ imageFilename WRITE setImageFilename NOTIFY imageFilenameChanged)
    Q_PROPERTY(QString distanceImageFilename READ distanceImageFilename WRITE setDistanceImageFilename NOTIFY distanceImageFilenameChanged)
    Q_PROPERTY(int remainingMovePoints READ remainingMovePoints WRITE setRemainingMovePoints NOTIFY remainingMovePointsChanged)
    Q_PROPERTY(bool hasMoved READ hasMoved WRITE setHasMoved NOTIFY hasMovedChanged)
    Q_PROPERTY(Player* player READ player WRITE setPlayer NOTIFY playerChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString originalName READ originalName WRITE setOriginalName NOTIFY originalNameChanged)
    Q_PROPERTY(int x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(int y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(int xField READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(int yField READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(bool hasAttacked READ hasAttacked WRITE setHasAttacked NOTIFY hasAttackedChanged)
    Q_PROPERTY(bool hasDistanceAttacked READ hasDistanceAttacked WRITE setHasDistanceAttacked NOTIFY hasDistanceAttackedChanged)
    Q_PROPERTY(QString unitClass READ unitClass WRITE setUnitClass NOTIFY unitClassChanged)
    Q_PROPERTY(bool alive READ alive WRITE setAlive NOTIFY aliveChanged)
    Q_PROPERTY(int originalHp READ originalHp WRITE setOriginalHp NOTIFY originalHpChanged)
    Q_PROPERTY(bool distanceAttackMode READ distanceAttackMode WRITE setDistanceAttackMode NOTIFY distanceAttackModeChanged)

public:
    explicit Creature(QObject *parent = 0);

    // Creature Properties
    inline QString species() const { return m_Species; }
    inline void setSpecies(QString species) { if (m_Species != species) { m_Species = species; emit speciesChanged(species); } }
    inline int category() const { return m_Category; }
    inline void setCategory(int category) { if (m_Category != category) { m_Category = category; emit categoryChanged(category); } }
    inline int manaCost() const { return m_ManaCost; }
    inline void setManaCost(int manaCost) { if (m_ManaCost != manaCost) { m_ManaCost = manaCost; emit manaCostChanged(manaCost); } }
    inline int hp() const { return m_HP; }
    inline void setHp(int hp) { if (m_HP != hp) { m_HP = hp; emit hpChanged(hp); } }
    inline int strength() const { return m_Strength; }
    inline void setStrength(int strength) { if (m_Strength != strength) { m_Strength = strength; emit strengthChanged(strength); } }
    inline int defense() const { return m_Defense; }
    inline void setDefense(int defense) { if (m_Defense != defense) { m_Defense = defense; emit defenseChanged(defense); } }
    inline int speed() const { return m_Speed; }
    inline void setSpeed(int speed) { if (m_Speed != speed) { m_Speed = speed; emit speedChanged(speed); } }
    inline bool hasDistanceAttack() const { return m_hasDistanceAttack; }
    inline void setHasDistanceAttack(bool hasDistanceAttack) { if (m_hasDistanceAttack != hasDistanceAttack) { m_hasDistanceAttack = hasDistanceAttack; emit hasDistanceAttackChanged(hasDistanceAttack); } }
    inline int distanceRange() const { return m_DistanceRange; }
    inline void setDistanceRange(int distanceRange) { if (m_DistanceRange != distanceRange) { m_DistanceRange = distanceRange; emit distanceRangeChanged(distanceRange); } }
    inline int distanceStrength() const { return m_DistanceStrength; }
    inline void setDistanceStrength(int distanceStrength) { if (m_DistanceStrength != distanceStrength) { m_DistanceStrength = distanceStrength; emit distanceStrengthChanged(distanceStrength); } }
    inline QString immune() const { return m_Immune; }
    inline void setImmune(QString immune) { if (m_Immune != immune) { m_Immune = immune; emit immuneChanged(immune); } }
    inline QString imageFilePattern() const { return m_ImageFilePattern; }
    inline void setImageFilePattern(QString imageFilePattern) { if (m_ImageFilePattern != imageFilePattern) { m_ImageFilePattern = imageFilePattern; emit imageFilePatternChanged(imageFilePattern); } }
    inline QString cheat() const { return m_Cheat; }
    inline void setCheat(QString cheat) { if (m_Cheat != cheat) { m_Cheat = cheat; emit cheatChanged(cheat); } }
    inline QString distanceImageFilePattern() const { return m_DistanceImageFilePattern; }
    inline void setDistanceImageFilePattern(QString distanceImageFilePattern) { if (m_DistanceImageFilePattern != distanceImageFilePattern) { m_DistanceImageFilePattern = distanceImageFilePattern; emit distanceImageFilePatternChanged(distanceImageFilePattern); } }

    // Runtime Properties
    inline QString name() const { return m_Name; }
    inline void setName(QString name) { if (m_Name != name) { m_Name = name; emit nameChanged(name); }/*emit propertyChanged();*/ }
    inline QString imageFilename() const { return m_ImageFilename; }
    inline void setImageFilename(QString imageFilename) { if (m_ImageFilename != imageFilename) { m_ImageFilename = imageFilename; emit imageFilenameChanged(m_ImageFilename); } }
    inline QString distanceImageFilename() const { return m_DistanceImageFilename; }
    inline void setDistanceImageFilename(QString distanceImageFilename) { if (m_DistanceImageFilename != distanceImageFilename) { m_DistanceImageFilename = distanceImageFilename; emit distanceImageFilenameChanged(distanceImageFilename); } }
    inline double remainingMovePoints() const { return m_RemainingMovePoints; }
    inline void setRemainingMovePoints(double remainingMovePoints) { if (m_RemainingMovePoints != remainingMovePoints) { m_RemainingMovePoints = remainingMovePoints; emit remainingMovePointsChanged(remainingMovePoints); } }
    inline bool hasMoved() const { return m_hasMoved; }
    inline void setHasMoved(bool hasMoved) { if (m_hasMoved != hasMoved) { m_hasMoved = hasMoved; emit hasMovedChanged(hasMoved); } }
    inline Player* player() const { return m_Player; }
    inline void setPlayer(Player* player) { if (m_Player != player) { m_Player = player; emit playerChanged(player); } }

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

    void speciesChanged(QString arg);
    void hpChanged(int arg);
    void categoryChanged(int arg);
    void manaCostChanged(int arg);
    void strengthChanged(int arg);
    void defenseChanged(int arg);
    void speedChanged(int arg);
    void hasDistanceAttackChanged(bool arg);
    void distanceRangeChanged(int arg);
    void distanceStrengthChanged(int arg);
    void immuneChanged(QString arg);
    void imageFilePatternChanged(QString arg);
    void cheatChanged(QString arg);
    void distanceImageFilePatternChanged(QString arg);
    void distanceImageFilenameChanged(QString arg);
    void remainingMovePointsChanged(int arg);
    void hasMovedChanged(bool arg);
    void typeChanged(QString type);
    void playerChanged(Player* arg);
    void nameChanged(QString arg);
    void originalNameChanged(QString arg);
    void hasDistanceAttackedChanged(bool arg);
    void hasAttackedChanged(bool arg);
    void unitClassChanged(QString arg);

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
            emit originalNameChanged(arg);
        }
    }
    void setHasAttacked(bool arg)
    {
        if (m_hasAttacked != arg) {
            m_hasAttacked = arg;
            emit hasAttackedChanged(arg);
        }
    }
    void setHasDistanceAttacked(bool arg)
    {
        if (m_hasDistanceAttacked != arg) {
            m_hasDistanceAttacked = arg;
            emit hasDistanceAttackedChanged(arg);
        }
    }
    void setType(QString arg)
    {
        if (m_type != arg) {
            m_type = arg;
            emit typeChanged(arg);
        }
    }
    void setUnitClass(QString arg)
    {
        if (m_unitClass != arg) {
            m_unitClass = arg;
            emit unitClassChanged(arg);
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
