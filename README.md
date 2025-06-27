# 📚 Bookverse – A Digital Library System in C++

## 📌 Overview
**Bookverse** is a console-based digital library system developed in **C++** using **Object-Oriented Programming (OOP)** principles and **file I/O** operations. The system supports managing books and user activities like borrowing and returning books. It simulates a functional digital library **without using any database**, relying entirely on file storage (CSV format).

## 👥 User Roles

- 👨‍🎓 **Student**
- 🛠️ **Admin**

Each user type has specific access and functionalities.

## 🚀 Key Features

### 🔐 Authentication
- Registration and login functionality.
- Passwords are stored securely using hashing.

### 👨‍💼 Admin Functionalities
- ➕ Add a book
- ❌ Delete a book
- 📚 View all books
- 📄 View borrow logs (history of borrow/return actions)

### 👨‍🎓 Student Functionalities
- 🔍 Search books by:
  - Category
  - Title/Name
  - Author
- 📖 Borrow a book (with real-time borrow date tracking)
- 📥 Return a book (real-time return date tracking)
- 📘 View personal borrow history

## 💾 File Handling

- Uses `fstream` and `ofstream` for all I/O operations.
- Data is stored and updated in **CSV (Comma Separated Values)** format.
- Book records, user details, and logs are persistently stored.

## 🧠 OOP Concepts Used

- 🔁 **Polymorphism**
- 🧬 **Inheritance**
- 🔒 **Encapsulation**
- 📌 **Static members/functions**
- 📂 Classes like `Book`, `User`, `Admin`, `Student`, `LibrarySystem`

## 🧱 System Design

- Real-time borrow and return date tracking.
- Separation of concerns between Admin and Student modules.
- Password hashing implemented for secure user storage.

## 📚 Learnings

- Applied core **OOP principles** in a real-world scenario.
- Understood and implemented **file-based data storage**.
- Practiced **input/output formatting** for better user interaction.
- Learned to manage **role-based access control** in C++.

## 📌 Future Improvements

- GUI using C++ frameworks or migrate to web-based UI.
- Switch from file storage to database (e.g., SQLite or MySQL).
- Add features like late fee calculation, book reservation, or overdue alerts.

---

📁 *All source code, CSV data files are included in the repository.*
