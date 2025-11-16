# 🕵️ Offline Google Search Engine (Structured Programming Version)

## Overview<br>

This project is the **first deliverable** of our Object-Oriented Programming (OOP) course.<br>
It implements an **Offline Google Search Engine** using **structured programming concepts** like arrays, pointers, dynamic memory allocation, and file handling.<br>
The **next deliverable** will convert this project into a fully **object-oriented version**.<br><br>

## Features<br>

- 🔍 Search keywords within offline dataset files.<br>
- ⚠️ Notify the user when no results are found and suggest alternative keywords.<br>
- 📁 Modular design:<br>
  - **Data Handling Module:** Manages datasets and file operations.<br>
  - **Search Engine Module:** Handles keyword searching, result matching, and ranking.<br>
  - **User Interface Module:** Provides console-based interaction.<br>
- 📝 Supports both **text** and **binary** file operations for offline usage.<br><br>

## Project Structure<br>

Offline-search-engine/<br>
├── data/<br>
│ ├── dataset.bin<br>
│ └── filelist.txt<br>
├── include/<br>
│ └── engine.h<br>
├── src/<br>
│ ├── main.cpp<br>
│ ├── module1_data/<br>
│ ├── module2_search/<br>
│ └── module3_ui/<br>
├── LICENSE<br>
└── README.md<br><br>

## Contributors<br>

Javeria Akram - BCSF24A005 – Repo Owner<br>
Syeda Laraib – BCSF24A002 - contributor<br><br>
Ayesha Arif – BCSF24A040 - contributor<br><br>

## Usage<br>

1. Clone the repository:<br>
   ```bash
   git clone https://github.com/Javeriaakram-07/Offline-search-engine.git

   ```
2. Navigate to the project folder and compile:<br>
   ```bash
   g++ src/main.cpp src/module1.cpp src/module2.cpp src/module3.cpp -Iinclude -o searchEngine

   ```
3. Run the executable:<br>
   ```bash
   ./searchEngine   # on Linux/macOS
   searchEngine.exe # on Windows

   ```
4. Enter keywords to search the offline dataset.<br><br>

## Notes<br>

✅ This version is implemented using structured programming; the OOP version will be done in the next deliverable.<br>
✅ Only arrays, pointers, dynamic memory allocation, and file handling are used. No advanced data structures like vectors.<br>
✅ Modular structure allows multiple contributors to work on separate modules simultaneously.<br>
✅ Proper handling of cases when search results are not found with user-friendly messages.<br><br>

---

If you copy-paste this into your `README.md`, **everything will render correctly on GitHub**.<br>

- Headings ✅<br>
- Bullets ✅<br>
- Folder structure ✅<br>
- Contributors and notes ✅<br><br>

I can also make an **even more “professional GitHub-ready” version** with **badges, emojis, and links** to license, contributors, and live demo if you want it to really stand out.<br>
