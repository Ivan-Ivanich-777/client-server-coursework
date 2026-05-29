# MathSolver - Client-Server Application

## Описание
Клиент-серверное приложение для построения графиков кусочно-заданной функции с авторизацией, регистрацией и визуализацией.

## Функция
f(x) = { a*x^2, x < 0; x^3 - b*x, 0 <= x < 2; c*(x^4-4x^3+4x^2), x >= 2 }

## Структура проекта
- Server/ — TCP сервер (C++, Qt 6, порт 33333)
- Client/ — GUI клиент (Qt Widgets)
- docs/ — UML диаграммы, тесты
- Doxygen/ — сгенерированная документация
- docker/ — Dockerfile для сервера

## Технологии
C++, Qt 6, SQLite, Docker, Doxygen, Qt Test

## Быстрый запуск
Сервер: Server/build/debug/Server.exe
Клиент: Client/Client.exe

## Авторы
- Гусев Иван — Team Lead
- Сиухин Александр — Developer
- Мережкин Дмитрий — Developer
- Строгалов Никита — Developer

## Ссылки
https://github.com/Ivan-Ivanich-777/client-server-coursework
