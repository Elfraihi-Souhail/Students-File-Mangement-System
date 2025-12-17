#include "library.h"

//---------------------ABSTRACT MACHINE:---------------------//
int Open(Lnof **file ,char *name , char *mode){
    FILE *g;
    switch (*mode)
    {
    case 'E':
        (*file)->f = fopen(name, "rb+");
        if ((*file)->f == NULL) return -1;
        g = (*file)->f;
        rewind((*file)->f);
        fread(&((*file)->HEADER), sizeof(header), 1, (*file)->f);
        (*file)->f = g;
        break;
    case 'N':
        (*file)->f = fopen(name, "wb+");
        if ((*file)->f == NULL) return -1;
        (*file)->HEADER.head = 1;
        fwrite(&((*file)->HEADER), sizeof(header), 1, (*file)->f);
        break;
    default:
        return -1;
        break;
    }
    return 0;
}
int Close(Lnof *file){
    rewind(file->f);
    fwrite(&(file->HEADER), sizeof(header), 1, file->f);
    fclose(file->f);
    return 0;
}
int getHeader(Lnof file, int i){
    switch (i)
    {
    case  1:
        return file.HEADER.head;
        break;
    case 2:
        return file.HEADER.current;
        break;
    default:
        return -1;
        break;
    }
}
int setHeader(Lnof *file, int i, int val){
    switch (i)
    {
    case 1:
        file->HEADER.head = val;
        break;
    case 2:
        file->HEADER.current = val;
        break;
    default:
        return -1;
        break;
    }
    return 0;
}
int readBlock(Lnof file, int i, block_LnOF *Buf){
    fseek(file.f , sizeof(header) + (i - 1) * sizeof(block_LnOF) , SEEK_SET);
    fread(Buf , sizeof(block_LnOF) , 1 ,file.f);
    return Buf ? 0 : 1;
}
int writeBlock(Lnof *file, int i, block_LnOF *Buf){
    fseek(file->f , sizeof(header) + (i - 1) * sizeof(block_LnOF) , SEEK_SET);
    fwrite(Buf , sizeof(block_LnOF) , 1 ,file->f);
    return Buf ? 0 : 1;
}
int AllocBlock(Lnof *file) {
    if (file->f == NULL) return -1;
    int next = getHeader(*file , 2);
    setHeader(file , 2 , next + 1);
    return next;
}
//-----------------------------------------------------------//

//------------------------RANDOMIZER-------------------------//
int random_value(int mode){
    switch (mode)
    {
    case 1:
        return MIN_STUDENT_ID + rand() % (MAX_STUDENT_ID - MIN_STUDENT_ID + 1);
        break;
    case 2:
        return MIN_NAME_LENGTH + rand() % (MAX_NAME_LENGTH - MIN_NAME_LENGTH + 1);
        break;
    case 3:
        return MIN_BIRTH_YEAR + rand() % (MAX_BIRTH_YEAR - MIN_BIRTH_YEAR + 1);
        break;
    case 4:
        return MIN_WILAYA + rand() % (MAX_WILAYA - MIN_WILAYA + 1);
        break;
    case 5:
        return 1 + rand() % (2);
        break;
    case 6:
        return 1 + rand() % (8);
        break;
    case 7:
        return MIN_YEAR_STUDY + rand() % (MAX_YEAR_STUDY- MIN_YEAR_STUDY + 1);
        break;
    case 8:
        return 1 + rand() % (4);
        break;
    case 9:
        return rand() % (2);
        break;
    default:
        break;
    }
    return -1;
}
//-----------------------------------------------------------//

int main() {
    return 0; 
}