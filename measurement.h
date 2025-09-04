#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <QString>
#include <QDate>

struct Measurement {
    QString name;
    QDate date;
    QString joint;
    int startAngle;
    int endAngle;
    int rom;
};

#endif // MEASUREMENT_H
