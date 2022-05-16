#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QInputDialog* qid = new QInputDialog();
    bool ok;
    countOfVerticles = qid->getInt(this, "Количество вершин", "Введите количество вершин графа(1-9):", 0, 0, 9, 1, &ok);
    initTable();
    this->ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QAction::connect(ui->action, &QAction::triggered, this, &MainWindow::saveAs);
    QAction::connect(ui->action_2, &QAction::triggered, this, &MainWindow::loadFromFile);
    QAction::connect(ui->action_3, &QAction::triggered, this, &MainWindow::initInput);
    QAction::connect(ui->action_4, &QAction::triggered, this, &MainWindow::close);
}

void MainWindow::initTable()
{
    ui->tableWidget->setRowCount(countOfVerticles);
    ui->tableWidget->setColumnCount(countOfVerticles);
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
        for(int j = 0; j < ui->tableWidget->columnCount(); j++)
            this->ui->tableWidget->setItem(i, j, new QTableWidgetItem("0"));

    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        for(int j = 0; j < ui->tableWidget->columnCount(); j++)
        {
            if(i >= j)
            {
                ui->tableWidget->item(i,j)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                ui->tableWidget->item(i, j)->setBackground(Qt::gray);
            }
        }
    }
}

bool MainWindow::validateData(QString elem)
{
    if(elem.toInt())
        return true;
    return false;
}

void MainWindow::initInput()
{
    clear();
    QInputDialog* qid = new QInputDialog();
    bool ok;
    countOfVerticles = qid->getInt(this, "Количество вершин", "Введите количество вершин графа(1-9):", 0, 0, 9, 1, &ok);
    initTable();
}

bool MainWindow::validateTable()
{
    for(int i = 0 ; i < countOfVerticles; i++)
    {
        for(int j = 0; j < countOfVerticles; j++)
        {
            if(ui->tableWidget->item(i, j) != nullptr)
            {
                if(ui->tableWidget->item(i,j)->text().isEmpty())
                    return false;
            }
            else
                return false;
        }
    }
    return true;
}

void MainWindow::saveAs()
{
    if(!validateTable())
    {
        QMessageBox::warning(this, "Непредвиденная ошибка!","Таблица некорректна!");
        return;
    }
    QString usingFile;
    usingFile = QFileDialog::getSaveFileName(this, "Сохранить как");
    QFile file(usingFile);
    if(!file.open(QFile::WriteOnly|QFile::Text))
    {
       QMessageBox::warning(this, "Непредвиденная ошибка!","Невозможно открыть файл: " + file.errorString());
       return;
    }
    QTextStream loadTo(&file);
    loadTo<<"Количество вершин: " + QString::number(countOfVerticles);
    loadTo<<"\nМатрица смежности:\n";
    for(int i = 0; i < countOfVerticles; i++)
    {
        for(int j = 0; j < countOfVerticles; j++)
        {
            loadTo<<ui->tableWidget->item(i, j)->text() + ' ';
        }
        loadTo<<"\n";
    }
}

void MainWindow::loadFromFile()
{
    QString usingFile;
    usingFile = QFileDialog::getOpenFileName(this, "Выберите файл");
    QFile in(usingFile);
    if(!in.open(QIODevice::ReadOnly|QFile::Text))
    {
        QMessageBox::warning(this,"Непредвиденная ошибка!","Невозможно открыть файл: " + in.errorString());
        return;
    }
    QString line;
    QList<QStringList> values;
    int count = 0;
    while(!in.atEnd())
    {
        line = in.readLine();
        if(line.contains("Количество вершин:"))
        {
            QStringList tmp = line.split(' ', Qt::SkipEmptyParts);
            count = tmp[2].trimmed().toInt();
        }
        if(line.contains("Матрица смежности:"))
        {
            for(int i = 0; i < count; i++)
            {
                line = in.readLine();
                values.push_back(line.split(' ', Qt::SkipEmptyParts));
            }
        }
    }
    if(count)
        countOfVerticles = count;
    else
    {
        QMessageBox::warning(this,"Непредвиденная ошибка!","Ошибка в данных!" + in.errorString());
        return;
    }
    if(values.size() != countOfVerticles)
    {
        QMessageBox::warning(this,"Непредвиденная ошибка!","Ошибка в данных!" + in.errorString());
        return;
    }
    clear();
    initTable();
    for(int i = 0; i < countOfVerticles; i++)
    {
        for(int j = 0; j < countOfVerticles; j++)
        {
            QTableWidgetItem* item = new QTableWidgetItem(values.at(i).at(j));
            this->ui->tableWidget->setItem(i, j, item);

            if(i >= j)
            {
                ui->tableWidget->item(i,j)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                ui->tableWidget->item(i, j)->setBackground(Qt::gray);
            }
        }
    }
}

void MainWindow::clear()
{
    ui->tableWidget->clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    if(!validateData(item->text()))
        item->setText("0");
    if(item->column() > item->row())
    {
        ui->tableWidget->setItem(item->column(), item->row(), new QTableWidgetItem(item->text()));
        ui->tableWidget->item(item->column(), item->row())->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        ui->tableWidget->item(item->column(), item->row())->setBackground(Qt::gray);
    }
    else
        return;
}

int** MainWindow::getMatrix()
{
    int** matrix = new int*[countOfVerticles];
    for(int i = 0; i < countOfVerticles; i++)
    {
        matrix[i] = new int[countOfVerticles];
        for(int j = 0; j < countOfVerticles; j++)
            matrix[i][j] = ui->tableWidget->item(i,j)->text().toInt();
    }
    return matrix;
}

void MainWindow::on_pushButton_clicked()
{
    GraphWidget* widget = new GraphWidget(nullptr, countOfVerticles, getMatrix());
    widget->show();
}
