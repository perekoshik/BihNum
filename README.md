# Long Numbers

Этот проект реализует библиотеку длинной арифметики на C++ с поддержкой вычислений с фиксированной точкой заданной точности.

- **Класс LongNum** – представляет число с фиксированной точкой, реализованный с помощью динамического массива цифр.
- Поддерживаются:
  - Конструктор, конструктор копирования, оператор присваивания и деструктор (явно = default).
  - Базовые арифметические операции: сложение, вычитание, умножение, деление.
  - Операторы сравнения: `==`, `!=`, `<`, `>`.
  - Литеральный оператор для создания длинного числа из литерала с плавающей точкой.
  - Возможность задавать точность вычислений (количество двоичных разрядов после запятой).

## Тестирование

- Файл `main.cpp` содержит тестовый набор для проверки работы каждой функции библиотеки.
- При запуске тестов выводится вердикт: OK или FAIL (в данном примере выводятся результаты операций).
