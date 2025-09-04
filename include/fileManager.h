#pragma once
#include <QString>
#include <QList>
#include "measurement.h"
#include "database.h"

class FileManager {
public:
    static bool exportToCSV(const QString &filePath, const QList<Measurement> &measurements);
    static void importFromCSV(const QString &filePath, Database &db);
};
