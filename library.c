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
    } else {
        // There is room in the current block: append in-place
        offset = buff.NB;        
        buff.tab[offset] = record;
        buff.NB++;
        writeBlock_Lnof(file , current_blk , &buff);
        C34++;
    }
    insert_index(table , record.Student_id , current_blk , offset);
    insert_query_index(query , record , current_blk , offset);
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
        del_buff.tab[del_offset] = last_record;
        writeBlock_Lnof(file, del_blk, &del_buff);
        C35++;
        
        int moved_idx = index_search(*table, last_record_id);
        if(moved_idx != -1) {
            table->arr[moved_idx].blck_num = del_blk;
            table->arr[moved_idx].offset = del_offset;
        }
        
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
        printf("Invalid blood type: %s\n", blood_type);
        return -1;
    }

    if (Open_Lnof(&file, "Student_ESI.bin", "E") != 0) {
        printf("Cannot open Student_ESI.bin\n");
        return -1;
    }

    Node current = query.blood_idx.arr[code];
    int C41 = 0;
    int total_reads = 0;

    printf("\n");
    printf("================================================================================\n");
    printf("                    STUDENTS WITH BLOOD TYPE: %s\n", blood_type);
    printf("================================================================================\n\n");

    if (current == NULL) {
        printf("No students found with blood type %s\n\n", blood_type);
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
            total_reads++;
            last_blk = blk_num;
        }

        Student *s = &buff.tab[offset];
        C41++;

        printf("Record #%-4d | ID: %-6u | %s %s\n",
               C41, s->Student_id, s->first_name, s->family_name);
        printf("             Birth: %04d-%02d-%02d | Wilaya: %s\n",
               s->date_birth.year, s->date_birth.month, s->date_birth.day,
               s->wilaya_birth);
        printf("             Gender: %c | Study: %s | Resident: %s\n",
               s->gender, s->year_study, s->resident_uc ? "Yes" : "No");
        printf("             Speciality: %s\n", s->speciality);
        printf("--------------------------------------------------------------------------------\n");

        current = Next(current);
    }

    printf("\nTotal: %d students with blood type %s\n", C41, blood_type);
    printf("Blocks read: %d\n\n", total_reads);

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
    int C42 = 0;
    int total_reads = 0;

    printf("\n");
    printf("================================================================================\n");
    printf("                    STUDENTS IN SPECIALITY\n");
    printf("                    %s\n", speciality);
    printf("================================================================================\n\n");

    if (current == NULL) {
        printf("No students found in this speciality\n\n");
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
            total_reads++;
            last_blk = blk_num;
        }

        Student *s = &buff.tab[offset];
        C42++;

        printf("Record #%-4d | ID: %-6u | %s %s\n",
               C42, s->Student_id, s->first_name, s->family_name);
        printf("             Birth: %04d-%02d-%02d | Year: %s | Blood: %s\n",
               s->date_birth.year, s->date_birth.month, s->date_birth.day,
               s->year_study, s->blood_type);
        printf("--------------------------------------------------------------------------------\n");

        current = Next(current);
    }

    printf("\nTotal: %d students in this speciality\n", C42);
    printf("Blocks read: %d\n\n", total_reads);

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
    int C43 = 0;
    int total_reads = 0;

    printf("\n");
    printf("================================================================================\n");
    printf("                    STUDENTS IN YEAR: %s\n", year_study);
    printf("================================================================================\n\n");

    if (current == NULL) {
        printf("No students found in year %s\n\n", year_study);
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
            total_reads++;
            last_blk = blk_num;
        }

        Student *s = &buff.tab[offset];
        C43++;

        printf("Record #%-4d | ID: %-6u | %s %s | %s\n",
               C43, s->Student_id, s->first_name, s->family_name, s->speciality);
        printf("             Blood: %s | Resident: %s\n",
               s->blood_type, s->resident_uc ? "Yes" : "No");
        printf("--------------------------------------------------------------------------------\n");

        current = Next(current);
    }

    printf("\nTotal: %d students in year %s\n", C43, year_study);
    printf("Blocks read: %d\n\n", total_reads);

    Close_Lnof(file);
    return C43;
}
/* Display all students born in a specific year */
int display_students_by_birth_year(Lnof *file, Query_index query, int birth_year) {
    int code = get_birth_year_code(birth_year);
    if (code == -1) {
        printf("Invalid birth year: %d (valid range: 2003-2008)\n", birth_year);
        return -1;
    }

    if (Open_Lnof(&file, "Student_ESI.bin", "E") != 0) {
        printf("Cannot open Student_ESI.bin\n");
        return -1;
    }

    Node current = query.birth_idx.arr[code];
    int C44 = 0;
    int total_reads = 0;

    printf("\n");
    printf("================================================================================\n");
    printf("                    STUDENTS BORN IN %d\n", birth_year);
    printf("================================================================================\n\n");

    if (current == NULL) {
        printf("No students found born in %d\n\n", birth_year);
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
            total_reads++;
            last_blk = blk_num;
        }

        Student *s = &buff.tab[offset];
        C44++;

        printf("Record #%-4d | ID: %-6u | %s %s\n",
               C44, s->Student_id, s->first_name, s->family_name);
        printf("             Birth: %04d-%02d-%02d | Wilaya: %s\n",
               s->date_birth.year, s->date_birth.month, s->date_birth.day,
               s->wilaya_birth);
        printf("             Year: %s | Speciality: %s\n",
               s->year_study, s->speciality);
        printf("--------------------------------------------------------------------------------\n");

        current = Next(current);
    }

    printf("\nTotal: %d students born in %d\n", C44, birth_year);
    printf("Blocks read: %d\n\n", total_reads);

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


/* ==================== DISPLAY UTILITIES ====================
   Clean, well-formatted display functions for debugging and
   presenting file contents.
/* ============  DISPLAY SINGLE STUDENT (Detailed) ============ */
void display_student(Student s) {
    printf("\n╔══════════════════════════════════════════════════════╗\n");
    printf("║              STUDENT RECORD DETAILS                  ║\n");
    printf("╠══════════════════════════════════════════════════════╣\n");
    printf("║ Student ID       : %-33d ║\n", s.Student_id);
    printf("║ Family Name      : %-33s ║\n", s.family_name);
    printf("║ First Name       : %-33s ║\n", s.first_name);
    printf("║ Birth Date       : %04d-%02d-%02d%24s║\n", 
           s.date_birth.year, s.date_birth.month, s.date_birth.day, "");
    printf("║ Wilaya Birth     : %-33s ║\n", s.wilaya_birth);
    printf("║ Gender           : %-33c ║\n", s.gender);
    printf("║ Blood Type       : %-33s ║\n", s.blood_type);
    printf("║ Year Study       : %-33s ║\n", s.year_study);
    printf("║ Speciality       : %-33s ║\n", s.speciality);
    printf("║ Resident UC      : %-33s ║\n", s.resident_uc ? "Yes" : "No");
    printf("╚══════════════════════════════════════════════════════╝\n");
}

/* ============  DISPLAY LNOF FILE (Clean List Format) ============ */
void display_Lnof_table(const char *filename) {
    Lnof *file = malloc(sizeof(Lnof));
    if (!file) {
        printf("Memory allocation failed\n");
        return;
    }
    
    if (Open_Lnof(&file, (char *)filename, "E") != 0) {
        printf("Cannot open file: %s\n", filename);
        free(file);
        return;
    }

    printf("\n");
    printf("=================================================================================\n");
    printf("                           STUDENT RECORDS FILE                        \n");
    printf("                           File: %s\n", filename);
    printf("=================================================================================\n\n");

    block_LnOF buf;
    int blk = 1;
    int total_records = 0;
    int total_blocks = 0;
    int record_num = 1;

    while (blk != -1) {
        if (readBlock_Lnof(*file, blk, &buf) != 0) break;
        total_blocks++;

        for (unsigned int j = 0; j < buf.NB; j++) {
            Student *s = &buf.tab[j];
            
            printf("Record #%-4d | ID: %-6u | %s %s\n",
                   record_num,
                   s->Student_id,
                   s->first_name,
                   s->family_name);
            printf("             Birth: %04d-%02d-%02d | Wilaya: %s\n",
                   s->date_birth.year, s->date_birth.month, s->date_birth.day,
                   s->wilaya_birth);
            printf("             Gender: %c | Blood: %s | Study: %s | Resident: %s\n",
                   s->gender,
                   s->blood_type,
                   s->year_study,
                   s->resident_uc ? "Yes" : "No");
            printf("             Speciality: %s\n",
                   s->speciality);
            printf("----------------------------------------------------------------------------------\n");
            
            total_records++;
            record_num++;
        }

        blk = buf.link;
    }

    printf("\nTotal: %d records in %d blocks\n\n", total_records, total_blocks);

    Close_Lnof(file);
    free(file);
}

/* ============  DISPLAY TOF INDEX FILE ============ */
void display_TOF_table(const char *filename) {
    Tof *tof = malloc(sizeof(Tof));
    if (!tof) {
        printf("Memory allocation failed\n");
        return;
    }
    
    if (Open_TOF(&tof, (char *)filename, "E") != 0) {
        printf("Cannot open file: %s\n", filename);
        free(tof);
        return;
    }

    int nblc = getHeader_TOF(*tof, 1);
    int nrec = getHeader_TOF(*tof, 2);

    printf("\n");
    printf("+========================================================+\n");
    printf("|           TOF INDEX FILE: %-25s|\n", filename);
    printf("+========================================================+\n");
    printf("| Blocks: %-5d | Records: %-5d                       |\n", nblc, nrec);
    printf("+========================================================+\n\n");

    printf("+-------+-------+--------------------------------------------------+\n");
    printf("| Block | Count |              Index Entries                       |\n");
    printf("+-------+-------+--------------------------------------------------+\n");

    block_TOF buf;
    int total_keys = 0;

    for (int i = 1; i <= nblc; i++) {
        if (readBlock_TOF(*tof, i, &buf) != 0) break;
        
        // Build the entries string
        char entries[256] = "";
        for (unsigned int j = 0; j < buf.NB; j++) {
            char entry[32];
            snprintf(entry, sizeof(entry), "%d(%d,%d)", 
                    buf.tab[j].key, buf.tab[j].blck_num, buf.tab[j].offset);
            strcat(entries, entry);
            if (j + 1 < buf.NB) strcat(entries, " ");
            total_keys++;
        }
        
        printf("| %5d | %5u | %-48s |\n", i, buf.NB, entries);
    }

    printf("+-------+-------+--------------------------------------------------+\n");
    printf("Total keys: %d\n\n", total_keys);

    Close_TOF(tof);
    free(tof);
}

/* ============  DISPLAY LOF FILE ============ */
void display_Lof_table(const char *filename) {
    Lof *file = malloc(sizeof(Lof));
    if (!file) {
        printf("Memory allocation failed\n");
        return;
    }
    
    if (Open_Lof(&file, (char *)filename, "E") != 0) {
        printf("Cannot open file: %s\n", filename);
        free(file);
        return;
    }

    printf("\n");
    printf("=================================================================================\n");
    printf("                           STUDENT RECORDS FILE (LOF)                  \n");
    printf("                           File: %s\n", filename);
    printf("=================================================================================\n\n");

    block_LOF buf;
    int blk = 1;
    int total_records = 0;
    int total_blocks = 0;
    int record_num = 1;

    while (blk != -1) {
        if (readBlock_Lof(*file, blk, &buf) != 0) break;
        total_blocks++;

        for (unsigned int j = 0; j < buf.NB; j++) {
            Student *s = &buf.tab[j];
            
            printf("Record #%-4d | ID: %-6u | %s %s\n",
                   record_num,
                   s->Student_id,
                   s->first_name,
                   s->family_name);
            printf("             Birth: %04d-%02d-%02d | Wilaya: %s\n",
                   s->date_birth.year, s->date_birth.month, s->date_birth.day,
                   s->wilaya_birth);
            printf("             Gender: %c | Blood: %s | Study: %s | Resident: %s\n",
                   s->gender,
                   s->blood_type,
                   s->year_study,
                   s->resident_uc ? "Yes" : "No");
            printf("             Speciality: %s\n",
                   s->speciality);
            printf("----------------------------------------------------------------------------------\n");
            
            total_records++;
            record_num++;
        }

        blk = buf.link;
    }

    printf("\nTotal: %d records in %d blocks\n\n", total_records, total_blocks);

    Close_Lof(file);
    free(file);
}

/* ============  DISPLAY INDEX TABLE (In-Memory) ============ */
void display_index_table(Index table) {
    if (table.size == 0) {
        printf("Index table is empty\n");
        return;
    }

    printf("\n");
    printf("+==============================================================+\n");
    printf("|              IN-MEMORY INDEX TABLE                           |\n");
    printf("+==============================================================+\n");
    printf("| Size: %-5d | Capacity: %-5d                               |\n", 
           table.size, table.max_capacity);
    printf("+==============================================================+\n\n");

    printf("+-------+----------+-------+--------+\n");
    printf("| Index |    Key   | Block | Offset |\n");
    printf("+-------+----------+-------+--------+\n");

    int display_limit = (table.size > 50) ? 50 : table.size;
    
    for (int i = 0; i < display_limit; i++) {
        printf("| %5d | %8d |  %3d  |   %2d   |\n",
               i,
               table.arr[i].key,
               table.arr[i].blck_num,
               table.arr[i].offset);
    }

    if (table.size > 50) {
        printf("+-------+----------+-------+--------+\n");
        printf("|  ...  |   ...    |  ...  |  ...   |\n");
        printf("+-------+----------+-------+--------+\n");
        
        for (int i = table.size - 5; i < table.size; i++) {
            printf("| %5d | %8d |  %3d  |   %2d   |\n",
                   i,
                   table.arr[i].key,
                   table.arr[i].blck_num,
                   table.arr[i].offset);
        }
    }

    printf("+-------+----------+-------+--------+\n");
    printf("Showing %d of %d entries\n\n", display_limit, table.size);
}

/* ============  DISPLAY FILE HEADER INFO ============ */
void display_Lnof_header(const char *filename) {
    Lnof *file = malloc(sizeof(Lnof));
    if (!file) {
        printf("❌ Memory allocation failed\n");
        return;
    }
    
    if (Open_Lnof(&file, (char *)filename, "E") != 0) {
        printf("❌ Cannot open file: %s\n", filename);
        free(file);
        return;
    }

    printf("\n");
    printf("╔════════════════════════════════════════════════════╗\n");
    printf("║              FILE HEADER INFORMATION               ║\n");
    printf("╠════════════════════════════════════════════════════╣\n");
    printf("║ File: %-44s ║\n", filename);
    printf("║ Head Field    : %-5d                              ║\n", getHeader_Lnof(*file, 1));
    printf("║ Current Field : %-5d (Last Block)                 ║\n", getHeader_Lnof(*file, 2));
    printf("╚════════════════════════════════════════════════════╝\n\n");

    Close_Lnof(file);
    free(file);
}

/* ============  DISPLAY STATISTICS ============ */
void display_file_statistics(const char *filename) {
    Lnof *file = malloc(sizeof(Lnof));
    if (!file) {
        printf("❌ Memory allocation failed\n");
        return;
    }
    
    if (Open_Lnof(&file, (char *)filename, "E") != 0) {
        printf("❌ Cannot open file: %s\n", filename);
        free(file);
        return;
    }

    block_LnOF buf;
    int blk = 1;
    int total_records = 0;
    int total_blocks = 0;
    int min_records = MAX_RECORDS_PER_BLOCK;
    int max_records = 0;

    while (blk != -1) {
        if (readBlock_Lnof(*file, blk, &buf) != 0) break;
        
        total_blocks++;
        total_records += buf.NB;
        
        if ((int)buf.NB < min_records) min_records = buf.NB;
        if ((int)buf.NB > max_records) max_records = buf.NB;

        blk = buf.link;
    }

    double avg_records = (total_blocks > 0) ? (double)total_records / total_blocks : 0;
    double fill_rate = (avg_records / MAX_RECORDS_PER_BLOCK) * 100;

    printf("\n");
    printf("╔════════════════════════════════════════════════════╗\n");
    printf("║              FILE STATISTICS                       ║\n");
    printf("╠════════════════════════════════════════════════════╣\n");
    printf("║ File: %-44s ║\n", filename);
    printf("║ Total Blocks          : %-5d                      ║\n", total_blocks);
    printf("║ Total Records         : %-5d                      ║\n", total_records);
    printf("║ Min Records/Block     : %-5d                      ║\n", min_records);
    printf("║ Max Records/Block     : %-5d                      ║\n", max_records);
    printf("║ Avg Records/Block     : %5.2f                      ║\n", avg_records);
    printf("║ Fill Rate             : %5.1f%%                     ║\n", fill_rate);
    printf("╚════════════════════════════════════════════════════╝\n\n");

    Close_Lnof(file);
    free(file);
}

void search_and_display_student(Lnof *file, Index table, int id) {
    int blk_num, offset;
    
    if (search_student(table, id, &blk_num, &offset) != 0) {
        printf("❌ Student ID %d not found in index\n", id);
        return;
    }

    if (Open_Lnof(&file, "Student_ESI.bin", "E") != 0) {
        printf("❌ Cannot open file\n");
        return;
    }

    block_LnOF buf;
    if (readBlock_Lnof(*file, blk_num, &buf) != 0) {
        printf("❌ Error reading block %d\n", blk_num);
        Close_Lnof(file);
        return;
    }

    printf("\n✅ Found student at Block %d, Offset %d:\n", blk_num, offset);
    display_student(buf.tab[offset]);

    Close_Lnof(file);
}
/* ================= COMPLETE MAIN PROGRAM ================= */
void display_menu() {
    printf("\033[2J\033[H");
    printf("\n");
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║         STUDENT MANAGEMENT SYSTEM - MAIN MENU          ║\n");
    printf("╠════════════════════════════════════════════════════════╣\n");
    printf("║  1. Create new database                                ║\n");
    printf("║  2. Load existing database                             ║\n");
    printf("║  3. Insert new student                                 ║\n");
    printf("║  4. Search student by ID                               ║\n");
    printf("║  5. Delete student                                     ║\n");
    printf("║  6. Modify student first name                          ║\n");
    printf("║  ──────────────────────────────────────────────────    ║\n");
    printf("║  7. Display students by blood type                     ║\n");
    printf("║  8. Display students by speciality                     ║\n");
    printf("║  9. Display students by year of study                  ║\n");
    printf("║ 10. Display students by birth year                     ║\n");
    printf("║  ──────────────────────────────────────────────────    ║\n");
    printf("║ 11. Display all students                               ║\n");
    printf("║ 12. Display file statistics                            ║\n");
    printf("║ 13. Create STUDENTS_CP.BIN (CP students only)          ║\n");
    printf("║  ──────────────────────────────────────────────────    ║\n");
    printf("║  0. Save and Exit                                      ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n");
    printf("Enter your choice: ");
}
void printESI()
{
    printf("\033[2J\033[H");
    printf("\n\t        \033[0;107m                  \033[0m          \033[0;107m                  \033[0m     \033[0;104m         \033[0m \n");
    sleep_ms(150);
    printf("\t      \033[0;107m                      \033[0m      \033[0;107m                      \033[0m   \033[0;104m         \033[0m  \n");
    sleep_ms(150);
    printf("\t     \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m      \033[0m            \033[0;107m      \033[0m \n");
    sleep_ms(150);
    printf("\t     \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;104m         \033[0m \n");
    sleep_ms(150);
    printf("\t     \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m      \033[0m            \033[0;107m      \033[0m             \n");
    sleep_ms(150);
    printf("\t     \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m       \033[0m  \n");
    sleep_ms(150);
    printf("\t     \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m      \033[0m                      \033[0;107m       \033[0m       L'ECOLE  NATIONALE\n");
    sleep_ms(150);
    printf("\t     \033[0;107m                        \033[0m    \033[0;107m                        \033[0m    \033[0;107m       \033[0m  \n");
    sleep_ms(150);
    printf("\t     \033[0;107m                        \033[0m    \033[0;107m                        \033[0m    \033[0;107m       \033[0m       SUPERIEURE\n");
    sleep_ms(150);
    printf("\t     \033[0;107m      \033[0m                                        \033[0;107m      \033[0m    \033[0;107m       \033[0m  \n");
    sleep_ms(150);
    printf("\t     \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m       \033[0m       D'INFORMATIQUE\n");
    sleep_ms(150);
    printf("\t      \033[0;107m      \033[0m          \033[0;107m      \033[0m      \033[0;107m      \033[0m          \033[0;107m      \033[0m     \033[0;107m       \033[0m  \n");
    sleep_ms(150);
    printf("\t      \033[0;107m                      \033[0m      \033[0;107m                      \033[0m     \033[0;107m       \033[0m  \n");
    sleep_ms(150);
    printf("\t        \033[0;107m                  \033[0m          \033[0;107m                  \033[0m       \033[0;107m       \033[0m  \n\n\n");
    sleep_ms(220);
    printf("\t\t\t                            \033[91mPlease                                 \n");
    printf("\t\t\t                  Click any key to continue...\033[0m               ");
    getchar();

    printf("\033[2J\033[H");
}

int main() {
    srand(time(NULL));
    
    Index table = {NULL, 0, 0};
    Query_index query;
    Lnof *file = malloc(sizeof(Lnof));
    Lof *lof_file = malloc(sizeof(Lof));
    
    int choice;
    int db_loaded = 0;
    
    init_query_index(&query);
    printESI();
    while (1) {
        display_menu();
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch (choice) {
            case 1: {
                if (db_loaded) {
                    printf("Database already loaded! Save and restart to create new.\n");
                    break;
                }
                printf("\n╔══════════════════════════════════════════════════════╗\n");
                printf("║           CREATING NEW DATABASE                      ║\n");
                printf("╚══════════════════════════════════════════════════════╝\n");
                
                create_Lnof(file, &table, &query);
                create_TOF("Student_INDEX.idx", table);
                save_query_indexes(&query);
                db_loaded = 1;
                
                printf("\n✓ Database created successfully!\n");
                printf("  Files created:\n");
                printf("  • Student_ESI.bin (main data file)\n");
                printf("  • Student_INDEX.idx (primary key index)\n");
                printf("  • Blood_IDX.idx (blood type index)\n");
                printf("  • Spec_IDX.idx (speciality index)\n");
                printf("  • Year_IDX.idx (year of study index)\n");
                printf("  • Birth_IDX.idx (birth year index)\n");
                break;
            }
            
            case 2: {
                if (db_loaded) {
                    printf("Database already loaded!\n");
                    break;
                }
                printf("\n╔══════════════════════════════════════════════════════╗\n");
                printf("║           LOADING EXISTING DATABASE                  ║\n");
                printf("╚══════════════════════════════════════════════════════╝\n");
                
                if (Load_index(&table, "Student_INDEX.idx") == 0) {
                    if (load_query_indexes(&query) == 0) {
                        db_loaded = 1;
                        printf("\n✓ Database loaded successfully!\n");
                        printf("  - Records: %d\n", table.size);
                    } else {
                        printf("⚠ Warning: Query indexes not found. Rebuilding...\n");
                        // Rebuild query indexes from file
                        if (Open_Lnof(&file, "Student_ESI.bin", "E") == 0) {
                            init_query_index(&query);
                            block_LnOF buff;
                            int blk = 1;
                            while (blk != -1) {
                                readBlock_Lnof(*file, blk, &buff);
                                for (unsigned int i = 0; i < buff.NB; i++) {
                                    insert_query_index(&query, buff.tab[i], blk, i);
                                }
                                blk = buff.link;
                            }
                            Close_Lnof(file);
                            save_query_indexes(&query);
                            db_loaded = 1;
                            printf("✓ Query indexes rebuilt successfully!\n");
                        }
                    }
                } else {
                    printf("✗ Failed to load database. Create new one first.\n");
                }
                break;
            }
            
            case 3: {
                if (!db_loaded) {
                    printf("Please load or create database first!\n");
                    break;
                }
                printf("\n╔══════════════════════════════════════════════════════╗\n");
                printf("║           INSERTING NEW STUDENT                      ║\n");
                printf("╚══════════════════════════════════════════════════════╝\n");
                
                int cost = insert_student(file, &table, &query);
                printf("✓ Student inserted successfully! (I/O Cost: %d)\n", cost);
                
                // Update index files
                create_TOF("Student_INDEX.idx", table);
                save_query_indexes(&query);
                break;
            }
            
            case 4: {
                if (!db_loaded) {
                    printf("Please load or create database first!\n");
                    break;
                }
                int id;
                printf("Enter student ID: ");
                scanf("%d", &id);
                clearInputBuffer();
                
                search_and_display_student(file, table, id);
                break;
            }
            
            case 5: {
                if (!db_loaded) {
                    printf("Please load or create database first!\n");
                    break;
                }
                int id;
                printf("Enter student ID to delete: ");
                scanf("%d", &id);
                clearInputBuffer();
                
                int cost = delete_student(file, &table, &query, id);
                if (cost > 0) {
                    // Update index files
                    create_TOF("Student_INDEX.idx", table);
                    save_query_indexes(&query);
                }
                break;
            }
            
            case 6: {
                if (!db_loaded) {
                    printf("Please load or create database first!\n");
                    break;
                }
                int id;
                printf("Enter student ID: ");
                scanf("%d", &id);
                
                int cost = modify_firstName(file, table, id);
                if (cost > 0) {
                    printf("✓ First name modified successfully! (I/O Cost: %d)\n", cost);
                }
                break;
            }
            
            case 7: {
                if (!db_loaded) {
                    printf("Please load or create database first!\n");
                    break;
                }
                printf("\nAvailable blood types:\n");
                printf("  O+, A+, B+, O-, A-, AB+, B-, AB-\n");
                printf("Enter blood type: ");
                char blood[5];
                fgets(blood, sizeof(blood), stdin);
                blood[strcspn(blood, "\n")] = '\0';
                
                display_students_by_blood(file, query, blood);
                break;
            }
            
            case 8: {
                if (!db_loaded) {
                    printf("Please load or create database first!\n");
                    break;
                }
                printf("\nAvailable specialities:\n");
                printf("  0. Integrated Preparatory Classes\n");
                printf("  1. Common Core\n");
                printf("  2. Information Systems and Technologies (SIT)\n");
                printf("  3. Computer Systems (SIQ)\n");
                printf("  4. Software and Computer Systems (SIL)\n");
                printf("  5. Intelligent Systems and Data (SID)\n");
                printf("Enter choice (0-5): ");
                int spec_choice;
                scanf("%d", &spec_choice);
                clearInputBuffer();
                
                const char *specs[] = {
                    "Integrated Preparatory Classes",
                    "Common Core",
                    "Information Systems and Technologies (SIT)",
                    "Computer Systems (SIQ)",
                    "Software and Computer Systems (SIL)",
                    "Intelligent Systems and Data (SID)"
                };
                
                if (spec_choice >= 0 && spec_choice < 6) {
                    display_students_by_speciality(file, query, specs[spec_choice]);
                } else {
                    printf("Invalid choice!\n");
                }
                break;
            }
            
            case 9: {
                if (!db_loaded) {
                    printf("Please load or create database first!\n");
                    break;
                }
                printf("\nAvailable years:\n");
                printf("  1CP, 2CP, 1CS, 2CS, 3CS\n");
                printf("Enter year: ");
                char year[5];
                fgets(year, sizeof(year), stdin);
                year[strcspn(year, "\n")] = '\0';
                
                display_students_by_year(file, query, year);
                break;
            }
            
            case 10: {
                if (!db_loaded) {
                    printf("Please load or create database first!\n");
                    break;
                }
                int birth_year;
                printf("Enter birth year (2003-2008): ");
                scanf("%d", &birth_year);
                clearInputBuffer();
                
                display_students_by_birth_year(file, query, birth_year);
                break;
            }
            
            case 11: {
                if (!db_loaded) {
                    printf("Please load or create database first!\n");
                    break;
                }
                display_Lnof_table("Student_ESI.bin");
                break;
            }
            
            case 12: {
                if (!db_loaded) {
                    printf("Please load or create database first!\n");
                    break;
                }
                display_file_statistics("Student_ESI.bin");
                break;
            }
            
            case 13: {
                if (!db_loaded) {
                    printf("Please load or create database first!\n");
                    break;
                }
                printf("\n╔══════════════════════════════════════════════════════╗\n");
                printf("║        CREATING STUDENTS_CP.BIN (CP only)            ║\n");
                printf("╚══════════════════════════════════════════════════════╝\n");
                
                int cost = create_Lof(lof_file, &table);
                printf("✓ STUDENTS_CP.BIN created! (I/O Cost: %d blocks)\n", cost);
                display_Lof_table("STUDENTS_CP.BIN");
                break;
            }
            
            case 0: {
                if (db_loaded) {
                    if (table.arr) free(table.arr);
                    free(file);
                    free(lof_file);
                    return 0;
                }   
            }
        
            default:
                printf("Invalid choice! Please try again.\n");
            break;
    }
    
    printf("\nPress Enter to continue...");
    getchar();
    }
    return 0;
}
