#include "sincdistancefromzero.h"

SincDistanceFromZero::SincDistanceFromZero(QWidget *parent)
{
    name = "Sinc distance from zero";
    calc(renderXMin, renderXMax, renderZMin, renderZMax, renderStep);
}
void SincDistanceFromZero::calc()
{
    calc(this->renderXMin, this->renderXMax, this->renderZMin, this->renderZMax, this->renderStep);
}
void SincDistanceFromZero::calc(float startX, float finishX, float startY, float finishY, int step)
{
    data->clear();
    data->reserve(step);
    for (int i = 0; i < step; i++) {
        QSurfaceDataRow *newRow = new QSurfaceDataRow(step);
        float z = (finishY - startY) / float(step - 1) * float(i) + startY;
        for (int j = 0; j < step; j++) {
            float x = (finishX - startX) / float(step - 1) * float(j) + startX;

            float r = sqrt(x * x + z * z);
            float y = sinc(r);

            (*newRow)[j].setPosition(QVector3D(x, y, z));
        }
        *data << newRow;
    }
}
