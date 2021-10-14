import numpy as np
import math

def clasterise(array, array_size, h):
    X = np.array(array).reshape(array_size,2)
    All = None #Создаем пустую переменную которая будет хранить все кластеры
    Rest = X[1:] #Создаем массив который будет хранить все значения переданного массива, кроме первого вектора
    #h = 2.0  #радиус круга
    cluster = np.array(([X[0]])) #Создаем первый кластер, в который записывается первый вектор
    iteration = 0 #Счетчик итераций -- отвечает за номер кластера

    #Цикл который будет создавать новые кластеры
    while True:
        #Если все векторы уже расспределены по класстерам, выходим из цикла
        if len(Rest) == 0:

            break;
        k = 0 #Обнуление счетчка добавленных векторов в кластер

        #Цикл который будет перебирать значения пока в наборе не останется подходящих векторов для кластера
        while True:
            vectors = Rest #Копия остатка векторов
            cluster_center = 1/len(cluster) * np.sum(cluster, axis = 0) #Находится центр кластера
            length = np.zeros((len(vectors))) #Создаем пустой массив для хранения расстояния точки до центра кластера

            #Цикл находит расстояние i-той точки до центра кластера, если расстояние меньше чем h, добавляем точку в кластер
            for i in range(len(length)):
                #Находим расстояние
                length[i] = math.sqrt(pow(vectors[i][0] - cluster_center[0], 2) + pow(vectors[i][1] - cluster_center[1], 2))
                #print(length[i])
                #Если расстояние меньше h
                if length[i] < h:
                    cluster = np.append(cluster, [vectors[i]], axis = 0) #Добавляем точку в кластер
                    Rest = np.delete(Rest, i - k, axis = 0) #Удаляем вектор из набора векторов
                    k = k + 1 #Увеличиваем счетчик добавленных точек в кластер на 1
            #print('\n\n')

            #Если новых точек в кластер не добавилось
            if k == 0:

                #Если нулевая итерация
                if iteration == 0:
                    All = [cluster.tolist()] #Создаем вектор и заполняем его первым кластером

                #Иначе, если итерация не нулевая
                else:
                    All.append(cluster.tolist()) #Добавляем в вектор новый полученный кластер

                #Если во время цикла while в наборе векторов остались значения
                if len(Rest) != 0:
                    cluster = np.array(([Rest[0]])) #Устанавливаем центр для нового кластера
                    Rest = Rest[1:] #Убираем новый центр из набора
                    iteration = iteration + 1 #Увеличиваем счетчик итерация на 1
                    if len(Rest) == 0:
                        All.append(cluster.tolist())
                break #Останавливаем цикл while

            #Иначе, если точки добавлялись в кластер, обнуляем счетчкик добавлянных точку и возвращаемся к началу цикла while
            else:
                k = 0
    return All

#Находит дистацию от точки до точки, в основном для нахождения дистанции от вектора до центра кластера
def dist(v, c):
    d = math.sqrt(pow(v[0] - c[0], 2) + pow(v[1] - c[1], 2))
    return d


def maxmin(array, array_size, gama):
    vectors = np.array(array).reshape(array_size,2)
    c = []
    claster1 = np.array(()) #Cоздаем первый пустой кластер
    claster2 = np.array(()) #Создаем второй пустой кластер

    #gama = 0.5 #Гамма по варианту

    #Назначаем первый вектор центром кластера X1
    center_X1 = vectors[0]

    #Находим расстояния всех векторов от центра и максимальное расстояние будет центром второго кластера
    lc1 = np.zeros(len(vectors)) #Пустой массив, который будет хранить расстоянии от вектора до центра1
    lc2 = np.zeros(len(vectors)) #Пустой массив, который будет хранить расстоянии от вектора до центра1
    for i in range(len(lc1)):
        lc1[i] = dist(vectors[i], center_X1)
    center_X2 = vectors[list(lc1).index(max(lc1))] #Центром второго кластера точку, которая будет самой далекой до центра 1
    #Сравниваем расстояние векторов от центра1 до центра2

    comp = np.zeros(len(vectors)*2).reshape(len(vectors),2) #Массив который будет держать расстоянтия точек к центрам разных кластеров
    for i in range(len(lc1)):
        lc2[i] = dist(vectors[i], center_X2) #Находим расстоянтие
        comp[i][0] = lc1[i] #Первый столбец comp будет держать расстоение до центра1
        comp[i][1] = lc2[i] #Второй столбец comp будет держать расстоение до центра2
        #Записываем векторы в кластеры в зависимости от расстояния
        if comp[i][0] < comp[i][1]:
            claster1 = np.append(claster1, vectors[i], axis = 0)
        else:
            claster2 = np.append(claster2, vectors[i], axis = 0)

    #Преобразование массива кластеров в массив с двумя столбцами
    claster1 = np.array(claster1).reshape(int(len(claster1)/2),2)
    claster2 = np.array(claster2).reshape(int(len(claster2)/2),2)

    #Создание массива с центрами из двух основных центров
    centers = np.array((center_X1, center_X2))

    #Добавление двух основным кластеров в список
    c.append(claster1.tolist())
    c.append(claster2.tolist())

    #Cчетчик количества кластеров
    x = 2

    #Цикл, который будет находить самый отдаленный вектор от своего кластера
    while True:
        max_distances = np.array(()) #Массив будет хранить в себе максимальные дистанции
        max_index = np.array(()) #Индексы элементов с максимальными значениями
        for i in range(len(c)):
            l = np.zeros(len(c[i])) #Создает матрицу, размер которой будет равен количеству элементов в i-том кластере
            for j in range(len(l)):
                l[j] = dist(c[i][j], centers[i]) #Находим дистанцию
            ind = list(l).index(max(l)) #Находим индекс максимального элемента i-того кластера
            max_index = np.append(max_index, ind) #Добавляем максимальный элемент i-го кластера в массив
            max_distances = np.append(max_distances, dist(c[i][ind], centers[i])) #Добавляем максимальные дистанции в массив
        max_claster_id = list(max_distances).index(max(max_distances)) #Находим кластер, в котором самый отдаленный элемент
        m_i = int(max_index[max_claster_id]) #Индекс самого отдаленного элемента
        ttt = [c[max_claster_id][m_i]] #Создаем массив 1х2 с самым отдаленным вектором
        xc = dist(ttt[0], centers[max_claster_id]) #Расстояние вектора от своего кластера
        c1c2 = gama*dist(centers[0], centers[1]) #Расстояние от центра 0 до центра 1 умноженное на гамму

        #Если элемент расположен дальше чем 1/2*с0-с1, то далаем этот вектор центром нового кластера иначе останавливаем основной цикл while
        if xc > c1c2:
            centers = np.append(centers, ttt, axis = 0)
        else:
            break

        #Если новый центр добавлен и количество кластеров больше чем счетчки
        if len(centers) > x:

                #Массив который будет держать расстоянтия точек к центрам разных кластеров
                comp = np.zeros(len(vectors)*len(centers)).reshape(len(vectors),len(centers))
                #Обнуляем матрицу класстеров
                c = []

                #Добавляем пустой массив в матрицу кластаров
                for i in range(len(centers)):
                    tmp = []
                    c.append(tmp)

                #Находим расстояния всех векторов ко всем центрам кластеров, и в зависимости от расстояния добавляем их в определенные матрицу
                for i in range(len(vectors)):
                    for j in range(len(centers)):

                        comp[i][j] = dist(vectors[i], centers[j])
                    min_id = list(comp[i]).index(min(comp[i]))
                    c[min_id].append(vectors[i].tolist())
                #Увеличиваем счетчик кластеров на 1
                x = x + 1
        else:
            break
    return c

def fill(vectors, centers):
    l_v = len(vectors)
    l_c = len(centers)
    comp = np.zeros(l_v*l_c).reshape(l_v,l_c)
    new_clusters = []
    for i in range(l_c):
            tmp = []
            new_clusters.append(tmp)

    for i in range(l_v):
        for j in range(l_c):
            comp[i][j] = dist(vectors[i], centers[j])

        min_id = list(comp[i]).index(min(comp[i]))
        new_clusters[min_id].append(vectors[i].tolist())
    print("Расстояние точек до центров кластеров:\n", comp, "\n\n")

    return new_clusters

def findCenter(cl, c):
    new_centers = []
    l_c = len(cl)
    for i in range(l_c):
            tmp = []
            new_centers.append(tmp)
            for j in range(2):
                tmp = []
                new_centers[i].append(tmp)
    for i in range(l_c):
            tmp = 1/len(cl[i]) * np.sum(cl[i], axis = 0)
            for j in range(2):
                new_centers[i][j] = tmp[j]
    new_centers = np.array(np.around(new_centers, decimals=1))
    return new_centers

def kmean(array, array_size, clasters_Qty):
    vectors = np.array(array).reshape(array_size,2)
    centers = np.array((vectors[:int(clasters_Qty)]))
    clusters = []
    print("Cформированные начальные центры кластеров:\n",centers, "\n\n")
    while True:
        clusters = fill(vectors, centers)
        print("Cформированы новые кластеры:\n",clusters, "\n\n")
        centers = findCenter(clusters, centers)
        print("Cформированы новые центры кластеров:\n",centers, "\n\n")

        clusters1 = fill(vectors, centers)
        print("Cформированы новые кластеры:\n",clusters1, "\n\n")
        centers1 = findCenter(clusters1, centers)
        print("Cформированы новые центры кластеров:\n",centers1, "\n\n")
        #print(centers)
        #print(centers1)
        #print(centers - centers1)


        check = True
        for i in range(2):
            for j in range(2):
                tmp = centers[i][j] - centers1[i][j]
                print(str(centers[i][j]) + " - " + str(centers1[i][j]) + " = " + str(tmp))
                if centers[i][j] - centers1[i][j] != 0:
                    check = False
                    break
        if check == True:
            break
        else:
            continue
    return clusters1
