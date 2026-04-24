# Math Solver — Клиент-Серверное приложение

## Описание
TCP-сервер и Qt-клиент для визуализации и анализа кусочной функции.

## Функция
f(x) = {
    x²,              x < 0
    x³ - 3x,         0 ≤ x < 2
    x⁴ - 4x³ + 4x²,  x ≥ 2
}

## Протокол обмена
| Команда | Формат | Ответ |
|---------|--------|-------|
| Авторизация | auth&login&pass | auth+&login / auth- |
| Регистрация | reg&login&pass&email | reg+&login / reg- |
| Статистика | stat&login | stat&X&Y&Z |
| Проверка | check&task&x&y | check+ / check- |

## Сборка
- Сервер: qmake Server.pro && make
- Клиент: qmake Client.pro && make

## Docker
docker build -t myserver .
docker run -p 33333:33333 myserver
