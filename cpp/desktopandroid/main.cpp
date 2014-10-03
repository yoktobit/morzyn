#include <QApplication>
//#include "qtquick2applicationviewer.h"
#include <QQmlApplicationEngine>
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
#include <QScreen>
#ifdef Q_OS_WIN32
#include <dir.h>
#endif

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
    QApplication app(argc, argv);
#ifdef Q_OS_WIN32
#ifndef _DEBUG
    //chdir(app.applicationDirPath().toStdString().c_str());
#endif
#endif

    QString version("v0.7.16");

    QCoreApplication::setOrganizationDomain("de.yoktobit");
    QCoreApplication::setOrganizationName("yoktobit");
    QCoreApplication::setApplicationName("morzyn");
    QCoreApplication::setApplicationVersion(version);

    qmlRegisterType<Creature>("harbour.morzyn", 1,0 , "Creature");
    qmlRegisterType<Player>("harbour.morzyn", 1,0 , "Player");
    qmlRegisterType<Game>("harbour.morzyn", 1,0 , "Game");
    qmlRegisterType<Scroll>("harbour.morzyn", 1,0 , "Scroll");
    qmlRegisterType<Statistics>("harbour.morzyn", 1,0 , "Statistics");
    //qmlRegisterType<IAI>("harbour.morzyn", 1,0 , "IAI");

#ifdef Q_OS_WIN32
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "yoktobit", "morzyn");
#else
    QSettings settings;
#endif
#ifdef Q_OS_WIN32
    if (!settings.contains("fullscreen"))
    {
        settings.setValue("fullscreen", QVariant((bool)true));
    }
#else
    settings.setValue("fullscreen", QVariant((bool)true));
#endif
    if (!settings.contains("music"))
        settings.setValue("music", QVariant((bool)true));
    if (!settings.contains("sound"))
        settings.setValue("sound", QVariant((bool)true));

    bool bFullscreen = settings.value("fullscreen").toBool();
    qDebug() << "Fullscreen:" << bFullscreen;
    settings.sync();

    qsrand ( time(NULL) );

    double minHorizontalSize = 150.0;
    double minVerticalSize = 90.0;
    //double minHorizontalSize = 300.0;
    //double minVerticalSize = 200.0;

    QScreen* screen = app.primaryScreen();
    QSizeF screenSize = screen->physicalSize();
    qDebug() << "screen width:" << screenSize.width() << "screen height:" << screenSize.height();
    QString locale = QLocale::system().name();

    QTranslator translator;
    translator.load(QString("morzyn_") + locale.left(2).toLower(), ":/translations");
    app.installTranslator(&translator);

    //QtQuick2ApplicationViewer viewer;
    QQmlApplicationEngine viewer;

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
    //gs.viewer = &viewer;

    if (screenSize.width() < minHorizontalSize || screenSize.height() < minVerticalSize)
    {
        c.HCOUNT = 10;
        c.VCOUNT = 10;
    }

#ifndef Q_OS_ANDROID
    //viewer.setIcon(QIcon("qml/morzyn/images/morzyn.png"));
#endif
    viewer.rootContext()->setContextProperty("gameService", &gs);
    viewer.rootContext()->setContextProperty("game", &g);
    viewer.rootContext()->setContextProperty("statistics", &s);
    viewer.rootContext()->setContextProperty("library", &l);
#ifdef Q_OS_ANDROID
    QString os = "android";
#else
    QString os = "windows";
#endif
    viewer.rootContext()->setContextProperty("os", QVariant(os));
    viewer.rootContext()->setContextProperty("hCount", QVariant(c.HCOUNT));
    viewer.rootContext()->setContextProperty("vCount", QVariant(c.VCOUNT));
    viewer.rootContext()->setContextProperty("version", QVariant(version));
    viewer.rootContext()->setContextProperty("locale", QVariant(locale));
    //viewer.setMainQmlFile(QStringLiteral("qml/main.qml"));
    //viewer.setTitle(QString("Morzyn %0").arg(version));
#ifdef Q_OS_ANDROID
    viewer.load(QUrl(QStringLiteral("assets:/qml/main.qml")));
#else
    viewer.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
#endif
    //viewer.showExpanded();
    /*if (bFullscreen)
        viewer.showFullScreen();
    else
        viewer.show();*/

    return app.exec();
}
