#include "data.h"
//Конструктор
Data::Data()
{

}
//Деструктор
Data::~Data()
{
    //Уничтожение данных
    delete[] m_Data;
    m_Data = 0;
    m_Count = 0;
}
//Функция для создание массива точек с указанным количеством точек
void Data::Coordinates(int numberOfLines)
{
    m_Count = numberOfLines;
    m_Data = new XY[numberOfLines];
}
//Функция для получения данных массива
const XY* Data::getData()
{
    return m_Data;
}
//Функция для получение размера массива
int Data::getDataCount()
{
    return m_Count;
}

void Data::Clear()
{
    delete[] m_Data;
    m_Data = 0;
    m_Count = 0;
}

