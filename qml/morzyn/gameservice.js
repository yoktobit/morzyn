Qt.include("database.js")

function getRandom(min, max) {
    if(min > max) {
        return -1;
    }

    if(min === max) {
        return min;
    }

    var r;

    do {
        r = Math.random();
    }
    while(r === 1.0);

    return min + parseInt(r * (max-min+1));
}

function generateDistinctNumberList(count, min, max)
{
    var lst = new Array();
    if (min >= max) return lst;
    if ((max - min + 1) < count)
    {
        count = max - min + 1;
    }
    while (lst.length < count)
    {
        var number = getRandom(min, max);
        if (lst.indexOf(number) === -1)
        {
            lst.push(number);
        }
    }
}

function addPlayer(ii)
{
    var newCreature = Qt.createQmlObject("import QtQuick 1.1; Creature { }", game);
    newCreature.name = "Player " + (ii + 1);
    newCreature.originalName = "Player " + (ii + 1);
    if (ii >= humanPlayerCountView.selected)
    {
        newCreature.ai = true;
    }
    game.creatures.append(newCreature);
    game.players.append(ii);
}

function addPlayers()
{
    for (var ii = 0; ii < totalPlayerCountView.selected; ii++)
    {
        addPlayer(ii);
    }
}

function loadPlayerSpells(player)
{
    var lst1 = generateDistinctNumberList(6, 0, getData().Scrolls.length);
    var lst2 = generateDistinctNumberList(5, 0, getData().Scrolls.length);
    var lst3 = generateDistinctNumberList(5, 0, getData().Scrolls.length);
    var lst4 = generateDistinctNumberList(5, 0, getData().Scrolls.length);
    for(i in lst1)
    {
        player.possibleCreatures.append(Library::GetInstance().lstCategory1[i]);
    }
    for(i in lst2)
    {
        player.possibleCreatures.append(Library::GetInstance().lstCategory2[i]);
    }
    for(i in lst3)
    {
        player.possibleCreatures.append(Library::GetInstance().lstCategory3[i]);
    }
    for(i in lst4)
    {
        player.possibleCreatures.append(Library::GetInstance().lstCategory4[i]);
    }
}


function placePlayers()
{
    var placedPlayers = new Array();
    var lastplace = 0;
    for (var ii = 0; ii < game.creatures.count; ii++)
    {
        var rnd = 0;
        do
        {
            rnd = getRandom(0, game.creatures.count - 1);
        } while(placedPlayers.indexOf(rnd) >= 0);

        var player = game.creatures.get(rnd);
        player.xField = Math.round(ii * (13 / game.creatures.count));
        var randomplace = 0;
        do
        {
            randomplace = getRandom(0, 12);
        } while (Math.abs(randomplace - lastplace) < 4);
        lastplace = randomplace;
        player.yField = randomplace;
        console.log(game.creatures.get(rnd).yField);
        placedPlayers.push(rnd);
    }
    game.currentPlayer = game.creatures.get(placedPlayers[0]);
}
