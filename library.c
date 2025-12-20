#include "library.h"

//---------------------ABSTRACT MACHINE TOF:---------------------//
int Open_TOF(Tof **file ,char *name , char *mode){
    FILE *g;
    switch (*mode)
    {
    case 'E':
        (*file)->f = fopen(name, "rb+");
        if ((*file)->f == NULL) return -1;
        g = (*file)->f;
        rewind((*file)->f);
        fread(&((*file)->header), sizeof(header_Tof), 1, (*file)->f);
        (*file)->f = g;
        break;
    case 'N':
        (*file)->f = fopen(name, "wb+");
        if ((*file)->f == NULL) return -1;
        (*file)->header.nblc = 0;
        (*file)->header.nrec = 0;
        (*file)->header.ndel = 0;        
        rewind((*file)->f);
        fwrite(&((*file)->header), sizeof(header_Tof), 1, (*file)->f);
        break;
    default:
        return -1;
        break;
    }
    return 0;
}
int Close_TOF(Tof *file){
    rewind(file->f);
    fwrite(&(file->header), sizeof(header_Tof), 1, file->f);
    fclose(file->f);
    return 0;
}
int getHeader_TOF(Tof file, int i){
    switch (i)
    {
    case  1:
        return file.header.nblc;
        break;
    case 2:
        return file.header.nrec;
        break;
    default:
        return -1;
        break;
    }
}
int setHeader_TOF(Tof *file, int i, int val){
    switch (i)
    {
    case 1:
        file->header.nblc = val;
        break;
    case 2:
        file->header.nrec = val;
        break;
    default:
        return -1;
        break;
    }
    return 0;
}
int readBlock_TOF(Tof file, int i, block_TOF *Buf){
    fseek(file.f , sizeof(header_Tof) + (i - 1) * sizeof(block_TOF) , SEEK_SET);
    fread(Buf , sizeof(block_TOF) , 1 ,file.f);
    return Buf ? 0 : 1;
}
int writeBlock_TOF(Tof *file, int i, block_TOF *Buf){
    fseek(file->f , sizeof(header_Tof) + (i - 1) * sizeof(block_TOF) , SEEK_SET);
    fwrite(Buf , sizeof(block_TOF) , 1 ,file->f);
    return Buf ? 0 : 1;
}
//----------------------------------------------------------------//

//---------------------ABSTRACT MACHINE LNOF:---------------------//
int Open_Lnof(Lnof **file ,char *name , char *mode){
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
        (*file)->HEADER.current = 1;
        fwrite(&((*file)->HEADER), sizeof(header), 1, (*file)->f);
        break;
    default:
        return -1;
        break;
    }
    return 0;
}
int Close_Lnof(Lnof *file){
    rewind(file->f);
    fwrite(&(file->HEADER), sizeof(header), 1, file->f);
    fclose(file->f);
    return 0;
}
int getHeader_Lnof(Lnof file, int i){
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
int setHeader_Lnof(Lnof *file, int i, int val){
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
int readBlock_Lnof(Lnof file, int i, block_LnOF *Buf){
    fseek(file.f , sizeof(header) + (i - 1) * sizeof(block_LnOF) , SEEK_SET);
    fread(Buf , sizeof(block_LnOF) , 1 ,file.f);
    return Buf ? 0 : 1;
}
int writeBlock_Lnof(Lnof *file, int i, block_LnOF *Buf){
    fseek(file->f , sizeof(header) + (i - 1) * sizeof(block_LnOF) , SEEK_SET);
    fwrite(Buf , sizeof(block_LnOF) , 1 ,file->f);
    return Buf ? 0 : 1;
}
int AllocBlock_Lnof(Lnof *file) {
    if (file->f == NULL) return -1;
    int next = getHeader_Lnof(*file , 2);
    setHeader_Lnof(file , 2 , next + 1);
    return next + 1;
}
//-----------------------------------------------------------//

//------------------------RANDOMIZER-------------------------//
int random_value(int mode){
    switch (mode) {
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

//-------------------Date utility functions:-----------------//
bool is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
int days_in_month(int month, int year) {
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && is_leap_year(year)) {
        return 29;
    }
    return days[month - 1];
}
void generate_random_date(Date *date) {
    // Generate year between 2003 and 2008
    date->year = random_value(3);
    
    // Generate month between 1 and 12
    date->month = 1 + rand() % 12;
    
    // Generate day based on the month and year (respecting leap years)
    int max_days = days_in_month(date->month, date->year);
    date->day = 1 + rand() % max_days;
}
//-----------------------------------------------------------//

//-------------------File related functions:-----------------//
int table_init(Index *table , int capacity){
    table->arr = malloc(capacity * sizeof(cell));
    table->size = 0;
    table->max_capacity = capacity;
    return 0;
}
void ensure_capacity(Index *table) {
    if (table->size >= table->max_capacity) {
        int new_capacity = (table->max_capacity == 0) ? 4 : table->max_capacity * 2;
        cell *new_arr = realloc(table->arr, new_capacity * sizeof(cell));
        if (!new_arr) {
            // handle allocation error
            printf("Index table realloc failed\n");
            return;
        }
        table->arr = new_arr;
        table->max_capacity = new_capacity;
    }
}
int insert_index(Index *table , int id , int block , int deplacement) {
    ensure_capacity(table); 
    
    // Find the correct position to insert (keeping array sorted)
    if(table->size == 0){
        table->arr[0].key = id;
        table->arr[0].blck_num = block;
        table->arr[0].offset = deplacement;
        table->size++;
        return 0;
    }
    int i = table->size - 1;
    while (i >= 0 && id < table->arr[i].key) {
        table->arr[i + 1] = table->arr[i];
        i--;
    }
    
    // Insert the new element at position i+1
    table->arr[i + 1].key = id;
    table->arr[i + 1].blck_num = block;
    table->arr[i + 1].offset = deplacement;
    
    // Increment the effective size
    table->size++;
    
    return 0;
}
int index_search(Index table , int key){
    int left = 0;
    int right = table.size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (table.arr[mid].key == key)
            return mid;
        else if (table.arr[mid].key < key)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1;
}
int create_record(Student *record , Index table) {
    // Student Id generator
    int id;
    do {
        id = random_value(1);
    } while (index_search(table , id) != -1);
    record->Student_id = id;

    // Family name generator
    int length =  random_value(2);
    char fam_name[length + 1];
    fam_name[0] = 'A' + rand() % 26; // First char of the name is BIG
    for(int i = 1 ; i < length ; i++){
        fam_name[i] = 'a' + rand() % 26; // The rest chars are SMALL
    }
    fam_name[length] = '\0';
    strcpy(record->family_name, fam_name);

    // First name generator
    length =  random_value(2);
    char name[length + 1];
    name[0] = 'A' + rand() % 26; // First char of the name is BIG
    for(int i = 1 ; i < length ; i++){
        name[i] = 'a' + rand() % 26; // The rest chars are SMALL
    }
    name[length] = '\0';
    strcpy(record->first_name, name);

    // Birth date generator
    generate_random_date(&(record->date_birth));

    int index;
    // Wilaya of birth generator
    index = random_value(4);
    strcpy(record->wilaya_birth , wilayat[index - 1]);

    // Gender generator
    index = random_value(5);
    record->gender = (index == 1) ? 'M' : 'F';

    // Blood type generator
    index = random_value(6);
    strcpy(record->blood_type , blood_types[index - 1]);

    // Year of study
    int year_index = random_value(7);
    strcpy(record->year_study , year_of_study[year_index - 1]);

    // Speciality generator
    if(year_index <= 2){
        strcpy(record->speciality , "Integrated Preparatory Classes");
    } else if (year_index == 3){
        strcpy(record->speciality , "Common Core");
    } else {
        index = random_value(8); 
        strcpy(record->speciality , specialities[index - 1]);
    }
    
    // Resident or not generator 
    record->resident_uc = random_value(9);
    return 0;
}

int create_Lnof(Lnof *file , Index *table) {

    // Error handling 
    if(Open_Lnof(&file , "Student_ESI.bin" , "N") != 0){
        printf("The file can not get opened.");
        return -1;
    };

    // Reading the maximum number of records
    int N , C2 = 0;
    printf("Enter the number of records to create : \n");
    scanf("%d", &N);

    // Initialization of the index table (memory allocation)
    table_init(table , N);

    // Genrerate and write the records to the file
    int offset = 0 , blk_num = 1;
    block_LnOF buff;

    for(int i = 1 ; i <= N ; i++){
        Student record;
        create_record(&record, *table);
        insert_index(table , record.Student_id , blk_num, offset);
        buff.tab[offset] = record;
        offset++; 
        if(offset == 40 ) {
            buff.link = (i == N) ? -1 : AllocBlock_Lnof(file);
            buff.NB = offset;
            writeBlock_Lnof(file , blk_num , &buff);
            offset = 0;
            C2++;
            blk_num++;
        }
    } 
    // Writing the last block if needed
    if (offset > 0) {
        buff.link = -1 ;
        buff.NB = offset;
        writeBlock_Lnof(file , blk_num , &buff);
        C2++;
    }
    // Setting the last block number (in case we need to insert a new record at the end or delete a record and use the last record)
    setHeader_Lnof(file , 2 , blk_num);
    printf("total number of blocks : %d" , blk_num);
    Close_Lnof(file);

    return C2;
}
int create_TOF(Tof *file , Index table ){

    // Error handling 
    if (table.size == 0)
    {
       printf("The index table is empty , Create the main file first");
       return -1;
    }
    
    if(Open_TOF(&file , "Student_INDEX.idx" , "N") != 0){
        printf("The file cannot be opened.");
        return -1;
    };

    // Bulk load of TOF using index table
    int C31 = 0;
    block_TOF buff;
    int i = 0 , j = 0 , k = 0;
    while (k < table.size)
    {
        if (j > 0.7*b)
        {
            buff.NB = j;
            writeBlock_TOF(file , i , &buff);
            C31++;
            buff.tab[0] = table.arr[k];
            buff.deleted[0] = false;
            j = 1;
            i++;
        } else {
            buff.tab[j] = table.arr[k];
            buff.deleted[j] = false;
            j++;
        }
        k++;
    }
    if (j > 1)
    {
        buff.NB = j;
        writeBlock_TOF(file , i , &buff);
        C31++;
        i++;
    }
    setHeader_TOF(file , 1 , i);
    setHeader_TOF(file , 2 , k);
    setHeader_TOF(file , 3 , 0);
    Close_TOF(file);
    return C31;
    
}
int Load_index (Index *table , char *filename){
    Tof *file = malloc(sizeof(Tof));
    // Error handling
    if (file == NULL) {
        printf("Memory allocation failed\n");
        return -1;
    }
    if (Open_TOF(&file , filename , "E") != 0) {
        printf("Cannot open file %s\n", filename);
        free(file);
        return -1;
    }

    block_TOF buf;
    int total_records = getHeader_TOF(*file , 2);
    table_init(table , total_records);
    int nblc = getHeader_TOF(*file , 1);
    for(int i =1 ; i <= nblc ; i++){
        readBlock_TOF(*file, i, &buf);
        for (unsigned int j = 0; j < buf.NB; j++) {
            insert_index(table , buf.tab[j].key , buf.tab[j].blck_num , buf.tab[j].offset);
        }
    }
    Close_TOF(file);
    free(file);
    return 0;
}
int insert_student(Lnof *file , Index *table){
    // Error handling 
    if(Open_Lnof(&file , "Student_ESI.bin" , "E") != 0){
        printf("The file cannot get opened.");
        return -1;
    };

    int C34 = 0 , current_blk = getHeader_Lnof(*file , 2), offset = 0;
    Student record;
    create_record(&record, *table);
    block_LnOF buff;
    readBlock_Lnof(*file , current_blk , &buff);
    C34++;
    if(buff.NB == 40){
        int new_blk = AllocBlock_Lnof(file);
        buff.link = new_blk;
        writeBlock_Lnof(file , current_blk , &buff);
        C34++;


        block_LnOF new_buff;
        new_buff.tab[0] = record;
        new_buff.NB = 1;
        new_buff.link = -1;

        writeBlock_Lnof(file , new_blk , &new_buff);
        C34++;
    } else {
        offset = buff.NB;        
        buff.tab[offset] = record;
        buff.NB++;
        writeBlock_Lnof(file , current_blk , &buff);
        C34++;
    }
    insert_index(table , record.Student_id , current_blk , offset);
    return C34;
}

int delete_student(Lnof *file , Index *table , int id) {
    // Error handling   
    if(Open_Lnof(&file , "Student_ESI.bin" , "E") != 0){
        printf("The file cannot get opened.");
        return -1;
    };
    int C35 = 0;
    int i = index_search(*table , id);
    if(i == -1) return -1;
    int last_blk = getHeader_Lnof(*file , 2);
    block_LnOF buff;
    Student record;
    readBlock_Lnof(*file , last_blk , &buff);
    C35++;
    buff.NB--;
    if(table->arr[i].blck_num == last_blk && buff.NB == 0) { // if the deleted record is in last block and it 's on its own
        readBlock_Lnof(*file , last_blk-- , &buff);
        C35++;
        buff.link = -1;
        writeBlock_Lnof(file , last_blk , &buff);
        C35++;
        setHeader_Lnof(file , 2 , last_blk);
    } else { // else we move the last record in the place of the deleted record
        record = buff.tab[buff.NB];
        readBlock_Lnof(*file , table->arr[i].blck_num , &buff);
        C35++;
        buff.tab[table->arr[i].offset] = record;
        writeBlock_Lnof(file , last_blk , &buff);
        C35++;
    }
    for (int j = i; i < table->size; j++) // Shifting in the index table
    {
        table->arr[j] = table->arr[j+1];
    }
    Close_Lnof(file);
    return C35;  
}

//-----------------------------------------------------------//

//---------------------DEBUG FUNCTION:--------------------//
void display_student(Student s) {
    printf("\n========== STUDENT RECORD ==========\n");
    printf("Student ID    : %d\n", s.Student_id);
    printf("Family Name   : %s\n", s.family_name);
    printf("First Name    : %s\n", s.first_name);
    printf("Birth Date    : %04d-%02d-%02d\n", s.date_birth.year, s.date_birth.month, s.date_birth.day);
    printf("Wilaya Birth  : %s\n", s.wilaya_birth);
    printf("Gender        : %c\n", s.gender);
    printf("Blood Type    : %s\n", s.blood_type);
    printf("Year Study    : %s\n", s.year_study);
    printf("Speciality    : %s\n", s.speciality);
    printf("Resident UC   : %s\n", s.resident_uc ? "Yes" : "No");
    printf("====================================\n\n");
}
void display_Lnof_file(char *filename) {
    Lnof *file = malloc(sizeof(Lnof));
    if (file == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    if (Open_Lnof(&file , "Student_ESI.bin" , "E") != 0) {
        printf("Cannot open file %s\n", filename);
        free(file);
        return;
    }
    block_LnOF buf;
    int i = 1;
    int nb_records = 0 , nb_blk = 0;
    printf("File: %s\n", filename);

    while (i != -1) {
        readBlock_Lnof(*file, i, &buf);
        printf("BUFF.NB = %d\n" , buf.NB);
        nb_blk++;
        for (unsigned int j = 0; j < buf.NB; j++) {
            nb_records++;
            printf("student record number : %d" , j+1);
            display_student(buf.tab[j]);
        }
        i = buf.link;
        printf("-----------------------------------------------------------------------------------------\n");
    }
    printf("number of blocks : %d\n number of records : %d\n", nb_blk , nb_records);

    Close_Lnof(file);
    free(file);
}
void display_TOF_file(char *filename) {
    Tof *tof_file = malloc(sizeof(Tof));
    if (tof_file == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    if (Open_TOF(&tof_file , filename , "E") != 0) {
        printf("Cannot open file %s\n", filename);
        free(tof_file);
        return;
    }

  
    block_TOF buf;
    int i = 1;
    int nb_records = 0 , nb_blk = 0;

    while (i <= getHeader_TOF(*tof_file , 1)) {
        printf("Block number : %d\n", i);
        readBlock_TOF(*tof_file, i, &buf);
        nb_blk++;
        for (unsigned int j = 0; j < buf.NB; j++) {
            nb_records++;
            printf("Key =  %d , Block : %d , Offset: %d\n", buf.tab[j].key, buf.tab[j].blck_num, buf.tab[j].offset);
        }
        i++;
        printf("-----------------------------------------------------------------------------------------\n");
    }

    
}
int main() {
    srand(time(NULL));
    Index table;
    Lnof *file = malloc(sizeof(Lnof));
    Tof *file_tof = malloc(sizeof(Tof));
    create_Lnof(file , &table);
    insert_student(file , &table);
    display_Lnof_file("Student_ESI.bin");
    create_TOF(file_tof , table);
    display_TOF_file("Student_INDEX.idx");
  /*  for (int i = 0; i < table.size; i++)
    {
        printf("\nKEY = %d ,   BLOCK = %d \n", table.arr[i].key, table.arr[i].blck_num );
    }    
    */
    return 0;
}