
#include <QGuiApplication>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include "game.h"
#include "constants.h"
#include "gameservice.h"
#include "dbloader.h"
#include "creature.h"
#include "scroll.h"
#include "statistics.h"
#include <time.h>
#include <QTranslator>
#include <QScreen>

#include <sailfishapp.h>

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QGuiApplication* app = SailfishApp::application(argc, argv);

    qmlRegisterType<Creature>("harbour.morzyn", 1,0 , "Creature");
    qmlRegisterType<Player>("harbour.morzyn", 1,0 , "Player");
    qmlRegisterType<Game>("harbour.morzyn", 1,0 , "Game");
    qmlRegisterType<Scroll>("harbour.morzyn", 1,0 , "Scroll");
    qmlRegisterType<Statistics>("harbour.morzyn", 1,0 , "Statistics");
    qmlRegisterType<Library>("harbour.morzyn", 1,0 , "Library");

    qsrand ( time(NULL) );

    QString locale = QLocale::system().name();

    QTranslator translator;
    translator.load(QString("morzyn_") + locale, "translations");
    app->installTranslator(&translator);

    GameService gs;
    Game g;
    Constants c;
    c.HCOUNT = 10;
    c.VCOUNT = 10;
    Library library;
    Statistics s;

    gs.setGame(&g);
    gs.setConstants(&c);
    gs.setLibrary(&library);
    gs.setStatistics(&s);

    QQuickView* view = SailfishApp::createView();
    view->rootContext()->setContextProperty("gameService", &gs);
    view->rootContext()->setContextProperty("game", &g);
    view->rootContext()->setContextProperty("library", &library);
    view->rootContext()->setContextProperty("statistics", &s);
    view->rootContext()->setContextProperty("os", QVariant("sailfish"));
    view->rootContext()->setContextProperty("hCount", QVariant(c.HCOUNT));
    view->rootContext()->setContextProperty("vCount", QVariant(c.VCOUNT));
    view->setTitle("Morzyn v0.7.14");
    view->setSource(SailfishApp::pathTo("qml/morzyn.qml"));
    view->show();
    
    return app->exec();
}


