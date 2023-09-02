#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QDir"
#include "QtWidgets/qgridlayout.h"
#include <SincDistanceFromZero.h>
#include <SincXSincZ.h>
#include <plot.h>
#include <plotsurface.h>
#include <QGroupBox>
#include <QMainWindow>
#include <QSettings>
#include <QSplitter>
#include <QStatusBar>
#include <qcoreapplication.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    PlotSurface *getPlot();

    Plot *getFirstPlot();

    Plot *getSecondPlot();

    Plot *getPlotNow();

    int getPlotNowNum();

    void setPlot(PlotSurface *plotSurface);

    QLinearGradient *getFirstGradient();

    QLinearGradient *getSecondGradient();

    void setPlotNow(int value);

    void save();

    void inputSettings();

    //    void setMessage(QString message){
    //        statusBar->showMessage(message);;
    //    }
    void showPoint() {}

    QStatusBar *getStatusBar();

    void printMessage(QString str);

private:
    QStatusBar *statusBar;
    QSettings *  settings = new QSettings(QCoreApplication::applicationDirPath() +"/settings.ini", QSettings::IniFormat);
//    QDir::currentPath();
//    QSettings *settings = new QSettings("/Users/tulenevvadim/untitled3/settings.ini",
//                                        QSettings::IniFormat);
    QGridLayout *layout;
    QSplitter *splitter;
    PlotSurface *plotSurface;
    QVector<Plot *> *plots = new QVector<Plot *>();
    QLinearGradient *firstGradient;
    QLinearGradient *secondGradient;
    int plotNow;
    QToolBar *toolbar;
    //    QStatusBar *statusBar;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
