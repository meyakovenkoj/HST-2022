# Гибридные вычисления

## Задание

Расчет N косинусных мер. Входные данные - 2 матрицы NxM (M - длина вектора, N - количество векторов). Результат - вектор косинусных мер.

## Этапы реализации

- Последовательная реализация
- Реализация через OpenMP
- Реализация через MPI
- Реализация на CUDA
- Реализация MPI+CUDA

## Особенности реализации

- Считывание данных происходит из файла (либо данные передаются по протоколу TCP)
- Данные генерируются утилитой, принимающей в качестве параметров размер данных для обработки в мегабайтах и имя файла (TCP хост-порт) куда будут выгружены данные
- Программа выполняет бизнес-логику и записывает результат в выходной файл (отправляет данные на порт возврата результатов программы-генератора по TCP, сохранение файла с результатами осуществляет программа-генератор)
- В конце файла с результатами сохраняется информация о времени выполнения вычислений и размере обработанных данных

## Описание бизнес-логики

Для данного задания на первом этапе выполнена генерация данных в бинарном виде в два файла. Рандомные значения получаются в диапазоне от 0 до 1 с помощью `(float)rand() / (float)(RAND_MAX)`

Генератор (код в `src/generator`) создает два файла и пишет в них данные для матриц NxM единым блоком
В качестве аргументов принимает ключи

    `-b` размер будет измеряться в байтах
    `-k` размер будет измеряться в килобайтах
    `-h` данные будут записаны в человеко-читаемом виде
    `-l M` длина строки матрицы
    `-s S` размер в мегабайтах (если не указаны -b -k)

Основаная программа читает данные из указанных файлов и выполняет вычисления
Вычислительная логика описана в `src/plain/plain.c`

Алгоритм работы:
- Читаем строку первой матрицы
- Читаем строку второй матрицы
- Выполняем расчет косинусного расстояния ( формула (u*v / (sqrt(u*u) * sqrt(u*u))), где * скалярное произвдение )

