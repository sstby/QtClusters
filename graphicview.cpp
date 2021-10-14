#include "graphicview.h"
#include "ui_graphicview.h"

//Конструктор
graphicView::graphicView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::graphicView)
{
    ui->setupUi(this);
    dataCoords = new Data;


}
//Деструктор
graphicView::~graphicView()
{
    delete [] dataCoords;
    delete[] dataInClasterQty;
    dataCoords = NULL;
    dataInClasterQty = NULL;
    delete ui;
}


//Метод для получения данных из класса mainwindow
void graphicView::getData(Data *array, int clQty, int * clIQty)
{
    dataCoords = array;
    clastersQty = clQty;
    dataInClasterQty = new int[clastersQty];
    dataInClasterQty = clIQty;
    n_rows = dataCoords->getDataCount();

}
//Метод для масштабирования графика
int graphicView::setScale()
{
    //Нахождения наиболее отдалённой точки в графике для того что-бы масштабировать график так,
    //что все точки будут отображены
    int new_scale = 0;
    int max_val_x = 0;
    int max_val_y = 0;
    for (int i = 0; i < n_rows; i++)
    {
        if (max_val_x < dataCoords->m_Data[i].x)
        {
            max_val_x = dataCoords->m_Data[i].x;
        }
        if (max_val_y < dataCoords->m_Data[i].y)
        {
            max_val_y = dataCoords->m_Data[i].y;
        }
    }
    if (max_val_x > max_val_y)
        new_scale = max_val_x;
    else
        new_scale = max_val_y;
    return new_scale;
}

//Метод для получения точки Х на графике в радианах
int graphicView::getScaledX(double x){
    return this->width() - (int)((this->width() * (scale - x))/ (scale * 2));
}

//Метод для получения точки У на графике в радианах
int graphicView::getScaledY(double y){
    return this->height() - (int)((this->height() * (scale + y))/ (scale * 2));
}

//Метод для прорисовки осей графика, включая клетку
void graphicView::drawAxis(QPainter &painter)
{



    scale = setScale() + 5;//Установка масштаба графика. Самая дальная точка + ещё 5 клеток
    painter.setPen(Qt::white);
    //Прорисовка клетки
    for (int i = 1; i < scale+1; i++)
    {
        painter.drawLine(getScaledX(-i), getScaledY(-scale), getScaledX(-i), getScaledY(scale));
        painter.drawLine(getScaledX(i), getScaledY(-scale), getScaledX(i), getScaledY(scale));
        painter.drawLine(getScaledX(-scale), getScaledY(-i), getScaledX(scale), getScaledY(-i));
        painter.drawLine(getScaledX(-scale), getScaledY(i), getScaledX(scale), getScaledY(i));
    }
    painter.setPen(Qt::black); //Устанавливаем кисть

    //Ось Х
    painter.drawLine(0, this->width()/2, this->width(), this->width()/2);
    //Ось Y
    painter.drawLine(this->width()/2, 0, this->width()/2, this->width());
}

//Рисование
void graphicView::paintEvent(QPaintEvent *)
{

    QPainter painter(this);//Создаем объект класса QPainter
    painter.fillRect(0, 0, this->width(), this->height(), Qt::lightGray);//Заливаем окно белым цветом

    //Прорисовываем точки
    drawAxis(painter);
    painter.setRenderHint(QPainter::Antialiasing);
    double x_center = 0;
    double y_center = 0;

    int skip = 0;
    for (int i = 0; i < clastersQty; i++)
    {
        x_center = 0;
        y_center = 0;
        for(int j = 0; j < dataInClasterQty[i];j++)
        {
            drawDot(painter, dataCoords->m_Data[skip], Qt::GlobalColor(i+7));
            x_center += dataCoords->m_Data[skip].x;
            y_center += dataCoords->m_Data[skip].y;
            skip++;
        }
    }

}

//Метод для прорисовки точек на графике
void graphicView::drawDot(QPainter &painter, XY coord, QColor color)
{
    QPen pen(color); //Создаем кисть
    QBrush brush(color); //Создаем заливку

    painter.setPen(pen); //Устанавливаем кисть
    painter.setBrush(brush);//Устанавливаем заливку
    painter.drawEllipse(QPoint(getScaledX(coord.x),getScaledY(coord.y)),2,2);//Рисуем круг
}


