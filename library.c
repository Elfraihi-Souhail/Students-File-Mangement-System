#include "library.h"

/*
 * library.c
 * ---------
 * Implementation file for the student file management system used in
 * the TP coursework. It contains:
 *
 * - abstract-machine helpers for TOF, LOF and LNOF file types
 * - block read/write and allocation routines
 * - index and query-index (bucketed linked lists) utilities
 * - record generation, creation, insertion, modification, and deletion
 * - display helpers for debugging and inspection
 */

/* --------------------- ABSTRACT MACHINE  LLC --------------------- //
/*
 * allocate
 * --------
 * Allocate and initialize a new Node object on the heap.
 * Returns: pointer to the new Node or NULL on allocation failure.
 */
Node allocate(void){
    Node p = malloc(sizeof(*p));
    if(p == NULL) return NULL;
    p->blk_num = 0;
    p->offset = 0;
    p->next = NULL;
    return p;
}
/*
 * ass_adr
 * -------
 * Set the next pointer of node `p` to `q`.
 * Returns 1 on success, -1 if `p` is NULL.
 */
int ass_adr(Node p , Node q){
    if(p == NULL) return -1;
    p->next = q;
    return 1;
}
/*
 * ass_val
 * -------
 * Store physical location values in node `p`.
 * Returns -1 if `p` is NULL, otherwise 1.
 */
int ass_val(Node p , int blk_num , int offset){
    if(p == NULL) return -1;
    p->blk_num = blk_num;
    p->offset = offset;
    return 1;
}
/*
 * value
 * -----
 * Retrieve blk_num and offset from node `p` into out-parameters.
 * Returns -1 if `p` is NULL.
 */
int value(Node p , int *blk_num , int *offset){
    if(p == NULL) return -1;
    *blk_num = p->blk_num;
    *offset = p->offset;
    return 1;
}
/*
 * Next
 * ----
 * Return p->next (NULL-safe wrapper).
 */
Node Next(Node p){
    if(p == NULL) return NULL;
    return p->next;
}
/*
 * Free
 * ----
 * Free node `p`. Returns 0 on success, -1 if `p` was NULL.
 * Note: free(NULL) is safe; callers may prefer to NULL their pointer.
 */
int Free(Node p){
    if(p == NULL) return -1;
    free(p);
    return 0;
}
// ---------------------------------------------------------------- //



/* --------------------- ABSTRACT MACHINE TOF --------------------- //
   TOF: table-of-keys file abstraction. Functions open/close the file,
   read/write headers and blocks. Each function below performs low-level
   block I/O using the Tof wrapper. */
int Open_TOF(Tof **file ,char *name , char *mode){
    FILE *g;
    switch (*mode)
    {
    case 'E':
        // Open existing file for read/write binary
        (*file)->f = fopen(name, "rb+");
        if ((*file)->f == NULL) return -1;
        g = (*file)->f;
        // Ensure file pointer at start and read header
        rewind((*file)->f);
        fread(&((*file)->header), sizeof(header_Tof), 1, (*file)->f);
        (*file)->f = g;
        break;
    case 'N':
        // Create new file (truncate) and initialize header
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
    // Write header back to the beginning and close
    rewind(file->f);
    fwrite(&(file->header), sizeof(header_Tof), 1, file->f);
    fclose(file->f);
    return 0;
}
int getHeader_TOF(Tof file, int i){
    // Return requested header field (1 -> nblc, 2 -> nrec)
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
    // Set requested header field (1 -> nblc, 2 -> nrec)
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
    // Seek to block i (after header) and read a block into Buf
    fseek(file.f , sizeof(header_Tof) + (i - 1) * sizeof(block_TOF) , SEEK_SET);
    fread(Buf , sizeof(block_TOF) , 1 ,file.f);
    // Return 0 on success, 1 if Buf is NULL (simple check)
    return Buf ? 0 : 1;
}
int writeBlock_TOF(Tof *file, int i, block_TOF *Buf){
    // Seek to block i and write Buf contents
    fseek(file->f , sizeof(header_Tof) + (i - 1) * sizeof(block_TOF) , SEEK_SET);
    fwrite(Buf , sizeof(block_TOF) , 1 ,file->f);
    return Buf ? 0 : 1;
}
// ---------------------------------------------------------------- //


/* --------------------- ABSTRACT MACHINE LOF ---------------------
   LOF: list-of-files style format. Similar interface to TOF but stores
   full Student records with links between blocks. */
int Open_Lof(Lof **file ,char *name , char *mode){
    FILE *g;
    switch (*mode)
    {
    case 'E':
        // Open existing LOF file and read header
        (*file)->f = fopen(name, "rb+");
        if ((*file)->f == NULL) return -1;
        g = (*file)->f;
        rewind((*file)->f);
        fread(&((*file)->HEADER), sizeof(header), 1, (*file)->f);
        (*file)->f = g;
        break;
    case 'N':
        // Create new LOF and initialize header values
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
    // Persist header to disk and close
    rewind(file->f);
    fwrite(&(file->HEADER), sizeof(header), 1, file->f);
    fclose(file->f);
    return 0;
}
int getHeader_Lof(Lof file, int i){
    // Return LOF header fields (1 -> head, 2 -> current)
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
    // Set LOF header fields
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
    // Read a LOF block at index i
    fseek(file.f , sizeof(header) + (i - 1) * sizeof(block_LOF) , SEEK_SET);
    fread(Buf , sizeof(block_LOF) , 1 ,file.f);
    return Buf ? 0 : 1;
}
int writeBlock_Lof(Lof *file, int i, block_LOF *Buf){
    // Write a LOF block at index i
    fseek(file->f , sizeof(header) + (i - 1) * sizeof(block_LOF) , SEEK_SET);
    fwrite(Buf , sizeof(block_LOF) , 1 ,file->f);
    return Buf ? 0 : 1;
}
int AllocBlock_Lof(Lof *file) {
    // Allocate a new block index by incrementing header.current
    if (file->f == NULL) return -1;
    int next = getHeader_Lof(*file , 2);
    setHeader_Lof(file , 2 , next + 1);
    return next + 1;
}
// ---------------------------------------------------------------- //


/* --------------------- ABSTRACT MACHINE LNOF ---------------------
   LNOF: variant similar to LOF but used for the main Student_ESI.bin.
   Same set of helpers: open/create, read/write blocks, allocate blocks. */
int Open_Lnof(Lnof **file ,char *name , char *mode){
    FILE *g;
    switch (*mode)
    {
    case 'E':
        // Open existing LNOF and load header
        (*file)->f = fopen(name, "rb+");
        if ((*file)->f == NULL) return -1;
        g = (*file)->f;
        rewind((*file)->f);
        fread(&((*file)->HEADER), sizeof(header), 1, (*file)->f);
        (*file)->f = g;
        break;
    case 'N':
        // Create a new LNOF file and initialize header
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
    // Persist header and close file
    rewind(file->f);
    fwrite(&(file->HEADER), sizeof(header), 1, file->f);
    fclose(file->f);
    return 0;
}
int getHeader_Lnof(Lnof file, int i){
    // Return LNOF header fields
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
    // Set LNOF header fields
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
    // Read LnOF block at position i (after header)
    fseek(file.f , sizeof(header) + (i - 1) * sizeof(block_LnOF) , SEEK_SET);
    fread(Buf , sizeof(block_LnOF) , 1 ,file.f);
    return Buf ? 0 : 1;
}
int writeBlock_Lnof(Lnof *file, int i, block_LnOF *Buf){
    // Write LnOF block at position i
    fseek(file->f , sizeof(header) + (i - 1) * sizeof(block_LnOF) , SEEK_SET);
    fwrite(Buf , sizeof(block_LnOF) , 1 ,file->f);
    return Buf ? 0 : 1;
}
int AllocBlock_Lnof(Lnof *file) {
    // Allocate a new LnOF block number by incrementing header.current
    if (file->f == NULL) return -1;
    int next = getHeader_Lnof(*file , 2);
    setHeader_Lnof(file , 2 , next + 1);
    return next + 1;
}
// ----------------------------------------------------------- //


/* ------------------------ RANDOMIZER ------------------------
   Helper that returns a pseudo-random value based on the 'mode'
   parameter. Used across create_record to choose random fields. */
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
// ----------------------------------------------------------- //


/* ------------------ String utility functions ------------------
   Validation and normalization helpers for names and input buffering. */
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

    // Make first letter uppercase and rest lowercase
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

    // Auto-fix case
    normalizeName(name);

    return 1;
}
void clearInputBuffer(void) {
    // Consume leftover stdin characters until newline or EOF
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
// ----------------------------------------------------------- //


/* ------------------- Date utility functions -------------------
   Simple helpers for leap-year logic and random date generation. */
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
    // Fill Date struct with a random valid date in allowed year range
    date->year = random_value(3);
    date->month = 1 + rand() % 12;
    int max_days = days_in_month(date->month, date->year);
    date->day = 1 + rand() % max_days;
}
// ----------------------------------------------------------- //

/* ------------------- INDEX utility functions -------------------
   Simple helpers for INDEX logic */
int get_blood_type_code(char *blood_type) {
    if (strcmp(blood_type, "O+") == 0) return 0;
    if (strcmp(blood_type, "A+") == 0) return 1;
    if (strcmp(blood_type, "B+") == 0) return 2;
    if (strcmp(blood_type, "O-") == 0) return 3;
    if (strcmp(blood_type, "A-") == 0) return 4;
    if (strcmp(blood_type, "AB+") == 0) return 5;
    if (strcmp(blood_type, "B-") == 0) return 6;
    if (strcmp(blood_type, "AB-") == 0) return 7;
    return -1;
}
// Map speciality string to code (0-5)
int get_speciality_code(char *spec) {
    if (strcmp(spec, "Integrated Preparatory Classes") == 0) return 0;
    if (strcmp(spec, "Common Core") == 0) return 1;
    if (strcmp(spec, "Information Systems and Technologies (SIT)") == 0) return 2;
    if (strcmp(spec, "Computer Systems (SIQ)") == 0) return 3;
    if (strcmp(spec, "Software and Computer Systems (SIL)") == 0) return 4;
    if (strcmp(spec, "Intelligent Systems and Data (SID)") == 0) return 5;
    return -1;
}
// Map year of study string to code (0-4)
int get_year_study_code(char *year) {
    if (strcmp(year, "1CP") == 0) return 0;
    if (strcmp(year, "2CP") == 0) return 1;
    if (strcmp(year, "1CS") == 0) return 2;
    if (strcmp(year, "2CS") == 0) return 3;
    if (strcmp(year, "3CS") == 0) return 4;
    return -1;
}
// Map birth year to code (0-5)
int get_birth_year_code(int year){
    if (year == 2003) return 0;
    if (year == 2004) return 1;
    if (year == 2005) return 2;
    if (year == 2006) return 3;
    if (year == 2007) return 4;
    if (year == 2008) return 5;
    return -1;
}
// ----------------------------------------------------------- //

/* ------------------- File related functions -------------------
   Index table management, record creation and higher-level file
   operations (create LNOF, TOF, load index, insert/delete/modify). */
void init_query_index(Query_index *q) {
    for (int i = 0; i < 8; ++i) q->blood_idx.arr[i] = NULL;
    for (int i = 0; i < 6; ++i) q->spec_idx.arr[i]  = NULL;
    for (int i = 0; i < 5; ++i) q->year_idx.arr[i]  = NULL;
    for (int i = 0; i < 6; ++i) q->birth_idx.arr[i] = NULL;
}
/*
 * insert_query_index
 * ------------------
 * Insert a record's physical location into all relevant Query_index
 * bucket lists (blood, speciality, year, birth year). Each bucket
 * entry gets its own node storing (blk_num, offset).
 */
void insert_query_index(Query_index *q , Student record , int blk_num , int offset){
    int blood = get_blood_type_code(record.blood_type);
    int spec = get_speciality_code(record.speciality);
    int year = get_year_study_code(record.year_study);
    int birth = get_birth_year_code(record.date_birth.year);

    // Create and insert node for blood index
    Node p = allocate();
    if (p) {
        ass_val(p, blk_num, offset);
        ass_adr(p, q->blood_idx.arr[blood]);
        q->blood_idx.arr[blood] = p;
    }

    // Create and insert node for speciality index
    Node r = allocate();
    if (r) {
        ass_val(r, blk_num, offset);
        ass_adr(r, q->spec_idx.arr[spec]);
        q->spec_idx.arr[spec] = r;
    }

    // Create and insert node for year index
    Node s = allocate();
    if (s) {
        ass_val(s, blk_num, offset);
        ass_adr(s, q->year_idx.arr[year]);
        q->year_idx.arr[year] = s;
    }

    // Create and insert node for birth year index
    Node t = allocate();
    if (t) {
        ass_val(t, blk_num, offset);
        ass_adr(t, q->birth_idx.arr[birth]);
        q->birth_idx.arr[birth] = t;
    }
}
int table_init(Index *table , int capacity){
    // Allocate array of cells and initialize meta fields
    table->arr = malloc(capacity * sizeof(cell));
    table->size = 0;
    table->max_capacity = capacity;
    return 0;
}
void ensure_capacity(Index *table) {
    // Grow index array when needed
    if (table->size >= table->max_capacity) {
        int new_capacity = (table->max_capacity == 0) ? 4 : table->max_capacity * 2;
        cell *new_arr = realloc(table->arr, new_capacity * sizeof(cell));
        if (!new_arr) {
            // Allocation error reported to stdout
            printf("Index table realloc failed\n");
            return;
        }
        table->arr = new_arr;
        table->max_capacity = new_capacity;
    }
}
int insert_index(Index *table , int id , int block , int deplacement) {
    // Insert a new index cell while keeping array sorted by key
    ensure_capacity(table); 
    
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
    
    table->arr[i + 1].key = id;
    table->arr[i + 1].blck_num = block;
    table->arr[i + 1].offset = deplacement;
    table->size++;
    
    return 0;
}
void query_search(Node *prev , Node *ptr , int blk_num , int offset){
    *prev = NULL;
    while (*ptr){
        if((*ptr)->blk_num == blk_num && (*ptr)->offset == offset){
            break;
        }
        *prev = *ptr;
        *ptr = Next(*ptr);
    }  
}
int index_search(Index table , int key){
    // Binary search on the in-memory index array
    if (table.size == 0) {  
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
    // Fill a Student struct with pseudo-random but valid values.
    int id;
    do {
        id = random_value(1);
    } while (index_search(table , id) != -1);
    record->Student_id = id;

    // Family name
    int length =  random_value(2);
    char fam_name[length + 1];
    fam_name[0] = 'A' + rand() % 26;
    for(int i = 1 ; i < length ; i++){
        fam_name[i] = 'a' + rand() % 26;
    }
    fam_name[length] = '\0';
    strcpy(record->family_name, fam_name);

    // First name
    length =  random_value(2);
    char name[length + 1];
    name[0] = 'A' + rand() % 26;
    for(int i = 1 ; i < length ; i++){
        name[i] = 'a' + rand() % 26;
    }
    name[length] = '\0';
    strcpy(record->first_name, name);

    // Birth date
    generate_random_date(&(record->date_birth));

    int index;
    // Wilaya of birth
    index = random_value(4);
    strcpy(record->wilaya_birth , wilayat[index - 1]);

    // Gender
    index = random_value(5);
    record->gender = (index == 1) ? 'M' : 'F';

    // Blood type
    index = random_value(6);
    strcpy(record->blood_type , blood_types[index - 1]);

    // Year of study
    int year_index = random_value(7);
    strcpy(record->year_study , year_of_study[year_index - 1]);

    // Speciality logic based on year_of_study
    if(year_index <= 2){
        strcpy(record->speciality , "Integrated Preparatory Classes");
    } else if (year_index == 3){
        strcpy(record->speciality , "Common Core");
    } else {
        index = random_value(8); 
        strcpy(record->speciality , specialities[index - 1]);
    }
    
    // Resident flag
    record->resident_uc = random_value(9);
    return 0;
}
int create_Lnof(Lnof *file , Index *table , Query_index *query) {

    // Open (create) the LnOF main file Student_ESI.bin
    if(Open_Lnof(&file , "Student_ESI.bin" , "N") != 0){
        printf("The file can not get opened.");
        return -1;
    };

    // Prompt user for number of records
    int N , C2 = 0;
    printf("Enter the number of records to create : \n");
    scanf("%d", &N);

    // Initialize index table for N entries
    table_init(table , N);

    // Intialize query indexes to NULL
    init_query_index(query);

    // Generate and write records block by block
    int offset = 0 , blk_num = 1;
    block_LnOF buff;

    for(int i = 1 ; i <= N ; i++){
        Student record;
        create_record(&record, *table);
        insert_index(table , record.Student_id , blk_num, offset);
        insert_query_index(query , record , blk_num , offset);
        buff.tab[offset] = record;
        offset++; 
        if(offset == 40 ) {
            // When block full, set link and NB then write block
            buff.link = (i == N) ? -1 : AllocBlock_Lnof(file);
            buff.NB = offset;
            writeBlock_Lnof(file , blk_num , &buff);
            offset = 0;
            C2++;
            blk_num++;
        }
    } 
    // Write any remaining partial block
    if (offset > 0) {
        buff.link = -1 ;
        buff.NB = offset;
        writeBlock_Lnof(file , blk_num , &buff);
        C2++;
    } else {
        // If last block was exactly full, adjust blk_num
        blk_num--;
    }
    // Update header with last block number
    setHeader_Lnof(file , 2 , blk_num);
    Close_Lnof(file);

    return C2;
}
int queryIndex_TOF(Node *index_array, int num_categories, const char *filename) {
    
    Tof *file = malloc(sizeof(Tof));
    if (Open_TOF(&file, (char*)filename, "N") != 0) {
        printf("Cannot create file: %s\n", filename);
        free(file);
        return -1;
    }

    block_TOF buff;
    int blk_num = 1;
    int offset = 0;
    int total_records = 0;

    // Iterate through all categories
    for (int category = 0; category < num_categories; category++) {
        Node current = index_array[category];
        
        // Traverse linked list for this category
        while (current != NULL) {
            buff.tab[offset].key = category;
            buff.tab[offset].blck_num = current->blk_num;
            buff.tab[offset].offset = current->offset;
            offset++;
            total_records++;

            // Write block when full
            if (offset == b) {
                buff.NB = offset;
                writeBlock_TOF(file, blk_num, (block_TOF*)&buff);
                blk_num++;
                offset = 0;
            }

            current = Next(current);
        }
    }

    // Write remaining partial block
    if (offset > 0) {
        buff.NB = offset;
        writeBlock_TOF(file, blk_num, (block_TOF*)&buff);
    } else {
        blk_num--;
    }

    setHeader_TOF(file, 1, blk_num);
    setHeader_TOF(file, 2, total_records);
    Close_TOF(file);
    free(file);

    return 0;
}
int save_query_indexes(Query_index *query) {
    printf("Saving query indexes to disk...\n");
    
    int success = 0;
    
    // Save blood type index (8 categories)
    if (queryIndex_TOF(query->blood_idx.arr, 8, "Blood_IDX.idx") == 0) {
        success++;
    } else {
        printf("Warning: Failed to save blood type index\n");
    }
    
    // Save speciality index (6 categories)
    if (queryIndex_TOF(query->spec_idx.arr, 6, "Spec_IDX.idx") == 0) {
        success++;
    } else {
        printf("Warning: Failed to save speciality index\n");
    }
    
    // Save year study index (5 categories)
    if (queryIndex_TOF(query->year_idx.arr, 5, "Year_IDX.idx") == 0) {
        success++;
    } else {
        printf("Warning: Failed to save year study index\n");
    }
    
    // Save birth year index (6 categories)
    if (queryIndex_TOF(query->birth_idx.arr, 6, "Birth_IDX.idx") == 0) {
        success++;
    } else {
        printf("Warning: Failed to save birth year index\n");
    }
    
    if (success == 4) {
        printf("✓ All query indexes saved successfully (4/4)\n");
        return 0;
    } else {
        printf("⚠ Some indexes failed to save (%d/4 saved)\n", success);
        return -1;
    }
}
int create_TOF(char *filename, Index table) {
    // Create TOF index file from in-memory index table
    if (table.size == 0) {
        printf("The index table is empty. Create the main file first.\n");
        return -1;
    }
    Tof *file = malloc(sizeof(Tof));
    if(Open_TOF(&file, filename, "N") != 0) {
        printf("The file cannot be opened.\n");
        return -1;
    }

    printf("Creating TOF index with %d records...\n", table.size);

    int C31 = 0;
    block_TOF buff;
    int blk_num = 1;  
    int offset = 0;   // Records in current block
    int k = 0;        // Index into table.arr
    int load_factor_limit = (int)(0.7 * b);
    
    while (k < table.size) {
        // Pack entries into TOF blocks until load factor or end
        buff.tab[offset] = table.arr[k];
        offset++;
        k++;
        
        if (offset >= load_factor_limit || k == table.size) {
            buff.NB = offset;
            writeBlock_TOF(file, blk_num, &buff);
            C31++;
            
            blk_num++;
            offset = 0;  // Reset for next block
        }
    }
    
    // Set headers and close
    int total_blocks = blk_num - 1;
    setHeader_TOF(file, 1, total_blocks);   // nblc
    setHeader_TOF(file, 2, table.size);     // nrec
    Close_TOF(file);   
    return C31;
}
int load_queryIndex(Node *index_array, int num_categories, const char *filename) {
    Tof *file = malloc(sizeof(Tof));
    if (Open_TOF(&file, (char*)filename, "E") != 0) {
        printf("Cannot open file: %s (may not exist yet)\n", filename);
        free(file);
        return -1;
    }

    // Initialize all index arrays to NULL
    for (int i = 0; i < num_categories; i++) {
        index_array[i] = NULL;
    }

    int nblc = getHeader_TOF(*file, 1);
    if (nblc == 0) {
        Close_TOF(file);
        free(file);
        return 0;
    }

    // Arrays to track tail of each linked list for efficient append
    Node *tails = malloc(num_categories * sizeof(Node));
    for (int i = 0; i < num_categories; i++) {
        tails[i] = NULL;
    }

    block_TOF buff;
    for (int i = 1; i <= nblc; i++) {
        readBlock_TOF(*file, i, (block_TOF*)&buff);
        
        for (unsigned int j = 0; j < buff.NB; j++) {
            int category = buff.tab[j].key;
            
            // Validate category
            if (category < 0 || category >= num_categories) continue;
            
            // Create new node
            Node new_node = allocate();
            if (new_node == NULL) continue;
            
            ass_val(new_node, buff.tab[j].blck_num, buff.tab[j].offset);
            ass_adr(new_node, NULL);

            // Append to appropriate list
            if (index_array[category] == NULL) {
                index_array[category] = new_node;
                tails[category] = new_node;
            } else {
                ass_adr(tails[category], new_node);
                tails[category] = new_node;
            }
        }
    }

    free(tails);
    Close_TOF(file);
    free(file);
    return 0;
}
int load_query_indexes(Query_index *query) {
    printf("Loading query indexes from disk...\n");
    
    init_query_index(query);
    
    int success = 0;
    
    // Load blood type index (8 categories)
    if (load_queryIndex(query->blood_idx.arr, 8, "Blood_IDX.idx") == 0) {
        success++;
    }
    
    // Load speciality index (6 categories)
    if (load_queryIndex(query->spec_idx.arr, 6, "Spec_IDX.idx") == 0) {
        success++;
    }
    
    // Load year study index (5 categories)
    if (load_queryIndex(query->year_idx.arr, 5, "Year_IDX.idx") == 0) {
        success++;
    }
    
    // Load birth year index (6 categories)
    if (load_queryIndex(query->birth_idx.arr, 6, "Birth_IDX.idx") == 0) {
        success++;
    }
    
    if (success == 4) {
        printf("✓ All query indexes loaded successfully (4/4)\n");
        return 0;
    } else {
        printf("⚠ Some index files missing (%d/4 loaded)\n", success);
        return -1;
    }
}
int Load_index (Index *table , char *filename){
    // Load TOF index file into in-memory Index table
    Tof *file = malloc(sizeof(Tof));
    if (file == NULL) {
        printf("\033[0;31m✗ Memory allocation failed\033[0m\n");
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
    // Look up student id in index and return physical location via pointers
    int i = index_search(table , id);
    if(i == -1) {
        printf("Student does not exist.");
        return -1;
    }
    *blk_num = table.arr[i].blck_num;
    *offset = table.arr[i].offset;
    return 0;
}
int insert_student(Lnof *file , Index *table , Query_index *query){
    // Insert a new record at the end (append), possibly allocating new block
    if(Open_Lnof(&file , "Student_ESI.bin" , "E") != 0){
        printf("The file can not get opened.");
        return -1;
    };

    int C34 = 0 , current_blk = getHeader_Lnof(*file , 2), offset = 0;
    Student record;
    int final_blk, final_offset;
    create_record(&record, *table);
    block_LnOF buff;
    readBlock_Lnof(*file , current_blk , &buff);
    C34++;
    if(buff.NB == 40){
        // Current block full, allocate a new one and append there
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

        final_blk = new_blk;
        final_offset = 0;
    } else {
        // There is room in the current block: append in-place
        final_offset = buff.NB;        
        buff.tab[final_offset] = record;
        buff.NB++;
        writeBlock_Lnof(file , current_blk , &buff);
        C34++;
        
        final_blk = current_blk;
    }
    insert_index(table , record.Student_id , final_blk , final_offset);
    insert_query_index(query , record , final_blk , final_offset);
    Close_Lnof(file);
    return C34;
}
void delete_from_query_index(Query_index *q , Student record , int blk_num, int offset) {
    if (!q) return;
    
    int blood = get_blood_type_code(record.blood_type);
    int spec = get_speciality_code(record.speciality);
    int year = get_year_study_code(record.year_study);
    int birth = get_birth_year_code(record.date_birth.year);
    
    // Delete from blood index
    if (blood >= 0 && blood < 8) {
        Node prev = NULL, cur = q->blood_idx.arr[blood];
        query_search(&prev, &cur, blk_num, offset);
        if (cur) {
            if (prev) {
                prev->next = cur->next;
            } else {
                q->blood_idx.arr[blood] = cur->next;
            }
            free(cur);
        }
    }
    
    // Delete from speciality index
    if (spec >= 0 && spec < 6) {
        Node prev = NULL, cur = q->spec_idx.arr[spec];
        query_search(&prev, &cur, blk_num, offset);
        if (cur) {
            if (prev) {
                prev->next = cur->next;
            } else {
                q->spec_idx.arr[spec] = cur->next;
            }
            free(cur);
        }
    }
    
    // Delete from year index
    if (year >= 0 && year < 5) {
        Node prev = NULL, cur = q->year_idx.arr[year];
        query_search(&prev, &cur, blk_num, offset);
        if (cur) {
            if (prev) {
                prev->next = cur->next;
            } else {
                q->year_idx.arr[year] = cur->next;
            }
            free(cur);
        }
    }
    
    // Delete from birth index
    if (birth >= 0 && birth < 6) {
        Node prev = NULL, cur = q->birth_idx.arr[birth];
        query_search(&prev, &cur, blk_num, offset);
        if (cur) {
            if (prev) {
                prev->next = cur->next;
            } else {
                q->birth_idx.arr[birth] = cur->next;
            }
            free(cur);
        }
    }
}
int delete_student(Lnof *file , Index *table , Query_index *query , int id) {
    // Delete student by id: replace deleted slot with last record (if not last),
    // update indices and possibly free last block if it becomes empty.
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
    
    int last_offset = last_buff.NB - 1;  // Offset of last record
    Student del_record;
    // Case 1: Deleting the last record itself
    if(del_blk == last_blk && del_offset == last_offset) {
        del_record = last_buff.tab[last_offset];
        last_buff.NB--;
        
        // If last block becomes empty and it's not the first block, remove it
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
            
            // Make previous block the new tail
            prev_buff.link = -1;
            writeBlock_Lnof(file, prev_blk, &prev_buff);
            C35++;
            
            setHeader_Lnof(file, 2, prev_blk);
        } else {
            // Just update last block's NB
            writeBlock_Lnof(file, last_blk, &last_buff);
            C35++;
        }
    }
    // Case 2: Replace deleted record with last record (shuffle last into deleted slot)
    else {
        Student last_record = last_buff.tab[last_offset];
        int last_record_id = last_record.Student_id;
        
        // Read block with deleted record
        block_LnOF del_buff;
        readBlock_Lnof(*file, del_blk, &del_buff);
        C35++;
        
        // Replace/save deleted record with last record and update index
        del_record = del_buff.tab[del_offset];
        delete_from_query_index(query, last_record, last_blk, last_offset);
        del_buff.tab[del_offset] = last_record;
        writeBlock_Lnof(file, del_blk, &del_buff);
        C35++;

        int moved_idx = index_search(*table, last_record_id);
        if(moved_idx != -1) {
            table->arr[moved_idx].blck_num = del_blk;
            table->arr[moved_idx].offset = del_offset;
        }
        insert_query_index(query, last_record, del_blk, del_offset);
        
        // Decrement last block's NB and possibly remove it if empty
        last_buff.NB--;
        
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
            writeBlock_Lnof(file, last_blk, &last_buff);
            C35++;
        }
    }
    delete_from_query_index(query , del_record , table->arr[idx].blck_num , table->arr[idx].offset);
    // Remove index entry by shifting subsequent entries left
    for(int j = idx; j < table->size - 1; j++) {
        table->arr[j] = table->arr[j + 1];
    }
    table->size--;
    Close_Lnof(file);
    
    printf("Deleted student ID %d (Cost: %d)\n", id, C35);
    return C35;
}
int modify_firstName(Lnof *file, Index table, int id) {
    // Modify the first name of a student given id. Validates input name.
    int blk_num, offset;

    if (search_student(table, id, &blk_num, &offset) == -1)
        return -1;

    char input_name[31];

    clearInputBuffer(); 

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
// ----------------------------------------------------------- //

/* ================= QUERY DISPLAY FUNCTIONS ================= */
/* Display all students with a specific blood type */
int display_students_by_blood(Lnof *file, Query_index query, const char *blood_type) {
    int code = get_blood_type_code((char*)blood_type);
    if (code == -1) {
        printf("\033[0;31m✗ Invalid blood type:\033[0m %s\n", blood_type);
        return -1;
    }

    if (Open_Lnof(&file, "Student_ESI.bin", "E") != 0) {
        printf("\033[0;31m✗ Cannot open Student_ESI.bin\033[0m\n");
        return -1;
    }

    Node current = query.blood_idx.arr[code];
    int total_rec = 0;
    int C41 = 0;

    printf("\n\033[0;36m╔═══════════════════════════════════════════════════════════════════════════╗\033[0m\n");
    printf("\033[0;36m║\033[0;33m                    STUDENTS WITH BLOOD TYPE:\033[0;31m%-30s\033[0;33m\033[0;36m║\033[0m\n", blood_type);
    printf("\033[0;36m╚═══════════════════════════════════════════════════════════════════════════╝\033[0m\n\n");

    if (current == NULL) {
        printf("\033[0;33m⚠ No students found with blood type %s\033[0m\n\n", blood_type);
        Close_Lnof(file);
        return 0;
    }

    block_LnOF buff;
    int last_blk = -1;

    while (current != NULL) {
        int blk_num = current->blk_num;
        int offset = current->offset;

        // Only read block if different from last one (optimization)
        if (blk_num != last_blk) {
            readBlock_Lnof(*file, blk_num, &buff);
            C41++;
            last_blk = blk_num;
        }

        Student *s = &buff.tab[offset];
        total_rec++;

        printf("\033[0;32m─ Record #%-4d ─\033[0m ID: \033[0;35m%u\033[0m | \033[0;33m%s %s\033[0m\n",
               total_rec, s->Student_id, s->first_name, s->family_name);
        printf("   Birth: \033[0;33m%04d-%02d-%02d\033[0m | Wilaya: \033[0;33m%s\033[0m\n",
               s->date_birth.year, s->date_birth.month, s->date_birth.day,
               s->wilaya_birth);
        printf("   Gender: \033[0;33m%c\033[0m | Study: \033[0;33m%s\033[0m | Resident: %s\n",
               s->gender, s->year_study, s->resident_uc ? "\033[0;32mYes\033[0m" : "\033[0;31mNo\033[0m");
        printf("   Speciality: \033[0;33m%s\033[0m\n", s->speciality);
        printf("\033[0;90m   ────────────────────────────────────────────────────────────────────\033[0m\n");

        current = Next(current);
    }

    printf("\n\033[0;33m✓ Total: %d students with blood type %s\033[0m\n", total_rec, blood_type);
    printf("\033[0;33m✓ Blocks read: %d\033[0m\n\n", C41);

    Close_Lnof(file);
    return C41;
}
/* Display all students in a specific speciality */
int display_students_by_speciality(Lnof *file, Query_index query, const char *speciality) {
    int code = get_speciality_code((char*)speciality);
    if (code == -1) {
        printf("Invalid speciality\n");
        return -1;
    }

    if (Open_Lnof(&file, "Student_ESI.bin", "E") != 0) {
        printf("Cannot open Student_ESI.bin\n");
        return -1;
    }

    Node current = query.spec_idx.arr[code];
    int total_rec = 0;
    int C42 = 0;

    printf("\n\033[0;36m╔═══════════════════════════════════════════════════════════════════════════╗\033[0m\n");
    printf("\033[0;36m║\033[0;33m                        STUDENTS IN SPECIALITY                             \033[0;36m║\033[0m\n");
    printf("\033[0;36m║\033[0m                    %-40s \033[0;36m              ║\033[0m\n", speciality);
    printf("\033[0;36m╚═══════════════════════════════════════════════════════════════════════════╝\033[0m\n\n");

    if (current == NULL) {
        printf("\033[0;33m⚠ No students found in this speciality\033[0m\n\n");
        Close_Lnof(file);
        return 0;
    }

    block_LnOF buff;
    int last_blk = -1;

    while (current != NULL) {
        int blk_num = current->blk_num;
        int offset = current->offset;

        if (blk_num != last_blk) {
            readBlock_Lnof(*file, blk_num, &buff);
            C42++;
            last_blk = blk_num;
        }

        Student *s = &buff.tab[offset];
        total_rec++;

        printf("\033[0;32m─ Record #%-4d ─\033[0m ID: \033[0;35m%u\033[0m | \033[0;33m%s %s\033[0m\n",
               total_rec, s->Student_id, s->first_name, s->family_name);
        printf("   Birth: \033[0;33m%04d-%02d-%02d\033[0m | Year: \033[0;33m%s\033[0m | Blood: \033[0;31m%s\033[0m\n",
               s->date_birth.year, s->date_birth.month, s->date_birth.day,
               s->year_study, s->blood_type);
        printf("\033[0;90m   ────────────────────────────────────────────────────────────────────\033[0m\n");

        current = Next(current);
    }

    printf("\n\033[0;33m✓ Total: %d students in this speciality\033[0m\n", total_rec);
    printf("\033[0;33m✓ Blocks read: %d\033[0m\n\n", C42);

    Close_Lnof(file);
    return C42;
}
/* Display all students in a specific year of study */
int display_students_by_year(Lnof *file, Query_index query, const char *year_study) {
    int code = get_year_study_code((char*)year_study);
    if (code == -1) {
        printf("Invalid year of study: %s\n", year_study);
        return -1;
    }

    if (Open_Lnof(&file, "Student_ESI.bin", "E") != 0) {
        printf("Cannot open Student_ESI.bin\n");
        return -1;
    }

    Node current = query.year_idx.arr[code];
    int total_rec = 0;
    int C43 = 0;

    printf("\n\033[0;36m╔═══════════════════════════════════════════════════════════════════════════╗\033[0m\n");
    printf("\033[0;36m║\033[0;33m                        STUDENTS IN YEAR: %-5s                            \033[0;36m║\033[0m\n", year_study);
    printf("\033[0;36m╚═══════════════════════════════════════════════════════════════════════════╝\033[0m\n\n");

    if (current == NULL) {
        printf("\033[0;33m⚠ No students found in year %s\033[0m\n\n", year_study);
        Close_Lnof(file);
        return 0;
    }

    block_LnOF buff;
    int last_blk = -1;

    while (current != NULL) {
        int blk_num = current->blk_num;
        int offset = current->offset;

        if (blk_num != last_blk) {
            readBlock_Lnof(*file, blk_num, &buff);
            C43++;
            last_blk = blk_num;
        }

        Student *s = &buff.tab[offset];
        total_rec++;

        printf("\033[0;32m─ Record #%-4d ─\033[0m ID: \033[0;35m%u\033[0m | \033[0;33m%s %s\033[0m | %s\n",
               total_rec, s->Student_id, s->first_name, s->family_name, s->speciality);
        printf("   Blood: \033[0;31m%s\033[0m | Resident: %s\n",
               s->blood_type, s->resident_uc ? "\033[0;32mYes\033[0m" : "\033[0;31mNo\033[0m");
        printf("\033[0;90m   ────────────────────────────────────────────────────────────────────\033[0m\n");

        current = Next(current);
    }

    printf("\n\033[0;33m✓ Total: %d students in year %s\033[0m\n", total_rec, year_study);
    printf("\033[0;33m✓ Blocks read: %d\033[0m\n\n", C43);

    Close_Lnof(file);
    return total_rec;
}
/* Display all students born in a specific year */
int display_students_by_birth_year(Lnof *file, Query_index query, int birth_year1 , int birth_year2) {
    int code1 = get_birth_year_code(birth_year1) , code2 = get_birth_year_code(birth_year2);
    if (code1 == -1 || code2 == -1) {
        printf("Invalid birth year , (valid range: 2003-2008)\n");
        return -1;
    }
    if(code1 > code2){
        printf("Invalid range , %d is after %d\n",birth_year1 , birth_year2);
        return -1;        
    }

    if (Open_Lnof(&file, "Student_ESI.bin", "E") != 0) {
        printf("Cannot open Student_ESI.bin\n");
        return -1;
    }

    int total_records = 0;
    int C44 = 0;

    printf("\n\033[0;36m╔═══════════════════════════════════════════════════════════════════════════╗\033[0m\n");
    printf("\033[0;36m║\033[0;33m                    STUDENTS BORN IN %d-%d                             \033[0;36m║\033[0m\n", birth_year1 , birth_year2);
    printf("\033[0;36m╚═══════════════════════════════════════════════════════════════════════════╝\033[0m\n\n");


    block_LnOF buff;
    int last_blk = -1;
    for(int i = code1 ; i <= code2 ; i++){
        Node current = query.birth_idx.arr[i];
        while (current != NULL) {
            int blk_num = current->blk_num;
            int offset = current->offset;

            if (blk_num != last_blk) {
                readBlock_Lnof(*file, blk_num, &buff);
                C44++;
                last_blk = blk_num;
            }

            Student *s = &buff.tab[offset];
            total_records++;

            printf("\033[0;32m─ Record #%-4d ─\033[0m ID: \033[0;35m%u\033[0m | \033[0;33m%s %s\033[0m\n",
                total_records, s->Student_id, s->first_name, s->family_name);
            printf("   Birth: \033[0;33m%04d-%02d-%02d\033[0m | Wilaya: \033[0;33m%s\033[0m\n",
                s->date_birth.year, s->date_birth.month, s->date_birth.day,
                s->wilaya_birth);
            printf("   Year: \033[0;33m%s\033[0m | Speciality: \033[0;33m%s\033[0m\n",
                s->year_study, s->speciality);
            printf("\033[0;90m   ────────────────────────────────────────────────────────────────────\033[0m\n");

            current = Next(current);
        }
    }

    printf("\n\033[0;33m✓ Total: %d students born in %d-%d\033[0m\n",total_records , birth_year1 , birth_year2);
    printf("\033[0;33m✓ Blocks read: %d\033[0m\n\n", C44);

    Close_Lnof(file);
    return C44;
}
// ----------------------------------------------------------- //

/* ---------------------- CREATE LOF --------------------------
   Build an LOF file (STUDENTS_CP.BIN) selecting only certain years
   from the main LnOF file using the in-memory index table. */
int create_Lof(Lof *file , Index *table){
    if (table == NULL || table->size == 0) return -1;

    // Create new LOF file
    if (Open_Lof(&file, "STUDENTS_CP.BIN", "N") != 0) {
        printf("Cannot create STUDENTS_CP.BIN\n");
        return -1;
    }

    // Open existing LnOF to read source records
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


    block_LOF buff1;
    int offset = 0; 

    block_LnOF buff2;
    int blk_num = 1;  
    int C5 = 0;

    for (int i = 0; i < table->size; i++) {
        cell index = table->arr[i];

        // Read block containing target student
        if (readBlock_Lnof(*lnof, index.blck_num, &buff2) != 0) {
            // skip on read error
            continue;
        }

        Student record = buff2.tab[index.offset];

        // Filter: only keep 1CP and 2CP students
        if (strcmp(record.year_study, "1CP") == 0 || strcmp(record.year_study, "2CP") == 0) {
            // Append to LOF block buffer
            buff1.tab[offset] = record;
            offset++;

            // Flush block when reaching load limit
            if (offset > 0.75 * b) {
                buff1.NB =  offset;
                buff1.link = (i == table->size - 1) ? -1 : AllocBlock_Lof(file);
                writeBlock_Lof(file, blk_num, &buff1);
                C5++;
                blk_num++;
                offset = 0;
            }
        }
    }

    // Write any remaining LOF block
    if (offset > 0) {
        buff1.link = -1;
        buff1.NB = offset;
        writeBlock_Lof(file, blk_num, &buff1);
        C5++;
    } else {
        blk_num--; // No final block written
    }

    // Update LOF header: total number of blocks written
    setHeader_Lof(file, 2, blk_num);
    Close_Lnof(lnof);
    free(lnof);
    Close_Lof(file);

    return C5;
}
// ----------------------------------------------------------- //
