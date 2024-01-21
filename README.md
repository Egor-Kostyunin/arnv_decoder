# Декодер arnv файлов

## Описание
Декодер считывает arnv файл с данными переданными по протоколу ARNAVI и выводит их 
в удобном для чтения человеком виде.

## Использование
<b><i>./arnv_decoder путь_до_arnv_файла</i></b>

## Сборка
### Нужны
- gcc
- gnu make
#### Команда сборки
<b><i>make arnv_decoder</i></b>

## Ограничения
- Декодер считывает только arnv файлы
- Декодер выводит данные только из кадров с типом DATA
- Декодер работает только с одним arnv файлом за раз

## Структура arnv файла с переданными данными

| Пакет кадров(Один или несколко) | |                                                          |
|:---------------------|:--------:| -----------------------------------------------------------|
| Начало набора пакетов| 1 байт   | 5B                                                         |
| Номер посылки        | 1 байт   | От 0x01 до 0xFB                                            |
| Кадр(Один и более)   |          |                                                            |
| Тип кадра            | 1 байт   | 0x00 - PING</br>0x01 - DATA</br>0x03 - TEXT</br>0x04 - FILE|
| Длина данных         | 2 байта  |                                                            |
| Время снятия данных  | 4 байта  | Unix time(число)                                           |
| Данные               | N байт   | Завист от типа                                             |
| Контрольная сумма    | 1 байт   |                                                            |
| Конец набора пакетов | 1 байт   | 5D                                                         |

Кадр типа DATA содержит в данных теги

|Структура тега|        |
|--------------|--------|
|Номер тега    | 1 байт |
|Значение      | 4 байта|