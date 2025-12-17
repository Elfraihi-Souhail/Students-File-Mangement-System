#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define b 3
//------------------------------Abstrace data type------------------------------//

typedef struct TypeRecord {
    char name[20];
    int age;
    int id;
}  TypeRecord;

typedef struct TypeBlock {
    TypeRecord data[b];
    int NB;
} TypeBlock;

typedef struct TypeHeader {
    int nblck;
    int nrec;
} TypeHeader;

//------------------------------Abstract machine------------------------------//

//Header functions
int Open(FILE **f ,char *name , char *mode , TypeHeader *HEAD);
int Close(FILE *f , TypeHeader *HEAD);
int getHeader(TypeHeader HEAD, int i);
int setHeader(TypeHeader *HEAD, int i, int val);
int readBlock(FILE *f, int i, TypeBlock *Buf);
int writeBlock(FILE *f, int i, TypeBlock *Buf);
int loadFile(FILE *f, TypeHeader *HEAD);
int Display(FILE *f, TypeHeader HEAD);

//Record functions
void fillRecord(TypeRecord *R);
void displayRecord(TypeRecord R);

//Implementation functions
int Open(FILE **f ,char *name , char *mode , TypeHeader *HEAD){
    FILE *g;
    switch (*mode)
    {
    case 'E':
        *f = fopen(name, "rb+");
        if (*f == NULL) return -1;
        g = *f;
        rewind(*f);
        fread(HEAD, sizeof(TypeHeader), 1, *f);
        *f = g;
        break;
    case 'N':
        *f = fopen(name, "wb+");
        if (*f == NULL) return -1;
        HEAD->nblck = 1;
        HEAD->nrec = 0;
        fwrite(HEAD, sizeof(TypeHeader), 1, *f);
        break;

    default:
        return -1;
        break;
    }
    return 0;
}
int Close (FILE *f , TypeHeader *HEAD){
    rewind(f);
    fwrite(HEAD, sizeof(TypeHeader), 1, f);
    fclose(f);
    return 0;
}
int getHeader(TypeHeader HEAD, int i) {
    switch (i)
    {
    case  1:
        return HEAD.nblck;
        break;
    case 2:
        return HEAD.nrec;
        break;
    default:
        return -1;
        break;
    }
}
int setHeader(TypeHeader *HEAD, int i, int val){
    switch (i)
    {
    case 1:
        HEAD->nblck = val;
        break;
    case 2:
        HEAD->nrec = val;
        break;
    default:
        return -1;
        break;
    }
    return 0;
}
int readBlock(FILE *f, int i, TypeBlock *Buf) {
    fseek(f , sizeof(TypeHeader) + (i - 1) * sizeof(TypeBlock) , SEEK_SET);
    fread(Buf , sizeof(TypeBlock) , 1 ,f);
    return Buf ? 0 : 1;
}
int writeBlock(FILE *f, int i, TypeBlock *Buf) {
    fseek(f , sizeof(TypeHeader) + (i - 1) * sizeof(TypeBlock) , SEEK_SET);
    fwrite(Buf , sizeof(TypeBlock) , 1 ,f);
    return Buf ? 0 : 1;
}



int loadFile(FILE *f,TypeHeader *HEAD){
    if(f == NULL) return 1;
    TypeBlock buff;
    int lastID = 0;
    int j = 0 , k = 0 , num; //num stands for the number of records , k stands for the number of blocks
    printf("Enter the number of records : ");
    scanf("%d" , &num);
    fseek(f , sizeof(TypeHeader) , SEEK_SET);
    for (int i = 1; i <= num; i++) {
        if (j < 0.7*b)
        {
            printf("----------------PERSON %d-----------------\n" , i);
            printf("Enter the id of person %d : ", i);
            scanf("%d" , &buff.data[j].id);
            if (j != 0 || k != 0 ) {
                while(buff.data[j].id <= lastID){
                    printf("The file must be ordered by id (must be a positive integer)\n");
                    printf("Enter the id of person %d : ", i);
                    scanf("%d" , &buff.data[j].id);
                }
            };
            lastID = buff.data[j].id;
            printf("Enter the name of person %d : " , i);
            scanf("%s" , buff.data[j].name);
            printf("Enter the age of person %d : " , i);
            scanf("%d" , &buff.data[j].age);  
            printf("-----------------------------------------\n");  
            j++;     
        }
        else {
            buff.NB = j ;
            k++;
            writeBlock(f , k , &buff);
            j = 0;
        };    
    }
    buff.NB = j;
    k++;
    writeBlock(f , k , &buff);
    setHeader(HEAD , 1 , k);
    setHeader(HEAD , 2 , num);
    return 0;
}
int Display(FILE *f, TypeHeader HEAD) {
    if(f == NULL) return 1;
    TypeBlock buff;
    int k = 1 , i = 1 , j = 0;
    while (!feof (f))
    {   
        readBlock(f , k ,&buff);
        printf("----------------PERSON %d-----------------\n" , i);
        printf("ID : %d\n", buff.data[j].id);
        printf("NAME : %s\n" , buff.data[j].name);
        printf("AGE : %d\n" , buff.data[j].age);
        printf("-----------------------------------------\n");  
        j > buff.NB ? j = 0 : j++; 
        k++;
        i++;
    }
    return 0;
}

int main() {
    FILE *f;
    TypeHeader HEAD;

    Open(&f, "students.txt", "E", &HEAD);
    Display(f , HEAD);
    Close(f, &HEAD);
    return 0; 
}