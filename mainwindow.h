#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QProcess>
#include "data.h"
#include "newdatadialog.h"
#include "graphicview.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
//Класс основного окно программы
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);//Конструктор основного окна
    ~MainWindow();//Деструктор
    void showClustersTable();//Метод для вывода таблицы кластеров
    void clearClustersTable();//Метод для очистки таблицы кластеров
    void makeClusterTableStyle();//Метод для установки ширины ячеек в таблице кластеров
    int n_rows=0;//Количество созданных точек координат
    int clastersQty = 0;//Переменная для хранения количества кластеров
    int * dataInClasterQty;//Переменная для хранения количества точек в каждом кластере
    double addVal = 1;//Переменная для хранения дополнительного значения указанного в окне
    int method_ID = 0;//Переменная для числового хранения выыбраного метода
    void Clear(); //Очистка данных


private slots:
    //Действие при нажатии на кнопку
    void on_pushButton_clicked();

    //Дествие при изменении метода кластеризации
    void on_clasteriseMethod_currentTextChanged(const QString &arg1);

    //Действие при нажатии на кнопку Open
    void on_actionOpen_triggered();

    //Действие при нажатии на кнопку New Data
    void on_actionNew_Data_triggered();

    //Функция для получения данных из диалогового окна NewData
    void getData(Data* array);
    void on_actionClear_triggered();

    void on_actionClose_triggered();

    void on_actionEdit_triggered();

    void on_actionAutor_triggered();

private:
    Ui::MainWindow *ui;//Интерфейс программы
    NewDataDialog * dlg;//Инициализация диалового окна
    graphicView * graphic;//Инициализация окна графика
    Data * dataCoords;//Инициализация массива точек координат
};
#endif // MAINWINDOW_H
