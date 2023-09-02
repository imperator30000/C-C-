#include "plot.h"

Plot::Plot() {}

void Plot::setShowGrid(bool value)
{
    this->showGrid = value;
}

void Plot::setShowLabel(bool value)
{
    this->showLabel = value;
}

void Plot::setShowLabelBorder(bool value)
{
    this->showLabelBorder = value;
}

void Plot::setItem(bool value)
{
    this->item = value;
}

void Plot::setGradient(QLinearGradient *gradient)
{
    this->gradient = gradient;
}

void Plot::setRenderXMin(int value)
{
    renderXMin = value;
}

void Plot::setRenderXMax(int value)
{
    renderXMax = value;
}

void Plot::setRenderZMin(int value)
{
    renderZMin = value;
}

void Plot::setRenderZMax(int value)
{
    renderZMax = value;
}

void Plot::setRenderStep(int value)
{
    this->renderStep = value;
}

int Plot::getRenderXMin()
{
    return renderXMin;
}

int Plot::getRenderXMax()
{
    return renderXMax;
}

int Plot::getRenderZMin()
{
    return renderZMin;
}

int Plot::getRenderZMax()
{
    return renderZMax;
}

int Plot::getRenderStep()
{
    return renderStep;
}

QString Plot::getName()
{
    return name;
}

bool Plot::getShowGrid()
{
    return showGrid;
}

bool Plot::getShowLabel()
{
    return showLabel;
}

bool Plot::getShowLabelBorder()
{
    return showLabelBorder;
}

bool Plot::getItem()
{
    return item;
}

QSurfaceDataArray *Plot::getData()
{
    return data;
}

QLinearGradient *Plot::getGradient()
{
    return gradient;
}

void Plot::setGradient(QString gradientString)
{
    QStringList colorStops = gradientString.split("; ");
    foreach (const QString &colorStop, colorStops) {
        QStringList parts = colorStop.split(":");
        if (parts.size() != 2)
            continue;

        QColor color(parts[0]);
        qreal position = parts[1].toDouble();

        gradient->setColorAt(position, color);
    }
}

QString Plot::getGradientToStrinq()
{
    QString sb;
    for (int i = 0; i < gradient->stops().size(); ++i) {
        const QGradientStop &stop = gradient->stops().at(i);
        QColor color = stop.second;
        qreal position = stop.first;

        sb += QString("%1:%2; ").arg(color.name()).arg(position);
    }
    return sb;
}
Plot::~Plot()
{
    delete gradient;
    delete data;
}
