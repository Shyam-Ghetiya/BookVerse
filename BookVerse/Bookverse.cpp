// BookVerse: A Digital Library Management System

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <algorithm>

using namespace std;

// ------------------- Utility Functions -------------------
string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    stringstream ss;
    ss << 1900 + ltm->tm_year << "-" << setw(2) << setfill('0') << 1 + ltm->tm_mon << "-" << setw(2) << setfill('0') << ltm->tm_mday;
    return ss.str();
}


string hashPassword(const string &password) {
    const unsigned int prime = 31;
    unsigned long long hash = 0;
    for (char ch : password) {
        hash = hash * prime + ch;
    }
    stringstream ss;
    ss << hex << hash;
    return ss.str();
}

// ------------------- Book Class -------------------
class Book {
public:
    int bookID;
    string title, author, category;
    int totalCopies, availableCopies;

    Book() {}
    Book(int id, string t, string a, string c, int copies) {
        bookID = id;
        title = t;
        author = a;
        category = c;
        totalCopies = copies;
        availableCopies = copies;
    }

    void display() const {
        cout << "\nBook ID    : " << bookID << "\n";
        cout << "Title      : " << title << "\n";
        cout << "Author     : " << author << "\n";
        cout << "Category   : " << category << "\n";
        cout << "Available  : " << availableCopies << " / " << totalCopies << "\n";
    }
};

// ------------------- Library Class -------------------
class Library {
public:
    static void saveBook(const Book &book);
    static vector<Book> loadBooks();
    static void overwriteBooks(const vector<Book> &books);
    static bool login(string role, string uname, string pass);
    static void registerUser();
};

// ------------------- User Base Class -------------------
class User {
protected:
    string username, password;
public:
    User(string u, string p) : username(u), password(p) {}
    virtual void displayMenu() = 0;
    string getUsername() { return username; }
};

// ------------------- Admin Class -------------------
class Admin : public User {
public:
    Admin(string u, string p) : User(u, p) {}

    void displayMenu() override {
        int choice;
        do {
            cout << "\n--- Admin Menu ---\n";
            cout << "1. Add Book\n2. Delete Book\n3. View All Books\n4. View Borrow Logs\n5. Exit\nChoice: ";
            cin >> choice;
            switch (choice) {
                case 1: addBook(); break;
                case 2: deleteBook(); break;
                case 3: viewBooks(); break;
                case 4: viewLogs(); break;
            }
        } while (choice != 5);
    }

    void addBook() {
        int id, copies;
        string title, author, category;
        cout << "\nEnter Book ID: "; cin >> id;
        cout << "Title: "; cin.ignore(); getline(cin, title);
        cout << "Author: "; getline(cin, author);
        cout << "Category: "; getline(cin, category);
        cout << "Total Copies: "; cin >> copies;

        Book book(id, title, author, category, copies);
        Library::saveBook(book);
        cout << "Book added successfully!\n";
    }

    void deleteBook() {
        int id;
        cout << "\nEnter Book ID to delete: "; cin >> id;
        vector<Book> books = Library::loadBooks();
        bool found = false;
        ofstream out("books.txt");
        for (auto &book : books) {
            if (book.bookID != id)
                out << book.bookID << "," << book.title << "," << book.author << "," << book.category << ","
                    << book.totalCopies << "," << book.availableCopies << "\n";
            else found = true;
        }
        out.close();
        if (found) cout << "Book deleted successfully.\n";
        else cout << "Book not found.\n";
    }

    void viewBooks() {
        vector<Book> books = Library::loadBooks();
        cout << "\n--- Book List ---\n";
        cout << string(85, '-') << endl;
        for (auto &b : books) b.display();
    }


    void viewLogs() {
        ifstream in("borrow.txt");
        string line;
        cout << "\n--- Borrow Logs ---\n";
        cout << left << setw(15) << "Username"
            << setw(10) << "Book ID"
            << setw(15) << "Borrow Date"
            << setw(15) << "Return Date" << endl;
        cout << string(55, '-') << endl;

        while (getline(in, line)) {
            stringstream ss(line);
            string uname, bid, borrowDate, returnDate;
            getline(ss, uname, ',');
            getline(ss, bid, ',');
            getline(ss, borrowDate, ',');
            getline(ss, returnDate);

            cout << left << setw(15) << uname
                << setw(10) << bid
                << setw(15) << borrowDate
                << setw(15) << returnDate << endl;
        }
        in.close();
    }
};

// ------------------- Student Class -------------------
class Student : public User {
public:
    Student(string u, string p) : User(u, p) {}

    void displayMenu() override {
        int choice;
        do {
            cout << "\n--- Student Menu ---\n";
            cout << "1. Search Book\n2. Borrow Book\n3. Return Book\n4. View Borrowing History\n5. Exit\nChoice: ";
            cin >> choice;
            switch (choice) {
                case 1: searchBook(); break;
                case 2: borrowBook(); break;
                case 3: returnBook(); break;
                case 4: viewHistory(); break;
            }
        } while (choice != 5);
    }

void searchBook() {
    vector<Book> books = Library::loadBooks();
    if (books.empty()) {
        cout << "\n📂 No books in the library.\n";
        return;
    }

    int option;
    cout << "\n--- Search Book ---\n";
    cout << "1. Select Topic\n2. Manual Search\nEnter your choice: ";
    cin >> option;

    // cin.ignore(); // Clear input buffer

    if (option == 1) {
        // Get unique categories
        vector<string> categories;
        for (auto &b : books) {
            if (find(categories.begin(), categories.end(), b.category) == categories.end()) {
                categories.push_back(b.category);
            }
        }

        // Show available topics
        cout << "\n📚 Available Topics:\n";
        for (int i = 0; i < categories.size(); i++) {
            cout << i + 1 << ". " << categories[i] << "\n";
        }

        int topicChoice;
        cout << "Select a topic number: ";
        cin >> topicChoice;

        if (topicChoice < 1 || topicChoice > categories.size()) {
            cout << "❌ Invalid topic choice.\n";
            return;
        }

        string selectedCategory = categories[topicChoice - 1];
        bool found = false;

        cout << "\n📘 Books under '" << selectedCategory << "':\n";
        cout << string(85, '-') << endl;
        for (auto &b : books) {
            if (b.category == selectedCategory) {
                b.display();
                found = true;
            }
        }
        if (!found) cout << "❌ No books found under this topic.\n";
    }

    else if (option == 2) {
        string keyword;
        cout << "\nEnter keyword (title/author/category): \n";
        cin.ignore();
        getline(cin, keyword);

        bool found = false;
        cout << "\n🔍 Search Results:\n";
        cout << string(85, '-') << endl;
        for (auto &b : books) {
            if (b.title.find(keyword) != string::npos || 
                b.author.find(keyword) != string::npos || 
                b.category.find(keyword) != string::npos) {
                b.display();
                found = true;
            }
        }
        if (!found) cout << "❌ No matching books found.\n";
    }

    else {
        cout << "❌ Invalid option.\n";
    }
}


    void borrowBook() {
        int id;
        cout << "\nEnter Book ID to borrow: "; cin >> id;
        vector<Book> books = Library::loadBooks();
        bool found = false;
        for (auto &b : books) {
            if (b.bookID == id && b.availableCopies > 0) {
                b.availableCopies--;
                found = true;
                ofstream log("borrow.txt", ios::app);
                log << getUsername() << "," << id << "," << getCurrentDate() << "," << "-" << endl;
                log.close();
                cout << "Book borrowed successfully.\n";
                break;
            }
        }
        Library::overwriteBooks(books);
        if (!found) cout << "Book not available or not found.\n";
    }

    void returnBook() {
        int id;
        cout << "\nEnter Book ID to return: "; cin >> id;
        vector<Book> books = Library::loadBooks();
        for (auto &b : books) {
            if (b.bookID == id) {
                b.availableCopies++;
                break;
            }
        }
        Library::overwriteBooks(books);
        ofstream out("borrow.txt", ios::app);
        out << getUsername() << "," << id << ",-," << getCurrentDate() << endl;
        out.close();
        cout << "Book returned successfully.\n";
    }

    void viewHistory() {
        ifstream in("borrow.txt");
        string line;
        cout << "\n--- Your Borrowing History ---\n";
        cout << left << setw(10) << "Book ID"
            << setw(20) << "Borrow Date"
            << setw(20) << "Return Date" << endl;
        cout << string(50, '-') << endl;

        while (getline(in, line)) {
            stringstream ss(line);
            string uname, bid, borrowDate, returnDate;
            getline(ss, uname, ',');
            getline(ss, bid, ',');
            getline(ss, borrowDate, ',');
            getline(ss, returnDate);

            if (uname == getUsername()) {
                cout << left << setw(10) << bid
                    << setw(20) << borrowDate
                    << setw(20) << returnDate << endl;
            }
        }
        in.close();
    }

};

// ------------------- Library Class -------------------
void Library::saveBook(const Book &book) {
    ofstream out("books.txt", ios::app);
    out << book.bookID << "," << book.title << "," << book.author << ","
        << book.category << "," << book.totalCopies << "," << book.availableCopies << "\n";
    out.close();
}

vector<Book> Library::loadBooks() {
    vector<Book> books;
    ifstream in("books.txt");
    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        string id, title, author, category, total, available;
        getline(ss, id, ','); getline(ss, title, ','); getline(ss, author, ',');
        getline(ss, category, ','); getline(ss, total, ','); getline(ss, available);
        books.push_back(Book(stoi(id), title, author, category, stoi(total)));
        books.back().availableCopies = stoi(available);
    }
    in.close();
    return books;
}

void Library::overwriteBooks(const vector<Book> &books) {
    ofstream out("books.txt");
    for (auto &b : books) {
        out << b.bookID << "," << b.title << "," << b.author << ","
            << b.category << "," << b.totalCopies << "," << b.availableCopies << "\n";
    }
    out.close();
}

bool Library::login(string role, string uname, string pass) {
   
    ifstream in("users.txt");
    string line;

    while (getline(in, line)) {
        stringstream ss(line);
        string r, u, p;
        getline(ss, r, ','); getline(ss, u, ','); getline(ss, p);

        string hashedInputPass = hashPassword(pass);
        if (r == role && u == uname && p == hashedInputPass) return true;
    }
    return false;
}


void Library::registerUser() {
    string role, uname, pass;
    cout << "\n--- Register New User ---\n";
    cout << "Enter role (Admin/Student): "; cin >> role;
    cout << "Enter username: "; cin >> uname;
    cout << "Enter password: "; cin >> pass;

    ofstream out("users.txt", ios::app);
    out << role << "," << uname << "," << hashPassword(pass) << endl;
    out.close();
    cout << "✅ User registered successfully! You can now login.\n";
}

int main() {
    cout << "📚 Welcome to BookVerse: Digital Library System 📚\n";
    int choice;
    string role, username, password;

    cout << "\n1. Login\n2. Register\nEnter choice: ";
    cin >> choice;

    if (choice == 2) {
        Library::registerUser();
    }

    cout << "\nEnter role (Admin/Student): ";
    cin >> role;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    if (Library::login(role, username, password)) {
        if (role == "Admin" || role == "admin") {
            Admin admin(username, password);
            admin.displayMenu();
        } else if (role == "Student" || role == "student") {
            Student student(username, password);
            student.displayMenu();
        } else {
            cout << "❌ Invalid role.\n";
        }
    } else {
        cout << "❌ Login failed. Please check your credentials.\n";
    }

    cout << "👋 Thank you for using BookVerse!\n";
    return 0;
}

// ------------------- End of Program -------------------