#include "ui_mainwindow.h"
#include <SincDistanceFromZero.h>
#include <SincXSincZ.h>
#include <mainwindow.h>
#include <plotsurface.h>
#include <tools.h>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    plotNow = 0;

    ui->setupUi(this);
    inputSettings();

    toolbar = new QToolBar(this);
    QMenuBar *menuBar = new QMenuBar(this);

    QMenu *fileMenu = menuBar->addMenu("File");

    // Создаем действия "Save Settings" и "Load Settings"
    QAction *saveAction = new QAction("Save Settings", this);

    connect(saveAction, &QAction::triggered, this, &MainWindow::save);

    saveAction->setStatusTip("Save the current settings");

    fileMenu->addAction(saveAction);

    toolbar->addAction(saveAction);

    this->setMenuBar(menuBar);
    this->addToolBar(toolbar);

    statusBar = new QStatusBar(this);
    this->setStatusBar(statusBar);
    //    statusbar->showMessage("This is a status message.");

    plotSurface = new PlotSurface();
    plotSurface->update(plots->at(plotNow));

    tools *tool = new tools(this);

    tool->update(getPlotNow());

    tool->setPlot(plotNow);

    layout = new QGridLayout();
    splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(plotSurface);
    splitter->addWidget(tool);
    layout->addWidget(splitter, 0, 0);
    //    save();

    ui->centralwidget->setLayout(layout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

PlotSurface *MainWindow::getPlot()
{
    return plotSurface;
}

Plot *MainWindow::getFirstPlot()
{
    return plots->at(0);
}

Plot *MainWindow::getSecondPlot()
{
    return plots->at(1);
}

Plot *MainWindow::getPlotNow()
{
    return plots->at(plotNow);
}

int MainWindow::getPlotNowNum()
{
    return plotNow;
}

void MainWindow::printMessage(QString str)
{
    statusBar->showMessage(str);
}

void MainWindow::setPlot(PlotSurface *plotSurface)
{
    this->plotSurface = plotSurface;
}

QLinearGradient *MainWindow::getFirstGradient()
{
    return firstGradient;
}

QLinearGradient *MainWindow::getSecondGradient()
{
    return secondGradient;
}

void MainWindow::setPlotNow(int value)
{
    this->plotNow = value;
}

void MainWindow::save()
{
    //        settings = new QSettings("/Users/tulenevvadim/untitled3/settings.ini", QSettings::IniFormat);

    Plot *dto = getPlotNow();
    settings->setValue("showGrid", dto->getShowGrid());
    settings->setValue("showLabel", dto->getShowLabel());
    settings->setValue("showLabelBorder", dto->getShowLabelBorder());
    settings->setValue("item", dto->getItem());
    settings->setValue("renderXMax", dto->getRenderXMax());
    settings->setValue("renderXMin", dto->getRenderXMin());
    settings->setValue("renderZMax", dto->getRenderZMax());
    settings->setValue("renderZMin", dto->getRenderZMin());
    settings->setValue("renderStep", dto->getRenderStep());
    settings->setValue("gradient", dto->getGradientToStrinq());
    settings->setValue("plotNow", plotNow);
    settings->sync();
}

void MainWindow::inputSettings()
{
    plots->append(new SincDistanceFromZero);
    plots->append(new SincXSincZ);
    if (settings->contains("plotNow")) {
        plotNow = settings->value("plotNow").toInt();
    } else {
        plotNow = 0;
    }
    if (settings->contains("showGrid")) {
        getPlotNow()->setShowGrid(settings->value("showGrid").toBool());
    }

    if (settings->contains("showLabel")) {
        getPlotNow()->setShowLabel(settings->value("showLabel").toBool());
    }

    if (settings->contains("showLabelBorder")) {
        getPlotNow()->setShowLabelBorder(settings->value("showLabelBorder").toBool());
    }

    if (settings->contains("item")) {
        getPlotNow()->setItem(settings->value("item").toBool());
    }
    if (settings->contains("renderXMax")) {
        getPlotNow()->setRenderXMax(settings->value("renderXMax").toInt());
    }

    if (settings->contains("renderXMin")) {
        getPlotNow()->setRenderXMin(settings->value("renderXMin").toInt());
    }

    if (settings->contains("renderZMax")) {
        getPlotNow()->setRenderZMax(settings->value("renderZMax").toInt());
    }

    if (settings->contains("renderZMin")) {
        getPlotNow()->setRenderZMin(settings->value("renderZMin").toInt());
    }

    if (settings->contains("renderStep")) {
        getPlotNow()->setRenderStep(settings->value("renderStep").toInt());
    }

    if (settings->contains("gradient")) {
        getPlotNow()->setGradient(settings->value("gradient").toString());
    }
    firstGradient = new QLinearGradient;
    firstGradient->setColorAt(0.0, QColor(0, 0, 0));
    firstGradient->setColorAt(0.25, QColor(255, 0, 0));
    firstGradient->setColorAt(0.5, QColor(255, 255, 255));
    firstGradient->setColorAt(0.75, QColor(255, 0, 0));
    firstGradient->setColorAt(1.0, QColor(255, 255, 0));

    secondGradient = new QLinearGradient;
    secondGradient->setColorAt(0.0, Qt::blue);
    secondGradient->setColorAt(0.33, Qt::red);
    secondGradient->setColorAt(0.66, Qt::yellow);
    secondGradient->setColorAt(1.0, Qt::white);

    plots->at(!plotNow)->setGradient(firstGradient);
    getPlotNow()->calc(getPlotNow()->getRenderXMin(),
                       getPlotNow()->getRenderXMax(),
                       getPlotNow()->getRenderZMin(),
                       getPlotNow()->getRenderZMax(),
                       getPlotNow()->getRenderStep());
}

QStatusBar *MainWindow::getStatusBar()
{
    return statusBar;
}
