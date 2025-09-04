#pragma once
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QList>
#include "measurement.h"

class Database {
public:
    Database(const QString &dbName);
    bool saveMeasurement(const Measurement &m);
    QList<Measurement> loadAllMeasurements();
    bool clearAllMeasurements();
    bool deleteMeasurement(const QString &name, const QString &date, const QString &joint);

private:
    QSqlDatabase db;
    bool createTableIfNotExists();
};
