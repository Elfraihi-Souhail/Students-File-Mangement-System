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

static const char *wilayat[58] = {"Adrar", "Chlef", "Laghouat", "Oum El Bouaghi", "Batna", "Béjaïa", "Biskra", "Béchar", "Blida", "Bouira", "Tamanrasset", "Tébessa", "Tlemcen", "Tiaret", "Tizi Ouzou", "Alger", "Djelfa", "Jijel", "Sétif", "Saïda", "Skikda", "Sidi Bel Abbès", "Annaba", "Guelma", "Constantine", "Médéa", "Mostaganem", "M'Sila", "Mascara", "Ouargla", "Oran", "El Bayadh", "Illizi", "Bordj Bou Arréridj", "Boumerdès", "El Tarf", "Tindouf", "Tissemsilt", "El Oued", "Khenchela", "Souk Ahras", "Tipaza", "Mila", "Aïn Defla", "Naâma", "Aïn Témouchent", "Ghardaïa", "Relizane", "Timimoun", "Bordj Badji Mokhtar", "Ouled Djellal", "Béni Abbès", "In Salah", "In Guezzam", "Touggourt", "Djanet", "El M'Ghair", "El Meniaa"};
static const char *blood_types[8] = {"O+" , "A+" , "B+" , "O-" , "A-" , "AB+" , "B-" , "AB-"};
static const char *year_of_study[5] = {"1CP" , "2CP" , "1CS" , "2CS" , "3CS"};
static const char *specialities[4] = {"Information Systems and Technologies (SIT)" , "Computer Systems (SIQ)" , "SOftware and Computer Systems (SIL)" , "Intelligent Systems and Data (SID)"};

//----------------------Type defintions:---------------------//
typedef struct Date {
    int year;
    int month;
    int day;
} Date;
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
typedef struct header
{
    unsigned int head;
    unsigned int current;
} header;
typedef struct block_LnOF {
    unsigned int NB;
    unsigned int link;
    Student tab[MAX_RECORDS_PER_BLOCK];
} block_LnOF;
typedef struct Lnof {
    FILE *f;
    header HEADER;
} Lnof;
typedef struct cell {
    int key;
    int blck_num;
    int offset;
} cell;
typedef struct index {
    cell *arr;   // max capacity
    int size;      // actual number of used entries
} Index;
//------------------------------------------------------------//

//-----------------ABSTRACT MACHINES HEADERS:-----------------//
int Open_Lnof(Lnof **file ,char *name , char *mode);
int Close_Lnof(Lnof *file);
int getHeader_Lnof(Lnof file, int i);
int setHeader_Lnof(Lnof *file, int i, int val);
int readBlock_Lnof(Lnof file, int i, block_LnOF *Buf);
int writeBlock_Lnof(Lnof *file , int i, block_LnOF *Buf);
int AllocBlock_Lnof(Lnof *file);
//------------------------------------------------------------//

//---------------------RANDOMIZER HEADER:--------------------//
int random_value(int mode);
//------------------------------------------------------------//

//---------------------DATE UTILITY HEADERS:------------------//
bool is_leap_year(int year);
int days_in_month(int month, int year);
void generate_random_date(Date *date);
// calculate_age() - optional, only if you want to show age in display
//------------------------------------------------------------//

//---------------------FILE RELATED HEADERS--------------------//
int exist_in_file(Index table , int key);
int create_record(Student *record , Index table);
int create_Lnof(Lnof *file , Index table);
