MathSolver - Инструкция по запуску

Клиент-серверное приложение для построения графиков кусочно-заданной функции.
Стек: C++, Qt 6, SQLite, Docker, Doxygen.

БЫСТРЫЙ ЗАПУСК

1. Клонировать репозиторий:
git clone https://github.com/Ivan-Ivanich-777/TIMP_PROJECT.git

2. Запустить сервер:
C:\MyProject\Server\build\debug\Server.exe

3. Запустить клиент:
C:\MyProject\Client\Client.exe

4. Код подтверждения при регистрации приходит на email.

НЕОБХОДИМОЕ ПО

Qt 6.x - сборка сервера и клиента
Docker Desktop - запуск сервера в контейнере
Git - клонирование репозитория
Doxygen 1.9+ - генерация документации

УСТАНОВКА ЗАВИСИМОСТЕЙ (Windows)

1. Скачать Qt Online Installer с https://www.qt.io/download
2. При установке выбрать: Qt 6.x, MinGW 64-bit, Qt Network, Qt SQL
3. Скачать Docker Desktop с https://www.docker.com/products/docker-desktop/

ЗАПУСК СЕРВЕРА

Через Qt Creator:
Файл - Открыть проект - Server/Server.pro - Release - Run

Или через командную строку:
cd Server
qmake Server.pro
make
./Server

DOCKER

Сборка образа:
docker build -t mathsolver-server .

Запуск контейнера:
docker run -p 33333:33333 mathsolver-server

ЗАПУСК КЛИЕНТА

Через Qt Creator:
Файл - Открыть проект - Client/Client.pro - Release - Run

Или через командную строку:
cd Client
qmake Client.pro
make
./Client

ПРОСМОТР ПОЧТЫ

При регистрации код подтверждения приходит на указанный email.
Проверьте папку Спам, если письма нет во Входящих.

ДОКУМЕНТАЦИЯ

Документация Doxygen: Docs/Doxygen/html/index.html
UML диаграммы: Docs/UML/
Тест-кейсы: Docs/Tests/test_cases.tsv
Вики: Docs/Wiki/

СТРУКТУРА ПРОЕКТА

MyProject/
  Server/          Сервер (C++, Qt)
  Client/          Клиент (Qt Widgets)
  Docs/            Документация
    Doxygen/       HTML документация
    UML/           Диаграммы
    Tests/         Тест-кейсы
    Wiki/          Вики
  Dockerfile       Docker образ
  README.md        Описание проекта
