#include "arnv_decoder.h"

int main(int argc,char **argv){
	if(argc < 2) return 1;
	LoadDict();
	int count = 0;
	tag* tags_array = ReadARNV(argv[1],&count);
	OutputTags(tags_array,count);
	return 0;
}
