#include "include/fileManager.h"
#include "include/database.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

bool FileManager::exportToCSV(const QString &filePath, const QList<Measurement> &measurements) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream out(&file);
    out << "Name,Date,Joint,StartAngle,EndAngle,ROM\n";
    for (const Measurement &m : measurements) {
        out << m.name << ","
            << m.date.toString("yyyy-MM-dd") << ","
            << m.joint << ","
            << m.startAngle << ","
            << m.endAngle << ","
            << m.rom << "\n";
    }
    file.close();
    return true;
}

void FileManager::importFromCSV(const QString &filePath, Database &db) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    QString header = in.readLine(); // To skip the header (Name, Date...)
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');
        if (fields.size() != 6) continue;
        Measurement m(fields[0], QDate::fromString(fields[1], "yyyy-MM-dd"),
                      fields[2], fields[3].toInt(), fields[4].toInt(), fields[5].toInt());

        db.saveMeasurement(m);
    }
}
