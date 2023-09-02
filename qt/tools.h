#ifndef TOOLS_H
#define TOOLS_H

#include "QtWidgets/qpushbutton.h"
#include "mainwindow.h"

#include <plotsurface.h>
#include <QCheckBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QSlider>
#include <QSpinBox>
#include <QWidget>
#include <qlabel.h>

class tools : public QWidget
{
    Q_OBJECT
public:
    explicit tools(MainWindow *parent = nullptr);
    void setPlot(bool value);
    void setSelectionMode(bool value);

    void update(Plot *dto);

    ~tools();

private:
    PlotSurface *plotSurface;
    QWidget *toolBar;
    QCheckBox *showGrid;
    QCheckBox *showLabelBorder;
    QCheckBox *showLabel;
    QGroupBox *plotGroup;
    QGroupBox *selectionModeGroup;
    QLabel *columnRange;
    QSlider *columnRangeSliderMin;
    QSlider *columnRangeSliderMax;
    QLabel *rowRange;
    QSlider *rowRangeSliderMin;
    QSlider *rowRangeSliderMax;
    QGroupBox *gradientGroup;
    QSpinBox *steps;

    QGroupBox *createGradientGroup(QWidget *parent);
    QGroupBox *createSelectionModeGroup(QWidget *parent);
    QGroupBox *createPlotGroup(QWidget *parent);

    MainWindow *parent;
    QString getGradientStyleSheet(QLinearGradient *gradient);
    QPushButton *saveButton;
    QLabel *columnRangeLabelMin;
    QLabel *columnRangeLabelMax;
    QLabel *rowRangeLabelMin;
    QLabel *rowRangeLabelMax;

public slots:
    void printPoint(const QString &label);
private slots:

    void onSelectionModeToggled(bool checked);
    void on_showGrid_stateChanged(int state);
    void on_showLabelBorder_stateChanged(int state);
    void on_showLabel_stateChanged(int state);
    void onPlotToggled(bool checked);
    void onColumnRangeMaxSlidervalueChanged(int value);

    void onColumnRangeMinSlidervalueChanged(int value);
    void onRowRangeMaxSlidervalueChanged(int value);
    void onRowRangeMinSlidervalueChanged(int value);
    void onStepValueChanged(int value);
    void onGradientFirstClicked();
    void onGradientSecondClicked();
signals:

protected slots:
};

#endif // TOOLS_H
