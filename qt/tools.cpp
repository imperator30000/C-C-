#include <mainwindow.h>
#include <tools.h>

#include <iostream>
#include <Q3DSurface>
#include <QPushButton>
#include <QRadioButton>
#include <QSurface3DSeries>
#include <QVBoxLayout>

tools::tools(MainWindow *parent)
    : QWidget{parent}
{
    int max_start = 10;
    int min_start = -10;
    int max_ = 20;
    int min_ = -20;
    int max_step = 50;
    int min_step = 1;
    int start_step = 50;
    this->parent = parent;
    plotSurface = parent->getPlot();
    toolBar = this;
    showGrid = new QCheckBox(tr("Show grid "), toolBar);
    showLabelBorder = new QCheckBox(tr("Show label border"), toolBar);
    showLabel = new QCheckBox(tr("Show label"), toolBar);
    plotGroup = createPlotGroup(toolBar);
    selectionModeGroup = createSelectionModeGroup(toolBar);
    columnRange = new QLabel("Column range", toolBar);
    columnRangeLabelMin = new QLabel(toolBar);
    columnRangeSliderMin = new QSlider(toolBar);
    columnRangeLabelMax = new QLabel(toolBar);
    columnRangeSliderMax = new QSlider(toolBar);
    rowRange = new QLabel("Row range", toolBar);
    rowRangeLabelMin = new QLabel(toolBar);
    rowRangeSliderMin = new QSlider(toolBar);
    rowRangeLabelMax = new QLabel(toolBar);
    rowRangeSliderMax = new QSlider(toolBar);
    gradientGroup = createGradientGroup(toolBar);
    steps = new QSpinBox(toolBar);
    saveButton = new QPushButton(toolBar);

    showGrid->move(0, 0);
    showLabelBorder->move(0, 20);
    showLabel->move(0, 40);
    plotGroup->move(0, 60);
    selectionModeGroup->move(0, 160);
    columnRange->move(0, 260);
    columnRangeLabelMin->move(0, 300);
    columnRangeSliderMin->move(0, 340);
    columnRangeLabelMax->move(0, 380);
    columnRangeSliderMax->move(0, 420);
    rowRange->move(0, 460);
    rowRangeLabelMin->move(0, 500);
    rowRangeSliderMin->move(0, 540);
    rowRangeLabelMax->move(0, 580);
    rowRangeSliderMax->move(0, 620);
    steps->move(0, 660);
    gradientGroup->move(0, 700);
    saveButton->move(0, 850);

    columnRangeLabelMax->setFixedWidth(100);
    columnRangeLabelMin->setFixedWidth(100);
    rowRangeLabelMax->setFixedWidth(100);
    rowRangeLabelMin->setFixedWidth(100);

    columnRangeSliderMin->setOrientation(Qt::Horizontal);
    columnRangeSliderMax->setOrientation(Qt::Horizontal);
    rowRangeSliderMin->setOrientation(Qt::Horizontal);
    rowRangeSliderMax->setOrientation(Qt::Horizontal);

    columnRangeSliderMin->setRange(min_, max_);
    columnRangeSliderMax->setRange(min_, max_);
    rowRangeSliderMin->setRange(min_, max_);
    rowRangeSliderMax->setRange(min_, max_);

    steps->setRange(min_step, max_step);

    columnRangeSliderMax->setValue(max_start);
    rowRangeSliderMax->setValue(max_start);
    columnRangeSliderMin->setValue(min_start);
    rowRangeSliderMin->setValue(min_start);

    steps->setValue(start_step);

    saveButton->setText("Save");

    plotGroup->setFixedSize(150, 100);
    selectionModeGroup->setFixedSize(150, 100);
    gradientGroup->setFixedSize(150, 150);
    saveButton->setFixedSize(150, 40);

    columnRangeSliderMin->setFixedWidth(150);
    columnRangeSliderMax->setFixedWidth(150);
    rowRangeSliderMin->setFixedWidth(150);
    rowRangeSliderMax->setFixedWidth(150);

    //    layout->addWidget(showGrid, 0);
    //    layout->addWidget(showLabelBorder, 1);
    //    layout->addWidget(showLabel, 2);
    //    layout->addWidget(plotGroup, 3);
    //    layout->addWidget(selectionModeGroup, 4);
    //    toolBar->setLayout(layout);

    connect(showGrid, &QCheckBox::stateChanged, this, &tools::on_showGrid_stateChanged);
    connect(showLabelBorder,
            &QCheckBox::stateChanged,
            this,
            &tools::on_showLabelBorder_stateChanged);
    connect(showLabel, &QCheckBox::stateChanged, this, &tools::on_showLabel_stateChanged);

    QRadioButton *sinc1 = plotGroup->findChild<QRadioButton *>("sinc1");
    QRadioButton *sinc2 = plotGroup->findChild<QRadioButton *>("sinc2");
    connect(sinc1, &QRadioButton::toggled, this, &tools::onPlotToggled);
    connect(sinc2, &QRadioButton::toggled, this, &tools::onPlotToggled);

    QRadioButton *noSelection = selectionModeGroup->findChild<QRadioButton *>("NoSelection");
    QRadioButton *item = selectionModeGroup->findChild<QRadioButton *>("Item");
    connect(noSelection, &QRadioButton::toggled, this, &tools::onSelectionModeToggled);
    connect(item, &QRadioButton::toggled, this, &tools::onSelectionModeToggled);

    connect(columnRangeSliderMax,
            &QSlider::valueChanged,
            this,
            &tools::onColumnRangeMaxSlidervalueChanged);
    connect(columnRangeSliderMin,
            &QSlider::valueChanged,
            this,
            &tools::onColumnRangeMinSlidervalueChanged);
    connect(rowRangeSliderMax,
            &QSlider::valueChanged,
            this,
            &tools::onRowRangeMaxSlidervalueChanged);
    connect(rowRangeSliderMin,
            &QSlider::valueChanged,
            this,
            &tools::onRowRangeMinSlidervalueChanged);

    connect(steps, &QSpinBox::valueChanged, this, &tools::onStepValueChanged);

    QPushButton *gradientFirst = gradientGroup->findChild<QPushButton *>("first");
    QPushButton *gradientSecond = gradientGroup->findChild<QPushButton *>("second");
    connect(gradientFirst, &QPushButton::clicked, this, &tools::onGradientFirstClicked);
    connect(gradientSecond, &QPushButton::clicked, this, &tools::onGradientSecondClicked);

    connect(saveButton, &QPushButton::clicked, parent, &MainWindow::save);
    //    connect(plotSurface->getSurface(), &Q3DSurface::positionChanged, this, &tools::printPoint);
    //    QWidget::connect(plotSurface->getSurface(), &QAbstract3DGraph::, this, &tools::printPoint);
    toolBar->setFixedWidth(150);
}

void tools::setPlot(bool value)
{
    plotGroup->findChild<QRadioButton *>(value == 0 ? "sinc1" : "sinc2")->setChecked(1);
}

void tools::setSelectionMode(bool value)
{
    selectionModeGroup->findChild<QRadioButton *>(value ? "Item" : "NoSelection")->setChecked(1);
}

void tools::update(Plot *dto)
{
    showGrid->setChecked(dto->getShowGrid());
    showLabelBorder->setChecked(dto->getShowLabelBorder());

    if (dto->getShowLabel()) {
        showLabel->setChecked(dto->getShowLabel());
        on_showLabel_stateChanged(2);

    } else {
        on_showLabel_stateChanged(0);
        plotSurface->setLabelBorder(0);
    }
    //        showLabel->setChecked(dto->getShowLabel());

    onColumnRangeMinSlidervalueChanged(dto->getRenderXMin());
    onColumnRangeMaxSlidervalueChanged(dto->getRenderXMax());
    onRowRangeMinSlidervalueChanged(dto->getRenderZMin());
    onRowRangeMaxSlidervalueChanged(dto->getRenderZMax());

    setSelectionMode(dto->getItem());
    //        onStepValueChanged();
    steps->setValue(dto->getRenderStep());
    printPoint(dto->getName());
}
QGroupBox *tools::createPlotGroup(QWidget *parent)
{
    QGroupBox *groupBox = new QGroupBox(tr("plotSurface"), parent);

    QRadioButton *sinc1 = new QRadioButton(tr("Sinc1"));
    QRadioButton *sinc2 = new QRadioButton(tr("Sinc2"));
    sinc1->setObjectName("sinc1");
    sinc2->setObjectName("sinc2");
    sinc1->setChecked(true);
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(sinc1);
    vbox->addWidget(sinc2);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);

    return groupBox;
}
QString tools::getGradientStyleSheet(QLinearGradient *gradient)
{
    QString styleSheet = "QPushButton { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1";
    QVector<QGradientStop> stops = gradient->stops();

    for (QGradientStop stop : stops) {
        QColor color = stop.second;
        styleSheet.append(QString(", stop: %1 rgb(%2,%3,%4)")
                              .arg(stop.first)
                              .arg(color.red())
                              .arg(color.green())
                              .arg(color.blue()));
    }

    styleSheet.append("); border-radius: 5px;}");

    return styleSheet;
}

QGroupBox *tools::createSelectionModeGroup(QWidget *parent)
{
    QGroupBox *groupBox = new QGroupBox(tr("Selection mode"), parent);

    QRadioButton *noSelection = new QRadioButton(tr("No selection"), groupBox);
    QRadioButton *item = new QRadioButton(tr("Item"), groupBox);
    noSelection->setObjectName("NoSelection");
    item->setObjectName("Item");

    noSelection->setChecked(true);
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(noSelection);
    vbox->addWidget(item);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);

    return groupBox;
}

QGroupBox *tools::createGradientGroup(QWidget *parent)
{
    QGroupBox *groupBox = new QGroupBox(tr("Custom gradient"), parent);

    QPushButton *gradientFirst = new QPushButton();
    QPushButton *gradientSecond = new QPushButton();
    gradientFirst->setObjectName("first");
    gradientSecond->setObjectName("second");

    gradientFirst->setStyleSheet(getGradientStyleSheet(this->parent->getFirstGradient()));
    //    gradientFirst->setStyleSheet("QPushButton {"
    //                                 "    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 purple, stop: 0.5 #ff0000, stop: 1 black);"
    //                                 //                                  "    border: none;"
    //                                 "    border-radius: 5px;"
    //                                 //                                 "    padding: 5px;"
    //                                 //                                 "    color: black;"
    //                                 "}");
    gradientSecond->setStyleSheet(getGradientStyleSheet(this->parent->getSecondGradient()));

    QHBoxLayout *vbox = new QHBoxLayout;
    gradientFirst->setFixedSize(53, 100);
    gradientSecond->setFixedSize(53, 100);
    vbox->addWidget(gradientFirst);
    vbox->addWidget(gradientSecond);
    groupBox->setLayout(vbox);

    return groupBox;
}

void tools::on_showGrid_stateChanged(int state)
{
    parent->getPlotNow()->setShowGrid(state == Qt::Checked);
    plotSurface->setGridVisible(state == Qt::Checked);
}

void tools::on_showLabelBorder_stateChanged(int state)
{
    parent->getPlotNow()->setShowLabelBorder(state == Qt::Checked);
    plotSurface->setLabelBorder(state == Qt::Checked);
}

void tools::on_showLabel_stateChanged(int state)
{
    parent->getPlotNow()->setShowLabel(state == Qt::Checked);
    if (state != Qt::Checked) {
        plotSurface->setLabelBorder(0);
        showLabelBorder->setEnabled(0);
    } else {
        if (showLabelBorder->isChecked()) {
            plotSurface->setLabelBorder(1);
        }
        showLabelBorder->setEnabled(1);
    }
    plotSurface->setLabel(state == Qt::Checked);
}

void tools::onSelectionModeToggled(bool checked)
{
    QRadioButton *selectedButton = qobject_cast<QRadioButton *>(sender());
    if (selectedButton && checked) {
        plotSurface->setSelectionMode(selectedButton->objectName() == "Item");
        parent->getPlotNow()->setItem(selectedButton->objectName() == "Item");
    }
}
void tools::onPlotToggled(bool checked)
{
    QRadioButton *selectedButton = qobject_cast<QRadioButton *>(sender());
    if (selectedButton && checked) {
        plotSurface->getSurface()->seriesList().at(0);
        parent->getStatusBar()->showMessage(selectedButton->objectName());
        if (selectedButton->objectName() == "sinc1") {
            parent->setPlotNow(0);

        } else {
            parent->setPlotNow(1);
        }
        update(parent->getPlotNow());
        parent->getPlot()->update(parent->getPlotNow());
    }
}

void tools::printPoint(const QString &label)
{
    parent->getStatusBar()->showMessage(label);
}

void tools::onColumnRangeMaxSlidervalueChanged(int value)
{
    columnRangeSliderMax->setValue(value);
    if (columnRangeSliderMin->value() > value) {
        columnRangeSliderMin->setValue(value);
        columnRangeLabelMin->setText(QString::number(value));
    }
    printPoint("column max changed: " + QString::number(value));

    columnRangeLabelMax->setText(QString::number(value));

    if (value > parent->getPlotNow()->getRenderXMax()) {
        parent->getPlotNow()->setRenderXMax(value);
        parent->getPlotNow()->calc();
    } else {
        parent->getPlotNow()->setRenderXMax(value);
    }

    plotSurface->cutAxesX(columnRangeSliderMin->value(), columnRangeSliderMax->value());
}

void tools::onColumnRangeMinSlidervalueChanged(int value)
{
    columnRangeSliderMin->setValue(value);
    if (columnRangeSliderMax->value() < value) {
        columnRangeSliderMax->setValue(value);
        columnRangeLabelMax->setText(QString::number(value));
    }
    printPoint("column min changed: " + QString::number(value));

    columnRangeLabelMin->setText(QString::number(value));

    if (value < parent->getPlotNow()->getRenderXMin()) {
        parent->getPlotNow()->setRenderXMin(value);
        parent->getPlotNow()->calc();
    } else {
        parent->getPlotNow()->setRenderXMin(value);
    }

    plotSurface->cutAxesX(columnRangeSliderMin->value(), columnRangeSliderMax->value());
}
void tools::onRowRangeMaxSlidervalueChanged(int value)
{
    rowRangeSliderMax->setValue(value);
    if (rowRangeSliderMin->value() > value) {
        rowRangeSliderMin->setValue(value);
        rowRangeLabelMin->setText(QString::number(value));
    }
    printPoint("row max changed: " + QString::number(value));
    rowRangeLabelMax->setText(QString::number(value));

    if (value > parent->getPlotNow()->getRenderZMax()) {
        parent->getPlotNow()->setRenderZMax(value);
        parent->getPlotNow()->calc();
    } else {
        parent->getPlotNow()->setRenderZMax(value);
    }

    plotSurface->cutAxesZ(rowRangeSliderMin->value(), rowRangeSliderMax->value());
}

void tools::onRowRangeMinSlidervalueChanged(int value)
{
    rowRangeSliderMin->setValue(value);
    if (rowRangeSliderMax->value() < value) {
        rowRangeSliderMax->setValue(value);
        rowRangeLabelMax->setText(QString::number(value));
    }
    printPoint("row min changed: " + QString::number(value));

    rowRangeLabelMin->setText(QString::number(value));

    if (value < parent->getPlotNow()->getRenderZMin()) {
        parent->getPlotNow()->setRenderZMin(value);
        parent->getPlotNow()->calc();
    } else {
        parent->getPlotNow()->setRenderZMin(value);
    }
    plotSurface->cutAxesZ(rowRangeSliderMin->value(), rowRangeSliderMax->value());
}

void tools::onStepValueChanged(int value)
{
    printPoint("steps changed: " + QString::number(value));

    parent->getPlotNow()->setRenderStep(value);
    parent->getPlotNow()->calc(columnRangeSliderMin->value(),
                               columnRangeSliderMax->value(),
                               rowRangeSliderMin->value(),
                               rowRangeSliderMax->value(),
                               value);
    parent->getPlot()->setData(parent->getPlotNow()->getData());
}
void tools::onGradientFirstClicked()
{
    printPoint("selected first gradient");
    parent->getPlotNow()->setGradient(parent->getFirstGradient());
    parent->getPlot()->setGradient(parent->getFirstGradient());
}
void tools::onGradientSecondClicked()

{
    printPoint("selected second gradient");
    parent->getPlotNow()->setGradient(parent->getSecondGradient());
    parent->getPlot()->setGradient(parent->getSecondGradient());
}
tools::~tools()
{
    delete plotSurface;
    delete toolBar;
    delete showGrid;
    delete showLabelBorder;
    delete showLabel;
    delete plotGroup;
    delete selectionModeGroup;
    delete columnRange;
    delete columnRangeSliderMin;
    delete columnRangeSliderMax;
    delete rowRange;
    delete rowRangeSliderMin;
    delete rowRangeSliderMax;
    delete gradientGroup;
    delete steps;

    delete parent;
    delete saveButton;
    delete columnRangeLabelMin;
    delete columnRangeLabelMax;
    delete rowRangeLabelMin;
    delete rowRangeLabelMax;
}
