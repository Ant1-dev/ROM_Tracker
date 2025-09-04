#include <include/mainwindow.h>
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QDate>
#include "../include/database.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , db("rom_tracker.db") // Initialize database
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
    initResultSearch();

    // Connect buttons
    connect(ui->pushButtonSaveMeasurement, &QPushButton::clicked,
            this, &MainWindow::onSaveMeasurementClicked);

    connect(ui->pushButtonClear, &QPushButton::clicked,
            this, &MainWindow::onClearFormClicked);

    connect(ui->pushButtonResetHistory, &QPushButton::clicked,
            this, &MainWindow::onResetHistoryClicked);



    // Initialize table
    initTable();

    // Load measurements from DB if any
    loadMeasurementsFromDB();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initTable()
{
    ui->tableWidgetHistory->setColumnCount(7);
    QStringList headers = { "Name", "Date", "Joint", "Start", "End", "ROM", "Delete" };
    ui->tableWidgetHistory->setHorizontalHeaderLabels(headers);
    ui->tableWidgetHistory->setSortingEnabled(true);
    ui->tableWidgetHistory->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetHistory->setAlternatingRowColors(true);
    ui->tableWidgetHistory->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::initResultSearch()
{
    ui->lineEditName->setPlaceholderText("Enter name...");

    QHBoxLayout *searchLayout = new QHBoxLayout();
    ui->groupBoxSearch->setLayout(searchLayout);
    QLabel *searchLabel = new QLabel("Search:");
    QLineEdit *searchLineEdit = new QLineEdit();
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchLineEdit);
    searchLineEdit->setPlaceholderText("Type to filter results...");

    connect(searchLineEdit, &QLineEdit::textChanged, this, &MainWindow::filterTable);

}

Measurement MainWindow::readInputFields()
{
    Measurement m;
    m.name = ui->lineEditName->text().trimmed();
    m.date = ui->dateEdit->date();
    m.joint = ui->comboBoxJointSelect->currentText();
    m.startAngle = ui->spinBoxStartAngle->value();
    m.endAngle = ui->spinBoxEndAngle->value();
    m.rom = m.endAngle - m.startAngle;
    return m;
}

bool MainWindow::validateMeasurement(const Measurement &m)
{
    if (m.name.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter patient name.");
        ui->labelROMValue->setStyleSheet("color: red;");
        return false;
    }
    if (m.joint.isEmpty()) {
        ui->labelROMValue->setText("⚠ Please select a joint");
        ui->labelROMValue->setStyleSheet("color: red;");
        return false;
    }
    if (m.rom < 0) {
        QMessageBox::warning(this, "Input Error", "End angle must be greater than start angle.");
        ui->labelROMValue->setStyleSheet("color: red;");
        return false;
    }
    return true;
}

void MainWindow::addMeasurementToTable(const Measurement &m)
{
    int row = ui->tableWidgetHistory->rowCount();
    ui->tableWidgetHistory->insertRow(row);
    ui->tableWidgetHistory->setItem(row, 0, new QTableWidgetItem(m.name));
    ui->tableWidgetHistory->setItem(row, 1, new QTableWidgetItem(m.date.toString("yyyy-MM-dd")));
    ui->tableWidgetHistory->setItem(row, 2, new QTableWidgetItem(m.joint));
    ui->tableWidgetHistory->setItem(row, 3, new QTableWidgetItem(QString::number(m.startAngle)));
    ui->tableWidgetHistory->setItem(row, 4, new QTableWidgetItem(QString::number(m.endAngle)));
    ui->tableWidgetHistory->setItem(row, 5, new QTableWidgetItem(QString::number(m.rom)));

    // Create delete button
    QPushButton *deleteButton = new QPushButton("Delete");
    ui->tableWidgetHistory->setCellWidget(row, 6, deleteButton);

    // Connect delete button to handler
    connect(deleteButton, &QPushButton::clicked, [this, row]() {
        onDeleteRowClicked(row);
    });
}

void MainWindow::displayROM(int rom)
{
    ui->labelROMValue->setStyleSheet("color: #333333;");
    ui->labelROMValue->setText(QString::number(rom) + " °");
}

// ------------------- Slots -------------------

void MainWindow::onSaveMeasurementClicked()
{
    Measurement m = readInputFields();
    if (!validateMeasurement(m)) return;

    measurements.append(m);
    addMeasurementToTable(m);
    displayROM(m.rom);

    if (!db.saveMeasurement(m)) {
        QMessageBox::warning(this, "Database Error", "Failed to save measurement.");
    }
}

void MainWindow::onClearFormClicked()
{
    ui->lineEditName->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->comboBoxJointSelect->setCurrentIndex(0);
    ui->spinBoxStartAngle->setValue(0);
    ui->spinBoxEndAngle->setValue(0);
    displayROM(0);
}

void MainWindow::onResetHistoryClicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Reset",
                                    "Are you sure you want to clear all history?",
                                    QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (db.clearAllMeasurements()) {
            measurements.clear();
            ui->tableWidgetHistory->setRowCount(0);
            QMessageBox::information(this,"History Cleared", "All measurements have been deleted.");
        } else {
            QMessageBox::warning(this, "Error", "Failed to clear history.");
        }
    }
}

void MainWindow::loadMeasurementsFromDB()
{
    QList<Measurement> list = db.loadAllMeasurements();
    for (int i = 0; i < list.size(); ++i) {
        const Measurement &m = list[i];
        measurements.append(m);
        addMeasurementToTable(m);
    }

}

void MainWindow::filterTable(const QString &query)
{
    for (int row = 0; row < ui->tableWidgetHistory->rowCount(); ++row) {
        bool match = false;
        for (int col = 0; col < ui->tableWidgetHistory->columnCount(); ++col) {
            QTableWidgetItem *item = ui->tableWidgetHistory->item(row, col);
            if (item && item->text().contains(query, Qt::CaseInsensitive)) {
                match = true;
                break;
            }
        }
        ui->tableWidgetHistory->setRowHidden(row, !match);
    }
}

void MainWindow::onDeleteRowClicked(int row)
{
    if (row < 0 || row >= ui->tableWidgetHistory->rowCount()) return;

    QString name = ui->tableWidgetHistory->item(row, 0)->text();
    QString date = ui->tableWidgetHistory->item(row, 1)->text();
    QString joint = ui->tableWidgetHistory->item(row, 2)->text();

    if (QMessageBox::question(this, "Delete Row",
                              "Are you sure you want to delete this measurement?") == QMessageBox::Yes)
    {
        db.deleteMeasurement(name, date, joint);

        // Remove from UI
        ui->tableWidgetHistory->removeRow(row);
    }
}


