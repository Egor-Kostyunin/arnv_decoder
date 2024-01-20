#include "arnv_decoder.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct tmp_tag_list_struct{
	tag aTag;
	struct tmp_tag_list_struct *next;
} tmp_tag_list;

tag* ReadARNV(char *file_path,int *count){

	tmp_tag_list *begin = NULL, *current = NULL;

	int allTagsCount = 0;

	FILE *arnvFile =  fopen(file_path,"r");

	while(feof(arnvFile) == 0){

		uint8_t framesSetBegin[2];

		if(arnvFile == NULL){
			perror("Не удалось открыть arnv файл");
			exit(5);
		}

		//Чтение начала набора кадров
		if(fread(framesSetBegin,1,2,arnvFile) != 2){
			perror("Не удалось считать начало набора кадров");
			exit(6);
		}

		//Проверка начала набора кадров
		if(framesSetBegin[0] != 0x5B ||
		   framesSetBegin[1] > 0xFB ||
		   framesSetBegin[1] < 0x01){
			perror("Недопустимое начало набора кадров");
			exit(7);
		}

		uint8_t frameType = 0;//Тип кадра
		uint16_t frameDataLength = 0;//Длина данных кадра
		int32_t frameDataTime = 0;//Время снятия данных
		
		//Чтение кадров
		while(fread(&frameType,1,1,arnvFile) == 1){

			//Проверка на конец набора кадров
			if(frameType == 0x5D) break;

			if(frameType != 0x01){
				puts("Пропущен кадр с неподдерживаемым типом");
				continue;
			}

			if(fread(&frameDataLength,2,1,arnvFile) != 1 ||
			   fread(&frameDataTime,4,1,arnvFile) != 1){
				perror("Не удалось считать кадр");
				exit(8);
			}

			int tagsCount = frameDataLength/5;

			//Проверка кратности длины данных кадра длине тега
			if(frameDataLength % 5 == 0){
				allTagsCount += tagsCount;
			}
			else{
				perror("Длина данных кадра не кратна 5");
				exit(9);
			}
			
			//Чтение тегов
			for(int i = 0; i < tagsCount; i++){
				//Выделение памяти под первый элемент листа тегов 
				if(begin == NULL){
					begin = (tmp_tag_list*)calloc(1,sizeof(tmp_tag_list));
					current = begin;
				}

				if(current == NULL){
					perror("Не удалось выделить память под тег");
					exit(10);
				}

				current->aTag.unixTime = frameDataTime;

				//Чтение тега
				if(fread(&current->aTag.tagNumber,1,1,arnvFile) != 1){
					perror("Не удалось считать номер тега");
					exit(11);
				}

				if(fread(&current->aTag.tagData,4,1,arnvFile) != 1){
					perror("Не удалось считать значение тега");
					exit(12);
				}

				//Следующий тег
				current->next = (tmp_tag_list*)calloc(1,sizeof(tmp_tag_list));
				current = current->next;
			}
			//Пропуск байта контрольной суммы
			fseek(arnvFile,1,SEEK_CUR);
		}
	}

	fclose(arnvFile);

	int index = 0;

	tag *tagsArray = (tag*)calloc(allTagsCount,sizeof(tag));

	for(tmp_tag_list *item = begin; item != NULL && index < allTagsCount ;index++){
		current = item;
		tagsArray[index] = current->aTag;
		item = item->next;
		free(current);
	}

	if(index < allTagsCount){
		printf("Не удалось считать все теги. Считано %d из %d",index,allTagsCount);
		*count = index;
	}
	else{
		*count = allTagsCount;
	}

	return tagsArray;
}
