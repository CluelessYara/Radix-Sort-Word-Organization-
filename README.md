# Radix Sort Word Organizer

A C-language project that reads strings from a file, sorts them using Radix Sort (character-based), and manages them using doubly linked lists. It offers an interactive menu for viewing, sorting, adding, deleting, and saving strings.

---

## Features

- Sorts strings using **Radix Sort** based on ASCII characters.
- Uses **doubly linked lists** and an **array of linked lists** to manage data.
- Pads strings dynamically to ensure uniform comparison length.
- Allows the user to:
  - Load words from a text file.
  - View words before and after sorting.
  - Add new words and auto-sort.
  - Delete existing words.
  - Save sorted words to a new file.
- Validates that words don’t start with a number.
- Includes memory management and input parsing.

---

## Concepts Used

- Radix Sort (character-based)
- Doubly Linked Lists
- Dynamic Memory Allocation
- File I/O (Reading and Writing)
- String Padding
- Menu-Driven CLI
- ASCII-based bucketing (128 total possible characters)

---

## How to Use

 **Compile the program:**

   ```bash
   gcc -o radix_sort radix_sort.c
