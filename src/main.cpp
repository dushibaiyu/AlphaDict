#include "alphadict.h"
#include "Util.h"
#include "Log.h"
#include "Application.h"

#if CONFIG_QT5
#include <QtWidgets/QApplication>
#include <QtCore/QTranslator>
#include <QtWidgets/QStyleFactory>
#include "gui/qt/mainwindow.h"
#endif

static void on_exit()
{
    g_log(LOG_INFO,"main.cpp on_exit()\n");
}

#ifdef _WINDOWS
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#else
int main(int argc, char* argv[])
#endif
{
#ifdef _LINUX
    setlocale(LC_ALL, "C.UTF-8");
#endif
    Util::getTimeMS(); // start to timing.
    g_application.initialization();

#if CONFIG_QT5
#ifdef _WINDOWS
    int argc = 0;
    char** argv = 0;
#endif
    QApplication a(argc, argv);
    QTranslator translator;
    if (g_application.m_configure->m_uilanID == UILAN_CN) {
        string trfile = g_application.m_configure->m_dataDir + "/uitr_cn";
        translator.load(QString::fromUtf8(trfile.c_str()));
        a.installTranslator(&translator);
    }
    a.setStyle(QStyleFactory::create("Fusion"));
    //a.setStyle(QStyleFactory::create("windowsvista"));
    MainWindow w;
    w.show();
    w.registerSysExit(on_exit);
    QObject::connect(&a, SIGNAL(aboutToQuit()), &w, SLOT(onAppExit()));
    return a.exec();
#endif
}
