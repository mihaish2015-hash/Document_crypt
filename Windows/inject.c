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
    FILE *g=fopen(argv[2], "r");
    while(fscanf(g,"%c",&c) != EOF) {
        if(ind>=content_size) {
            content_size*=2;
            content=realloc(content, content_size*sizeof(char));
        }
        content[ind++]=c;
    }
    const char *tag1;
    if(strstr(argv[2],"document"))
        tag1="<w:t";
    else if(strstr(argv[2],"slide"))
        tag1="<a:t";
    else
        tag1="<t";
    char *p=strstr(content,tag1);
    int len, clen;
    while(p) {
        while(*p!='>')
            p++;
        p++;
        int nr=0, ind=0, po=1;
        while(*(p+ind)!=' ' && *(p+ind)!='<'){
            nr+=po*(*(p+ind)-'0');
            po*=10;
            ind++;
        }
        for(int i=0;i<nr;i++)
            fscanf(f,"%c",p+i);
        p=strstr(p,tag1);
    }
    FILE *h=fopen(argv[2],"w");
    for(int i=0;i<ind;i++)
        fprintf(h,"%c",content[i]);
    fclose(f);
    fclose(g);
    fclose(h); 
    free(content);
}
