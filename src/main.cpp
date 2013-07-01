#include <QApplication>
#include <QDesktopWidget>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QSplashScreen>
#include "FenetrePrincipale.h"
#include "Constantes.h"

/** @mainpage glanc3d
    Abstract :
    <ul>
        <li><b>Description </b> : a brand new 3d mesh viewer and editor
        <li><b>Technologies </b> : C++, Qt, OpenGL
        <li><b>Version </b> : 2013-07-01, rév. 2
        <li><b>Editor </b> : the glanc3d team
        <li><b>Main developers</b> : jujurackham, blitzfis</li>
        <li><b>Hosted by GitHub</b> : http://github.com/jujurackham/glanc3d </li>
    </ul>

    <p>In order to develop this project, we have used the following tools :
    <ul>
        <li><b>Qt SDK 5.1 (open-source version)</b>, a multi-plateform object-oriented software framework,
        developed in C++ by Qt Development Frameworks/Digia. Released under GNU LGPL. URL: http://qt-project.org/ </li>
        <li><b>MicroPather</b>: software library implementing A* algorithm. Copyright (c) 2000-2009 Lee Thomason. URL: http://www.grinninglizard.com/MicroPather/ </li>
    </ul></p>
    
    <p>Copyright (c) 2012-2013, the glanc3d team.<br />
    This software is released under the terms of the Open Software License version 3.0.</p>
*/

int main(int argc, char *argv[])
{
    QApplication::setStyle("cleanlooks");
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(true);

    QSplashScreen screen;
    screen.setPixmap(QPixmap("./ressources/images/glanc3d.png"));
    screen.show();
    screen.showMessage(QString("Chargement..."), Qt::AlignRight, Qt::blue);
    app.processEvents();

    QString locale = QLocale::system().name().section('_', 0, 0);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTranslator translator;
    translator.load(QString("qt_") + locale,
                    QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&translator);

    FenetrePrincipale fenetre;
    screen.finish(&fenetre);
    fenetre.resize(fenetre.sizeHint());
    int zoneBureau = QApplication::desktop()->width() *
            QApplication::desktop()->height();
    int zoneFenetre = fenetre.width()*fenetre.height();
    if (((float)zoneFenetre/(float)zoneBureau) < 0.75)
        fenetre.show();
    else
        fenetre.showMaximized();
    return app.exec();
}
