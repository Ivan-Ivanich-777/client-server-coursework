# Архитектура (MVC)
## Сервер
myserver.cpp - TCP интерфейс | servercore.cpp - логика | dbmanager.cpp - БД (Singleton)
## Клиент
authform - вход | taskform - задание | dashboardform - график | socketmanager - TCP (Singleton)
## Протокол
Разделитель: & | auth/reg/verify/forgot/reset/check/calc_table
