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

//---------------------ABSTRACT MACHINE LOF:----------------------//
int Open_Lof(Lof **file ,char *name , char *mode){
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
int Close_Lof(Lof *file){
    rewind(file->f);
    fwrite(&(file->HEADER), sizeof(header), 1, file->f);
    fclose(file->f);
    return 0;
}
int getHeader_Lof(Lof file, int i){
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
int setHeader_Lof(Lof *file, int i, int val){
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
int readBlock_Lof(Lof file, int i, block_LOF *Buf){
    fseek(file.f , sizeof(header) + (i - 1) * sizeof(block_LOF) , SEEK_SET);
    fread(Buf , sizeof(block_LOF) , 1 ,file.f);
    return Buf ? 0 : 1;
}
int writeBlock_Lof(Lof *file, int i, block_LOF *Buf){
    fseek(file->f , sizeof(header) + (i - 1) * sizeof(block_LOF) , SEEK_SET);
    fwrite(Buf , sizeof(block_LOF) , 1 ,file->f);
    return Buf ? 0 : 1;
}
int AllocBlock_Lof(Lof *file) {
    if (file->f == NULL) return -1;
    int next = getHeader_Lof(*file , 2);
    setHeader_Lof(file , 2 , next + 1);
    return next + 1;
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

//------------------String utility functions:----------------//
int isOnlyLetters(const char *s) {
    for (int i = 0; s[i]; i++)
    {
        if (!isalpha((unsigned char)s[i]))
            return 0;
    }
    return 1;
}
void normalizeName(char *s){
    if (s[0] == '\0')
        return;

    s[0] = toupper((unsigned char)s[0]);

    for (int i = 1; s[i]; i++)
        s[i] = tolower((unsigned char)s[i]);
}
int validateFirstName(char *name){
    size_t len = strlen(name);

    if (len == 0 || len > 30)
        return 0;

    if (!isOnlyLetters(name))
        return 0;

    normalizeName(name);   // auto-fix case

    return 1;
}
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
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
    // Error handling 
    if (table.size == 0) {  
        printf("The index table is empty. Create the main file first.\n");
        return -1;
    }
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
    } else {
        blk_num--;
    }
    // Setting the last block number (in case we need to insert a new record at the end or delete a record and use the last record)
    setHeader_Lnof(file , 2 , blk_num);
    printf("total number of blocks : %d" , blk_num);
    Close_Lnof(file);

    return C2;
}
int create_TOF(Tof *file, Index table) {
    // Error handling 
    if (table.size == 0) {
        printf("The index table is empty. Create the main file first.\n");
        return -1;
    }
    
    if(Open_TOF(&file, "Student_INDEX.idx", "N") != 0) {
        printf("The file cannot be opened.\n");
        return -1;
    }

    printf("Creating TOF index with %d records...\n", table.size);

    // Bulk load of TOF using index table
    int C31 = 0;
    block_TOF buff;
    int blk_num = 1;  
    int offset = 0;   // Records in current block
    int k = 0;        // Index in table.arr
    int load_factor_limit = (int)(0.7 * b);
    
    while (k < table.size) {
        // Add current record to buffer
        buff.tab[offset] = table.arr[k];
        offset++;
        k++;
        
        // Write block when reaching load factor OR last record
        if (offset >= load_factor_limit || k == table.size) {
            buff.NB = offset;
            writeBlock_TOF(file, blk_num, &buff);
            C31++;
            
            blk_num++;
            offset = 0;  // Reset for next block
        }
    }
    
    // Set headers
    int total_blocks = blk_num - 1;
    setHeader_TOF(file, 1, total_blocks);   // nblc = number of blocks
    setHeader_TOF(file, 2, table.size);     // nrec = total records 
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
int search_student(Index table , int id , int *blk_num , int *offset){
    int i = index_search(table , id);
    if(i == -1) {
        printf("Student does not exist.");
        return -1;
    }
    *blk_num = table.arr[i].blck_num;
    *offset = table.arr[i].offset;
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

int delete_student(Lnof *file, Index *table, int id) {
    if(Open_Lnof(&file, "Student_ESI.bin", "E") != 0){
        printf("The file cannot be opened.\n");
        return -1;
    }
    
    int C35 = 0;
    
    // Find student in index
    int idx = index_search(*table, id);
    if(idx == -1) {
        printf("Student ID %d not found.\n", id);
        Close_Lnof(file);
        return -1;
    }
    
    int del_blk = table->arr[idx].blck_num;
    int del_offset = table->arr[idx].offset;
    int last_blk = getHeader_Lnof(*file, 2);
    
    // Read last block
    block_LnOF last_buff;
    readBlock_Lnof(*file, last_blk, &last_buff);
    C35++;
    
    int last_offset = last_buff.NB - 1;  // Offset of last record (before decrement)
    
    // Case 1: Deleting the last record itself
    if(del_blk == last_blk && del_offset == last_offset) {
        last_buff.NB--;
        
        // If last block becomes empty
        if(last_buff.NB == 0 && last_blk > 1) {
            // Find previous block
            int prev_blk = 1;
            block_LnOF prev_buff;
            readBlock_Lnof(*file, prev_blk, &prev_buff);
            C35++;
            
            while(prev_buff.link != last_blk && prev_buff.link != -1) {
                prev_blk = prev_buff.link;
                readBlock_Lnof(*file, prev_blk, &prev_buff);
                C35++;
            }
            
            // Make previous block the new tail
            prev_buff.link = -1;
            writeBlock_Lnof(file, prev_blk, &prev_buff);
            C35++;
            
            setHeader_Lnof(file, 2, prev_blk);
        } else {
            // Just write back the last block with decremented NB
            writeBlock_Lnof(file, last_blk, &last_buff);
            C35++;
        }
    }
    // Case 2: Replace deleted record with last record
    else {
        Student last_record = last_buff.tab[last_offset];
        int last_record_id = last_record.Student_id;
        
        // Read block with deleted record
        block_LnOF del_buff;
        readBlock_Lnof(*file, del_blk, &del_buff);
        C35++;
        
        // Replace deleted record with last record
        del_buff.tab[del_offset] = last_record;
        writeBlock_Lnof(file, del_blk, &del_buff);
        C35++;
        
        // Update index for moved record
        int moved_idx = index_search(*table, last_record_id);
        if(moved_idx != -1) {
            table->arr[moved_idx].blck_num = del_blk;
            table->arr[moved_idx].offset = del_offset;
        }
        
        // Decrement last block's NB
        last_buff.NB--;
        
        // If last block becomes empty
        if(last_buff.NB == 0 && last_blk > 1) {
            int prev_blk = 1;
            block_LnOF prev_buff;
            readBlock_Lnof(*file, prev_blk, &prev_buff);
            C35++;
            
            while(prev_buff.link != last_blk && prev_buff.link != -1) {
                prev_blk = prev_buff.link;
                readBlock_Lnof(*file, prev_blk, &prev_buff);
                C35++;
            }
            
            prev_buff.link = -1;
            writeBlock_Lnof(file, prev_blk, &prev_buff);
            C35++;
            
            setHeader_Lnof(file, 2, prev_blk);
        } else {
            // Write back the last block with decremented NB
            writeBlock_Lnof(file, last_blk, &last_buff);
            C35++;
        }
    }
    
    // Remove from index - FIXED loop
    for(int j = idx; j < table->size - 1; j++) {
        table->arr[j] = table->arr[j + 1];
    }
    table->size--;
    
    Close_Lnof(file);
    
    printf("Deleted student ID %d (Cost: %d)\n", id, C35);
    return C35;
}
int modify_firstName(Lnof *file, Index table, int id) {
    int blk_num, offset;

    if (search_student(table, id, &blk_num, &offset) == -1)
        return -1;

    char input_name[31];

    clearInputBuffer();   // IMPORTANT: clean leftover '\n' FIRST

    do {
        printf("Enter new first name: ");
        fgets(input_name, sizeof(input_name), stdin);
        input_name[strcspn(input_name, "\n")] = '\0';

        if (!validateFirstName(input_name)) {
            printf("Invalid name. Use only letters, max 30 characters, "
                   "first letter uppercase, rest lowercase.\n");
        }

    } while (!validateFirstName(input_name));

    if (Open_Lnof(&file, "Student_ESI.bin", "E") != 0) {
        printf("Cannot open file .");
        return -1;
    }
    block_LnOF buff;
    int C36 = 0;
    readBlock_Lnof(*file, blk_num, &buff);
    C36++;
    strcpy(buff.tab[offset].first_name, input_name);
    writeBlock_Lnof(file , blk_num , &buff);
    C36++;
    Close_Lnof(file);

    return C36;
}
//-----------------------------------------------------------//

//----------------------CREATE LOF---------------------------//
int create_Lof(Lof *file , Index *table){
    if (table == NULL || table->size == 0) return -1;

    // Open target LOF (new) file
    if (Open_Lof(&file, "STUDENTS_CP.BIN", "N") != 0) {
        printf("Cannot create STUDENTS_CP.BIN\n");
        return -1;
    }

    // Open source Lnof file to read student records
    Lnof *lnof = malloc(sizeof(Lnof));
    if (lnof == NULL) {
        Close_Lof(file);
        return -1;
    }
    if (Open_Lnof(&lnof, "Student_ESI.bin", "E") != 0) {
        printf("Cannot open Student_ESI.bin for reading\n");
        free(lnof);
        Close_Lof(file);
        return -1;
    }

    int load_limit = (int)(0.75 * b); 

    block_LOF buff1;
    buff1.NB = 0;
    buff1.link = -1;

    block_LnOF buff2;
    int blk_num = 1;     // output block number to write
    int C5 = 0;

    for (int i = 0; i < table->size; i++) {
        cell idx = table->arr[i];

        // read the block that contains the student
        if (readBlock_Lnof(*lnof, idx.blck_num, &buff2) != 0) {
            // skip on read error
            continue;
        }

        Student s = buff2.tab[idx.offset];

        // keep only 1CP and 2CP
        if (strcmp(s.year_study, "1CP") == 0 || strcmp(s.year_study, "2CP") == 0) {
            // append to current LOF block buffer
            buff1.tab[buff1.NB] = s;
            buff1.NB++;

            // if reached load limit, write block
            if (buff1.NB >= (unsigned)load_limit) {
                buff1.link = -1;
                writeBlock_Lof(file, blk_num, &buff1);
                C5++;
                blk_num++;
                buff1.NB = 0;
                buff1.link = -1;
            }
        }
    }

    // write remaining records if any
    if (buff1.NB > 0) {
        buff1.link = -1;
        writeBlock_Lof(file, blk_num, &buff1);
        C5++;
    } else {
        blk_num--; // no final block written
    }

    // update LOF header: set current = number of blocks
    int total_blocks = (blk_num >= 1) ? blk_num : 0;
    setHeader_Lof(file, 2, total_blocks);

    
    Close_Lnof(lnof);
    free(lnof);
    Close_Lof(file);

    return C5;
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
    if (Open_Lnof(&file, "Student_ESI.bin", "E") != 0) {
        printf("Cannot open file %s\n", filename);
        free(file);
        return;
    }
    
    block_LnOF buf;
    int i = 1;
    int nb_records = 0, nb_blk = 0;
    int max_blocks = 1000;  // Safety limit to prevent infinite loop
    
    printf("File: %s\n", filename);
    printf("Header - head: %d, current: %d\n", 
           getHeader_Lnof(*file, 1), getHeader_Lnof(*file, 2));

    while (i != -1 && nb_blk < max_blocks) {  // Added safety check
        readBlock_Lnof(*file, i, &buf);
        printf("\n=== Block %d ===\n", i);
        printf("NB: %d, Link: %d\n", buf.NB, buf.link);
        nb_blk++;
        
        for (unsigned int j = 0; j < buf.NB; j++) {
            nb_records++;
            printf("Record %d in block:\n", j + 1);
            display_student(buf.tab[j]);
        }
        
        i = buf.link;
        printf("Next block: %d\n", i);
        printf("-----------------------------------------------------------------------------------------\n");
        
        if (i == 0) {  // Invalid link
            printf("ERROR: Invalid link (0) detected!\n");
            break;
        }
    }
    
    if (nb_blk >= max_blocks) {
        printf("ERROR: Infinite loop detected! Stopped at %d blocks.\n", max_blocks);
    }
    
    printf("Total blocks: %d\n Total records: %d\n", nb_blk, nb_records);

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
    printf("number of blocks : %d\nnumber of records : %d\n", nb_blk , nb_records);
    
}
int main() {
    srand(time(NULL));
    Index table;
    Lnof *file = malloc(sizeof(Lnof));
    Tof *file_tof = malloc(sizeof(Tof));
    create_Lnof(file , &table);
    for (int i = 0; i < table.size; i++)
    {
        printf("RECORD %d :\n" , i);
        printf("KEY = %d ,   BLOCK = %d\n", table.arr[i].key, table.arr[i].blck_num );
    }
    display_Lnof_file("Student_ESI.bin");
    int id;
    scanf("%d" , &id);
    modify_firstName(file , table , id);
    display_Lnof_file("Student_ESI.bin");

    return 0;
}