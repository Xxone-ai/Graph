#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QFileDialog>
#include "graphnode.h"
#include "edge.h"
#include <QKeyEvent>
#include <QRandomGenerator>
#include "graphwidget.h"

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
    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_pushButton_clicked();

private:

    bool validateData(QString elem);

    bool validateTable();

    void saveAs();

    void loadFromFile();

    void clear();

    void initTable();

    int** getMatrix();

    void initInput();

    int countOfVerticles;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
