#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#include "automaton.h"
#include "automaton_uichar.h"
#include "automaton_file.h"

void _printCharacter(uichar_t c, boolean_t dotFormat) {
	char* ns;
	uichar_t sc[2];

	if ( ((c & 0xF0000000)==0xF0000000) ||
		((c & 0x00E00000)==0x00E00000) ||
		((c & 0x0000C000)==0x0000C000) ) {
// display Unicode character as a char string
			sc[0]=c; sc[1]='\0';

			if ((ns=uictoc(sc)) == NULL) return;

			printf("%s",ns);
			free(ns);
	}
	else {
// display unibyte character
		if (c == ' ' && dotFormat) printf("' '"); 
		else if (c == ',' && dotFormat) printf("','"); 
		else if (c == '\n')
			if (dotFormat) printf("'\\\\n'");
			else printf("\\n");
		else if (isprint(c)) printf("%c", (char) c);
		else printf("'0x%x'", (char) c); 
	}
}

uichar_t *uic_strcpy(uichar_t *dst, uichar_t *src) {
	uichar_t *p,*p1;
	for (p=src,p1=dst; *p; p++, p1++) *p1=*p;
	*p1='\0';

	return dst;
}

long int uic_strlen(uichar_t *s) {
	uichar_t *p;
	long int ln=0;
	for (p=s; *p; ++p) ln++;

	return ln;
}

uichar_t *ctouic(char *s) {
	char *p;
	uichar_t *ns,*pns;

	if ((ns=calloc(200,sizeof(uichar_t)))==NULL) {perror("calloc-ctouic"); return NULL;}

	for (p=s,pns=ns;*p;p++,pns++) {
		if ((*p & 0x80)==0) {	// unibyte char
			*pns=*p;
		} else if ((*p & 0xF0)==0xF0) {	// 4-bytes characters
			*pns=((unsigned char) *p++)<<24;
			*pns+=((unsigned char) *p++)<<16;
			*pns+=((unsigned char) *p++)<<8;
			*pns+=(unsigned char) *p;
		} else if ((*p & 0xE0)==0xE0) {	// 3-bytes characters
			*pns=((unsigned char) *p++)<<16;
			*pns+=((unsigned char) *p++)<<8;
			*pns+=(unsigned char) *p;
		} else if ((*p & 0xC0)==0xC0) {	// 2-bytes characters
			*pns=(unsigned char) (*p++)<<8;
			*pns+=(unsigned char) *p;
		}
	}
	*pns='\0';
	
	return ns;
}

char* uictoc(uichar_t *s) {
	uichar_t *p;
	char *ns, *pns;

	if((ns=calloc(1,(uic_strlen(s)*sizeof(uichar_t))+1)) == NULL) {perror("calloc-uictoc"); return NULL;}

	for (p=s, pns=ns; *p; p++, pns++) {
		if ((*p & 0xF0000000)==0xF0000000) {	// 4-bytes characters
			*(pns++) =(*p&0xFF000000)>>24;
			*(pns++) =(*p&0x00FF0000)>>16;
			*(pns++) =(*p&0x0000FF00)>>8;
			*pns     =(*p&0x000000FF);
		} else if ((*p & 0x00E00000)==0x00E00000) {	// 3-bytes characters
			*(pns++) =(*p&0x00FF0000)>>16;
			*(pns++) =(*p&0x0000FF00)>>8;
			*pns    =(*p&0x000000FF);
		} else if ((*p & 0x0000C000)==0x0000C000) {	// 2-bytes characters
			*(pns++) =(*p&0x0000FF00)>>8;
			*pns     =(*p&0x000000FF);
		} else if ((*p & 0x00000080)==0) 	// unibyte char
			*pns     =(*p&0x000000FF);
	}
	*pns='\0';
	
	return ns;
}

