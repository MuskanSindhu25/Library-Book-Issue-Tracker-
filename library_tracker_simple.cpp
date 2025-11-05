#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

// Structure to represent a Book
struct Book {
    int id;
    string title;
    string author;
    bool isIssued;
    string issuedTo;
    string issueDate;
    
    // Constructor
    Book() : id(0), isIssued(false) {}
    
    Book(int _id, string _title, string _author) 
        : id(_id), title(_title), author(_author), isIssued(false) {}
};

class LibraryTracker {
private:
    vector<Book> books;
    const string filename = "books_cpp.dat";

public:
    LibraryTracker() {
        loadBooks();
    }
    
    ~LibraryTracker() {
        saveBooks();
    }
    
    void addBook() {
        int id;
        string title, author;
        
        cout << "\n===== Add New Book =====" << endl;
        cout << "Enter Book ID: ";
        cin >> id;
        cin.ignore(); // Clear the newline from buffer
        
        // Check if ID already exists
        for (const auto& book : books) {
            if (book.id == id) {
                cout << "Book with this ID already exists!" << endl;
                return;
            }
        }
        
        cout << "Enter Book Title: ";
        getline(cin, title);
        
        cout << "Enter Author Name: ";
        getline(cin, author);
        
        books.emplace_back(id, title, author);
        cout << "Book added successfully!" << endl;
    }
    
    void displayBooks() const {
        cout << "\n===== All Books =====" << endl;
        if (books.empty()) {
            cout << "No books in the library." << endl;
            return;
        }
        
        cout << "ID\tTitle\t\t\tAuthor\t\t\tStatus" << endl;
        cout << "------------------------------------------------------------------------" << endl;
        
        for (const auto& book : books) {
            cout << book.id << "\t" << book.title << "\t\t" << book.author << "\t\t";
            
            if (book.isIssued) {
                cout << "Issued to " << book.issuedTo << " on " << book.issueDate;
            } else {
                cout << "Available";
            }
            cout << endl;
        }
    }
    
    void issueBook() {
        int book_id;
        cout << "\n===== Issue Book =====" << endl;
        cout << "Enter Book ID to issue: ";
        cin >> book_id;
        cin.ignore(); // Clear the newline from buffer
        
        for (auto& book : books) {
            if (book.id == book_id) {
                if (book.isIssued) {
                    cout << "Book is already issued to " << book.issuedTo 
                         << " on " << book.issueDate << endl;
                    return;
                }
                
                string name, date;
                cout << "Enter your name: ";
                getline(cin, name);
                
                cout << "Enter issue date (DD/MM/YYYY): ";
                getline(cin, date);
                
                book.isIssued = true;
                book.issuedTo = name;
                book.issueDate = date;
                
                cout << "Book issued successfully to " << name 
                     << " on " << date << endl;
                return;
            }
        }
        
        cout << "Book with ID " << book_id << " not found." << endl;
    }
    
    void returnBook() {
        int book_id;
        cout << "\n===== Return Book =====" << endl;
        cout << "Enter Book ID to return: ";
        cin >> book_id;
        
        for (auto& book : books) {
            if (book.id == book_id) {
                if (!book.isIssued) {
                    cout << "Book is not currently issued." << endl;
                    return;
                }
                
                cout << "Book '" << book.title << "' returned by " 
                     << book.issuedTo << endl;
                book.isIssued = false;
                book.issuedTo = "";
                book.issueDate = "";
                return;
            }
        }
        
        cout << "Book with ID " << book_id << " not found." << endl;
    }
    
    void saveBooks() const {
        ofstream file(filename, ios::binary);
        if (!file) {
            cout << "Error opening file for writing." << endl;
            return;
        }
        
        size_t count = books.size();
        file.write(reinterpret_cast<const char*>(&count), sizeof(count));
        
        for (const auto& book : books) {
            file.write(reinterpret_cast<const char*>(&book), sizeof(Book));
        }
        
        cout << "Books saved to file successfully." << endl;
    }
    
    void loadBooks() {
        ifstream file(filename, ios::binary);
        if (!file) {
            cout << "No existing data file found. Starting fresh." << endl;
            return;
        }
        
        size_t count;
        file.read(reinterpret_cast<char*>(&count), sizeof(count));
        
        books.clear();
        books.reserve(count);
        
        for (size_t i = 0; i < count; ++i) {
            Book book;
            file.read(reinterpret_cast<char*>(&book), sizeof(Book));
            books.push_back(book);
        }
        
        cout << "Loaded " << books.size() << " books from file." << endl;
    }
};

int main() {
    LibraryTracker tracker;
    int choice;
    
    while (true) {
        cout << "\n===== Library Book Issue Tracker =====" << endl;
        cout << "1. Add New Book" << endl;
        cout << "2. Display All Books" << endl;
        cout << "3. Issue Book" << endl;
        cout << "4. Return Book" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1: tracker.addBook(); break;
            case 2: tracker.displayBooks(); break;
            case 3: tracker.issueBook(); break;
            case 4: tracker.returnBook(); break;
            case 5: 
                cout << "Exiting program. Goodbye!" << endl;
                return 0;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
    
    return 0;
}