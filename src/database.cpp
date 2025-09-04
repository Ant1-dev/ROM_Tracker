#include "include/database.h"
#include <QMessageBox>

Database::Database(const QString &dbName) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);

    if (!db.open()) {
        QMessageBox::critical(nullptr, "Database Error", db.lastError().text());
    }

    createTableIfNotExists();
}

bool Database::createTableIfNotExists() {
    QSqlQuery query;
    return query.exec(
        "CREATE TABLE IF NOT EXISTS measurements ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT, "
        "date TEXT, "
        "joint TEXT, "
        "startAngle INTEGER, "
        "endAngle INTEGER, "
        "rom INTEGER)"
        );
}

bool Database::saveMeasurement(const Measurement &m) {
    QSqlQuery query;
    query.prepare("INSERT INTO measurements (name, date, joint, startAngle, endAngle, rom) "
                  "VALUES (?, ?, ?, ?, ?, ?)");
    query.addBindValue(m.name);
    query.addBindValue(m.date.toString("yyyy-MM-dd"));
    query.addBindValue(m.joint);
    query.addBindValue(m.startAngle);
    query.addBindValue(m.endAngle);
    query.addBindValue(m.rom);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Database Error", query.lastError().text());
        return false;
    }
    return true;
}

QList<Measurement> Database::loadAllMeasurements() {
    QList<Measurement> list;
    QSqlQuery query("SELECT name, date, joint, startAngle, endAngle, rom FROM measurements");

    while (query.next()) {
        Measurement m;
        m.name = query.value(0).toString();
        m.date = QDate::fromString(query.value(1).toString(), "yyyy-MM-dd");
        m.joint = query.value(2).toString();
        m.startAngle = query.value(3).toInt();
        m.endAngle = query.value(4).toInt();
        m.rom = query.value(5).toInt();
        list.append(m);
    }
    return list;
}

bool Database::clearAllMeasurements() {
    QSqlQuery query("DELETE FROM measurements");
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Database Error", query.lastError().text());
        return false;
    }
    return true;
}

bool Database::deleteMeasurement(const QString &name, const QString &date, const QString &joint) {
    QSqlQuery query;
    query.prepare("DELETE FROM measurements WHERE name = ? AND date = ? AND joint = ?");
    query.addBindValue(name);
    query.addBindValue(date);
    query.addBindValue(joint);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Database Error", query.lastError().text());
        return false;
    }
    return true;
}
