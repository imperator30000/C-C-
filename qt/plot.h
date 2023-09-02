#ifndef PLOT_H
#define PLOT_H

#include <Q3DSurface>
#include <QLinearGradient>

class Plot
{
public:
    Plot();
    Plot(float renderXMin,
         float renderXMax,
         float renderYMin,
         float renderYMax,
         QLinearGradient *gradient,
         Q3DSurface *surface,
         QWidget *container,
         QSurfaceDataArray *data);
    virtual void calc(float startX, float finishX, float startY, float finishY, int step) = 0;
    virtual void calc() = 0;
    void setShowGrid(bool value);
    void setShowLabel(bool value);
    void setShowLabelBorder(bool value);

    void setItem(bool value);
    void setGradient(QLinearGradient *gradient);

    void setRenderXMin(int value);
    void setRenderXMax(int value);
    void setRenderZMin(int value);
    void setRenderZMax(int value);
    void setRenderStep(int value);

    int getRenderXMin();

    int getRenderXMax();

    int getRenderZMin();

    int getRenderZMax();
    int getRenderStep();

    QString getName();
    bool getShowGrid();

    bool getShowLabel();

    bool getShowLabelBorder();
    bool getItem();
    QSurfaceDataArray *getData();
    QLinearGradient *getGradient();
    void setGradient(QString gradientString);
    QString getGradientToStrinq();

    ~Plot();

protected:
    QString name;
    int renderXMin = -10;
    int renderXMax = 10;
    int renderZMin = -10;
    int renderZMax = 10;
    int renderStep = 50;
    bool showGrid = 0;
    bool showLabel = 0;
    bool showLabelBorder = 0;
    bool item = 0;
    QLinearGradient *gradient = new QLinearGradient;
    QSurfaceDataArray *data = new QSurfaceDataArray();
    float sinc(float x)
    {
        if (x == 0.0)
            return 1.0;
        else
            return qSin(x) / x;
    }
};

#endif // PLOT_H
