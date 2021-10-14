#ifndef GRAPHICVIEW_H
#define GRAPHICVIEW_H

#include <QWidget>
#include <QPainter> //Рисование
#include <QPaintEvent> //Рисование
#include <QDebug>
#include "data.h"


namespace Ui {
class graphicView;
}

//Класс окна графика
class graphicView : public QWidget
{
    Q_OBJECT

public:

    explicit graphicView(QWidget *parent = nullptr);//Конструктор
    ~graphicView();//Деструктор
    void getData(Data *array, int clQty, int * clIQty);//Метод для получение данных из класса mainwindow
    void drawDot(QPainter & painter, XY coord, QColor color);//Метод для прорисовки точек на графике
    void drawAxis(QPainter & painter);//Метод для прорисовки осей графика, включая клетку
    int setScale();//Метод для масштабирования графика
    int getScaledX(double x);//Метод для получения точки Х на графике в радианах
    int getScaledY(double y);//Метод для получения точки У на графике в радианах
    int scale = 0; //Переменная для хранения коэффициента масштаба
    int n_rows;//Переменная для хранения количества точек на графике
    int clastersQty;//Переменная для хранения количества кластеров
    int * dataInClasterQty;//Переменная для хранения количества точек в каждом кластере

private:
    Ui::graphicView *ui;//Интерфейс окна
    Data * dataCoords;//Указатель на массив точек
protected:
    void paintEvent(QPaintEvent * event);//Метод для прорисовки графике при открытии окна
};

#endif // GRAPHICVIEW_H
