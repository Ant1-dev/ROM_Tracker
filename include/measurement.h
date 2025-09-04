#pragma once
#include <QString>
#include <QDate>

struct Measurement {
    QString name;
    QDate date;
    QString joint;
    int startAngle;
    int endAngle;
    int rom;

    Measurement() = default;
    Measurement(QString n, QDate d, QString j, int s, int e, int r)
        : name(n), date(d), joint(j), startAngle(s), endAngle(e), rom(r) {}
};
