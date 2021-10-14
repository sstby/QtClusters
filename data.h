#ifndef DATA_H
#define DATA_H

//Структора данных XY
typedef struct _tagXY
{
    double x;
    double y;
}XY;

//Класс для создание массива точек координат
class Data
{
public:
    Data();//Конструктор
    ~Data();//Деструктор
    void Coordinates(int numberOfLines);//Функция для создание массива точек с указанным количеством точек
    const XY* getData();//Функция для получения данных массива
    const XY* getData(int n);//Функция для получения конкретной точки в массиве
    int getDataCount();//Функция для получение размера массива
    int setClasterCount(int n);//Функция для установки размера массива
    XY* m_Data;//Перемення для хранение Х и У
    void Clear();//Очистка массива
private:
    int m_Count;//Количество точек массива
};

#endif // DATA_H
