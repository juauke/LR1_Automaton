#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#include "automaton_uichar.h"
#include "automaton_file.h"

void printCharacter(uichar_t c) {
	char *ns;
	uichar_t sc[2];


//	printf("prtchr=0x%08x",c);
	if ( ((c & 0xF0000000)==0xF0000000) ||
		((c & 0x00E00000)==0x00E00000) ||
		((c & 0x0000C000)==0x0000C000) ) {
			sc[0]=c;
			sc[1]='\0';
			if ((ns=uictoc(sc)) == NULL) return;
			printf("%s",ns);
			free(ns);
	} else {
       		if (c == ' ') printf("' '"); 
		else if (c == ',') printf("','"); 
		else if (c == '\n') printf("'\\n'"); 
		else if (isprint(c)) printf("%c", (char)c);
		else printf("'0x%x'",(char)c); 
	}
}

uichar_t *uic_strcpy(uichar_t *dst, uichar_t *src) {
	uichar_t *p,*p1;
	for (p=src,p1=dst; *p; p++, p1++) *p1=*p ;
	*p1='\0';
//	for (p=src;*p;p++) {
//		printf("src[%ld]c=%08x\n", (p-src), *p);
//	}
//	for (p=dst;*p;p++) {
//		printf("dst[%ld]c=%08x\n", (p-dst), *p);
//	}
	return(dst) ;
}

long int uic_strlen(uichar_t *s) {
	uichar_t *p;
	long int l=0;
	for (p=s; *p; ++p) l++ ;
	return(l) ;
}

uichar_t *ctouic(char *s) {
	char *p;
	uichar_t *ns,*pns;

//	printf("============ in c to uic (strlens)=%d\n", strlen(s));
//	printf("IN s=%s (strlen=%ld,sizeof=%ld)\n",s, strlen(s),sizeof(s));
//	for (p=s;*p;p++) {
//		printf("c=%02x\n", *p);
//	}

	if ((ns=calloc(200,sizeof(uichar_t)))==NULL) return NULL;

	for (p=s,pns=ns;*p;p++,pns++) {
		if ((*p & 0x80)==0) {
//			printf("s=1\n");
			*pns=*p;
		} else if ((*p & 0xF0)==0xF0) {
//			printf("s=4\n");
			*pns=((unsigned char)*p++)<<24;
			*pns+=((unsigned char)*p++)<<16;
			*pns+=((unsigned char)*p++)<<8;
			*pns+=(unsigned char)*p;
		} else if ((*p & 0xE0)==0xE0) {
//			printf("s=3\n");
			*pns=((unsigned char)*p++)<<16;
			*pns+=((unsigned char)*p++)<<8;
			*pns+=(unsigned char)*p;
		} else if ((*p & 0xC0)==0xC0) {
//			printf("s=2\n");
			*pns=(unsigned char)(*p++)<<8;
			*pns+=(unsigned char)*p;
		}
	}
	*pns='\0';
	
//	printf("OUT  (lstrlen=%ld,sizeof=%ld)\n",uic_strlen(ns),sizeof(ns));
//	for (pns=ns;*pns;pns++) {
//		printf("c=%08x\n", *pns);
//	}
	return(ns);
}

char *uictoc(uichar_t *s) {
	uichar_t *p;
	char *ns, *pns;

//	printf("======== in uic to c calloc(%ld)\n",uic_strlen(s));
//	return(NULL);
	if((ns=calloc(1,(uic_strlen(s)*sizeof(uichar_t))+1)) == NULL) return(NULL);
//	printf("======== after calloc\n");

	for (p=s,pns=ns;*p;p++,pns++) {
		if ((*p & 0xF0000000)==0xF0000000) {
//			printf("s=4\n");
			*(pns++) =(*p&0xFF000000)>>24;
			*(pns++) =(*p&0x00FF0000)>>16;
			*(pns++) =(*p&0x0000FF00)>>8;
			*pns     =(*p&0x000000FF);
		} else if ((*p & 0x00E00000)==0x00E00000) {
//			printf("s=3\n");
			*(pns++) =(*p&0x00FF0000)>>16;
			*(pns++) =(*p&0x0000FF00)>>8;
			*pns    =(*p&0x000000FF);
		} else if ((*p & 0x0000C000)==0x0000C000) {
//			printf("s=2\n");
			*(pns++) =(*p&0x0000FF00)>>8;
			*pns     =(*p&0x000000FF);
		} else if ((*p & 0x00000080)==0) {
//			printf("s=1\n");
			*pns     =(*p&0x000000FF);
		}
	}
	*pns='\0';
	
//	printf("IN  (lstrlen=%ld,sizeof=%ld)\n",uic_strlen(s),sizeof(s));
//	for (p=s;*p;p++) {
//		printf("c=%08x\n", *p);
//	}
//	printf("s=%s (strlen=%ld,sizeof=%ld)\n",ns, strlen(ns),sizeof(ns));
//	for (pns=ns;*pns;pns++) {
//		printf("c=%02x\n", *pns);
//	}
	return(ns);
}

