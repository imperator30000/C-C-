#ifndef SINCXSINCZ_H
#define SINCXSINCZ_H

#include "plot.h"

class SincXSincZ : public Plot
{
public:
    explicit SincXSincZ(QWidget *parent = nullptr);
    void calc(float startX, float finishX, float startY, float finishY, int step) override;
    void calc() override;
signals:
};
#endif // SINCXSINCZ_H
