#include "alphadict.h"
#include "Util.h"
#include "Application.h"

#if CONFIG_QT5
#include "gui/qt/mainwindow.h"
#include <QApplication>
#include <QTranslator>
#endif

static void on_exit()
{
    g_log(LOG_INFO,"main.cpp on_exit()\n");
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "C.UTF-8");
    Util::getTimeMS(); // start to timing.
    g_application.initialization();

#if CONFIG_QT5
    QApplication a(argc, argv);
    QTranslator translator;
    if (g_application.m_configure->m_uilanID == UILAN_CN) {
        translator.load("gui/qt/uitr_cn");
        a.installTranslator(&translator);
    }

    MainWindow w;
    w.show();
    w.registerSysExit(on_exit);
    QObject::connect(&a, SIGNAL(aboutToQuit()), &w, SLOT(onAppExit()));
    return a.exec();
#endif
}
