#include "arnv_decoder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

tags_dict *tagsDict;
int dictSize = 0;

void LoadDict(){
	FILE *dictFile = fopen("./tags.dict","r");

	if(dictFile == NULL){
		perror("Не удалось открыть файл tags.dict");
		exit(1);
	}

	if(fscanf(dictFile,"%d\n",&dictSize) != 1){
		perror("Не удалось считать размер словаря");
		exit(2);
	}

	tagsDict = (tags_dict*)calloc(dictSize,sizeof(tags_dict));

	if(tagsDict == NULL){
		perror("Не удалось выделить память под словарь тегов");
		exit(3);
	}

	int tagNumber;
	char tagFormatString[MAX_FORMAT_STR_SIZE];
	int tagsCount = 0;

	while(fscanf(dictFile,"%d %s\n",&tagNumber,tagFormatString) == 2){
		if(tagsCount == dictSize){
			perror("Размер словаря указанный в файле tags.dict меньше фактического размера часть словаря не будет загружена");
			break;
		}
		tagsDict[tagsCount].tagNumber = tagNumber;
		strcpy(tagsDict[tagsCount].tagFormatString,tagFormatString);
		tagsCount++;
	}

	if(ferror(dictFile) != 0){
		perror("Во время чтения файла tags.dict возникла ошибка");
		exit(4);
	}

	if(feof(dictFile) != 0){
		perror("Не был достигнут конец файла tags.dict");
	}

	fclose(dictFile);
}
