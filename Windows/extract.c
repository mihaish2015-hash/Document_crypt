#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LEN 200

int main(int argc, char **argv)
{
    int content_size=MAX_LEN, ind=0;
    char *content=malloc(content_size*sizeof(char));
    char c;
    FILE *f=fopen(argv[1], "r");
    if(!f) {
        printf("Cannot read from this file !\n");
        exit(1);
    }
    FILE *g=fopen(argv[2], "w");
    while(fscanf(f,"%c",&c) != EOF) {
        if(ind>=content_size) {
            content_size*=2;
            content=realloc(content, content_size*sizeof(char));
        }
        content[ind++]=c;
    }
    const char *tag1, *tag2;
    if(strstr(argv[1],"document")){
        tag1="<w:t";
        tag2="</w:t>";
    }
    else if(strstr(argv[1],"slide")) {
        tag1="<a:t";
        tag2="</a:t>";
    }
    else {
        tag1="<t";
        tag2="</t>";
    }
    char *q, *p=strstr(content,tag1);
    int len, clen;
    while(p) {
        while(*p!='>')
            p++;
        p++;
        q=strstr(p,tag2);
        len=q-p;
        for(int i=0;i<len;i++)
            fprintf(g,"%c",*(p+i));
        clen=len;
        int ind=0;
        while(clen) {
            *(p+ind)=(clen%10)+'0';
            clen/=10;
            ind++;
        }
        while(p+ind<q){
            *(p+ind)=' ';
            ind++;
        }
        p=strstr(p,tag1);
    }
    FILE *h=fopen(argv[1],"w");
    if(!h) {
        printf("Cannot write in this file !\n");
        exit(1);
    }
    for(int i=0;i<ind;i++)
        fprintf(h,"%c",content[i]);

    fclose(f);
    fclose(g);
    fclose(h);
    free(content);
}
