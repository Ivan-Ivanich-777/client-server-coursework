# Сборка TaMP

## Зависимости

- Qt 6.x (Core, Network, Sql, Widgets, Test)
- CMake ≥ 3.16
- C++17-совместимый компилятор (GCC/MinGW или MSVC)

## Сборка сервера (Windows)

```bash
cd TaMP_Server
cmake -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.11.0/mingw_64"
cmake --build build --config Release
```

Исполняемый файл: `build/TaMP.exe`

## Сборка клиента (Windows)

```bash
cd TaMP_Client
cmake -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.11.0/mingw_64"
cmake --build build --config Release
```

## Сборка тестов

```bash
cd TaMP_Tests
cmake -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.11.0/mingw_64"
cmake --build build
cd build && ctest --output-on-failure
```

## Конфигурация email (сервер)

Перед первым запуском скопируйте шаблон и заполните учётные данные:

```bash
cp TaMP_Server/email.txt.example TaMP_Server/email.txt
```

Формат файла `email.txt`:
```
sender@mail.ru
password
```

## Генерация документации

```bash
doxygen Doxyfile
```

HTML-документация появится в `docs/html/index.html`.
