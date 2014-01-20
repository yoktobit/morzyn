#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
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
#include "../iai.h"
#include <time.h>
#include <QTranslator>
#include <QSettings>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stdout, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stdout, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stdout, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stdout, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
}

int main(int argc, char *argv[])
{
    //qInstallMessageHandler(myMessageOutput); //install : set the callback
    QGuiApplication app(argc, argv);

    QCoreApplication::setOrganizationDomain("yoktobit");
    QCoreApplication::setOrganizationName("yoktobit");
    QCoreApplication::setApplicationName("morzyn");

    qmlRegisterType<Creature>("harbour.morzyn", 1,0 , "Creature");
    qmlRegisterType<Player>("harbour.morzyn", 1,0 , "Player");
    qmlRegisterType<Game>("harbour.morzyn", 1,0 , "Game");
    qmlRegisterType<Scroll>("harbour.morzyn", 1,0 , "Scroll");
    qmlRegisterType<Statistics>("harbour.morzyn", 1,0 , "Statistics");
    //qmlRegisterType<IAI>("harbour.morzyn", 1,0 , "IAI");

    QSettings settings;
    if (!settings.contains("fullscreen"))
        settings.setValue("fullscreen", QVariant((bool)true));
    bool bFullscreen = settings.value("fullscreen").toBool();
    qDebug() << "Fullscreen:" << bFullscreen;
    settings.sync();

    qsrand ( time(NULL) );

    QString locale = QLocale::system().name();

    QTranslator translator;
    translator.load(QString("morzyn_") + locale, "translations");
    app.installTranslator(&translator);

    QtQuick2ApplicationViewer viewer;

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
    gs.setSettings(&settings);
    gs.viewer = &viewer;

#ifndef Q_OS_ANDROID
    viewer.setIcon(QIcon("qml/morzyn/images/morzyn.png"));
#endif
    viewer.rootContext()->setContextProperty("gameService", &gs);
    viewer.rootContext()->setContextProperty("game", &g);
    viewer.rootContext()->setContextProperty("statistics", &s);
    viewer.rootContext()->setContextProperty("library", &l);
    viewer.rootContext()->setContextProperty("os", QVariant("desktop"));
    viewer.rootContext()->setContextProperty("hCount", QVariant(c.HCOUNT));
    viewer.rootContext()->setContextProperty("vCount", QVariant(c.VCOUNT));
    viewer.setMainQmlFile(QStringLiteral("qml/main.qml"));
    viewer.setTitle("Morzyn v0.7.15");
    //viewer.showExpanded();
    if (bFullscreen)
        viewer.showFullScreen();
    else
        viewer.show();

    return app.exec();
}
