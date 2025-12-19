#include "library.h"

//---------------------ABSTRACT MACHINE:---------------------//
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
int exist_in_file(Index table , int key){
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
    } while (exist_in_file(table , id) != -1);
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

}

int create_Lnof(Lnof *file , Index *table) {

    // Error handling 
    if(!Open_Lnof(&file , "Stuent_ESI.bin" , "N")){
        printf("The file can not get opened.");
        return -1;
    };

    // Reading the maximum number of records
    int N;
    printf("Enter the number of records to create : ");
    scanf("%d", &N);

    // Initialization of the index table (memory allocation)
    table_init(&table , N);

    // Genrerate and write the records to the file
    int j = 0 , offset = 0 , blk_num = 1;
    block_LnOF buff;
    for(int i = 1 ; i <= N ; i++){
        Student record;
        create_record(&record, *table);
        if(i % 40 == 0) {
            blk_num++;
            offset = 0;
            int next = (i == N) ? -1 : AllocBlock_Lnof(&file);
            writeBlock_Lnof(&file , blk_num , &file);
        }
        insert_index(&table , record.Student_id , blk_num , offset);

        
    }
    Close_Lnof(file);
    return 0;
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
int main() {
    srand(time(NULL));
    
    // Create empty index for testing
    Index table;
    table.size = 0;
    
    // Create and display a test record
    Student test_student;
    create_record(&test_student, table);
    
    printf("Testing record creation:\n");
    display_student(test_student);
    
    return 0;
}