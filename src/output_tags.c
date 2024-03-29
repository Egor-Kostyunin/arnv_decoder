#include "arnv_decoder.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

extern tags_dict *tagsDict;
extern int dictSize;

void OutputTags(tag *tags_array, int tags_count){
	for(int i = 0; i < tags_count; i++){
		char buffer[MAX_FORMAT_STR_SIZE];
		char tagFound = 0;

		//Поиск строки формата
		for(int j = 0; j < dictSize; j++){
			if(tagsDict[j].tagNumber == tags_array[i].tagNumber){
				strcpy(buffer,tagsDict[j].tagFormatString);
				tagFound = 1;
				break;
			}
		}

		if(tagFound == 0){
			puts("Пропущен не распознанный тег");
			printf("Номер тега: %d\n",tags_array[i].tagNumber);
			continue;
		}

		//Разбор строки формата
		int bitsNotRead = 32;
		for(char *varName = strtok(buffer,"%"); varName != NULL; varName = strtok(NULL,"%")){
			char *varType = strtok(NULL,"|");
			int varSize = 32;
			char isInteger = 1;
			int varTLen = strlen(varType);

			for(int i = 0; i < varTLen;i++){
				if(varType[i] == 'i'){
					isInteger = 1;
					break;
				}
				if(varType[i] == 'f'){
					isInteger = 0;
					break;
				}
				if(varType[i] == '\0'){
					puts("Не удалось определить тип данных");
					exit(13);
				}
				if(varType[i] == 'h') varSize /= 2;
				if(varType[i] == '1') varSize -= 1;
			}

			bitsNotRead -= varSize;
			uint32_t bitMask = 0;

			for(int k = 0; k < varSize - 1; k++){
				bitMask |= 1;
				bitMask <<= 1;			
			}
			bitMask |= 1;

			int32_t varValue = (tags_array[i].tagData >> bitsNotRead) & bitMask;
			time_t tagTime = tags_array[i].unixTime;
			struct tm *dt = localtime(&tagTime);\

			if(dt == NULL){
				puts("Не удалось определить время");
				exit(13);
			}
			
			//Вывод тега
			char base_str[2*MAX_FORMAT_STR_SIZE] = "[%d.%d.%d-%d:%d:%d] %s: ";
			if(isInteger){
				strcat(base_str,"%d\n");
				printf(base_str,dt->tm_mday,
					dt->tm_mon + 1,
					dt->tm_year + 1900,
					dt->tm_hour,
					dt->tm_min,
					dt->tm_sec,
					varName,
					varValue);
			}
			else{
				strcat(base_str,"%f\n");
				float *tmpFltPtr = (float*) &varValue;
				float varFValue = *tmpFltPtr;
				
				printf(base_str,dt->tm_mday,
					dt->tm_mon + 1,
					dt->tm_year + 1900,
					dt->tm_hour,
					dt->tm_min,
					dt->tm_sec,
					varName,
					varFValue);
			}
			

		}
	}
	puts("Конец вывода");
}
