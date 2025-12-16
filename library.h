#ifdef _WIN32
#include <windows.h>
#else
#include <stdio.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

// Constants
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




//----------------------Type defintions:----------------------//
typedef struct date {
    char year[4];   // 2003-2008
    char month[2];   // 1-12
    char day[2];    // 1-31
} Date;

typedef struct student {
    unsigned int Student_id;
    char family_name[30];
    char first_name[30];
    Date date_birth;
    unsigned int wilaya_birth;
    char gender;
    char blood_type[3];
    char year_study[3];
    char speciality[3];
    bool resident_uc;
} Student;

typedef struct block_LnOF {
    unsigned int NB;
    unsigned int link;
    Student tab[MAX_RECORDS_PER_BLOCK];
};

typedef struct  LnoF_header {
    unsigned int head;
    
};

