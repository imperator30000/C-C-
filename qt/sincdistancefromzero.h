#ifndef SINCDISTANCEFROMZERO_H
#define SINCDISTANCEFROMZERO_H

#include "plot.h"

class SincDistanceFromZero : public Plot
{
public:
    explicit SincDistanceFromZero(QWidget *parent = nullptr);
    void calc(float startX, float finishX, float startY, float finishY, int step) override;
    void calc() override;
signals:
};

#endif // SINCDISTANCEFROMZERO_H
