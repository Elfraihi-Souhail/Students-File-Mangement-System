#ifndef LIBRARY_H
#define LIBRARY_H

#ifdef _WIN32
#include <windows.h>
#define sleep_ms(ms) Sleep(ms)
#else
#include <unistd.h>
#define sleep_ms(ms) usleep((ms) * 1000)
#include <stdio.h>
#endif

/*
 * library.h
 * ---------
 * Project header for the student file management system used in
 * the TP coursework. This header declares the public types,
 * constants, and function prototypes used across the project.
 *
 * NOTE: This comment block is documentation-only and does not
 *       change any program behavior. It was added at the user's
 *       request to increase readability without modifying code.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

/* Constants used across the project:
   - MAX_RECORDS_PER_BLOCK: number of student records per LnOF block
   - MIN_/MAX_*: ranges used for random data generation/validation
   - b: legacy alias for block size (40) kept for compatibility */
#define MAX_RECORDS_PER_BLOCK 40
#define MIN_STUDENT_ID 1000
#define MAX_STUDENT_ID 9000
#define MIN_NAME_LENGTH 4
#define MAX_NAME_LENGTH 30
#define MIN_BIRTH_YEAR 2003
#define MAX_BIRTH_YEAR 2008
#define MIN_WILAYA 1
#define MAX_WILAYA 58
#define MIN_YEAR_STUDY 1
#define MAX_YEAR_STUDY 5
#define b 40

/* Lookup tables for randomized/validated fields */
static const char *wilayat[58] = {"Adrar", "Chlef", "Laghouat", "Oum El Bouaghi", "Batna", "Béjaïa", "Biskra", "Béchar", "Blida", "Bouira", "Tamanrasset", "Tébessa", "Tlemcen", "Tiaret", "Tizi Ouzou", "Alger", "Djelfa", "Jijel", "Sétif", "Saïda", "Skikda", "Sidi Bel Abbès", "Annaba", "Guelma", "Constantine", "Médéa", "Mostaganem", "M'Sila", "Mascara", "Ouargla", "Oran", "El Bayadh", "Illizi", "Bordj Bou Arréridj", "Boumerdès", "El Tarf", "Tindouf", "Tissemsilt", "El Oued", "Khenchela", "Souk Ahras", "Tipaza", "Mila", "Aïn Defla", "Naâma", "Aïn Témouchent", "Ghardaïa", "Relizane", "Timimoun", "Bordj Badji Mokhtar", "Ouled Djellal", "Béni Abbès", "In Salah", "In Guezzam", "Touggourt", "Djanet", "El M'Ghair", "El Meniaa"};
static const char *blood_types[8] = {"O+" , "A+" , "B+" , "O-" , "A-" , "AB+" , "B-" , "AB-"};
static const char *year_of_study[5] = {"1CP" , "2CP" , "1CS" , "2CS" , "3CS"};
static const char *specialities[4] = {"Information Systems and Technologies (SIT)" , "Computer Systems (SIQ)" , "Software and Computer Systems (SIL)" , "Intelligent Systems and Data (SID)"};

/*----------------------Type defintions:---------------------*/

/* Simple date structure */
typedef struct Date {
    int year;
    int month;
    int day;
} Date;

/* Student record structure:
   - fixed-size strings to simplify binary I/O */
typedef struct student {
    unsigned int Student_id;
    char family_name[31];
    char first_name[31];
    Date date_birth;
    char wilaya_birth[30];
    char gender;
    char blood_type[4];
    char year_study[4];
    char speciality[50];
    bool resident_uc;
} Student;

/* Header used in LnOF/LOF structures:
   - head: usually unused/reserved
   - current: tracks last/next position in file structure */
typedef struct header
{
    unsigned int head;
    unsigned int current; // Last postion at the tail 
} header;

/* Block for LnOF file format:
   - NB: number of records actually stored in this block
   - link: pointer/index to next block or -1
   - tab: array of Student records (MAX_RECORDS_PER_BLOCK) */
typedef struct block_LnOF {
    unsigned int NB;
    unsigned int link;
    Student tab[MAX_RECORDS_PER_BLOCK];
} block_LnOF;

/* Lnof file descriptor combining FILE* and header */
typedef struct Lnof {
    FILE *f;
    header HEADER;
} Lnof;

/* Index cell holding key and physical location */
typedef struct cell {
    int key;
    int blck_num;
    int offset;
} cell;

/* Dynamic index (array of cells) */
typedef struct index {
    cell *arr;   // max capacity
    int size;      // actual number of used entries
    int max_capacity;
} Index;

/* Header for TOF (table/overflow) file type */
typedef struct header_Tof {
    int nblc;
    int nrec;
} header_Tof;

/* Tof file descriptor (file + header) */
typedef struct Tof {
    FILE *f;
    header_Tof header;
} Tof;

/* Block types for TOF and LOF formats:
   - block_TOF: stores cell entries for TOF/indexing
   - block_LOF: LOF block stores Student records + deletion flags */
typedef struct block_TOF {
    unsigned int NB;
    cell tab[b];
} block_TOF;
typedef struct block_LOF {
    unsigned int NB;
    unsigned int link;
    Student tab[b];
    bool deleted[b];
} block_LOF;

/* Lof file descriptor (LOF uses same header type) */
typedef struct Lof {
    FILE *f;
    header HEADER;
} Lof;

typedef struct Node {
    int blk_num;
    int offset;
    struct Node *next;
} *Node;

typedef struct blood_resident_index {
    Node arr[8]; // 8 types of blood
} blood_resident_index;

typedef struct speciality_index {
    Node arr[6]; // 6 speacialities
} speciality_index;

typedef struct birth_year_index {
    Node arr[6]; // 2003-2008 "6 years"
} birth_year_index;

typedef struct year_study_index {
    Node arr[5]; // 1CP , 2CP , 1CS , 2CS , 3CS
} year_study_index;

typedef struct Query_index
{
    blood_resident_index blood_idx;
    speciality_index spec_idx;
    year_study_index year_idx;
    birth_year_index birth_idx;
} Query_index;


//------------------------------------------------------------//

/*-----------------ABSTRACT MACHINES HEADERS:-----------------*/
/* Linked-list abstract machine (Node helpers) */
/* Allocate a new node and return pointer (NULL on failure). */
Node allocate(void);
/* Set p->next = q. Returns 1 on success, -1 if p==NULL. */
int ass_adr(Node p , Node q);
/* Set node values blk_num and offset. Return -1 if p==NULL. */
int ass_val(Node p , int blk_num , int offset);
/* Retrieve node values into out-parameters. Returns -1 if p==NULL. */
int value(Node p , int *blk_num , int *offset);
/* Return the next pointer (NULL-safe). */
Node Next(Node p);
/* Free a node. Returns 0 on success (free(NULL) is a no-op). */
int Free(Node p);
//------------------------------------------------------------// 


/* Function prototypes for file abstractions (TOF, LOF, LNOF).
   These operate on FILE* wrapped structures and perform block-level I/O. */
/* TOF (table-of-keys) file operations: open/create, read/write blocks */
int Open_TOF(Tof **file ,char *name , char *mode);
/* Close TOF and persist header. */
int Close_TOF(Tof *file);
/* Get/Set header fields: i==1 -> nblc, i==2 -> nrec */
int getHeader_TOF(Tof file, int i);
int setHeader_TOF(Tof *file, int i, int val);
/* Block I/O for TOF. */
int readBlock_TOF(Tof file, int i, block_TOF *Buf);
int writeBlock_TOF(Tof *file, int i, block_TOF *Buf);

/* LOF (list-of-files) file operations: open/create, close, block I/O */
int Open_Lof(Lof **file ,char *name , char *mode);
int Close_Lof(Lof *file);
int getHeader_Lof(Lof file, int i);
int setHeader_Lof(Lof *file, int i, int val);
int readBlock_Lof(Lof file, int i, block_LOF *Buf);
int writeBlock_Lof(Lof *file, int i, block_LOF *Buf);
/* Allocate a new LOF block index (increment header.current). */
int AllocBlock_Lof(Lof *file);

/* LNOF (linked-normalized file) helpers: open/create, close, block I/O */
int Open_Lnof(Lnof **file ,char *name , char *mode);
int Close_Lnof(Lnof *file);
int getHeader_Lnof(Lnof file, int i);
int setHeader_Lnof(Lnof *file, int i, int val);
int readBlock_Lnof(Lnof file, int i, block_LnOF *Buf);
int writeBlock_Lnof(Lnof *file , int i, block_LnOF *Buf);
/* Allocate a new LNOF block number (increment header.current) */
int AllocBlock_Lnof(Lnof *file);
//------------------------------------------------------------//

/*---------------------RANDOMIZER HEADER:---------------------*/
/* Produce pseudo-random values used for record generation (modes defined in impl). */
int random_value(int mode);
//------------------------------------------------------------//

/*--------------------STRING UTILITY HEADERS:-----------------*/
/* Various string utilities:
   - isOnlyLetters: validate alphabetic-only strings
   - normalizeName: apply formatting to names
   - validateFirstName: specific name validation rules
   - clearInputBuffer: consume leftover stdin characters */
/* String utilities: validation and normalization helpers. */
int isOnlyLetters(const char *s);
void normalizeName(char *s);
int validateFirstName(char *name);
void clearInputBuffer(void);
//------------------------------------------------------------//
//---------------------DATE UTILITY HEADERS:------------------*/
/* Date helpers for generation and validation */
/* Date helpers. */
bool is_leap_year(int year);
int days_in_month(int month, int year);
void generate_random_date(Date *date);
// calculate_age() - optional
//------------------------------------------------------------//

/*---------------------FILE RELATED HEADERS--------------------*/
/* Index/table helpers and high-level file operations:
   - table_init: allocate/initialize Index structure
   - ensure_capacity: grow index array as needed
   - insert_index / index_search: manage in-memory index
   - create_record: populate a Student struct (likely randomized)
   - create_Lnof/create_TOF/create_Lof: functions to build files
   - Load_index: read index from persistent storage
   - search/insert/delete/modify: student operations on files */
/* Index and high-level file operations */
void init_query_index(Query_index *q);
/* Initialize an in-memory Index with given capacity. */
int table_init(Index *table , int capacity); 
/* Grow index array when needed. */
void ensure_capacity(Index *table); 
void insert_query_index(Query_index *q , Student record , int blk_num , int offset);
/* Insert a cell into the in-memory index keeping it sorted by key. */
int insert_index(Index *table , int id , int block , int deplacement);
/* Search helper for query linked-lists: given ptr and prev pointers, scan for node matching blk_num+offset. */
void query_search(Node *prev , Node *ptr , int blk_num , int offset);
/* Binary-search the in-memory Index by key. */
int index_search(Index table , int key);
/* Create a randomized Student record (ensures unique Student_id vs table). */
int create_record(Student *record , Index table);
/* Create main LNOF file and optionally build Query_index in memory. */
int create_Lnof(Lnof *file , Index *table , Query_index *query);
/* Persist / restore Query_index arrays to TOF files (helpers). */
int queryIndex_TOF(Node *index_array, int num_categories, const char *filename);
int save_query_indexes(Query_index *query);
int load_queryIndex(Node *index_array, int num_categories, const char *filename);
int load_query_indexes(Query_index *query);
/* TOF index helpers. */
int create_TOF(char *filename , Index table );
int Load_index (Index *table , char *filename);
/* Student-level operations: search/insert/delete/modify */
int search_student(Index table , int id , int *blk_num , int *offest);
int insert_student(Lnof *file , Index *table , Query_index *query);
/* Remove references from Query_index when deleting a record */
void delete_from_query_index(Query_index *q , Student record , int blk_num, int offset);
int delete_student(Lnof *file , Index *table , Query_index *query , int id);
int modify_firstName(Lnof *file, Index table, int id);
int create_Lof(Lof *file , Index *table);
/* Display helpers for query results */
int display_students_by_blood(Lnof *file, Query_index query, const char *blood_type);
int display_students_by_speciality(Lnof *file, Query_index query, const char *speciality);
int display_students_by_year(Lnof *file, Query_index query, const char *year_study);
int display_students_by_birth_year(Lnof *file, Query_index query, int birth_year1 , int birth_year2);
#endif