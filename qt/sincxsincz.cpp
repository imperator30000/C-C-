#include "sincxsincz.h"

SincXSincZ::SincXSincZ(QWidget *parent)

{
    name = "SincX sincZ";
    calc(renderXMin, renderXMax, renderZMin, renderZMax, renderStep);
}
void SincXSincZ::calc()
{
    calc(this->renderXMin, this->renderXMax, this->renderZMin, this->renderZMax, this->renderStep);
}

void SincXSincZ::calc(float startX, float finishX, float startY, float finishY, int step)
{
    data->clear();
    data->reserve(step);
    for (int i = 0; i < step; i++) {
        QSurfaceDataRow *newRow = new QSurfaceDataRow(step);
        float z = (finishY - startY) / float(step - 1) * float(i) + startY;
        for (int j = 0; j < step; j++) {
            float x = (finishX - startX) / float(step - 1) * float(j) + startX;
            float y = sinc(z) * sinc(x);
            (*newRow)[j].setPosition(QVector3D(x, y, z));
        }
        *data << newRow;
    }
}
