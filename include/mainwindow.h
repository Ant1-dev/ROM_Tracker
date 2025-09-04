#include <QMainWindow>
#include "measurement.h"
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSaveMeasurementClicked();
    void onClearFormClicked();
    void onResetHistoryClicked();
    void filterTable(const QString &query);
    void onDeleteRowClicked(int row);

private:
    Ui::MainWindow *ui;
    QList<Measurement> measurements;
    Database db;

    void initTable();
    void initResultSearch();
    Measurement readInputFields();
    bool validateMeasurement(const Measurement &m);
    void addMeasurementToTable(const Measurement &m);
    void displayROM(int rom);
    void loadMeasurementsFromDB();
};
