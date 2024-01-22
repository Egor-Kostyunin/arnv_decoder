#include "arnv_decoder.h"
#include <stdio.h>

int main(int argc,char **argv){
	if(argc < 2){
		puts("Использование ./arnv_decoder путь_до_arnv_файла");
		return 1;
	};
	LoadDict();
	int count = 0;
	tag* tags_array = ReadARNV(argv[1],&count);
	OutputTags(tags_array,count);
	return 0;
}
