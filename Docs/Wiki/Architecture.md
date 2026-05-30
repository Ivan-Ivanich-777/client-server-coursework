# Архитектура TaMP

## Общая схема

```
┌─────────────────────────────────────┐     TCP:12345      ┌─────────────────────────────────┐
│           TaMP_Client               │ ◄─────────────────► │         TaMP_Server             │
│  ┌──────────────────────────────┐   │                     │  ┌────────────────────────────┐ │
│  │  View (UI Forms)             │   │                     │  │  ServerInterface           │ │
│  │  LoginWindow                 │   │                     │  │  (QTcpServer, accept loop) │ │
│  │  RegisterWindow              │   │                     │  └─────────────┬──────────────┘ │
│  │  ResetPasswordWindow         │   │                     │                │                │
│  │  TaskWindow                  │   │                     │  ┌─────────────▼──────────────┐ │
│  └──────────┬───────────────────┘   │                     │  │  ServerLogic               │ │
│             │                       │                     │  │  (Request dispatcher)      │ │
│  ┌──────────▼───────────────────┐   │                     │  └─────────────┬──────────────┘ │
│  │  Controller (main.cpp)       │   │                     │                │                │
│  │  Window switching logic      │   │                     │  ┌─────────────▼──────────────┐ │
│  └──────────┬───────────────────┘   │                     │  │  DbHandler                 │ │
│             │                       │                     │  │  (SQLite: users)            │ │
│  ┌──────────▼───────────────────┐   │                     │  └────────────────────────────┘ │
│  │  TcpClient (Singleton)       │   │                     │                                 │
│  │  (QTcpSocket wrapper)        ├───┼─────────────────────┤                                 │
│  └──────────────────────────────┘   │                     └─────────────────────────────────┘
└─────────────────────────────────────┘
```

## Серверная часть — MVC

| Слой | Файлы | Роль |
|---|---|---|
| **View (Interface)** | `server_interface.h/.cpp` | Приём TCP-соединений, чтение/запись сокета |
| **Controller (Logic)** | `server_logic.h/.cpp` | Диспетчеризация команд, бизнес-правила |
| **Model (DB)** | `db_handler.h/.cpp` | Работа с SQLite (users) |
| **Protocol** | `protocol.h/.cpp` | Парсинг запросов, формирование ответов |
| **SMTP** | `smtpclient_mailru.h/.cpp` | Отправка кода сброса пароля |

## Клиентская часть — MVC

| Слой | Файлы | Роль |
|---|---|---|
| **View** | `loginwindow`, `registerwindow`, `resetpasswordwindow`, `taskwindow` | Формы взаимодействия с пользователем |
| **Controller** | `main.cpp` | Навигация между окнами по сигналам |
| **Model (Network)** | `tcpclient.h/.cpp` | Singleton-обёртка над QTcpSocket |

## Паттерн Singleton (TcpClient)

```cpp
// Улучшённая реализация: локальная статическая переменная
static TcpClient& getInstance() {
    static TcpClient instance;
    return instance;
}
```

Гарантирует единственное TCP-соединение на протяжении всего времени работы клиента.

## Схема базы данных

```sql
CREATE TABLE users (
    id    INTEGER PRIMARY KEY AUTOINCREMENT,
    name  TEXT    NOT NULL,
    login TEXT    NOT NULL UNIQUE,
    hash  TEXT    NOT NULL,   -- MD5-хэш пароля
    email TEXT    NOT NULL
);
```
