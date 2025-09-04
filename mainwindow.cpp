#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());

    // Style sheet for application
    qApp->setStyleSheet(
        "QWidget {"
        "    background-color: #f9f9f9;"
        "    font-size: 11pt;"
        "    font-family: \"Segoe UI\", \"Helvetica Neue\", Arial;"
        "    color: #000;"
        "}"
        "QGroupBox {"
        "    font-weight: bold;"
        "    border: 1px solid gray;"
        "    border-radius: 6px;"
        "    margin-top: 10px;"
        "    padding: 6px;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 10px;"
        "    padding: 0 5px;"
        "}"
        "QLineEdit, QDateEdit, QSpinBox, QComboBox {"
        "    border: 1px solid #ccc;"
        "    border-radius: 4px;"
        "    padding: 4px;"
        "    background-color: #ffffff;"
        "    color: #000;"
        "}"
        "QLineEdit:focus, QDateEdit:focus, QSpinBox:focus, QComboBox:focus {"
        "    border: 1px solid #2d89ef;"
        "    outline: none;"
        "}"
        "QPushButton {"
        "    background-color: #2d89ef;"
        "    color: white;"
        "    border-radius: 6px;"
        "    padding: 6px 12px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #1a5fb4;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #104a85;"
        "}"
        "#labelROMValue {"
        "    font-size: 16pt;"
        "    font-weight: bold;"
        "    color: #333333;"
        "}"
        );


    connect(ui->pushButtonSaveMeasurement, &QPushButton::clicked,
            this, &MainWindow::onSaveMeasurementClicked);

    connect(ui->pushButtonClear, &QPushButton::clicked,
            this, &MainWindow::onClearFormClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSaveMeasurementClicked()
{
    // Read inputs from ui
    QString name = ui ->lineEditName->text().trimmed();
    QDate date = ui->dateEdit->date();
    QString joint = ui->comboBoxJointSelect->currentText();
    int startAngle = ui ->spinBoxStartAngle->value();
    int endAngle = ui ->spinBoxEndAngle->value();

    // Input Validation
    if (name.isEmpty()) {
        ui->labelROMValue->setText("Please enter patient name");
        ui->labelROMValue->setStyleSheet("color: red;");
        return;
    }
    if (joint.isEmpty()) {
        ui->labelROMValue->setText("⚠ Please select a joint");
        ui->labelROMValue->setStyleSheet("color: red;");
        return;
    }



    // Compute Range of Motion
    int rom = endAngle - startAngle;

    // Validate angles
    if (rom < 0) {
        ui->labelROMValue->setText("Invalid angles");
        ui->labelROMValue->setStyleSheet("color: red;");
        return;
    }

    // Save into memory
    Measurement m {name, date, joint, startAngle, endAngle, rom };
    measurements.append(m);

    //Table intialization
    ui->tableWidgetHistory->setColumnCount(6);
    QStringList headers = { "Name", "Date", "Joint", "Start", "End", "ROM" };
    ui->tableWidgetHistory->setHorizontalHeaderLabels(headers);

    // Stretch columns to fit
    ui->tableWidgetHistory->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetHistory->setAlternatingRowColors(true);

    // Save to table
    int row = ui->tableWidgetHistory->rowCount();
    ui->tableWidgetHistory->insertRow(row);

    ui->tableWidgetHistory->setItem(row, 0, new QTableWidgetItem(m.name));
    ui->tableWidgetHistory->setItem(row, 1, new QTableWidgetItem(m.date.toString("yyyy-MM-dd")));
    ui->tableWidgetHistory->setItem(row, 2, new QTableWidgetItem(m.joint));
    ui->tableWidgetHistory->setItem(row, 3, new QTableWidgetItem(QString::number(m.startAngle)));
    ui->tableWidgetHistory->setItem(row, 4, new QTableWidgetItem(QString::number(m.endAngle)));
    ui->tableWidgetHistory->setItem(row, 5, new QTableWidgetItem(QString::number(m.rom)));

    // Display Range of Motion
    ui->labelROMValue->setStyleSheet("color: #333333;");
    ui->labelROMValue->setText(QString::number(rom) + " °");
}

// Clear button function
void MainWindow::onClearFormClicked()
{
    ui->lineEditName->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->comboBoxJointSelect->setCurrentIndex(0);
    ui->spinBoxStartAngle->setValue(0);
    ui->spinBoxEndAngle->setValue(0);
    ui->labelROMValue->setText(QString::number(0) + " °");
}

