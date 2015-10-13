#include "gameservice.h"
#include "game.h"
#include <QObject>
#include <QList>
//#include <math.h>
#include <QStringList>
#include <QDir>
#include <QDebug>
#include "constants.h"
#include "dbloader.h"
#include "easyai.h"
#include <QPropertyAnimation>
#include <QMediaPlaylist>
#ifdef SAILFISH
#include <sailfishapp.h>
#endif

#ifndef DEPLOYMENT_PATH
#define DEPLOYMENT_PATH ""
#endif

GameService::GameService(QObject *parent) :
    QObject(parent)
{
    //connect(statemanager, SIGNAL(playerSwitched(Player*)), SLOT(initPlayerForNewRound(Player*)));
    //connect(statemanager->totalPlayerCountState, SIGNAL(entered()), this, SLOT(resetGame()));
    titleSound = NULL;
    previousPlayedTitleSound = false;
    m_bWriteMessages = true;
}

QList<int> GameService::generateDistinctNumberList(int count, int min, int max)
{
    QList<int> lst;
    if (min >= max) return lst;
    if ((max - min + 1) < count)
        count = max - min + 1;
    while (lst.count() < count)
    {
        int number = (rand() % (max - min)) + min;
        if (!lst.contains(number))
            lst.append(number);
    }
    return lst;
}

// Zufallszahl zwischen min und max - 1 (also exkl. max)
int GameService::randomInteger(int min, int max)
{
    int number = (qrand() % (max - min)) + min;
    //qDebug() << "Random:" << number;
    return number;
}

double GameService::randomDouble()
{
    int number = randomInteger(0, 10);
    return double(number) / 10;
}

void GameService::setTotalPlayers(int number)
{
    game->totalPlayers = number;
}

void GameService::setHumanPlayers(int number)
{
    game->humanPlayers = number;
}

void GameService::addPlayer(Player *player)
{
    // wenn aktuelle Spieleranzahl schon die der menschlichen Spieler erreicht hat,
    // dann ist alles weitere NPC
    if (game->m_players.count() >= game->humanPlayers)
    {
        player->setIsNPC(true);
        IAI *ai = new EasyAI(player, this);
        //player->setName(getRandomCreatureName());
        game->m_AI.insert(player, ai);
    }
    player->setUnitClass("Player");
    player->setOriginalName(tr("Player %1", "standard player original name (for remembering it)").arg(game->m_players.count() + 1));
    player->setName(tr("Player %1", "standard player name").arg(game->m_players.count() + 1));
    // "rogue", "warrior", "sorcerer"
    int randRace = randomInteger(0, 3);
    if (randRace == 0)
        player->setRace("rogue");
    else if (randRace == 1)
        player->setRace("warrior");
    else if (randRace == 2)
        player->setRace("sorcerer");
    player->setX(0);
    player->setY(0);
    getRandomPlayerColor(player);
    //player->setImageFilename(library->lstPlayerColors.at(game->m_players.count())->file);
    game->m_players.append(player);
    game->m_creatures.append(player);
    emit creatureCasted(player);
}

void GameService::addPlayers(int nTotalPlayerCount, int nHumanPlayerCount)
{
    setTotalPlayers(nTotalPlayerCount);
    setHumanPlayers(nHumanPlayerCount);
    QSet<int> lstUsedNames;
    for (int ii = 0; ii < nTotalPlayerCount; ii++)
    {
        //qDebug() << "Creating Player Index" << ii;
        Player* p = new Player(game);
        addPlayer(p);
        if (p->isNPC())
        {
            int nName = randomInteger(0, library->lstEnemyNames.count());
            while (lstUsedNames.contains(nName) || library->lstEnemyNames[nName]->enemyClass != p->race())
                nName = randomInteger(0, library->lstEnemyNames.count());
            lstUsedNames.insert(nName);
            p->setName(library->lstEnemyNames[nName]->name);
        }
        else {
            if (game->arrLastPlayerNames.length() > ii)
            {
                p->setName(game->arrLastPlayerNames[ii]);
            }
        }
    }
    game->playersChanged();
    game->creaturesChanged();
}

void GameService::placePlayers()
{
    QList<int> placedPlayers;

    int ii = 0;
    int maxLoops = 30;
    Creature *nearest;
    int nearestDistance;
    while (placedPlayers.count() != game->m_players.count())
    {
        ii = 0;
        int rnd = 0;
        do
        {
            rnd = (rand() % (game->m_players.count()));
        } while (placedPlayers.contains(rnd));
        Player* player = game->m_players[rnd];
        int randomplaceX = 0;
        int randomplaceY = 0;
        do
        {
            randomplaceX = rand() % (config->HCOUNT - 1);
            randomplaceY = rand() % (config->VCOUNT - 1);
            //qDebug() << "RandomPlaceX: " << randomplaceX;
            //qDebug() << "RandomPlaceY: " << randomplaceY;
            ii++;
            // max 30x probieren
            if (ii > maxLoops) break;

            player->setX(randomplaceX);
            player->setY(randomplaceY);
            nearest = getNearestEnemy(player);
            nearestDistance = getDistance(player, nearest->x(), nearest->y());
            //qDebug() << player->name() << "nearest distance to" << ((Player*)nearest)->name() << ":" << nearestDistance;
        } while(nearestDistance < 3 || nearestDistance > 5);

        // komplett von vorn probieren
        if (ii > maxLoops)
        {
            placedPlayers.clear();
            continue;
        }

        emitCreatureMoved(player);
        placedPlayers.append(rnd);
    }
    game->setCurrentPlayer(game->m_players[placedPlayers[0]]);
    statistics->setFirstPlayer(placedPlayers[0]);
    game->playersChanged();
    game->creaturesChanged();

    //game->emitPropertyChanged();
}

bool creatureLessThan(const Creature *s1, const Creature *s2)
{
    return s1->manaCost() < s2->manaCost();
}

Scroll* addScrollToList(Player* player, QList<Creature*> *list, Scroll* c)
{
    Scroll* cNew = new Scroll(c->parent());
    cNew->load(c->sourceCode);
    // #62: 15% auf alles, außer Tiere...Manakosten aufrunden durch + 0,5
    if (player->race() == "sorcerer" && c->unitClass() == "Scroll")
    {
        int manaCost = cNew->manaCost();
        manaCost = qRound((double(manaCost) * 0.85) + 0.5);
        cNew->sourceCode->attributes().namedItem("manacost").setNodeValue(QString("%1").arg(manaCost));
        cNew->setManaCost(manaCost);
        //qDebug() << "New Mana Cost:" << manaCost;
    }
    list->append(cNew);
    return cNew;
}

void GameService::loadPlayerSpells(Player *player)
{
    //qDebug() << "Begin loadPlayerSpells";
    QSet<int> mana;
    QSet<Creature*> creatures;
    while(player->m_PossibleCreatures.length() < 1)
    {
        int r = randomInteger(0, library->lstCategory0.count());
        if (!creatures.contains(library->lstCategory0[r]) && !mana.contains(library->lstCategory0[r]->manaCost()))
        {
            player->m_PossibleCreatures.append(library->lstCategory0[r]);
            creatures.insert(library->lstCategory0[r]);
            mana.insert(library->lstCategory0[r]->manaCost());
        }
    }
    while(player->m_PossibleCreatures.length() < 6)
    {
        int r = randomInteger(0, library->lstCategory1.count());
        if (!creatures.contains(library->lstCategory1[r]) && !mana.contains(library->lstCategory1[r]->manaCost()))
        {
            player->m_PossibleCreatures.append(library->lstCategory1[r]);
            creatures.insert(library->lstCategory1[r]);
            mana.insert(library->lstCategory1[r]->manaCost());
        }
    }
    while(player->m_PossibleCreatures.length() < 11)
    {
        int r = randomInteger(0, library->lstCategory2.count());
        if (!creatures.contains(library->lstCategory2[r]) && !mana.contains(library->lstCategory2[r]->manaCost()))
        {
            player->m_PossibleCreatures.append(library->lstCategory2[r]);
            creatures.insert(library->lstCategory2[r]);
            mana.insert(library->lstCategory2[r]->manaCost());
        }
    }
    while(player->m_PossibleCreatures.length() < 16)
    {
        int r = randomInteger(0, library->lstCategory3.count());
        if (!creatures.contains(library->lstCategory3[r]) && !mana.contains(library->lstCategory3[r]->manaCost()))
        {
            player->m_PossibleCreatures.append(library->lstCategory3[r]);
            creatures.insert(library->lstCategory3[r]);
            mana.insert(library->lstCategory3[r]->manaCost());
        }
    }
    qSort(player->m_PossibleCreatures.begin(), player->m_PossibleCreatures.end(), creatureLessThan);
    mana.clear();
    while(player->m_PossibleCreatures.length() < 18)
    {
        int r = randomInteger(0, library->lstScrolls.count());
        if (!creatures.contains(library->lstScrolls[r]) && !mana.contains(library->lstScrolls[r]->manaCost()))
        {
            Scroll *c = addScrollToList(player, &player->m_PossibleCreatures, library->lstScrolls[r]);
            c->setUnitClass("Scroll");
            creatures.insert(library->lstScrolls[r]);
            mana.insert(library->lstScrolls[r]->manaCost());
        }
    }

    if (player->race() == "sorcerer")
    {
        Scroll *healing = new Scroll(player);
        healing->setUnitClass("Scroll");
        healing->setSpecies("Healing");
        healing->load(NULL);
        creatures.insert(healing);
        mana.insert(healing->manaCost());
        player->m_PossibleCreatures.append(healing);
    }

    Scroll *manaTransfer = new Scroll(player);
    manaTransfer->setUnitClass("Scroll");
    manaTransfer->setSpecies("Mana Transfer");
    manaTransfer->load(NULL);
    creatures.insert(manaTransfer);
    mana.insert(manaTransfer->manaCost());
    player->m_PossibleCreatures.append(manaTransfer);

    //qDebug() << "End loadPlayerSpells";
    /*foreach(int i, lst0)
    {
        player->m_PossibleCreatures.append(library->lstCategory0[i]);
    }
    foreach(int i, lst1)
    {
        player->m_PossibleCreatures.append(library->lstCategory1[i]);
    }
    foreach(int i, lst2)
    {
        player->m_PossibleCreatures.append(library->lstCategory2[i]);
    }
    foreach(int i, lst3)
    {
        player->m_PossibleCreatures.append(library->lstCategory3[i]);
    }
    foreach(int i, lst4)
    {
        player->m_PossibleCreatures.append(library->lstScrolls[i]);
    }*/
    player->emitPossibleCreaturesChanged();
}

void GameService::addCreature(Creature *c, int x, int y)
{
    Creature* newCreature = new Creature(c->parent());
    newCreature->load(c->sourceCode);
    //qDebug() << "Distance Attack?" << c->hasDistanceAttack();
    newCreature->setUnitClass("Creature");
    setCreatureImage(newCreature);
    //qDebug() << "creature Image = " << c->imageFilename();
    newCreature->setX(game->currentPlayer()->x());
    newCreature->setY(game->currentPlayer()->y());
    //qDebug() << "creature x = " << c->x() << "creature y" << c->y();
    newCreature->setPlayer(game->currentPlayer());
    QString newCreaturesName = newCreature->player()->name() + QString("'");
    if (!newCreature->player()->name().endsWith("s"))
        newCreaturesName += QString("s");
    newCreaturesName += QString(" %0").arg(newCreature->species());
    newCreature->setName(newCreaturesName);
    game->currentPlayer()->m_Creatures.append(newCreature);
    game->m_creatures.append(newCreature);
    game->emitPropertyChanged();
    game->emitCreaturesChanged();
    emitCreatureCasted(newCreature);
    emitCreatureMoved(newCreature);
    newCreature->setX(x);
    newCreature->setY(y);
    emitCreatureMoved(newCreature);
}

void GameService::castScroll(Scroll *s, int x, int y)
{
    Scroll* newScroll = new Scroll(s->parent());
    newScroll->setSpecies(s->species()); // wichtig für Scrolls ohne sourceCode
    //qDebug() << "loading scroll" << s->species();
    newScroll->load(s->sourceCode);
    //qDebug() << "loaded scroll" << newScroll->species();
    newScroll->setUnitClass("Scroll");

    if (newScroll->species() == "Healing")
    {
        castHealing(newScroll, x, y);
        return;
    }
    if (newScroll->species() == "Mana Transfer")
    {
        castManaTransfer(newScroll, x, y);
        return;
    }
    setCreatureImage(newScroll);
    //qDebug() << "Image: " << newScroll->imageFilename() << "Distance Image: " << newScroll->distanceImageFilename();
    newScroll->setX(game->currentPlayer()->x());
    newScroll->setY(game->currentPlayer()->y());
    newScroll->setPlayer(game->currentPlayer());
    game->m_scrolls.append(newScroll);
    game->emitPropertyChanged();
    emit scrollCasted(newScroll);
    if (getCreatureAt(x, y))
        scrollAttackCreature(newScroll, getCreatureAt(x, y));
    if (game->state() == "castSpellState") // wenn nicht schon GameOver ist, dann zum MoveState
        game->setState("moveState");
}

void GameService::setCreatureImage(Creature *creature)
{
    QStringList lstMatchingFiles = getCreatureImages(creature->imageFilePattern());
    int nFileNumber = rand() % lstMatchingFiles.count();
    QString strFileName = lstMatchingFiles[nFileNumber];
    creature->setImageFilename(strFileName);

    // distance image
    if (creature->hasDistanceAttack())
    {
        QString strFilePattern = creature->distanceImageFilePattern();
        lstMatchingFiles.clear();
        lstMatchingFiles = getCreatureImages(strFilePattern);
        nFileNumber = rand() % lstMatchingFiles.count();
        strFileName = lstMatchingFiles[nFileNumber];
        creature->setDistanceImageFilename(strFileName);
    }
}

bool GameService::canAfford(Player *p, Creature *c)
{
    if (p->spellPoints() < c->manaCost()) return false;
    return true;
}

void GameService::selectCreature(Creature *creature, bool emitSignal)
{
    game->setSelectedCreature(creature, emitSignal);
    if (isCreatureNearEnemy(creature))
    {
        setMessage(tr("Attack!"));
    }
    else
    {
        setMessage(tr("Move!"));
    }
    creature->setHasMoved(true);
    if (emitSignal)
        emitCreatureSelected(creature);
}

bool GameService::isSelectable(Creature *creature)
{
    if (!creature->alive())
    {
        setMessage(tr("Creature is dead!"));
        return false;
    }
    // Selections can only be done in moveState
    if (game->state() != "moveState")
    {
        return false;
    }
    // only creatures of the current player can be selected
    if (creature->player() != game->currentPlayer())
    {
        if (!game->selectedCreature())
        {
            setMessage(tr("This is not your creature!"));
        }
        return false;
    }
    // allready something selected? Then don't select multiple creatures, rightClick to deselect!
    if (game->selectedCreature())
    {
        return false;
    }
    // if creature has allready been moved in this round, no way to do that again
    if (creature->hasMoved())
    {
        setMessage(tr("This creature has already finished it's move!"));
        return false;
    }
    // all negative conditions = false? then here we go, you are able to select
    return true;
}

void GameService::resetMovementPoints(Creature *creature)
{
    creature->setRemainingMovePoints(creature->speed());
    creature->setHasMoved(false);
    creature->setHasAttacked(false);
    creature->setHasDistanceAttacked(false);
    creature->setDistanceAttackMode(false);
}

double GameService::getDistance(int sourceX, int sourceY, int targetX, int targetY)
{
    // distance is sqrt(dx^2 + dy^2) "beeline"
    double abst = sqrt(pow(targetX - sourceX, 2.0) + pow(targetY - sourceY, 2.0));
    return abst;
}

double GameService::getDistance(Creature *creature, int x, int y)
{
    return getDistance(creature->x(), creature->y(), x, y);
}

bool GameService::isMovementPossible(Creature *creature, int x, int y, bool message = true)
{
    qDebug("Begin isMovementPossible");
    if (game->state() != "moveState") return false;
    if (!game->selectedCreature()) return false;
    double dist = getDistance(creature, x, y);
    qDebug("dist: %f", dist);
    qDebug("m_RemainingMovePoints: %f", creature->remainingMovePoints());
    // same position? oh no, you don't have to move to where you allready are...
    if (dist == 0)
    {
        if (message)
            setMessage(tr("You are already there ;-)"));
        return false;
    }
    // more than one field? not possible
    if (dist > 1.5)
    {
        if (message)
            setMessage(tr("You have to move field per field"));
        return false;
    }
    // farer away as speed allows? not possible
    if (creature->remainingMovePoints() < 0.5)
    {
        if (message)
            setMessage(tr("You've got not enough move points to move there"));
        return false;
    }
    //if (creature->remainingMovePoints() < (int)dist) return false;
    // never two bodies at the same place at the same time
    if (getCreatureAt(x, y)) return false;
    // enemy in range? Sorry, you have to attack...
    if (isCreatureNearEnemy(creature))
    {
        if (message)
            setMessage(tr("You can't move, there's an enemy beside you!"));
        return false;
    }
    qDebug("End isMovementPossible");
    return true;
}

double GameService::getRealMovementDistance(Creature *creature, int x, int y)
{
    double dist = getDistance(creature, x, y);
    // if its possible to move, but diagonal, than the distance is 1.5
    // so it's not exactly sqrt(dx^2 + dy^2)
    if (dist > 1 && dist < 2) dist = 1.5;
    return dist;
}

void GameService::moveCreature(Creature *creature, int x, int y)
{
    setMessage("");
    //qDebug("Begin moveCreature");
    double realDist = getRealMovementDistance(creature, x, y);
    //qDebug("realDist: %f", realDist);
    creature->setX(x);
    creature->setY(y);
    creature->setHasMoved(true);
    creature->setRemainingMovePoints(creature->remainingMovePoints() - realDist);
    int left = (int)(creature->remainingMovePoints() + 0.5);
    if (left > 0)
    {
        if (isCreatureNearEnemy(creature))
        {
            setMessage(tr("Attack!"));
        }
        else
        {
            setMessage(tr("%0 moves left ...").arg(left));
        }
    }
    else
        setMessage("");
    emitCreatureMoved(creature);
    //qDebug("End moveCreature");
}

bool GameService::isCreatureNearEnemy(Creature *creature)
{
    foreach(Player* p, game->m_players)
    {
        if (p == game->currentPlayer()) continue;
        if (getDistance(creature, p->x(), p->y()) < 2 && p->alive())
            return true;
        foreach(Creature* c, p->m_Creatures)
        {
            if (getDistance(creature, c->x(), c->y()) < 2 && c->alive())
                return true;
        }
    }
    return false;
}

void GameService::initPlayerForNewRound(Player *player)
{
    //qDebug() << "begin initPlayerForNewRound" << "with" << player->name();
    // resetMovePoints for everybody
    resetMovementPoints(player);
    foreach(Creature *c, player->m_Creatures)
    {
        resetMovementPoints(c);
    }
    if (player->race() == "warrior" && statistics->rounds() != 1 && player->originalHp() > player->hp())
    {
        player->setHp(player->hp() + 1);
    }
    else if (player->race() == "sorcerer" && statistics->rounds() != 1)
    {
        if (player->spellPoints() < 28)
            player->setSpellPoints(player->spellPoints() + 3);
    }
    foreach(Creature *c, player->m_PossibleCreatures)
    {
        if (c->species() == "Mana Transfer")
        {
            c->setManaCost(getManaTransferAmount(player));
        }
    }
    //qDebug() << "end initPlayerForNewRound" << "with" << player->name();
}

void GameService::switchNextPlayer()
{
    int nCurrentPlayer = game->m_players.indexOf(game->currentPlayer());
    Player *nextPlayer = NULL;
    do
    {
        nCurrentPlayer++;
        nCurrentPlayer = nCurrentPlayer % game->m_players.count();
        nextPlayer = game->m_players[nCurrentPlayer];
    } while (!nextPlayer->alive());
    game->setCurrentPlayer(nextPlayer);
    // setRounds muss auf jeden Fall vor initPlayer... erfolgen,
    // damit dort bereits die richtige Runde drin steht
    if (nCurrentPlayer == statistics->firstPlayer())
        statistics->setRounds(statistics->rounds() + 1);
    initPlayerForNewRound(nextPlayer);
    emitPlayerSwitched(nextPlayer);
    game->setState("spellSelectState");
}

bool GameService::isAttackable(Creature *creature)
{
    return isAttackable(game->selectedCreature(), creature);
}

bool GameService::isAttackable(Creature *attacker, Creature *attacked)
{
    // no creature selected? nobody can attack
    if (!attacker) return false;
    // the attacking creature is the attacked? Don't harm yourself
    if (attacker == attacked)
    {
        return false;
    }
    // no friendly fire!
    if (attacker->player() == attacked->player())
    {
        setMessage(tr("You can't attack your own creatures"));
        return false;
    }
    // too far away (more than one field)?
    if (getRealMovementDistance(attacker, attacked->x(), attacked->y()) > 1.5)
    {
        if (!attacker->distanceAttackMode())
        {
            setMessage(tr("This one is too far away to attack"));
        }
        return false;
    }
    // farer away as speed allows? not possible
    if (attacker->remainingMovePoints() < 0.5)
    {
        if (!attacker->distanceAttackMode())
        {
            setMessage(tr("You don't have enough move points to attack this creature"));
        }
        return false;
    }
    // for distance attackers, who are still selected: do not attack, if movePoints are not ausreichend
    //if ((int)getDistance(game->selectedCreature(), creature->x(), creature->y()) > game->selectedCreature()->remainingMovePoints()) return false;
    // allready attacked? not once more!
    if (attacker->hasAttacked())
    {
        setMessage(tr("You can attack only once!"));
        return false;
    }

    //qDebug() << "Yeah, " << attacked->species() << " can be attacked, yoah.";
    return true;
}

void GameService::attackCreature(Creature *attackedCreature, bool emitSignal)
{
    setMessage("");
    Creature *attackingCreature = game->selectedCreature();
    // calculate damage
    int nDamage = calculateDamage(attackingCreature, attackedCreature);
    //qDebug() << "calculated Damage:" << nDamage;
    game->setLastDamage(nDamage);
    // calculate resulting spell point bonsu
    int nSpellPointBonus = calculateSpellPointBonus(nDamage);
    //qDebug() << "calculated spell point bonus" << nSpellPointBonus;
    // apply damage to attacked creature
    applyDamage(attackedCreature, nDamage, true);
    //qDebug() << nDamage << "damage applied";
    // apply spell point bonsu to player of selected creature
    applySpellPointBonus(attackingCreature->player(), nSpellPointBonus);
    if (attackingCreature->player())
    {
        //qDebug() << "This makes " << nDamage << " hit points damage to " << attackedCreature->species() << " and " << nSpellPointBonus << " bonus spell points for " << attackingCreature->player()->name();
    }
    attackingCreature->setHasAttacked(true);
    if (!attackingCreature->hasDistanceAttack())
        deselectAll(emitSignal);
    if (emitSignal)
        emitCreatureAttacked(attackingCreature, attackedCreature, nDamage);
}

bool GameService::isDistanceAttackable(Creature *creature)
{
    return isDistanceAttackable(game->selectedCreature(), creature);
}

bool GameService::isDistanceAttackable(Creature *attacker, Creature *attacked)
{
    if (!attacker)
    {
        //qDebug() << "no attacker";
        return false;
    }
    if (attacker == attacked)
    {
        //qDebug() << "It's me!";
        return false;
    }
    if (!attacker->hasDistanceAttack())
    {
        //qDebug() << "has no distance attack";
        setMessage(tr("Your selected creature has no distance attack"));
        return false;
    }
    if (attacker->hasDistanceAttacked())
    {
        //qDebug() << "already used distance attack";
        setMessage(tr("This creature already used it's distance attack"));
        return false;
    }
    //if (!attacker->hasAttacked()) return false;
    // no friendly fire!
    if (attacker->player() == attacked->player())
    {
        //qDebug() << "friendly fire is possible";
        setMessage(tr("Tip: Don't harm your own creatures ;-)"));
        //return false;
    }
    if (attacker->remainingMovePoints() > 0 && !attacker->hasAttacked())
    {
        //qDebug() << "is still in movement mode";
        setMessage(tr("Please cancel move mode first to use the distance attack"));
        return false;
    }
    if (!canSee(attacker, attacked))
    {
        setMessage(tr("Another creature is blocking the attack."));
        return false;
    }
    if (int(getRealMovementDistance(attacker, attacked->x(), attacked->y())) > attacker->distanceRange())
    {
        //qDebug() << "too far away";
        setMessage(tr("This one is too far away!"));
        return false;
    }
    //qDebug() << "isDistanceAttackable = true";
    // zurücksetzen
    setMessage("");
    return true;
}

bool GameService::canSee(Creature *attacker, Creature *attacked)
{
    int eyes11x = attacker->x() * 10 + 4;
    int eyes12x = attacker->x() * 10 + 6;
    int eyes11y = attacker->y() * 10 + 2;
    int eyes12y = eyes11y + 1;

    int eyes21x = attacked->x() * 10 + 4;
    int eyes22x = attacked->x() * 10 + 6;
    int eyes21y = attacked->y() * 10 + 2;
    int eyes22y = eyes21y + 1;

    QPolygon collisionPoly;
    collisionPoly << QPoint(eyes11x, eyes11y) << QPoint(eyes12x, eyes12y) << QPoint(eyes22x, eyes22y) << QPoint(eyes21x, eyes21y) << QPoint(eyes11x, eyes11y);
    QRegion collisionRegion(collisionPoly);
    for (int xx = 0; xx < config->HCOUNT; xx++)
    {
        for (int yy = 0; yy < config->VCOUNT; yy++)
        {
            Creature* currentCreature = getCreatureAt(xx, yy);
            if (currentCreature == attacker || currentCreature == attacked)
                continue;
            QRect currentRect(xx * 10, yy * 10, 10, 10);
            if (collisionRegion.intersects(currentRect) && currentCreature)
            {
                qDebug() << xx << yy << "is blocking the spell";
                return false;
            }
        }
    }
    return true;
}

void GameService::distanceAttackCreature(Creature *attackedCreature, bool emitSignal)
{
    Creature *attackingCreature = game->selectedCreature();
    // calculate damage
    int nDamage = calculateDistanceDamage(attackingCreature, attackedCreature);
    // calculate resulting spell point bonsu
    int nSpellPointBonus = calculateDistanceSpellPointBonus(nDamage);
    // apply damage to attacked creature
    applyDamage(attackedCreature, nDamage, false);
    // apply spell point bonsu to player of selected creature
    applySpellPointBonus(attackingCreature->player(), nSpellPointBonus);
    if (attackingCreature->player())
    {
        //qDebug() << "This makes " << nDamage << " hit points damage to " << attackedCreature->species() << " and " << nSpellPointBonus << " bonus spell points for " << attackingCreature->player()->name();
    }
    if (emitSignal)
        emitCreatureDistanceAttacked(attackingCreature, attackedCreature, nDamage);
    deselectAll(emitSignal);
}

void GameService::scrollAttackCreature(Scroll* scroll, Creature *attackedCreature)
{
    // calculate damage
    int nDamage = 0;
    // #45 10% Chance, dass Distanzattacke komplett versemmelt wird...
    int nRandomMiss = randomInteger(0, 100);
    if (nRandomMiss >= 10)
        nDamage = randomInteger(scroll->minDamage(), scroll->maxDamage() + 1);
    nDamage = nDamage > attackedCreature->hp() ? attackedCreature->hp() : nDamage;

    if (!scroll->immune().isEmpty() && (scroll->immune() == attackedCreature->immune() || attackedCreature->immune() == "ALL"))
    {
        // #56 Bei Immunität zufällig 0 oder 1 Schaden
        nDamage = randomInteger(0, 2);
    }

    // calculate resulting spell point bonsu
    int nSpellPointBonus = calculateDistanceSpellPointBonus(nDamage);
    // apply damage to attacked creature
    applyDamage(attackedCreature, nDamage, false);
    // apply spell point bonsu to player of selected creature
    applySpellPointBonus(scroll->player(), nSpellPointBonus);
    //qDebug() << "This makes " << nDamage << " hit points damage to " << attackedCreature->species() << " and " << nSpellPointBonus << " bonus spell points for " << scroll->player()->name();
    emitCreatureDistanceAttacked(scroll, attackedCreature, nDamage);
    deselectAll();
}

int GameService::calculateDamage(Creature *attacker, Creature *attacked)
{
    double defensefaktor = (1.0 / (1.0 * attacked->defense()));
    double zufall = attacker->strength() * randomDouble() + (1 / (attacker->strength() * defensefaktor));
    double Schaden = (-1.0 / (defensefaktor * zufall)) + attacker->strength() + 0.5;
    int iSchaden = ((int)qRound(Schaden) < attacked->hp()) ? (int)qRound(Schaden) : attacked->hp();
    // #57 zu 10% geht er daneben --> So ein Gefrickel
    int nDaneben = randomInteger(0, 100);
    if (nDaneben < 10)
    {
        iSchaden = 0;
    }
    if (attacker->player() == attacker && attacker->player()->race() == "rogue" && attacked != attacked->player())
    {
        int nVolltreffer = randomInteger(0, 100);
        if (nVolltreffer >= 25 && nVolltreffer < 50)
        {
            //qDebug() << "M-m-m-m-multikill! Naja fast, aber der Rogue hat voll zugeschlagen.";
            iSchaden = attacked->hp();
        }
    }
    return iSchaden;
}

int GameService::calculateSpellPointBonus(int nDamage)
{
    return nDamage * 2;
}

int GameService::calculateDistanceDamage(Creature *attacker, Creature *attacked)
{
    int DamageDiff = attacker->distanceStrength();
    double defensefaktor = (1.0 / (1.0 * attacked->defense()));
    double zufall = DamageDiff * randomDouble() + (1 / (DamageDiff * defensefaktor));
    double nSchaden = (-1.0 / (defensefaktor * zufall)) + DamageDiff + 0.5;
    // #57 zu 20% geht er daneben
    int nDaneben = randomInteger(0, 100);
    if (nDaneben < 20)
    {
        nSchaden = 0;
    }
    // #56: 0 oder 1 Schaden bei Immunität
    if (!attacker->immune().isEmpty() && (attacker->immune() == attacked->immune() || attacked->immune() == "ALL"))
    {
        nSchaden = randomInteger(0, 2);
    }
    int iSchaden = ((int)qRound(nSchaden) < attacked->hp()) ? (int)qRound(nSchaden) : attacked->hp();
    return iSchaden;
}

int GameService::calculateDistanceSpellPointBonus(int nDamage)
{
    return nDamage;
}

void GameService::applyDamage(Creature *attacked, int nDamage, bool moveToWhenDead)
{
    attacked->setHp(attacked->hp() - nDamage);
    tryKillCreature(attacked, moveToWhenDead);
}

void GameService::tryKillCreature(Creature *c, bool moveToWhenDead)
{
    game->setIsCreatureDying(false);
    if (isKillable(c))
    {
        int x = c->x();
        int y = c->y();
        killCreature(c);
        if (moveToWhenDead && !game->isOver)
        {
            game->setIsCreatureDying(true);
            moveCreature(game->selectedCreature(), x, y);
        }
    }
}

bool GameService::isKillable(Creature *c)
{
    if (c->hp() <= 0)
        return true;
    return false;
}

void GameService::killCreature(Creature *c)
{
    //qDebug() << "begin killCreature" << c->species();
    if (c->player() == c)
    {
        // kill player
        c->player()->setAlive(false);
        foreach(Creature *creature, c->player()->m_Creatures)
        {
            //game->m_creatures.removeAll(creature);
            creature->setAlive(false);
            emitCreatureDied(creature);
        }
        tryGameOver();
    }
    else
    {
        // kill creature
        //c->player()->m_Creatures.removeAll(c);
        //game->m_creatures.removeAll(c);
        c->setAlive(false);
        emitCreatureDied(c);
    }
    //game->emitPropertyChanged();
    //game->emitCreaturesChanged();
    //qDebug() << "end killCreature" << c->species();
}

void GameService::tryGameOver()
{
    if (isGameOver())
        goGameOver();
}

bool GameService::isGameOver()
{
    //qDebug() << "begin isGameOver";
    int playerCount = 0;
    foreach (Player *p, game->m_players)
    {
        if (p->alive()) playerCount++;
    }
    //qDebug() << "end isGameOver with " << (playerCount <= 1);
    if (playerCount <= 1)
        return true;
    return false;
}

void GameService::goGameOver()
{
    //qDebug() << "begin goGameOver";
    game->isOver = true;
    game->setState("gameOverState");
    emitGameOver();
    //qDebug() << "end goGameOver";
}

void GameService::applySpellPointBonus(Player* player, int nSpellPoints)
{
    if (player)
    {
        //qDebug() << "applying spell point bonus" << nSpellPoints;
        player->setSpellPoints(player->spellPoints() + nSpellPoints);
    }
}

void GameService::selectSpell(Creature *creature)
{
    tryBuy(creature);
    //game->currentPlayer()->setSpellPoints(game->currentPlayer()->spellPoints() - creature->manaCost());
    //emitSpellSelected(creature);
}

Creature* GameService::getCreatureAt(int x, int y)
{
    foreach(Player* player, game->m_players)
    {
        if (player->x() == x && player->y() == y && player->alive()) return player;
        foreach(Creature* creature, player->m_Creatures)
        {
            if (creature->x() == x && creature->y() == y && creature->alive()) return creature;
        }
    }
    return NULL;
}

bool GameService::tryAttackCreature(Creature *creature)
{
    //qDebug() << "begin tryAttackCreature";
    if (game->state() != "moveState") return false;
    if (isAttackable(creature))
    {
        attackCreature(creature);
        //qDebug() << "end tryAttackCreature = true";
        return true;
    }
    //qDebug() << "end tryAttackCreature = false";
    return false;
}

void GameService::tryAttacks(Creature *creature)
{
    if (!tryAttackCreature(creature))
        tryDistanceAttackCreature(creature);
}

void GameService::tryDistanceAttackCreature(Creature *creature)
{
    //qDebug() << "begin tryDistanceAttackCreature";
    if (game->state() != "moveState") return;
    if (isDistanceAttackable(creature))
        distanceAttackCreature(creature);
    //qDebug() << "end tryDistanceAttackCreature";
}

void GameService::tryMoveCreature(Creature *creature, int x, int y)
{
    if (game->state() != "moveState") return;
    if (isMovementPossible(creature, x, y))
    {
        moveCreature(creature, x, y);
        if (game->selectedCreature()->remainingMovePoints() < 0.5)
        {
            // mit so wenigen Spellpoints kann er sich eh nicht mehr bewegen...
            // Abwahl, es sei denn er könnte noch Distanzattacken ausführen
            if (!game->selectedCreature()->hasDistanceAttack())
            {
                deselectAll();
            }
            else
            {
                setMessage(tr("Choose a target!"));
                game->selectedCreature()->setDistanceAttackMode(true);
            }
        }
    }
}

void GameService::deselectAll(bool emitSignal)
{
    if (game->selectedCreature())
    {
        game->selectedCreature()->setHasDistanceAttacked(true);
        game->selectedCreature()->setHasMoved(true);
    }
    game->setSelectedCreature(NULL, emitSignal);
    setMessage(tr("Select a unit!"));
}

void GameService::abort(Creature *creature)
{
    if (game->state() == "spellSelectState")
    {
        game->setState("moveState");
    }
    else if (game->state() == "moveState")
    {
        if (game->selectedCreature())
        {
            //qDebug() << "Has Distance attack and not allready done it?" << game->selectedCreature()->hasDistanceAttack();
            if (game->selectedCreature()->hasDistanceAttack() && !game->selectedCreature()->hasDistanceAttacked() && !game->selectedCreature()->distanceAttackMode())
            {
                //qDebug() << "enabling distance attack";
                game->selectedCreature()->setRemainingMovePoints(0);
                game->increase();
                setMessage(tr("Choose a target!"));
                game->selectedCreature()->setDistanceAttackMode(true);
            }
            else
            {
                //qDebug() << "deselecting";
                game->selectedCreature()->setRemainingMovePoints(0);
                game->selectedCreature()->setHasDistanceAttacked(true);
                game->selectedCreature()->setDistanceAttackMode(false);
                deselectAll();
            }
        }
        else
        {
            switchNextPlayer();
        }
    }
    else if (game->state() == "castSpellState")
    {
        if (creature)
        {
            // Manakosten wieder erstatten
            game->currentPlayer()->setSpellPoints(game->currentPlayer()->spellPoints() + creature->manaCost());
        }
        game->setState("moveState");
    }
}

void GameService::start()
{
    game->arrLastPlayerNames.clear();
    foreach(Player* p, game->m_players)
    {
        p->initialize();
        loadPlayerSpells(p);
        if (game->manaMode())
        {
            p->setSpellPoints(p->spellPoints() + 100);
        }
        // #41 Namen merken
        if (!p->isNPC())
            game->arrLastPlayerNames.append(p->name());
    }
    statistics->reset();
    game->setHasBegun(true);
    game->setState("spellSelectState");
}

void GameService::emitPlayersCreaturesDie(Player *player)
{
    emit playersCreaturesDie(player);
}

void GameService::trySelectCreature(Creature *creature)
{
    //if (!statemanager->configuration().contains(statemanager->moveState)) return;
    if (isSelectable(creature))
        selectCreature(creature);
}

void GameService::tryCast(int x, int y, Creature *creature)
{
    // if (!statemanager->configuration().contains(statemanager->castState)) return;
    if (isCastable(x, y, creature))
    {
        if (QString(creature->metaObject()->className()).startsWith("Creature"))
        {
            castCreature(x, y, creature);
        }
        else
        {
            Scroll* scroll = qobject_cast<Scroll*>(creature);
            //qDebug() << "Scroll, here we go!";
            castScroll(scroll, x, y);
        }
    }
}

void GameService::tryBuy(Creature *creature)
{
    if (canAfford(game->currentPlayer(), creature))
    {
        buy(creature);
    }
}

void GameService::buy(Creature *creature)
{
    if (creature->species() != "Mana Transfer")
    {
        game->currentPlayer()->setSpellPoints(game->currentPlayer()->spellPoints() - creature->manaCost());
    }
    game->setState("castSpellState");
}

bool GameService::isCastable(int x, int y, Creature* creature)
{
    bool bIsCastable = true;
    //qDebug() << "begin isCastable at" << x << "," << y;
    Player* player = game->currentPlayer();
    int nPlayerX = player->x();
    int nPlayerY = player->y();
    int xDist = abs(nPlayerX - x);
    int yDist = abs(nPlayerY - y);
    //qDebug() << creature->metaObject()->className();
    if (QString(creature->metaObject()->className()).startsWith("Creature"))
    {
        //qDebug() << "It's yet another creature";
        if (xDist <= 1 && yDist <= 1 && !(xDist == 0 && yDist == 0))
        {
            //qDebug() << "end isCastable with true";
        }
        else
        {
            setMessage(tr("You have to cast your spell exactly beside you"));
            bIsCastable = false;
        }
        Creature* existingCreature = getCreatureAt(x, y);
        if (existingCreature && existingCreature->alive())
        {
            setMessage(tr("You can't cast a creature where another one exists."));
            bIsCastable = false;
        }
    }
    else
    {
        //qDebug() << "Wuhuuu, a scroll";
        Scroll* scroll = qobject_cast<Scroll*>(creature);
        if (scroll->type() != "friendly")
        {
            if (getDistance(game->currentPlayer(), x, y) <= scroll->range())
            {
                Creature *c = getCreatureAt(x, y);
                if (c && isEnemy(c))
                {
                    //qDebug() << "end isCastable with true (" << getDistance(game->currentPlayer(), x, y) << "<=" << scroll->range();
                    if (!canSee(game->currentPlayer(), c))
                    {
                        bIsCastable = false;
                    }
                }
                else
                {
                    setMessage(tr("You have to cast this spell on an enemy creature"));
                    bIsCastable = false;
                }
            }
            else
            {
                setMessage(tr("This creature is not in the range of your spell"));
                bIsCastable = false;
            }
        }
        else // d.h. (scroll->type = "friendly") z.B. healing
        {
            if (getDistance(game->currentPlayer(), x, y) <= scroll->range())
            {
                Creature *c = getCreatureAt(x, y);
                if (c)
                {
                    if (!canSee(game->currentPlayer(), c))
                    {
                        bIsCastable = false;
                    }
                }
                else
                {
                    setMessage(tr("You have to cast this spell on a creature"));
                    bIsCastable = false;
                }
            }
            else
            {
                setMessage(tr("This creature is not in the range of your spell"));
                bIsCastable = false;
            }
        }
        //qDebug() << getDistance(game->currentPlayer(), x, y) << ">" << scroll->range();
    }
    //qDebug() << "end isCastable with false";
    return bIsCastable;
}

void GameService::castCreature(int x, int y, Creature *creature)
{
    //qDebug() << "Casting creature";
    addCreature(creature, x, y);
    game->setState("moveState");
    //statemanager->gotoMoveState();
}

void GameService::getFreeFieldsAround(Creature *creature, QList<QPoint> &lstFree)
{
    for (int xx = -1; xx <= 1; xx++)
    {
        for (int yy = -1; yy <= 1; yy++)
        {
            if (xx == 0 && yy == 0) continue;
            if (creature->x() + xx < 0 || creature->x() + xx >= config->HCOUNT) continue;
            if (creature->y() + yy < 0 || creature->y() + yy >= config->VCOUNT) continue;
            if (!getCreatureAt(creature->x() + xx, creature->y() + yy))
                lstFree.append(QPoint(creature->x() + xx, creature->y() + yy));
        }
    }
}

Creature * GameService::getNearestEnemy(Creature *creature)
{
    QMap<int,QList<Creature*> > distMap;
    for (int ii = 0; ii < game->m_players.length(); ii++)
    {
        Player* player = game->m_players[ii];
        if (player == creature->player()) continue;
        if (!player->alive()) continue;
        // every single creature of player
        for (int jj = 0; jj < player->m_Creatures.length(); jj++)
        {
            Creature* enemy = player->m_Creatures[jj];
            if (!enemy->alive()) continue;
            double dDistance = getDistance(creature, enemy->x(), enemy->y());
            if (dDistance <= 1.5) dDistance = 1;
            if (!distMap.contains(dDistance))
            {
                QList<Creature*> newList; newList.append(enemy);
                distMap.insert(dDistance, newList);
            }
            else
            {
                distMap[dDistance].append(enemy);
            }
        }
        // player himself?
        double dDistance = getDistance(creature, player->x(), player->y());
        if (dDistance <= 1.5) dDistance = 1;
        if (!distMap.contains(dDistance))
        {
            QList<Creature*> newList; newList.append(player);
            distMap.insert(dDistance, newList);
        }
        else
        {
            distMap[dDistance].append(player);
        }
    }
    if (distMap.size() == 0)
        return NULL;
    QList<Creature*> nearestCreatures = distMap[distMap.keys().first()];
    int nRand = randomInteger(0, nearestCreatures.size());
    return nearestCreatures[nRand];
}

void GameService::getOrderedFieldsByEnemy(const QList<QPoint> &lstFree, Creature *creature, QMap<double, QPoint> &map)
{
    foreach (QPoint p, lstFree)
    {
        double distance = getDistance(creature, p.x(), p.y());
        map.insert(distance, p);
    }
    qDebug() << "Entfernungen" << map;
}

bool GameService::isEnemyAdjacent(Creature *creature, Creature *enemy)
{
    double distance = getDistance(creature, enemy->x(), enemy->y());
    if (distance < config->ATTACKDISTANCE)
    {
        return true;
    }
    return false;
}

bool GameService::isEnemy(Creature *creature)
{
    return creature->player() != game->currentPlayer();
}

QString GameService::getRandomCreatureName(Player* /*player*/)
{
    int nCount = library->lstEnemyNames.count();
    if (nCount <= 0) return "";
    int randomNumber = randomInteger(0, nCount);
    return QString(library->lstEnemyNames[randomNumber]->name);
}

void GameService::checkCheat(QString strNewCheatCharacter)
{
    qDebug() << "CheckCheat";
    if (game->state() == "playerNameState")
    {
        game->strCurrentCheat += strNewCheatCharacter;
        if (game->strCurrentCheat.toLower().endsWith("manamode"))
        {
            qDebug() << "Mana Mode!!";
            // MANA Mode!
            game->setManaMode(true);
            game->strCurrentCheat = "";
        }
        if (game->strCurrentCheat.length() > 100) game->strCurrentCheat = "";
        return;
    }
    else if (game->state() != "spellSelectState") return;
    game->strCurrentCheat += strNewCheatCharacter;
    if (game->strCurrentCheat.toLower().endsWith("gimmemana"))
    {
        game->currentPlayer()->setSpellPoints(game->currentPlayer()->spellPoints() + 10);
        qDebug() << "Damn Cheater! Here are 10 bucks for you!";
        game->strCurrentCheat = "";
    }
    if (game->strCurrentCheat.toLower().endsWith("gimmemoremana"))
    {
        game->currentPlayer()->setSpellPoints(game->currentPlayer()->spellPoints() + 20);
        qDebug() << "Damn Cheater! Here are 20 bucks for you!";
        game->strCurrentCheat = "";
    }

    // check if cheat is a creature
    foreach (Creature* c, library->lstCheatCreatures)
    {
        qDebug() << "Is cheat a" << c->species();
        if (game->strCurrentCheat.toLower().endsWith(c->cheat().toLower()))
        {
            if (game->currentPlayer()->m_PossibleCreatures.last()->category() > 3)
                game->currentPlayer()->m_PossibleCreatures.pop_back();
            game->currentPlayer()->m_PossibleCreatures.append(c);
            game->currentPlayer()->emitPossibleCreaturesChanged();
            qDebug() << "Damn Cheater! Now you can cast" << c->species();
            game->strCurrentCheat = "";
        }
    }

    if (game->strCurrentCheat.length() > 100) game->strCurrentCheat = "";
}

void GameService::setCreatureImage(int index, int currentColor, QString category)
{

    QString strFilename = "";
    if (category == "dark priest")
        strFilename = library->lstPlayerColors.at(currentColor)->file;
    else if (category == "sorcerer")
        strFilename = library->lstPlayerColorsSorcerer.at(currentColor)->file;
    else if (category == "rogue")
        strFilename = library->lstPlayerColorsRogue.at(currentColor)->file;
    else if (category == "warrior")
        strFilename = library->lstPlayerColorsWarrior.at(currentColor)->file;
    else return;
    game->m_creatures.at(index)->setImageFilename(strFilename);
    game->m_players.at(index)->setImageFilename(strFilename);
    //qDebug() << game->m_creatures.at(index)->imageFilename();
}

void GameService::getEnemiesAround(Creature *creature, QList<Creature *> &lstEnemies)
{
    for (int xx = -1; xx <= 1; xx++)
    {
        for (int yy = -1; yy <= 1; yy++)
        {
            if (xx == 0 && yy == 0) continue;
            if (creature->x() + xx < 0 || creature->x() + xx >= config->HCOUNT) continue;
            if (creature->y() + yy < 0 || creature->y() + yy >= config->VCOUNT) continue;
            Creature *enemy = NULL;
            enemy = getCreatureAt(creature->x() + xx, creature->y() + yy);
            if (enemy && enemy->player() != creature->player())
                lstEnemies.append(enemy);
        }
    }
}

bool GameService::isTitleSongPlaying()
{
    return titleSound && titleSound->state() == QMediaPlayer::PlayingState;
}

void GameService::playTitleSong(bool startstop)
{
    bool musicAllowed = settings->value("music", false).toBool();
    if (!titleSound)
    {
        titleSound = new QMediaPlayer;
#ifdef Q_OS_ANDROID
    //QUrl file("assets:/qml/sounds/morzyn intro.mp3");
    QUrl file("qrc:/qml/sounds/morzyn intro.mp3");
#else
#ifdef SAILFISH
    QUrl file = (SailfishApp::pathTo("qml/sounds/morzyn intro.mp3")/*.toLocalFile()*/);
#else
    QUrl file("qrc:/qml/sounds/morzyn intro.mp3");
    //QUrl file(QUrl::fromLocalFile("qml/sounds/morzyn intro.mp3"));
#endif
#endif
        QMediaPlaylist *playlist = new QMediaPlaylist;
        playlist->addMedia(file);
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
        titleSound->setPlaylist(playlist);
        qDebug() << titleSound->availability();
        qDebug() << "Loaded song " << "qml/morzyn/sounds/morzyn intro.mp3";
    }
    if (musicAllowed && startstop/* && titleSound->state() != QMediaPlayer::PlayingState*/)
    {
#ifndef SAILFISH
        QPropertyAnimation* animation = new QPropertyAnimation(titleSound, "volume", this);
        animation->setDuration(2000);
        //animation->setStartValue(0);
        animation->setEndValue(100);
        qDebug() << "Fading in song " << "qml/morzyn/sounds/morzyn intro.mp3";
        animation->start(QAbstractAnimation::DeleteWhenStopped);
#endif
        titleSound->play();
    }
    else if (!musicAllowed || (!startstop /*&& titleSound->state() == QMediaPlayer::PlayingState*/))
    {
#ifndef SAILFISH
        QPropertyAnimation* animation = new QPropertyAnimation(titleSound, "volume", this);
        animation->setDuration(2000);
        //animation->setStartValue(100);
        animation->setEndValue(0);
        qDebug() << "Fading out song " << "qml/morzyn/sounds/morzyn intro.mp3";
        connect(animation, SIGNAL(finished()), titleSound, SLOT(pause()));
        animation->start(QAbstractAnimation::DeleteWhenStopped);
#else
        titleSound->pause();
#endif
    }
}

void GameService::getRandomPlayerColor(Player *player)
{
    // geht davon aus, dass die Indexe der Farben immer gleich sind
    if (player->race() == "rogue")
    {
        int nColor = randomInteger(0, library->lstPlayerColorsRogue.count());
        while (game->arrUsedPlayerColors.contains(nColor))
            nColor = randomInteger(0, library->lstPlayerColorsRogue.count());
        game->arrUsedPlayerColors.insert(nColor);
        player->setImageFilename(library->lstPlayerColorsRogue.at(nColor)->file);
    }
    else if (player->race() == "sorcerer")
    {
        int nColor = randomInteger(0, library->lstPlayerColorsSorcerer.count());
        while (game->arrUsedPlayerColors.contains(nColor))
            nColor = randomInteger(0, library->lstPlayerColorsSorcerer.count());
        game->arrUsedPlayerColors.insert(nColor);
        player->setImageFilename(library->lstPlayerColorsSorcerer.at(nColor)->file);
    }
    else if (player->race() == "warrior")
    {
        int nColor = randomInteger(0, library->lstPlayerColorsWarrior.count());
        while (game->arrUsedPlayerColors.contains(nColor))
            nColor = randomInteger(0, library->lstPlayerColorsWarrior.count());
        game->arrUsedPlayerColors.insert(nColor);
        player->setImageFilename(library->lstPlayerColorsWarrior.at(nColor)->file);
    }
}

bool GameService::checkDuplicates()
{
    QSet<QString> usedColors;
    foreach(Player *p, game->m_players)
    {
        if(usedColors.contains(p->imageFilename()))
            return true;
        usedColors.insert(p->imageFilename());
    }
    return false;
}

int GameService::getColorIndex(QString imageFilename)
{
    for(int ii = 0; ii < library->lstPlayerColorsRogue.length(); ii++)
    {
        PlayerColor* pc = library->lstPlayerColorsRogue[ii];
        if (pc->file == imageFilename)
            return ii;
    }
    for(int ii = 0; ii < library->lstPlayerColorsSorcerer.length(); ii++)
    {
        PlayerColor* pc = library->lstPlayerColorsSorcerer[ii];
        if (pc->file == imageFilename)
            return ii;
    }
    for(int ii = 0; ii < library->lstPlayerColorsWarrior.length(); ii++)
    {
        PlayerColor* pc = library->lstPlayerColorsWarrior[ii];
        if (pc->file == imageFilename)
            return ii;
    }
    return 0;
}

void GameService::setFullScreen(bool fullscreen)
{
#ifdef Q_OS_WIN32
    setBoolSetting("fullscreen", fullscreen);
    game->setFullScreen(fullscreen);
#else
    setBoolSetting("fullscreen", true);
    game->setFullScreen(true);
#endif
}

void GameService::simulateFight(Creature *c1, Creature *c2)
{
    double dLeftPoints = 0.0;
    double dRightPoints = 0.0;
    Creature* cLeft = new Creature(c1->parent());
    cLeft->load(c1->sourceCode);
    Creature* cRight = new Creature(c2->parent());
    cRight->load(c2->sourceCode);
    for (int ii = 0; ii < 1000; ii++)
    {
        cLeft->setAlive(true);
        cLeft->setHp(cLeft->originalHp());
        cRight->setAlive(true);
        cRight->setHp(cRight->originalHp());
        Creature *cFirst = ii % 2 == 0 ? cLeft : cRight;
        Creature *cSecond = ii % 2 == 0 ? cRight : cLeft;
        while (cLeft->alive() && cRight->alive())
        {
            selectCreature(cFirst, false);
            //qDebug() << cLeft->species() << "selected";
            attackCreature(cSecond, false);

            //qDebug() << cRight->species() << "attacked";
            if (!cSecond->alive())
                break;
            if (cFirst->hasDistanceAttack())
            {
                distanceAttackCreature(cSecond, false);
                if (!cSecond->alive())
                    break;
            }
            //qDebug() << cRight->species() << "selected";
            selectCreature(cSecond, false);
            //qDebug() << cLeft->species() << "attacked";
            attackCreature(cFirst, false);

            if (!cFirst->alive())
                break;
            if (cSecond->hasDistanceAttack())
            {
                distanceAttackCreature(cFirst, false);
                if (!cFirst->alive())
                    break;
            }
        }
        if (cLeft->alive())
        {
            dLeftPoints++;
            qDebug() << "Left:" << dLeftPoints;
            dRightPoints += ((double)cLeft->originalHp() - (double)cLeft->hp()) / (double)cLeft->originalHp();
            qDebug() << "Right:" << dRightPoints;
        }
        else
        {
            dRightPoints++;
            qDebug() << "Right:" << dRightPoints;
            dLeftPoints += ((double)cRight->originalHp() - (double)cRight->hp()) / (double)cRight->originalHp();
            qDebug() << "Left:" << dLeftPoints;
        }
    }
    cLeft->deleteLater();
    cRight->deleteLater();
    int iLeftPoints = qRound(dLeftPoints);
    int iRightPoints = qRound(dRightPoints);
    if (iLeftPoints > iRightPoints)
    {
        setMessage(QString(tr("%0 wins (left side) %1:%2")).arg(c1->species()).arg(iLeftPoints).arg(iRightPoints));
    }
    else if (iLeftPoints == iRightPoints)
    {
        setMessage(QString(tr("Draw! (%0:%1)").arg(iLeftPoints).arg(iRightPoints)));
    }
    else
    {
        setMessage(QString(tr("%0 wins (right side) %1:%2")).arg(c2->species()).arg(iLeftPoints).arg(iRightPoints));
    }
    qDebug() << message();
}

QStringList GameService::getCreatureImages(QString filenamePattern)
{
    QString strFilePattern = filenamePattern;
    QStringList lstMatchingFiles;
#ifdef Q_OS_ANDROID
    //QDir dirImages("assets:/qml/images");
    QDir dirImages(QString(":/qml/images"));
#else
#ifdef SAILFISH
    QDir dirImages(SailfishApp::pathTo("qml/images").toLocalFile());
#else
    QDir dirImages(QString(":/qml/images"));
#endif
#endif
    QStringList lstFilters;
    lstFilters << "*.png";
    dirImages.setNameFilters(lstFilters);
    // creature image
    QRegExp rex(strFilePattern);
    foreach(QString strFileName, dirImages.entryList())
    {
        if (rex.exactMatch(strFileName))
            lstMatchingFiles.append(strFileName);
    }
    return lstMatchingFiles;
}

IAI *GameService::getAI(Player *player)
{
    if (!game->m_AI.contains(player)) return NULL;
    return game->m_AI[player];
}

void GameService::saveSettings(bool bMusic, bool bSound, bool bFullscreen)
{
    settings->setValue("fullscreen", QVariant((bool) bFullscreen));
}

bool GameService::getBoolSetting(QString name)
{
    //qDebug() << "Boolean Setting" << name << ":" << settings->value(name, QVariant((bool)false)).toBool();
    return settings->value(name, QVariant((bool)false)).toBool();
}

void GameService::setBoolSetting(QString name, bool bValue)
{
    settings->setValue(name, QVariant((bool)bValue));
    settings->sync();
}

void GameService::setGameMode(QString mode)
{
    game->mode = mode;
}

void GameService::initTutorialGame()
{
    game->mode = "tutorial";
    addPlayers(2, 1);
    start();
}

void GameService::addTutorialPlayers()
{
    setTotalPlayers(2);
    setHumanPlayers(1);

    Player* p = new Player(game);
    p->setName("Tutorial Player");
    addPlayer(p);

    Player* p2 = new Player(game);
    p2->setName("Tutorial Opponent");
    addPlayer(p2);

    game->playersChanged();
    game->creaturesChanged();
}

void GameService::addTutorialPlayer(Player *player)
{
    // wenn aktuelle Spieleranzahl schon die der menschlichen Spieler erreicht hat,
    // dann ist alles weitere NPC
    /*if (game->m_players.count() >= game->humanPlayers)
    {
        player->setIsNPC(true);
        IAI *ai = new EasyAI(player, this);
        //player->setName(getRandomCreatureName());
        game->m_AI.insert(player, ai);
    }
    player->setUnitClass("Player");
    player->setOriginalName(tr("Player %1", "standard player original name (for remembering it)").arg(game->m_players.count() + 1));
    player->setName(tr("Player %1", "standard player name").arg(game->m_players.count() + 1));
    // "rogue", "warrior", "sorcerer"
    int randRace = randomInteger(0, 3);
    if (randRace == 0)
        player->setRace("rogue");
    else if (randRace == 1)
        player->setRace("warrior");
    else if (randRace == 2)
        player->setRace("sorcerer");
    player->setX(0);
    player->setY(0);
    getRandomPlayerColor(player);
    //player->setImageFilename(library->lstPlayerColors.at(game->m_players.count())->file);
    game->m_players.append(player);
    game->m_creatures.append(player);
    emit creatureCasted(player);*/
}

void GameService::placeTutorialPlayers()
{

}

void GameService::quit()
{
    resetGame();
    qApp->quit();
}

bool GameService::getFullScreen()
{
    return settings->value("fullscreen", QVariant(true)).toBool();
}

QColor GameService::getColorOfEmptyField(int index, bool isLocked, int x, int y, Creature *selectedCreature, Player *currentPlayer, QString state, int counter)
{
    Q_UNUSED(counter)
    if (!currentPlayer) return Qt::transparent;
    QPoint fieldPoint(index % config->HCOUNT, index / config->HCOUNT);
    QList<QPoint> lstPoints;
    QColor color = Qt::transparent;
    m_bWriteMessages = false;
    if (isLocked) goto ende;
    if (currentPlayer->isNPC()) goto ende;
    if (state == "castSpellState")
    {
        if (!game->tempCreature()) goto ende;
        //qDebug() << "is" << game->tempCreature()->metaObject()->className();
        if (!QString(game->tempCreature()->metaObject()->className()).startsWith("Creature"))
        {
            if (isCastable(fieldPoint.x(), fieldPoint.y(), game->tempCreature()))
            {
                if (game->tempCreature()->type() == "friendly")
                {
                    color = QColor::fromRgba(0x5533FF33);
                }
                else
                {
                    color = QColor::fromRgba(0x55FF0000);
                }
                goto ende;
            }
        }
        else
        {
            getFreeFieldsAround(currentPlayer, lstPoints);
            if (lstPoints.contains(fieldPoint))
            {
                color = QColor::fromRgba(0x5533FF33);
                goto ende;
            }
        }
    }
    else if (state == "moveState")
    {
        if (!selectedCreature) goto ende;
        getFreeFieldsAround(selectedCreature, lstPoints);
        if (lstPoints.contains(fieldPoint))
            if (isMovementPossible(selectedCreature, fieldPoint.x(), fieldPoint.y(), false))
            {
                color = QColor::fromRgba(0x5533FF33);
                goto ende;
            }
            Creature *enemy = getCreatureAt(fieldPoint.x(), fieldPoint.y());
            if (enemy && isAttackable(selectedCreature, enemy))
            {
                color = QColor::fromRgba(0x55FF0000);
                goto ende;
            }
            else if (enemy && selectedCreature && isDistanceAttackable(selectedCreature, enemy))
            {
                color = QColor::fromRgba(0x55FF0000);
                goto ende;
            }
    }
    ende:
    m_bWriteMessages = true;
    return color;
}

void GameService::castHealing(Scroll *newScroll, int x, int y)
{
    newScroll->setX(game->currentPlayer()->x());
    newScroll->setY(game->currentPlayer()->y());
    newScroll->setPlayer(game->currentPlayer());
    game->m_scrolls.append(newScroll);
    game->emitPropertyChanged();

    Creature* c = getCreatureAt(x, y);
    if (c)
    {
        int percent = 0;
        // Bei Spielern: zwischen 30 und 50 Prozent wiederherstellen
        if (c->player() == c)
        {
            percent = randomInteger(30, 51);
        }
        // bei Kreaturen: zwischen 50 und 85 Prozent wiederherstellen
        else
        {
            percent = randomInteger(50, 86);
        }
        double dPercent = (double)percent / 100.0;
        int diff = dPercent * (double)c->hp();
        int newHp = c->hp() + diff;
        if (newHp > c->originalHp())
            newHp = c->originalHp();
        qDebug() << "newHp" << newHp;
        diff = newHp - c->hp();
        c->setHp(newHp);
        emit creatureHealed(c, diff);
    }
    game->setState("moveState");
}

int GameService::getManaTransferAmount(Player* player)
{
    return player->spellPoints() / 3;
}

void GameService::castManaTransfer(Scroll *newScroll, int x, int y)
{
    newScroll->setX(game->currentPlayer()->x());
    newScroll->setY(game->currentPlayer()->y());
    newScroll->setPlayer(game->currentPlayer());
    game->m_scrolls.append(newScroll);
    game->emitPropertyChanged();

    Creature* c = getCreatureAt(x, y);
    if (c)
    {
        int diff = getManaTransferAmount(game->currentPlayer());
        int nSpellPoints1 = game->currentPlayer()->spellPoints() - diff;
        int nSpellPoints2 = c->player()->spellPoints() + diff;
        game->currentPlayer()->setSpellPoints(nSpellPoints1);
        c->player()->setSpellPoints(nSpellPoints2);
        emit manaTransfered(game->currentPlayer(), c->player(), diff);
    }
    game->setState("moveState");
}

void GameService::setGame(Game *g)
{
    this->game = g;
    if (g)
    {
        connect(g, SIGNAL(stateChanged(QString)), this, SLOT(gameStateChanged(QString)));
    }
}

void GameService::setConstants(Constants *c)
{
    this->config = c;
}

void GameService::setLibrary(Library *l)
{
    this->library = l;
}

void GameService::setStatistics(Statistics *s)
{
    this->statistics = s;
}

void GameService::setSettings(QSettings *s)
{
    this->settings = s;
    game->setFullScreen(s->value("fullscreen", QVariant(true)).toBool());
}

void GameService::resetGame()
{
    deselectAll();
    game->setCurrentPlayer(NULL);
    game->totalPlayers = 0;
    game->humanPlayers = 0;
    game->isOver = false;
    game->setHasBegun(false);
    game->m_AI.clear();
    game->m_players.clear();
    game->m_creatures.clear();
    game->m_scrolls.clear();
    game->setSelectedCreature(NULL);
    game->setLastDamage(0);
    game->setIsCreatureDying(false);
    game->setIsLocked(false);
    game->emitCreaturesChanged();
    game->emitPlayersChanged();
    game->emitScrollsChanged();
    game->arrUsedPlayerColors.clear();
    game->setCounter(0);
    delete game->m_semaphore;
    game->m_semaphore = new QSemaphore(MAX_ANIMATION);
    emit gameReset();
}

void GameService::gameStateChanged(QString state)
{
    if (state == "moveState")
    {
        setMessage(tr("Select a unit!"));
    }
    else if (state == "castSpellState")
    {
        if (game->tempCreature() && QString(game->tempCreature()->metaObject()->className()).startsWith("Creature"))
            setMessage(tr("Cast the spell beside you!"));
        else
            setMessage(tr("Choose a target!"));
    }
    else if (state == "spellSelectState")
    {
        setMessage(tr("Select a spell!"));
    }
    else
    {
        setMessage("");
    }
}

void GameService::emitCreatureMoved(Creature *creature)
{
    emit creatureMoved(creature);
}

void GameService::emitPlayerSwitched(Player *player)
{
    emit playerSwitched(player);
}

void GameService::emitSpellSelected(Creature *creature)
{
    emit spellSelected(creature);
}

void GameService::emitCreatureCasted(Creature *creature)
{
    emit creatureCasted(creature);
}

void GameService::emitCreatureHovered(QObject *creature)
{
    emit creatureHovered(creature);
}

void GameService::emitCreatureUnhovered()
{
    emit creatureUnhovered();
}

void GameService::gotoMoveState()
{
    //qDebug() << "manual Transition to move state";
    emit moveStateGo();
}

void GameService::emitCreatureSelected(Creature *creature)
{
    emit creatureSelected(creature);
}

void GameService::emitCreatureAttacked(Creature *attackingCreature, Creature *attackedCreature, int nDamage)
{
    emit creatureAttacked(attackingCreature, attackedCreature, nDamage);
}

void GameService::emitCreatureDistanceAttacked(Creature *attackingCreature, Creature *attackedCreature, int nDamage)
{
    emit creatureDistanceAttacked(attackingCreature, attackedCreature, nDamage);
}

void GameService::emitCreatureDied(Creature *dyingcreature)
{
    emit creatureDied(dyingcreature);
}

void GameService::emitDeselectAll()
{
    emit deselectAll();
}

void GameService::emitGameOver()
{
    emit gameOver();
}

void GameService::emitPlayerDied(Creature *creature)
{
    emit playerDied(creature);
}
