
#include <QGuiApplication>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include "../game.h"
#include "../constants.h"
#include "../gameservice.h"
#include "../dbloader.h"
#include "../creature.h"
#include "../scroll.h"
#include "../statistics.h"
#include <time.h>
#include <QTranslator>
#include <QScreen>

#include "sailfishapplication.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> app(Sailfish::createApplication(argc, argv));

    qmlRegisterType<Creature>("de.yoktobit.software.morzyn", 1,0 , "Creature");
    qmlRegisterType<Player>("de.yoktobit.software.morzyn", 1,0 , "Player");
    qmlRegisterType<Game>("de.yoktobit.software.morzyn", 1,0 , "Game");
    qmlRegisterType<Scroll>("de.yoktobit.software.morzyn", 1,0 , "Scroll");
    qmlRegisterType<Statistics>("de.yoktobit.software.morzyn", 1,0 , "Statistics");

    qsrand ( time(NULL) );

    QString locale = QLocale::system().name();

    QTranslator translator;
    translator.load(QString("morzyn_") + locale, "translations");
    app.data()->installTranslator(&translator);

    GameService gs;
    Game g;
    Constants c;
    Library l;
    Statistics s;
    //gs.setStateManager(&sm);
    gs.setGame(&g);
    gs.setConstants(&c);
    gs.setLibrary(&l);
    gs.setStatistics(&s);

    //QScopedPointer<QQuickView> view(Sailfish::createView("qml/morzyn/main.qml"));
    QScopedPointer<QQuickView> view(Sailfish::createView());
    view.data()->rootContext()->setContextProperty("gameService", &gs);
    view.data()->rootContext()->setContextProperty("game", &g);
    view.data()->rootContext()->setContextProperty("statistics", &s);
    view.data()->setTitle("Morzyn v0.8.0");
    Sailfish::setView(view.data(), "qml/morzyn/main.qml");
    Sailfish::showView(view.data());
    
    return app->exec();
}


