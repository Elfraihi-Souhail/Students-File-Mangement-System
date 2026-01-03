#include "library.h"

/* ==================== DISPLAY UTILITIES ====================
   Clean, well-formatted display functions for debugging and
   presenting file contents.

/* ============  DISPLAY SINGLE STUDENT (Detailed) ============ */
void display_student(Student s) {
    printf("\033[0;36m────────────────────────────────────────────────────────────────\033[0m\n");
    printf("\033[0;36m\033[0m \033[0;32m%-22s\033[0m : \033[0;33m%-35u \033[0;36m\033[0m\n", "Student ID", s.Student_id);
    printf("\033[0;36m\033[0m \033[0;32m%-22s\033[0m : \033[0;33m%-35.30s \033[0;36m\033[0m\n", "Family Name", s.family_name);
    printf("\033[0;36m\033[0m \033[0;32m%-22s\033[0m : \033[0;33m%-35.30s \033[0;36m\033[0m\n", "First Name", s.first_name);
    printf("\033[0;36m\033[0m \033[0;32m%-22s\033[0m : \033[0;33m%04d-%02d-%02d%-26s\033[0;36m\033[0m\n", "Birth Date", 
           s.date_birth.year, s.date_birth.month, s.date_birth.day, "");
    printf("\033[0;36m\033[0m \033[0;32m%-22s\033[0m : \033[0;33m%-35.20s \033[0;36m\033[0m\n", "Wilaya Birth", s.wilaya_birth);
    printf("\033[0;36m\033[0m \033[0;32m%-22s\033[0m : \033[0;33m%-35c \033[0;36m\033[0m\n", "Gender", s.gender);
    printf("\033[0;36m\033[0m \033[0;32m%-22s\033[0m : \033[0;31m%-35.3s \033[0;36m\033[0m\n", "Blood Type", s.blood_type);
    printf("\033[0;36m\033[0m \033[0;32m%-22s\033[0m : \033[0;33m%-35.5s \033[0;36m\033[0m\n", "Year Study", s.year_study);
    printf("\033[0;36m\033[0m \033[0;32m%-22s\033[0m : \033[0;33m%-35.40s \033[0;36m\033[0m\n", "Speciality", s.speciality);
    printf("\033[0;36m\033[0m \033[0;32m%-22s\033[0m : \033[0;33m%-35s \033[0;36m\033[0m\n", "Resident UC", s.resident_uc ? "Yes" : "No");
    printf("\033[0;36m────────────────────────────────────────────────────────────────\033[0m\n");
}

/* ============  DISPLAY LNOF FILE ============ */
void display_Lnof(const char *filename) {
    Lnof *file = malloc(sizeof(Lnof));
    if (!file) {
        printf("\033[0;31m✗ Memory allocation failed\033[0m\n");
        return;
    }
    
    if (Open_Lnof(&file, (char *)filename, "E") != 0) {
        printf("\033[0;31m✗ Cannot open file:\033[0m %s\n", filename);
        free(file);
        return;
    }

    printf("\n\033[0;36m╔═══════════════════════════════════════════════════════════════════════════╗\033[0m\n");
    printf("\033[0;36m║\033[0;33m                           STUDENT RECORDS FILE                            \033[0;36m║\033[0m\n");
    printf("\033[0;36m║\033[0m                           File: \033[0;33m%-33s\033[0m         \033[0;36m║\033[0m\n", filename);
    printf("\033[0;36m╚═══════════════════════════════════════════════════════════════════════════╝\033[0m\n\n");

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
            
            printf("\033[0;32m─ Record #%-4d ─\033[0m ID: \033[0;35m%u\033[0m | \033[0;33m%s %s\033[0m\n",
                   record_num,
                   s->Student_id,
                   s->first_name,
                   s->family_name);
            printf("   Birth: \033[0;33m%04d-%02d-%02d\033[0m | Wilaya: \033[0;33m%s\033[0m\n",
                   s->date_birth.year, s->date_birth.month, s->date_birth.day,
                   s->wilaya_birth);
            printf("   Gender: \033[0;33m%c\033[0m | Blood: \033[0;31m%s\033[0m | Study: \033[0;33m%s\033[0m | Resident: %s\n",
                   s->gender,
                   s->blood_type,
                   s->year_study,
                   s->resident_uc ? "\033[0;32mYes\033[0m" : "\033[0;31mNo\033[0m");
            printf("   Speciality: \033[0;33m%s\033[0m\n",
                   s->speciality);
            printf("\033[0;90m   ────────────────────────────────────────────────────────────────────\033[0m\n");
            
            total_records++;
            record_num++;
        }

        blk = buf.link;
    }

    printf("\n\033[0;33m✓ Total: %d records in %d blocks\033[0m\n\n", total_records, total_blocks);

    Close_Lnof(file);
    free(file);
};

/* ============ DISPLAY TOF FILE ============ */
void display_TOF(const char *filename) {
    Tof *file = malloc(sizeof(Tof));
    if (!file) {
        printf("\033[0;31m✗ Memory allocation failed\033[0m\n");
        return;
    }
    
    if (Open_TOF(&file, (char*)filename, "E") != 0) {
        printf("\033[0;31m✗ Cannot open file:\033[0m %s\n", filename);
        free(file);
        return;
    }

    int nblc = getHeader_TOF(*file, 1);
    int nrec = getHeader_TOF(*file, 2);

    printf("\n\033[0;36m╔═══════════════════════════════════════════════════════════════════════════╗\033[0m\n");
    printf("\033[0;36m║\033[0;33m                          TOF INDEX FILE                                   \033[0;36m║\033[0m\n");
    printf("\033[0;36m║\033[0m File: \033[0;33m%-65s\033[0m   \033[0;36m║\033[0m\n", filename);
    printf("\033[0;36m╠═══════════════════════════════════════════════════════════════════════════╣\033[0m\n");
    printf("\033[0;36m║\033[0m Blocks: \033[0;33m%-5d\033[0m | Records: \033[0;33m%-5d\033[0m                                            \033[0;36m║\033[0m\n", nblc, nrec);
    printf("\033[0;36m╚═══════════════════════════════════════════════════════════════════════════╝\033[0m\n\n");

    if (nblc == 0) {
        printf("\033[0;33m⚠  File is empty (no blocks)\033[0m\n\n");
        Close_TOF(file);
        free(file);
        return;
    }

    block_TOF buf;
    int total_records = 0;

    for (int i = 1; i <= nblc; i++) {
        if (readBlock_TOF(*file, i, &buf) != 0) {
            printf("\033[0;31m✗ Error reading block %d\033[0m\n", i);
            continue;
        }

        printf("\033[0;36m┌─────────────────────────────────────────────────────────────────────────┐\033[0m\n");
        printf("\033[0;36m│\033[0;33m BLOCK #%-2d\033[0m                                       Records: \033[0;33m%-3u\033[0m            \033[0;36m│\033[0m\n", i, buf.NB);
        printf("\033[0;36m├─────────────────────────────────────────────────────────────────────────┤\033[0m\n");
        printf("\033[0;36m│\033[0m   \033[0;32m#\033[0m    \033[0;35mKey (ID)\033[0m      \033[0;33mBlock Number\033[0m    \033[0;33mOffset\033[0m                             \033[0;36m│\033[0m\n");
        printf("\033[0;36m├─────────────────────────────────────────────────────────────────────────┤\033[0m\n");

        for (unsigned int j = 0; j < buf.NB; j++) {
            total_records++;
            printf("\033[0;36m│\033[0m  \033[0;32m%-4d\033[0m  \033[0;35m%-10u\033[0m     \033[0;33m%-12d\033[0m    \033[0;33m%-5d\033[0m                             \033[0;36m│\033[0m\n",
                   total_records,
                   buf.tab[j].key,
                   buf.tab[j].blck_num,
                   buf.tab[j].offset);
        }

        printf("\033[0;36m└─────────────────────────────────────────────────────────────────────────┘\033[0m\n\n");
    }

    printf("\033[0;33m✓ Total: %d records in %d blocks\033[0m\n\n", total_records, nblc);

    Close_TOF(file);
    free(file);
}

/* ============  DISPLAY SPECIFIC TOF BLOCK ============ */
void display_TOF_block(const char *filename, int block_num) {
    Tof *file = malloc(sizeof(Tof));
    if (!file) {
        printf("\033[0;31m✗ Memory allocation failed\033[0m\n");
        return;
    }
    
    if (Open_TOF(&file, (char*)filename, "E") != 0) {
        printf("\033[0;31m✗ Cannot open file:\033[0m %s\n", filename);
        free(file);
        return;
    }

    int nblc = getHeader_TOF(*file, 1);
    
    if (block_num < 1 || block_num > nblc) {
        printf("\033[0;31m✗ Invalid block number. File has %d blocks.\033[0m\n", nblc);
        Close_TOF(file);
        free(file);
        return;
    }

    block_TOF buf;
    if (readBlock_TOF(*file, block_num, &buf) != 0) {
        printf("\033[0;31m✗ Error reading block %d\033[0m\n", block_num);
        Close_TOF(file);
        free(file);
        return;
    }

    printf("\n\033[0;36m╔═══════════════════════════════════════════════════════════════════════════╗\033[0m\n");
    printf("\033[0;36m║\033[0;33m                        BLOCK %d DETAILS (TOF)                              \033[0;36m║\033[0m\n", block_num);
    printf("\033[0;36m╠═══════════════════════════════════════════════════════════════════════════╣\033[0m\n");
    printf("\033[0;36m║\033[0m File: \033[0;33m%-65s\033[0m   \033[0;36m║\033[0m\n", filename);
    printf("\033[0;36m║\033[0m Number of Records : \033[0;33m%-5u\033[0m                                                 \033[0;36m║\033[0m\n", buf.NB);
    printf("\033[0;36m╚═══════════════════════════════════════════════════════════════════════════╝\033[0m\n\n");

    printf("\033[0;36m┌─────────────────────────────────────────────────────────────────────────┐\033[0m\n");
    printf("\033[0;36m│\033[0m   \033[0;32m#\033[0m    \033[0;35mKey (ID)\033[0m      \033[0;33mBlock Number\033[0m    \033[0;33mOffset\033[0m                             \033[0;36m│\033[0m\n");
    printf("\033[0;36m├─────────────────────────────────────────────────────────────────────────┤\033[0m\n");

    for (unsigned int j = 0; j < buf.NB; j++) {
        printf("\033[0;36m│\033[0m  \033[0;32m%-4u\033[0m  \033[0;35m%-10u\033[0m     \033[0;33m%-12d\033[0m    \033[0;33m%-5d\033[0m                             \033[0;36m│\033[0m\n",
               j + 1,
               buf.tab[j].key,
               buf.tab[j].blck_num,
               buf.tab[j].offset);
    }

    printf("\033[0;36m└─────────────────────────────────────────────────────────────────────────┘\033[0m\n\n");

    Close_TOF(file);
    free(file);
}

/* ============  DISPLAY TOF HEADER (Updated to match style) ============ */
void display_TOF_header(const char *filename) {
    Tof *file = malloc(sizeof(Tof));
    if (!file) {
        printf("\033[0;31m✗ Memory allocation failed\033[0m\n");
        return;
    }
    
    if (Open_TOF(&file, (char*)filename, "E") != 0) {
        printf("\033[0;31m✗ Cannot open file:\033[0m %s\n", filename);
        free(file);
        return;
    }

    int nblc = getHeader_TOF(*file, 1);
    int nrec = getHeader_TOF(*file, 2);

    printf("\n\033[0;36m╔═══════════════════════════════════════════════════════════════════════════╗\033[0m\n");
    printf("\033[0;36m║\033[0;33m                    FILE HEADER INFORMATION (TOF)                          \033[0;36m║\033[0m\n");
    printf("\033[0;36m╠═══════════════════════════════════════════════════════════════════════════╣\033[0m\n");
    printf("\033[0;36m║\033[0m File: \033[0;33m%-65s\033[0m   \033[0;36m║\033[0m\n", filename);
    printf("\033[0;36m║\033[0m Number of Blocks  : \033[0;33m%-5d\033[0m                                                 \033[0;36m║\033[0m\n", nblc);
    printf("\033[0;36m║\033[0m Number of Records : \033[0;33m%-5d\033[0m                                                 \033[0;36m║\033[0m\n", nrec);
    printf("\033[0;36m╚═══════════════════════════════════════════════════════════════════════════╝\033[0m\n\n");

    Close_TOF(file);
    free(file);
}

/* ============  DISPLAY LOF FILE ============ */
void display_Lof(const char *filename) {
    Lof *file = malloc(sizeof(Lof));
    if (!file) {
        printf("\033[0;31m✗ Memory allocation failed\033[0m\n");
        return;
    }
    
    if (Open_Lof(&file, (char *)filename, "E") != 0) {
        printf("\033[0;31m✗ Cannot open file:\033[0m %s\n", filename);
        free(file);
        return;
    }

    printf("\n\033[0;36m╔═══════════════════════════════════════════════════════════════════════════╗\033[0m\n");
    printf("\033[0;36m║\033[0;33m                      STUDENT RECORDS FILE (LOF)                           \033[0;36m║\033[0m\n");
    printf("\033[0;36m║\033[0m                     File: \033[0;33m%-30s                 \033[0m \033[0;36m║\033[0m\n", filename);
    printf("\033[0;36m╚═══════════════════════════════════════════════════════════════════════════╝\033[0m\n\n");

    int last_blk = getHeader_Lof(*file, 2);
    if (last_blk == 0 || last_blk < 1) {
        printf("\033[0;33m⚠  File is empty (no blocks)\033[0m\n\n");
        Close_Lof(file);
        free(file);
        return;
    }

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
            
            printf("\033[0;32m─ Record #%-4d ─\033[0m ID: \033[0;35m%u\033[0m | \033[0;33m%s %s\033[0m\n",
                   record_num,
                   s->Student_id,
                   s->first_name,
                   s->family_name);
            printf("   Birth: \033[0;33m%04d-%02d-%02d\033[0m | Wilaya: \033[0;33m%s\033[0m\n",
                   s->date_birth.year, s->date_birth.month, s->date_birth.day,
                   s->wilaya_birth);
            printf("   Gender: \033[0;33m%c\033[0m | Blood: \033[0;31m%s\033[0m | Study: \033[0;33m%s\033[0m | Resident: %s\n",
                   s->gender,
                   s->blood_type,
                   s->year_study,
                   s->resident_uc ? "\033[0;32mYes\033[0m" : "\033[0;31mNo\033[0m");
            printf("   Speciality: \033[0;33m%s\033[0m\n",
                   s->speciality);
            printf("\033[0;90m   ────────────────────────────────────────────────────────────────────\033[0m\n");
            
            total_records++;
            record_num++;
        }

        blk = buf.link;
    }

    printf("\n\033[0;33m✓ Total: %d records in %d blocks\033[0m\n\n", total_records, total_blocks);

    Close_Lof(file);
    free(file);
}

/* ============  DISPLAY FILE HEADER INFO ============ */
void display_Lnof_header(const char *filename) {
    Lnof *file = malloc(sizeof(Lnof));
    if (!file) {
        printf("Memory allocation failed\n");
        return;
    }
    
    if (Open_Lnof(&file, (char *)filename, "E") != 0) {
        printf("\033[0;31m✗ Cannot open file:\033[0m %s\n", filename);
        free(file);
        return;
    }

    printf("\n\033[0;36m╔═══════════════════════════════════════════════════════════════════════════╗\033[0m\n");
    printf("\033[0;36m║\033[0;33m                        FILE HEADER INFORMATION                            \033[0;36m║\033[0m\n");
    printf("\033[0;36m╠═══════════════════════════════════════════════════════════════════════════╣\033[0m\n");
    printf("\033[0;36m║\033[0m File: \033[0;33m%-65s\033[0m   \033[0;36m║\033[0m\n", filename);
    printf("\033[0;36m║\033[0m Head Field    : \033[0;33m%-5d\033[0m                                                     \033[0;36m║\033[0m\n", getHeader_Lnof(*file, 1));
    printf("\033[0;36m║\033[0m Current Field : \033[0;33m%-5d\033[0m (Last Block)                                        \033[0;36m║\033[0m\n", getHeader_Lnof(*file, 2));
    printf("\033[0;36m╚═══════════════════════════════════════════════════════════════════════════╝\033[0m\n\n");

    Close_Lnof(file);
    free(file);
}

/* ============  DISPLAY STATISTICS ============ */
void display_file_statistics(const char *filename) {
    Lnof *file = malloc(sizeof(Lnof));
    if (!file) {
        printf("\033[1;31m✗ Memory allocation failed\033[0m\n");
        return;
    }
    
    if (Open_Lnof(&file, (char *)filename, "E") != 0) {
        printf("\033[0;31m✗ Cannot open file:\033[0m %s\n", filename);
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

    printf("\n\033[0;36m╔════════════════════════════════════════════════════════════════════════╗\033[0m\n");
    printf("\033[0;36m║\033[0;33m                           FILE STATISTICS                              \033[0;36m║\033[0m\n");
    printf("\033[0;36m╠════════════════════════════════════════════════════════════════════════╣\033[0m\n");
    printf("\033[0;36m║\033[0m File: \033[0;33m%-65s\033[0m\033[0;36m║\033[0m\n", filename);
    printf("\033[0;36m║\033[0m Total Blocks          : \033[0;33m%-5d\033[0m                                          \033[0;36m║\033[0m\n", total_blocks);
    printf("\033[0;36m║\033[0m Total Records         : \033[0;33m%-5d\033[0m                                          \033[0;36m║\033[0m\n", total_records);
    printf("\033[0;36m║\033[0m Min Records/Block     : \033[0;33m%-5d\033[0m                                          \033[0;36m║\033[0m\n", min_records);
    printf("\033[0;36m║\033[0m Max Records/Block     : \033[0;33m%-5d\033[0m                                          \033[0;36m║\033[0m\n", max_records);
    printf("\033[0;36m║\033[0m Avg Records/Block     : \033[0;33m%5.2f\033[0m                                          \033[0;36m║\033[0m\n", avg_records);
    printf("\033[0;36m║\033[0m Fill Rate             : \033[0;33m%5.1f%%\033[0m                                         \033[0;36m║\033[0m\n", fill_rate);
    printf("\033[0;36m╚════════════════════════════════════════════════════════════════════════╝\033[0m\n\n");

    Close_Lnof(file);
    free(file);
}

/* ============  DISPLAY STUDENT AFTER SEARCH ============ */
void search_and_display_student(Lnof *file, Index table, int id) {
    int blk_num, offset;
    
    if (search_student(table, id, &blk_num, &offset) != 0) {
        printf("\033[0;31m✗ Student ID %d not found in index\033[0m\n", id);
        return;
    }

    if (Open_Lnof(&file, "Student_ESI.bin", "E") != 0) {
        printf("\033[0;31m✗ Cannot open file\033[0m\n");
        return;
    }

    block_LnOF buf;
    if (readBlock_Lnof(*file, blk_num, &buf) != 0) {
        printf("\033[0;31m✗ Error reading block %d\033[0m\n", blk_num);
        Close_Lnof(file);
        return;
    }

    printf("\n\033[0;32m✓ Found student at Block %d, Offset %d:\033[0m\n", blk_num, offset);
    display_student(buf.tab[offset]);

    Close_Lnof(file);
}

/* ============ DISPLAY SPECIFIC BLOCK ============ */
void display_Lnof_block(const char *filename, int block_num) {
    Lnof *file = malloc(sizeof(Lnof));
    if (!file) {
        printf("✗ Memory allocation failed\n");
        return;
    }
    
    if (Open_Lnof(&file, (char *)filename, "E") != 0) {
        printf("\033[0;31m✗ Cannot open file:\033[0m %s\n", filename);
        free(file);
        return;
    }

    int last_blk = getHeader_Lnof(*file, 2);
    if (block_num < 1 || block_num > last_blk) {
        printf("Invalid block number. File has %d blocks.\n", last_blk);
        Close_Lnof(file);
        free(file);
        return;
    }

    block_LnOF blk;
    readBlock_Lnof(*file, block_num, &blk);

    printf("\n\033[0;36m╔═══════════════════════════════════════════════════════════════════════════╗\033[0m\n");
    printf("\033[0;36m║\033[0;33m                           BLOCK %d DETAILS                                 \033[0;36m║\033[0m\n", block_num);
    printf("\033[0;36m╠═══════════════════════════════════════════════════════════════════════════╣\033[0m\n");
    printf("\033[0;36m║\033[0m Number of Records : \033[0;33m%-5u\033[0m                                                 \033[0;36m║\033[0m\n", blk.NB);
    printf("\033[0;36m║\033[0m Link to Next      : \033[0;33m%-5d\033[0m                                                 \033[0;36m║\033[0m\n", blk.link);
    printf("\033[0;36m╚═══════════════════════════════════════════════════════════════════════════╝\033[0m\n\n");

    for (unsigned int j = 0; j < blk.NB; j++) {
        printf("──────────────────── Record %u ────────────────────\n", j);
        display_student(blk.tab[j]);
    }

    Close_Lnof(file);
    free(file);
}

/* ============ DISPLAY LOF BLOCK ============ */
void display_Lof_block(const char *filename, int block_num) {
    Lof *file = malloc(sizeof(Lof));
    if (!file) {
        printf("✗ Memory allocation failed\n");
        return;
    }
    
    if (Open_Lof(&file, (char *)filename, "E") != 0) {
        printf("\033[0;31m✗ Cannot open file:\033[0m %s\n", filename);
        free(file);
        return;
    }

    int last_blk = getHeader_Lof(*file, 2);
    if (block_num < 1 || block_num > last_blk) {
        printf("Invalid block number. File has %d blocks.\n", last_blk);
        Close_Lof(file);
        free(file);
        return;
    }

    block_LOF blk;
    readBlock_Lof(*file, block_num, &blk);

    printf("\n\033[0;36m╔═══════════════════════════════════════════════════════════════════════════╗\033[0m\n");
    printf("\033[0;36m║\033[0;33m                        BLOCK %d DETAILS (LOF)                              \033[0;36m║\033[0m\n", block_num);
    printf("\033[0;36m╠═══════════════════════════════════════════════════════════════════════════╣\033[0m\n");
    printf("\033[0;36m║\033[0m Number of Records : \033[0;33m%-5u\033[0m                                                 \033[0;36m║\033[0m\n", blk.NB);
    printf("\033[0;36m║\033[0m Link to Next      : \033[0;33m%-5d\033[0m                                                 \033[0;36m║\033[0m\n", blk.link);
    printf("\033[0;36m╚═══════════════════════════════════════════════════════════════════════════╝\033[0m\n\n");

    for (unsigned int j = 0; j < blk.NB; j++) {
        printf("──────────────────── Record %u ────────────────────\n", j);
        display_student(blk.tab[j]);
    }

    Close_Lof(file);
    free(file);
}

/* ============ DISPLAY HEADERS ============ */
void display_Lof_header(const char *filename) {
    Lof *file = malloc(sizeof(Lof));
    if (!file) {
        printf("✗ Memory allocation failed\n");
        return;
    }
    
    if (Open_Lof(&file, (char *)filename, "E") != 0) {
        printf("\033[0;31m✗ Cannot open file:\033[0m %s\n", filename);
        free(file);
        return;
    }

    printf("\n\033[0;36m╔═══════════════════════════════════════════════════════════════════════════╗\033[0m\n");
    printf("\033[0;36m║\033[0;33m                    FILE HEADER INFORMATION (LOF)                          \033[0;36m║\033[0m\n");
    printf("\033[0;36m╠═══════════════════════════════════════════════════════════════════════════╣\033[0m\n");
    printf("\033[0;36m║\033[0m File: \033[0;33m%-65s\033[0m   \033[0;36m║\033[0m\n", filename);
    printf("\033[0;36m║\033[0m Head Field    : \033[0;33m%-5d\033[0m                                                     \033[0;36m║\033[0m\n", getHeader_Lof(*file, 1));
    printf("\033[0;36m║\033[0m Current Field : \033[0;33m%-5d\033[0m (Last Block)                                        \033[0;36m║\033[0m\n", getHeader_Lof(*file, 2));
    printf("\033[0;36m╚═══════════════════════════════════════════════════════════════════════════╝\033[0m\n\n");

    Close_Lof(file);
    free(file);
}

/* ================= COMPLETE MAIN PROGRAM ================= */
void display_menu() {
    printf("\033[2J\033[H");
    printf("\033[0;36m");  // Softer cyan (not bold)
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║\033[0;33m         STUDENT MANAGEMENT SYSTEM - MAIN MENU          \033[0;36m║\n");
    printf("╠════════════════════════════════════════════════════════╣\n");
    printf("\033[0m");
    
    printf("\033[0;32m  1.\033[0m Create new database\n");
    printf("\033[0;32m  2.\033[0m Load existing database\n");
    printf("\033[0;32m  3.\033[0m Insert new student\n");
    printf("\033[0;32m  4.\033[0m Search student by ID\n");
    printf("\033[0;32m  5.\033[0m Delete student\n");
    printf("\033[0;32m  6.\033[0m Modify student first name\n");
    printf("\033[0;90m  ──────────────────────────────────────────────────\033[0m\n");
    printf("\033[0;32m  7.\033[0m Display students by blood type\n");
    printf("\033[0;32m  8.\033[0m Display students by speciality\n");
    printf("\033[0;32m  9.\033[0m Display students by year of study\n");
    printf("\033[0;32m 10.\033[0m Display students by birth year\n");
    printf("\033[0;90m  ──────────────────────────────────────────────────\033[0m\n");
    printf("\033[0;32m 11.\033[0m Create STUDENTS_CP.BIN (CP students only)\n");
    printf("\033[0;90m  ──────────────────────────────────────────────────\033[0m\n");
    printf("\033[0;32m 12.\033[0m Display file header\n");
    printf("\033[0;32m 13.\033[0m Display specific block\n");
    printf("\033[0;32m 14.\033[0m Display entire file\n");
    printf("\033[0;32m 15.\033[0m Display file statistics\n");
    printf("\033[0;90m  ──────────────────────────────────────────────────\033[0m\n");
    printf("\033[0;31m  0.\033[0m Save and Exit\n");
    
    printf("\033[0;36m╚════════════════════════════════════════════════════════╝\033[0m\n");
    printf("\033[0;33mEnter your choice: \033[0m");
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
                    printf("✓ Student deleted successfully! (I/O Cost: %d)\n", cost);
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
                int birth_year1 , birth_year2;
                printf("Enter birth year range (Y1-Y2): ");
                printf("\nY1 = ");
                scanf("%d", &birth_year1);
                printf("Y2 = ");                
                scanf("%d", &birth_year2);
                clearInputBuffer();
                
                int cost = display_students_by_birth_year(file, query, birth_year1 , birth_year2);
                if(cost > 0) printf("✓ I/O Cost: %d\n", cost);
                break;
            }
            
            case 11: {
                if (!db_loaded) {
                    printf("Please load or create database first!\n");
                    break;
                }
                printf("\n╔══════════════════════════════════════════════════════╗\n");
                printf("║        CREATING STUDENTS_CP.BIN (CP only)            ║\n");
                printf("╚══════════════════════════════════════════════════════╝\n");
                
                int cost = create_Lof(lof_file, &table);
                printf("✓ STUDENTS_CP.BIN created! (I/O Cost: %d blocks)\n", cost);
                break;
            }
            
            case 12: {
                // Display file header - SUBMENU
                printf("\033[2J\033[H");  // Clear screen
                printf("\n\033[0;36m╔═══════════════════════════════════════════════════════════════════════════╗\033[0m\n");
                printf("\033[0;36m║\033[0;33m                        DISPLAY FILE HEADER                                \033[0;36m║\033[0m\n");
                printf("\033[0;36m╠═══════════════════════════════════════════════════════════════════════════╣\033[0m\n");
                printf("\033[0;36m║\033[0m  \033[0;32m1.\033[0m Student_ESI.bin (LNOF - Main Data File)                               \033[0;36m║\033[0m\n");
                printf("\033[0;36m║\033[0m  \033[0;32m2.\033[0m STUDENTS_CP.BIN (LOF - CP Students Only)                              \033[0;36m║\033[0m\n");
                printf("\033[0;36m║\033[0m  \033[0;32m3.\033[0m Student_INDEX.idx (TOF - Primary Key Index)                           \033[0;36m║\033[0m\n");
                printf("\033[0;36m║\033[0m  \033[0;31m0.\033[0m Back to Main Menu                                                     \033[0;36m║\033[0m\n");
                printf("\033[0;36m╚═══════════════════════════════════════════════════════════════════════════╝\033[0m\n");
                printf("\033[0;33mEnter your choice: \033[0m");
                
                int file_choice;
                scanf("%d", &file_choice);
                clearInputBuffer();
                
                switch(file_choice) {
                    case 1:
                        display_Lnof_header("Student_ESI.bin");
                        break;
                    case 2:
                        display_Lof_header("STUDENTS_CP.BIN");
                        break;
                    case 3:
                        display_TOF_header("Student_INDEX.idx");
                        break;
                    case 0:
                        break;
                    default:
                        printf("\033[0;31m✗ Invalid choice!\033[0m\n");
                }
                break;
            }
            
            case 13: {
                // Display specific block - SUBMENU
                printf("\033[2J\033[H");  // Clear screen
                printf("\n\033[0;36m╔═══════════════════════════════════════════════════════════════════════════╗\033[0m\n");
                printf("\033[0;36m║\033[0;33m                        DISPLAY SPECIFIC BLOCK                             \033[0;36m║\033[0m\n");
                printf("\033[0;36m╠═══════════════════════════════════════════════════════════════════════════╣\033[0m\n");
                printf("\033[0;36m║\033[0m  \033[0;32m1.\033[0m Student_ESI.bin (LNOF - Main Data File)                               \033[0;36m║\033[0m\n");
                printf("\033[0;36m║\033[0m  \033[0;32m2.\033[0m STUDENTS_CP.BIN (LOF - CP Students Only)                              \033[0;36m║\033[0m\n");
                printf("\033[0;36m║\033[0m  \033[0;32m3.\033[0m Student_INDEX.idx (TOF - Primary Key Index)                           \033[0;36m║\033[0m\n");
                printf("\033[0;36m║\033[0m  \033[0;31m0.\033[0m Back to Main Menu                                                     \033[0;36m║\033[0m\n");
                printf("\033[0;36m╚═══════════════════════════════════════════════════════════════════════════╝\033[0m\n");
                printf("\033[0;33mEnter your choice: \033[0m");
                
                int file_choice;
                scanf("%d", &file_choice);
                
                if (file_choice == 0) {
                    break;
                }
                
                int block_num;
                printf("\033[0;33mEnter block number: \033[0m");
                scanf("%d", &block_num);
                clearInputBuffer();
                
                switch(file_choice) {
                    case 1:
                        display_Lnof_block("Student_ESI.bin", block_num);
                        break;
                    case 2:
                        display_Lof_block("STUDENTS_CP.BIN", block_num);
                        break;
                    case 3:
                        display_TOF_block("Student_INDEX.idx", block_num);
                        break;
                    default:
                        printf("\033[0;31m✗ Invalid choice!\033[0m\n");
                }
                break;
            }
                      
            case 14: {
                // Display entire file - SUBMENU
                printf("\033[2J\033[H");  // Clear screen
                printf("\n\033[0;36m╔═══════════════════════════════════════════════════════════════════════════╗\033[0m\n");
                printf("\033[0;36m║\033[0;33m                        DISPLAY ENTIRE FILE                                \033[0;36m║\033[0m\n");
                printf("\033[0;36m╠═══════════════════════════════════════════════════════════════════════════╣\033[0m\n");
                printf("\033[0;36m║\033[0m  \033[0;32m1.\033[0m Student_ESI.bin (LNOF - Main Data File)                               \033[0;36m║\033[0m\n");
                printf("\033[0;36m║\033[0m  \033[0;32m2.\033[0m STUDENTS_CP.BIN (LOF - CP Students Only)                              \033[0;36m║\033[0m\n");
                printf("\033[0;36m║\033[0m  \033[0;32m3.\033[0m Student_INDEX.idx (TOF - Primary Key Index)                           \033[0;36m║\033[0m\n");
                printf("\033[0;36m║\033[0m  \033[0;31m0.\033[0m Back to Main Menu                                                     \033[0;36m║\033[0m\n");
                printf("\033[0;36m╚═══════════════════════════════════════════════════════════════════════════╝\033[0m\n");
                printf("\033[0;33mEnter your choice: \033[0m");
                
                int file_choice;
                scanf("%d", &file_choice);
                clearInputBuffer();
                
                switch(file_choice) {
                    case 1:
                        display_Lnof("Student_ESI.bin");
                        break;
                    case 2:
                        display_Lof("STUDENTS_CP.BIN");
                        break;
                    case 3:
                        display_TOF("Student_INDEX.idx");
                        break;
                    case 0:
                        break;
                    default:
                        printf("\033[0;31m✗ Invalid choice!\033[0m\n");
                }
                break;
            }
            
            case 15: {
                // Display file statistics
                if (!db_loaded) {
                    printf("Please load or create database first!\n");
                    break;
                }
                display_file_statistics("Student_ESI.bin");
                break;
            }

            case 0: {
                if (db_loaded) {
                    if (table.arr) free(table.arr);
                    free(file);
                    free(lof_file);
                }   
                return 0;
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
