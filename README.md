# Student File Management System

A comprehensive file management system for student records implemented in C, featuring multiple file organization strategies (LNOF, LOF, TOF) and efficient query capabilities.

## 📋 Overview

This project implements a complete student database management system with:
- **LNOF (Linked Normalized File)**: Main data storage with linked blocks
- **LOF (List of Files)**: Filtered student records (CP students only)
- **TOF (Table of Keys File)**: Efficient indexing for quick lookups
- **Query Indexes**: Secondary indexes for blood type, speciality, year, and birth year

## 🏗️ Architecture

### File Types
- **Student_ESI.bin** (LNOF): Primary student database with linked blocks
- **Student_INDEX.idx** (TOF): Primary key index for fast student lookup
- **STUDENTS_CP.BIN** (LOF): Contains only CP (1CP/2CP) students
- **Query Indexes**: Blood_IDX.idx, Spec_IDX.idx, Year_IDX.idx, Birth_IDX.idx

### Data Structures
- **Student**: Complete student record with all personal/academic information
- **Index**: Dynamic array for primary key indexing
- **Query_index**: Linked lists for secondary indexing
- **Blocks**: Fixed-size blocks (40 records) with linking capabilities

## ✨ Features

### Core Operations
1. **Database Creation** - Generate random student records
2. **Student Management** - Insert, delete, modify students
3. **Search Capabilities** - By ID, blood type, speciality, year, birth year
4. **File Operations** - Create/maintain LOF files for specific student groups
5. **Index Management** - Automatic index creation and maintenance

### Advanced Features
- **Binary Search** for efficient primary key lookups
- **Linked List Indexes** for multi-criteria queries
- **Block Optimization** with 70% load factor for TOF files
- **Memory Management** with dynamic allocation
- **File Persistence** with automatic save/load

## 📁 Project Structure

```
├── library.h          # Header file with all declarations and constants
├── library.c          # Implementation of all file operations and utilities
├── main.c            # User interface and display functions
├── Student_ESI.bin   # Main database (generated at runtime)
├── Student_INDEX.idx # Primary index (generated at runtime)
└── Query Indexes/    # Secondary indexes (generated at runtime)
```

## 🚀 Getting Started

### Prerequisites
- GCC compiler
- Standard C libraries

### Compilation
```bash
gcc -o student_manager main.c library.c -std=c99
```

### Usage
```bash
./student_manager
```

## 🎮 Menu Options

### Database Operations
1. **Create New Database** - Generate new student records
2. **Load Existing Database** - Load previously saved data
3. **Insert New Student** - Add a student to the database
4. **Search Student by ID** - Find student using primary key
5. **Delete Student** - Remove student from database
6. **Modify Student First Name** - Update student information

### Query Operations
7. **Display by Blood Type** - Filter students by blood group
8. **Display by Speciality** - Filter by academic speciality
9. **Display by Year of Study** - Filter by study year
10. **Display by Birth Year** - Filter by birth year range

### File Operations
11. **Create STUDENTS_CP.BIN** - Generate file with CP students only
12. **Display File Headers** - View file metadata
13. **Display Specific Block** - Examine individual blocks
14. **Display Entire File** - View complete file contents
15. **Display File Statistics** - Get file metrics and performance data

## 📊 Technical Details

### Constants
- `MAX_RECORDS_PER_BLOCK`: 40 records
- `MIN_STUDENT_ID`: 1000
- `MAX_STUDENT_ID`: 9000
- `MIN_BIRTH_YEAR`: 2003
- `MAX_BIRTH_YEAR`: 2008

### Indexing Strategy
- **Primary Index**: Sorted array with binary search (O(log n))
- **Secondary Indexes**: Bucketed linked lists for categorical queries
- **Automatic Updates**: All indexes updated on insert/delete operations

### File Formats
- **LNOF**: Header (head, current) + linked blocks of Student records
- **TOF**: Header (nblc, nrec) + blocks of index cells (key, block, offset)
- **LOF**: Similar to LNOF but with additional deletion flags

## 🔧 Performance

- **Block I/O Optimized**: Minimized disk reads through caching
- **Memory Efficient**: Dynamic allocation with capacity growth
- **Fast Queries**: Multiple indexing strategies for different query types
- **Load Balancing**: TOF blocks maintain 70% load factor for efficiency

## 📝 Notes

- The system uses pseudo-random data generation for testing
- All files are in binary format for efficient storage
- Index files are automatically regenerated if missing
- Block numbers start from 1 (not 0)
- File headers are updated on close operations

## 🤝 Contributing

Feel free to fork the project and submit pull requests for:
- Bug fixes
- Performance improvements
- Additional features
- Documentation enhancements

## 📄 License

This project is available for educational purposes.

## 🎓 Educational Value

This project demonstrates:
- File organization strategies
- Indexing techniques
- Memory management
- Binary file operations
- Database design principles
- Query optimization

---

*Developed as part of a File Structures & External Data Structures practical work*
