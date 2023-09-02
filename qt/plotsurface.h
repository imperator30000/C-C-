#ifndef PLOT2_H
#define PLOT2_H

#include "plot.h"
#include <Q3DSurface>
#include <QObject>
#include <QWidget>

class PlotSurface : public QWidget
{
    Q_OBJECT
public:
    explicit PlotSurface(QWidget *parent = nullptr);
    void setGridVisible(bool onOf);
    void setLabelBorder(bool onOf);
    void setLabel(bool onOf);
    void setSelectionMode(bool onOf);
    void cutAxesX(float start, float finish);
    void cutAxesZ(float start, float finish);
    void setGradient(QLinearGradient *gradient);
    void setData(QSurfaceDataArray *data);
    Q3DSurface *getSurface();

    QSurface3DSeries *getSeries();
    QSurfaceDataArray *getData();

    void update(Plot *dto);
    QLinearGradient *getGradient();

    ~PlotSurface();
private slots:

    void handlePointSelected(const QPoint &position);

protected:
    QSurfaceDataArray *data;
    QPoint *selectedPoint;
    QLinearGradient *gradient;
    Q3DSurface *surface;
    QWidget *container;
    QSurface3DSeries *series;
    QSurfaceDataProxy *proxy;
    float sinc(float x);
signals:
};

#endif // PLOT2_H
