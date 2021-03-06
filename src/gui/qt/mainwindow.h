#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtCore/QModelIndexList>
#include <QtWidgets/QSystemTrayIcon>

#include "Application.h"

namespace Ui {
class MainWindow;
}

class QtMessager;
class DictIndexModel;
class QListWidgetItem;
class VBookModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void registerSysExit(void (*exit)()) { onSysExit = exit; }    

    QtMessager *m_messager;

public slots:
    void onUpdateText(void *v);
    void onSetLanComboBox(const QString& src, const QString& det, void *v);

private slots:
    void on_srcLanComboBox_currentIndexChanged(const QString &arg1);

    void on_detLanComboBox_currentIndexChanged(const QString &arg1);

    void on_inputLineEdit_editingFinished();

    void on_inputLineEdit_textChanged(const QString &arg1);

    void on_queryButton_clicked();

    void on_indexListView_clicked(const QModelIndex &index);

    void on_indexListView_activated(const QModelIndex &index);

    void on_pgdownToolButton1_clicked();

    void on_pgdownToolButton2_clicked();

    void on_pgupToolButton1_clicked();

    void on_pgupToolButton2_clicked();
    
    void on_dictListWidget_clicked(const QModelIndex &index);

    void on_dictUpToolButton_clicked();

    void on_dictDownToolButton_clicked();

    void on_uilanComboBox_activated(int index);

    void on_indexLineEdit_editingFinished();

    void on_saveButton_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_vbdelToolButton_clicked();

    void on_vbclearToolButton_clicked();

    void on_vbpreItemTlBtn_clicked();

    void on_vbnextItemTlBtn_clicked();

    void on_vbookListView_clicked(const QModelIndex &index);

    void on_vbookListView_activated(const QModelIndex &index);
    
    void on_vbInput_editingFinished();
    //void on_vocabularyTabWidget_currentChanged(int index);

    void onAppExit();

    void onActionSettingPageAdded();

    void onActionVcbularyPageAdded();

    //void OnSysTrayActivated(QSystemTrayIcon::ActivationReason reason);

protected :
    //bool winEvent( MSG * message, long * result);
     //bool nativeEvent(const QByteArray & eventType, void * message, long * result);
private:
    void showToolTip(QString info, QWidget* w, int displayTimeMS=1500);

    DictIndexModel* m_dictIndexModel;
    VBookModel*     m_vbookModel;

    Ui::MainWindow *ui;
    QSystemTrayIcon* m_systray;

    Configure* m_config;
    bool m_initSettingPage;

    void (*onSysExit)();
};

#endif // MAINWINDOW_H
