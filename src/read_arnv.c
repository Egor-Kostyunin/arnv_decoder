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

	FILE arnvFile =  open(file_path,"r");

	while(feof(arnvFile) == 0){

		uint8_t framesSetBegin[2];

		if(arnvFile == NULL){
			perror("Не удалось открыть arnv файл");
			exit(5);
		}

		if(fread(arnvFile,1,2,framesSetBegin) != 2){
			perror("Не удалось считать начало набора кадров");
			exit(6);
		}

		if(framesSetBegin[0] != 0x5B ||
		   framesSetBegin[1] > 0xFB ||
		   framesSetBegin[1] < 0x01){
			perror("Недопустимое начало набора кадров");
			exit(7);
		}

		uint8_t frameType = 0;
		uint16_t frameDataLength = 0;
		int32_t frameDataTime = 0;

		while(fread(arnvFile,1,1,&frameType) == 1){

			if(frameType != 0x01){
				puts("Пропущен кадр с неподдерживаемым типом");
				continue;
			}

			if(frameType == 0x5D) break;

			if(fread(arnvFile,2,1,&frameDataLength) != 1 ||
			   fread(arnvFile,4,1,&frameDataTime) != 1){
				perror("Не удалось считать кадр");
				exit(8);
			}

			if(begin == NULL){
				begin = (tmp_tag_list*)calloc(1,sizeof(tmp_tag_list));
				current = begin;
			}

			int tagsCount = frameDataLength/5;

			if(tagsCount % 5 == 0){
				allTagsCount += tagsCount;
			}
			else{
				perror("Длина данных тега не кратна 5");
				exit(9);
			}

			for(int i = 0; i < tagsCount; i++){

				if(current == NULL){
					perror("Не удалось выделить память под тег");
					exit(10);
				}

				current->aTeg.unixTime = frameDataTime;

				if(fread(arnvFile,1,1,&current->aTag.tagNumber) != 1){
					perror("Не удалось считать номер тега");
					exit(11);
				}

				if(fread(arnvFile,4,1,&current->aTag.tagData) != 1){
					perror("Не удалось считать значение тега");
					exit(12);
				}

				current->next = (tmp_tag_list*)calloc(1,sizeof(tmp_tag_list));
				current = current->next;
			}
			fseek(arnvFile,1,SEEK_CUR);
		}
	}

	fclose(arnvFile);

	int index = 0;

	tag *tagsArray = (tag*)calloc(allTagsCount,sizeof(tags));

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
