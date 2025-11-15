# Offline Google Search Engine (Structured Programming Version)

## Overview
This project implements an **Offline Google Search Engine** as the first deliverable of an Object-Oriented Programming (OOP) course.  
The current version is built using **structured programming** concepts, including arrays, dynamic memory allocation, pointers, and file handling.  
The next deliverable will refactor this project into a fully **object-oriented design**.

## Features
- Search for keywords within offline dataset files efficiently.
- Display relevant results or prompt the user to try alternative keywords if no match is found.
- Modular design separated into three main components:
  1. **Data Handling Module** – manages datasets and file operations.
  2. **Search Engine Module** – handles keyword searching, result matching, and ranking.
  3. **User Interface Module** – provides user interaction through console input/output.
- Supports both text and binary file operations for offline functionality.

## Project Structure
Offline-search-engine/
├── data/
│ ├── dataset.bin
│ └── filelist.txt
├── include/
│ └── engine.h
├── src/
│ ├── main.cpp
│ ├── module1_data/
│ ├── module2_search/
│ └── module3_ui/
├── LICENSE
└── README.md

bash
Copy code

## Usage
1. Clone the repository:
   ```bash
   git clone https://github.com/Javeriaakram-07/Offline-search-engine.git
Navigate to the project directory and compile the code using a C++ compiler:

bash
Copy code
g++ src/main.cpp -o OfflineSearchEngine
Run the executable:

bash
Copy code
./OfflineSearchEngine
Enter keywords to search within the offline dataset.

Contributors
Javeria Akram (Repository Owner)

Ayesha Arif

Syed Al Araib

Notes
This version is implemented using structured programming; the OOP version will be released in the next deliverable.

Only arrays, pointers, dynamic memory allocation, and file handling are used. No advanced data structures (like vectors) are included.

The project is modular and scalable, allowing multiple contributors to work on different modules simultaneously.