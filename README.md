# 🕵️ Offline Google Search Engine (Structured Programming Version)

## Overview
This project is the **first deliverable** of our Object-Oriented Programming (OOP) course.  
It implements an **Offline Google Search Engine** using **structured programming concepts** like arrays, pointers, dynamic memory allocation, and file handling.  
The **next deliverable** will convert this project into a fully **object-oriented version**.

## Features
- 🔍 Search keywords within offline dataset files.
- ⚠️ Notify the user when no results are found and suggest alternative keywords.
- 📁 Modular design:
  - **Data Handling Module:** Manages datasets and file operations.
  - **Search Engine Module:** Handles keyword searching, result matching, and ranking.
  - **User Interface Module:** Provides console-based interaction.
- 📝 Supports both **text** and **binary** file operations for offline usage.

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

shell
Copy code

## Contributors
Javeria Akram – Rep Owner
Ayesha Arif –  contributor
Syed Al Araib –  contributor

bash
Copy code

## Usage
1. Clone the repository:
   ```bash
   git clone https://github.com/Javeriaakram-07/Offline-search-engine.git
Navigate to the project folder and compile:

bash
Copy code
g++ src/main.cpp -o OfflineSearchEngine
Run the executable:

bash
Copy code
./OfflineSearchEngine
Enter keywords to search the offline dataset.

Notes
✅ This version is implemented using structured programming; the OOP version will be done in the next deliverable.

✅ Only arrays, pointers, dynamic memory allocation, and file handling are used. No advanced data structures like vectors.

✅ Modular structure allows multiple contributors to work on separate modules simultaneously.

✅ Proper handling of cases when search results are not found with user-friendly messages