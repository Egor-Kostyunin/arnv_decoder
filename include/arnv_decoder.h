/**@file - Содержит объявления типов tag и tag_dict и объявления функций LoadDict,ReadARNV,OutputTags
 * @authors Kostyunin Egor, 2024
 * @date 18.01.2024
 * @version 1.0
 */
#ifndef ARNV_DECODER_H
#define ARNV_DECODER_H
#include <stdint.h>

#define MAX_FORMAT_STR_SIZE 512

/**@brief tag - хранит данные, а также время их снятия
 * @details unixTime хранит время снятия данных
 * tagNumber номер тега нужен для определения какие данные хранит тег
 * tagData данные хранящиеся внутри тега
 */
typedef struct tag_struct{
	int32_t unixTime;
	uint8_t tagNumber;
	int32_t tagData;
} tag;

/**@brief tags_dict - хранит строки формата для кждого тега с нмером tagNumber
 * @details tagFormatString - содержит строку формата для определения
 * и вывода хранящихся в теге данных. Тег может хранить несколько переменных
 * поэтому строка формата имеет следующий вид:
 * "Переменная1 %тип данных|Переменная2 %тип данных"
 *
 * Перемменые расположенны от старшего байта к младшему.
 *
 * Тип данных определяет будет ли число целым(обозначение %i) 
 * или вещественным(%f)
 *
 * Тип данных может иметь приставку указывающую на размер h - 2 байта
 * hh - 1 байт. Если пристваки нет то перемменая занимает все 4 байта
 */
typedef struct tags_dict_struct{
	uint8_t tagNumber;
	char tagFormatString[MAX_FORMAT_STR_SIZE];
} tags_dict;

/**@brief DictLoad - загружает словарь тегов из файла tags.dict
 * @details Вызвать эту функцию первой
 */
void DictLoad();

/**@brief ReadARNV - Извлекает теги из arnv файла
 * @details Считвает arnv файл и возвращает массив извлеченых тегов
 * Функция выделяет память под массив тегов.
 * @param file_path - путь до arnv файла
 * @param tags_count - указатель на переменную куда будет записанно кол-во извлеченных тегов
 * @return tags_array - указатель на массив тегов
 */
tag* ReadARNV(char *file_path,int *tags_count);

/**@brief OutputTags - выводит массив тегов
 * @details Для каждого тега сопоставляет строку формата 
 * из словоря, определяет по ней какие данные записаны в 
 * теге и выводит содержащиеся в теге данные.
 * @param tags_array - указатель на массив тегов
 * @param tags_count - кол-во тегов в массиве
 */
void OutputTags(tag *tags_array,int tags_count);
#endif
