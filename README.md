# 🏆 Treap Contest Participants Manager

This project is a **C++ program** that manages contest participants using a **Treap** data structure. It allows for insertion, deletion, searching, and statistical queries based on player data.

---

## 📌 Table of Contents

- [Features](#-features)
- [What is a Treap?](#-what-is-a-treap)
- [Requirements](#-requirements)
- [Installation & Execution](#-installation--execution)
- [Input File Format](#-input-file-format)
- [Example Input](#-example-input)
- [Program Functionality](#-program-functionality)
- [Technical Details](#-technical-details)
- [License](#-license)

---

## 🎯 Features

✅ Load participants from a file  
✅ Delete participant by `ID` or `signup date`  
✅ Find the k-th best player based on `rank`  
✅ Print participants registered between two timestamps  
✅ Find best player before a certain date  
✅ Efficient operations using a self-balancing Treap

---

## 🌳 What is a Treap?

A **Treap** is a combination of:

- A **Binary Search Tree (BST)** based on `signupTime`
- A **Min-Heap** based on a random `priority` value

This combination guarantees **logarithmic time complexity** for most operations, maintaining a balanced structure with randomized priorities.

---

## ⚙️ Requirements

- C++ compiler (`g++`, `clang++`, etc.)
- Input file: `contest_participants.txt`

---

## 🚀 Installation & Execution

## Compile

```bash
g++ -std=c++11 -o contest treap_program.cpp
```
## 📄 Input File Format
The program reads data from a file named contest_participants.txt, where each line represents a participant in the format:
```text
<Name>,<ID>,<Rank>,<Country>,<SignupTime>
```
ID: unique integer identifier

Rank: smaller is better (1 = best)

SignupTime: integer representing the registration timestamp
## ✏️ Example Input
**contest_participants.txt**
```bash
Alice,1,5,USA,1001
Bob,2,3,Greece,1002
Carol,3,1,Germany,1003
Dave,4,2,France,1004
Eve,5,4,Italy,1005
```
---
## 🔍 Technical Details
- Node class holds each participant's data:
- Name, ID, Rank, Country, Signup Time, Priority
- Treap operations:
- insert(Node*): inserts participant while preserving BST and heap properties
- split(), merge(): used for fast deletion or range operations
- deleteByID(int id): deletes a participant with a specific ID
- deleteBySignupDate(int date): deletes all participants with exact signupTime
- findKthBestRank(k): uses a max-heap to retrieve the k-th best ranked player
- printIDsInRange(t1, t2): in-order traversal to collect IDs
- findBestRankBeforeDate(date): traverses nodes to find minimum rank before a date
---
## 📜 License
This project is free to use for **educational and personal** purposes. You are allowed to:

- Copy, modify, and distribute the code
- Use it in academic or learning projects
- Reference the author when reusing the logic or code
