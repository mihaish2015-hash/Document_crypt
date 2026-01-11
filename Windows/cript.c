#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define NR 73

typedef struct {
    char val;
    bool change;
} char_info;

char *special=".,+-=*/()@;";

int det(int **mat, int n)
{
    if(n==1)
        return mat[0][0];
    int sum=0, rez;
    for(int i=0;i<n;i++){
        int **a = malloc((n-1)*sizeof(int *));
        for(int j=0;j<(n-1);j++)
            a[j] = malloc((n-1)*sizeof(int));
        for(int j=0;j<i;j++)
            for(int k=1;k<n;k++)
                a[j][k-1]=mat[j][k];
        for(int j=i+1;j<n;j++)
            for(int k=1;k<n;k++)
                a[j-1][k-1]=mat[j][k];
        rez = mat[i][0]*det(a,n-1);
        if(i%2)
            sum -= rez;
        else
            sum += rez;
        for(int j=0;j<n-1;j++)
            free(a[j]);
        free(a);
    }
    return sum;
}

void adj(int **mat, int n, int **rez)
{
    if(n==1) {
        rez[0][0]=1;
        return;
    }
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++){
            int **a = malloc((n-1)*sizeof(int*));
            for(int k=0;k<n-1;k++)
                a[k]= malloc((n-1)*sizeof(int));
            for(int k=0;k<i;k++)
                for(int l=0;l<j;l++)
                    a[k][l]=mat[k][l];
            for(int k=0;k<i;k++)
                for(int l=j+1;l<n;l++)
                    a[k][l-1]=mat[k][l];
            for(int k=i+1;k<n;k++)
                for(int l=0;l<j;l++)
                    a[k-1][l]=mat[k][l];
            for(int k=i+1;k<n;k++)
                for(int l=j+1;l<n;l++)
                    a[k-1][l-1]=mat[k][l];
            int d = det(a,n-1);
            if((i+j)%2)
                rez[j][i]=-d;
            else rez[j][i]=d;
            rez[j][i]%=NR;
            if(rez[j][i]<0)rez[j][i]+=NR;
            for(int k=0;k<n-1;k++)
                free(a[k]);
            free(a);
        }
}

int to_numbers(int ind, char *s)
{
    if(s[ind]>='0' && s[ind]<='9')
        return s[ind]-'0';
    if(s[ind]>='a' && s[ind]<='z')
        return 10+s[ind]-'a';
    if(s[ind]>='A' && s[ind]<='Z')
        return 36+s[ind]-'A';
    return (int)(strchr(special,s[ind])-special)+62;
}

char to_alphabet(int ind, int *s)
{
    if(s[ind]>=0 && s[ind]<=9)
        return '0' + s[ind];
    if(s[ind]>=10 && s[ind]<=35)
        return 'a'+s[ind]-10;
    if(s[ind]>=36 && s[ind]<=61)
        return 'A'+s[ind]-36;
    return special[s[ind]-62];
}

int invers(int x)
{
    for(int i=1;i<NR;i++){
        if((x*i)%NR==1)
            return i;
	}
	return 0;
}

int **matrix_calc(char *key, int size)
{
    int **key_mat=malloc(size*sizeof(int *));
    for(int i=0;i<size;i++)
        key_mat[i]=malloc(size*sizeof(int));
    int ind = 0;
	for(int i=0;i<size;i++)
		for(int j=0;j<size;j++){
			key_mat[i][j]=to_numbers(ind,key);
			ind++;
		}
    return key_mat;
}

void decript(int **key_mat, int key_size, int *s_mat, int s_size, int *rez, int **aux)
{
    //decrypt the word s_mat with s_size characters (multiple of key_mat)
    //and write the result in rez
    int ind = 0, d;
    d =det(key_mat,key_size)%NR;
    if(d<0)d+=NR;
    d=invers(d);
    adj(key_mat,key_size,aux);
    //we will write the inverse of key_mat in aux matrix
    for(int i=0;i<key_size;i++)
        for(int j=0;j<key_size;j++)
            aux[i][j]=(aux[i][j]*d)%NR;

    while(ind + key_size <= s_size){
        for(int i=0;i<key_size;i++){
            rez[ind+i]=0;
            for(int j=0;j<key_size;j++)
                rez[ind+i]+=aux[i][j]*s_mat[ind+j];
            rez[ind+i]%=NR;
        }
        ind += key_size;
    }
}

void cript(int **key_mat, int key_size, int *s_mat, int s_size, int *rez)
{
    //crypt the word s_mat with s_size characters (multiple of key_size)
    //and write the result in rez
    int ind = 0;
    while(ind + key_size <= s_size){
        for(int i=0;i<key_size;i++){
            rez[ind+i]=0;
            for(int j=0;j<key_size;j++)
                rez[ind+i]+=key_mat[i][j]*s_mat[ind+j];
            rez[ind+i]%=NR;
        }
        ind += key_size;
    }
}

int main(int argc, char **argv)
{
    char *all="0123456789.,+-=*/()@;qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM";
    int capacity=1000, buffer_size=0, text_size=0, size=1;
    int len=strlen(argv[3]);
    for(int i=0;i<len;i++)
        if(!strchr(all,argv[3][i])) {
            printf("Cheia contine caractere invalide !\n");
            exit(1);
        }
    while(size*size < len)
        size++;
    if(size*size > len) {
        printf("Cheia poate fi formata doar din 1, 4, 9 sau 16 caractere\n");
        exit(1);
    }
    int **key_mat=matrix_calc(argv[3], size);
    if(!(det(key_mat,size)%NR)) {
        printf("Din pacate, aceasta cheie nu poate fi folosita. Incearca alta !\n");
        exit(1);
    }
    char c;
    char_info *text=malloc(capacity*sizeof(char_info));
    char *buffer=malloc(capacity*sizeof(char));
    FILE *f=fopen(argv[1], "r");
    //read the characters from the file
    while((c = fgetc(f)) != EOF) {
        if(text_size>=capacity) {
            capacity*=2;
            buffer=realloc(buffer, capacity*sizeof(char));
            text=realloc(text,capacity*sizeof(char_info));
        }
        text[text_size].val=c;
        if(c=='&') {
            text[text_size++].change=false;
            while(c != ';') {
                c=fgetc(f);
                text[text_size].val=c;
                text[text_size++].change=false;
            }
            continue;
        }
        if(strchr(all, c)) { 
            buffer[buffer_size++]=c;
            text[text_size].change=true;
        }
        else
            text[text_size].change=false;
        text_size++;
    }

    int *rez = malloc(buffer_size*sizeof(int));
    int *s_mat = malloc(buffer_size*sizeof(int));
    for(int i=0;i<buffer_size;i++)
        s_mat[i]=to_numbers(i, buffer);
        
    int buffer_size_tr=buffer_size-buffer_size%size;
    int new_dim=buffer_size-buffer_size_tr;

    //calculate matrix for the remaining characters
    int **key_mat2=malloc(new_dim*sizeof(int *));
    for(int i=0;i<new_dim;i++)
        key_mat2[i]=malloc(new_dim*sizeof(int));
    int ind=0;
    for(int i=0;i<new_dim;i++)
        for(int j=0;j<new_dim;j++){
            key_mat2[i][j]= key_mat[ind/size][ind-(ind/size)*size];
            ind++;
    }
    if(new_dim && !(det(key_mat2,new_dim)%NR)) {
        printf("Din pacate, aceasta cheie nu poate fi folosita. Incearca alta !\n");
        exit(1);
    }
            
    if(atoi(argv[2])==1) {
        cript(key_mat, size, s_mat, buffer_size_tr, rez);
        for(int i=0;i<buffer_size_tr;i++)
            buffer[i]=to_alphabet(i, rez);

        //for remaining characters in the buffer
        for(int i=0;i<new_dim;i++)
            s_mat[i]=s_mat[i+buffer_size_tr];
        if(new_dim)cript(key_mat2, new_dim, s_mat, new_dim, rez);
        for(int i=0;i<new_dim;i++)
            buffer[i+buffer_size_tr]=to_alphabet(i, rez); 

    } else {
        int **aux= malloc(size*sizeof(int*));
        for(int i=0; i<size;i++)
            aux[i]=malloc(size*sizeof(int));

        decript(key_mat, size, s_mat, buffer_size_tr, rez, aux);
        for(int i=0;i<buffer_size_tr;i++)
            buffer[i]=to_alphabet(i, rez);

        //for the remaining characters in the buffer
        for(int i=0;i<new_dim;i++)
            s_mat[i]=s_mat[i+buffer_size_tr];
        if(new_dim)decript(key_mat2, new_dim, s_mat, new_dim, rez, aux);
        for(int i=0;i<new_dim;i++)
            buffer[i+buffer_size_tr]=to_alphabet(i, rez);

        for(int i=0;i<size;i++)
            free(aux[i]);
        free(aux);
    }
    ind=0;
    for(int i=0;i<text_size;i++)
        if(text[i].change)
            text[i].val=buffer[ind++];

    fclose(f);
    FILE *g=fopen(argv[1], "w");
    for(int i=0;i<text_size;i++)
        fprintf(g,"%c",text[i].val);
    fclose(g);
    exit(0);
}
