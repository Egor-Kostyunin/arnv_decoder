#include "arnv_decoder.h"

int main(int argc,char **argv){
	if(argc < 2) return 1;
	LoadDict();
	int count = 0;
	OutputTags(ReadARNV(argv[1],&count),count);
	return 0;
}
