/*
	nand-injector by Ehsan Varasteh

	Use this program to inject file into bare nand dumps, it calculates OOB
	and fills OOB.
	After .bin file generated, you can overwrite .bin file into position you provided
	in this program. Using dd or any hex editor.
	
	* The generate_ecc function is not implemented yet due to various types of ECC algorithms
	  used by various manufacturers. Only FF written to make sure no bad block/page occur.
*/

#include <stdio.h>
#include <stdlib.h>

void generate_ecc(unsigned char *data, int oobn, unsigned char *oobc) {
	// currently nothing implemented!
	int n;
	
	for(n=0;n<oobn;n++)
		oobc[n] = (unsigned char)'\xFF';
}

int main(int argn, char *argv[]) {
	if(argn < 2) {
		printf("Usage: %s <page:oob> <infile> <position> [nand-data-file]\r\n", argv[0]);
		
		return 0;
	}
	
	int n=0,pn=0;
	FILE *fi, *fo;
	unsigned char *pagec, *oobc;
	size_t nread = 0;
	long position = atoi(argv[3]), offset, page, oob;
	
	sscanf(argv[1], "%u:%u", &page, &oob);
	if(page < oob | !page | !oob) {
		printf("Wrong page:oob input! %u:%u \r\n", page, oob);
		
		return -1;
	}
	
	pagec = (unsigned char*) malloc(page);
	oobc = (unsigned char*) malloc(oob);
	
	if(position<0) {
		printf("Wrong position! %u \r\n", atoi(argv[3]));
		
		return -2;
	}
	
	fi = fopen(argv[2], "rb");
	if(fi == NULL) {
		printf("Wrong input file! %s \r\n", argv[2]);
		
		return -3;
	}
	
	sprintf((char*)pagec, "%s-%s.bin", argv[2], argv[3]);
	fo = fopen((char*)pagec, "wb");
	if(fo == NULL) {
		printf("Cannot open file! %s \r\n", pagec);
		fclose(fi);
		
		return -4;
	}
	
	printf(".$%u:%u,%s=>%s,@%s[%u]::", page, oob, argv[2], pagec, (argn>4?argv[4]:"NULL"), position);
	
	pn = (position/(page+oob));
	offset = position%(page+oob);
	fread(pagec, 1, page-offset, fi);
	fwrite(pagec, 1, page-offset, fo);
	generate_ecc(NULL, oob, oobc);
	fwrite(oobc, 1, oob, fo);
	while(!feof(fi)) {
		nread = fread(pagec, 1, page, fi);
		fwrite(pagec, 1, nread, fo);
		if(nread == page) {
			generate_ecc(pagec, oob, oobc);
			fwrite(oobc, 1, oob, fo);
		}
		
		while(--n>0) printf("\b");
		printf("%u%n", pn++, &n); n++;
	}
	
	fcloseall();
	free(pagec);
	free(oobc);
	
	printf("\r\n");
	
	return 0;
}
