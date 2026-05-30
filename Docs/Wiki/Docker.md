# Развёртывание в Docker

## Требования

- Docker Engine ≥ 20.x
- Файл `TaMP_Server/email.txt` (скопировать из `email.txt.example`)

## Сборка образа

```bash
# Из корня репозитория
docker build -f docker/Dockerfile -t tamp-server .
```

## Запуск контейнера

```bash
docker run -d \
  --name tamp \
  -p 12345:12345 \
  tamp-server
```

## Проверка работы

```bash
# Подключиться через telnet/PuTTY
telnet localhost 12345

# Отправить тестовый запрос (вручную):
auth&testuser&5f4dcc3b5aa765d61d8327deb882cf99
# Ожидаемый ответ:
# auth- (если пользователь не зарегистрирован)
```

## Остановка и удаление

```bash
docker stop tamp
docker rm tamp
docker rmi tamp-server
```

## Структура Dockerfile

```dockerfile
FROM ubuntu:22.04
# Устанавливает Qt6, CMake, OpenSSL
# Копирует исходники и email.txt
# Собирает сервер
# EXPOSE 12345
# ENTRYPOINT ["./TaMP"]
```
