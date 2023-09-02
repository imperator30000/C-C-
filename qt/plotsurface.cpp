#include "plotsurface.h"
#include <QtDataVisualization>
#include <Q3DSurface>
#include <QGridLayout>
PlotSurface::PlotSurface(QWidget *parent)
    : QWidget{parent}
{
    surface = new Q3DSurface();
    container = QWidget::createWindowContainer(surface, this);

    QGridLayout *layout = new QGridLayout();
//    Q3DInputHandler *inputHandler = static_cast<Q3DInputHandler *>(surface->activeInputHandler());
//    connect(inputHandler, &Q3DInputHandler::selectedPointChanged, this, &PlotSurface::handlePointSelected);


//    connect(surface, Q3DSurface::selectedElementChanged this, &PlotSurface::handlePointSelected);

    layout->addWidget(container, 0, 0);
    this->move(0, 100);
    this->setLayout(layout);
    this->setGridVisible(0);
}
void PlotSurface::handlePointSelected(const QPoint &position){

}
float PlotSurface::sinc(float x) {
    if (x == 0.0)
        return 1.0;
    else
        return qSin(x) / x;
}
void PlotSurface::setGridVisible(bool onOf)
{
    surface->activeTheme()->setGridEnabled(onOf);
}


void PlotSurface::setLabelBorder(bool onOf){
    surface->activeTheme()->setLabelBorderEnabled(onOf);
}
void PlotSurface::setLabel(bool onOf){
    surface->activeTheme()->setLabelTextColor(onOf ? QColor::fromRgb(0,0,0): QColor::fromRgb(255,255,255));
}
void PlotSurface::setSelectionMode(bool onOf){
    surface->setSelectionMode(onOf ? QAbstract3DGraph::SelectionItem:QAbstract3DGraph::SelectionNone);
}
void PlotSurface::cutAxesX(float start, float finish){
    surface->axisX()->setRange(start, finish);
}

void PlotSurface::cutAxesZ(float start, float finish){
    surface->axisZ()->setRange(start, finish);
}

void PlotSurface::setGradient(QLinearGradient *gradient)
{
    this->gradient = gradient;
    QList<QColor> colorList;
    QVector<QGradientStop> stops = gradient->stops();
    for (const QGradientStop &stop : stops) {
        colorList << stop.second;
    }

    Q3DTheme *theme = new Q3DTheme();
    theme->setBaseGradients(QList<QLinearGradient>() << *gradient);
    theme->setColorStyle(Q3DTheme::ColorStyleRangeGradient);

    surface->activeTheme()->setBaseGradients(theme->baseGradients());
    surface->activeTheme()->setColorStyle(theme->colorStyle());
}

void PlotSurface::setData(QSurfaceDataArray *data){
    this->data = data;
    surface->removeSeries(series);
    proxy = new QSurfaceDataProxy();
    series = new QSurface3DSeries(proxy);
    series->setDrawMode(QSurface3DSeries::DrawSurface);
    proxy->resetArray(data);

    surface->addSeries(series);
}

Q3DSurface *PlotSurface::getSurface(){
    return this->surface;
}

QSurface3DSeries *PlotSurface::getSeries(){

    return this->series;
}

QSurfaceDataArray *PlotSurface::getData(){
    return data;
}

void PlotSurface::update(Plot *dto){
    setData(dto->getData());
    setGradient(dto->getGradient());
    //        setLabel(dto->getShowLabel());
    //        setLabelBorder(dto->getShowLabelBorder());
    //        setGridVisible(dto->getShowGrid());
    setSelectionMode(dto->getItem());
}

QLinearGradient *PlotSurface::getGradient(){
    return gradient;
}
PlotSurface::~PlotSurface()
{
    delete gradient;
    delete data;
    delete selectedPoint;
    delete surface;
    delete series;
    delete proxy;
}
