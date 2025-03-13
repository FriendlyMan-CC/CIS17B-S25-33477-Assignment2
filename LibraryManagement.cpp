#include <iostream>
#include <string>
#include <vector>
using namespace std;


// book class
// represents book's id, title, author, ISBN, and availability

class Book {
public:
    int id;             // unique id for book
    string title;       // book title
    string author;      // book author
    string ISBN;        // isbn # from book
    bool available;     // Availability status (true = available, false = borrowed)

    // constructor to initialize a Book object
    Book(int _id, const string& _title, const string& _author, const string& _ISBN)
        : id(_id), title(_title), author(_author), ISBN(_ISBN), available(true) { }
};


// user Base Class
// represents user with id, name and list of borrowed book ids. 

class User {
public:
    int id;                        // id for user
    string name;                   // User Name
    vector<int> borrowedBooks;     // Book id list that user borrowed

    // Constructor initialize a user object
    User(int _id, const string& _name) : id(_id), name(_name) { }

    // virtual function to return the type of user (overridden in derived classes)
    virtual string getType() { return "User"; }

    // Virtual destructor for proper cleanup in derived classes
    virtual ~User() { }
};


// student class (from User)
// Represents a student user.
class Student : public User {
public:
    // constructor: pass the id and name to the User constructor
    Student(int _id, const string& _name) : User(_id, _name) { }

    // override getType() to return "Student"
    string getType() override { return "Student"; }
};


// faculty class (derived from user)
// represents faculty user.

class Faculty : public User {
public:
    // constructor: Pass the id and name to the User constructor
    Faculty(int _id, const string& _name) : User(_id, _name) { }

    // override getType() to return "faculty"
    string getType() override { return "Faculty"; }
};


// singleton class for library
// manages books, users, and transactions
//implements singleton
class Library {
private:

    // static instance pointer for the Singleton

    static Library* instance;


    // vectors to store pointers to dynamically allocated books and users

    vector<Book*> books;
    vector<User*> users;


    // counters to assign unique IDs to books and users

    int nextBookId;
    int nextUserId;


    // private constructor enforce Singleton pattern.

    Library() : nextBookId(0), nextUserId(0) { }


    // utility function to find a book index by exact title match.
    // returns the index in the books vector or -1 if not found.

    int findBookIndexByTitle(const string& title) {
        for (size_t i = 0; i < books.size(); i++) {
            if (books[i]->title == title)
                return i;
        }
        return -1;
    }


    // utility function to find a user index by user ID.
    // returns the index in the users vector or -1 if not found.

    int findUserIndexById(int id) {
        for (size_t i = 0; i < users.size(); i++) {
            if (users[i]->id == id)
                return i;
        }
        return -1;
    }

public:

    // delete copy constructor and assignment operator to prevent multiple instances.

    Library(const Library&) = delete;
    Library& operator=(const Library&) = delete;

    // static method to get the single instance of Library.

    static Library* getInstance() {
        if (instance == nullptr) {
            instance = new Library();
        }
        return instance;
    }

    // add new book to library.
    // prompts the user for details and adds the book if not cancelled.
    void addBook() {
        cout << "\n>> Add a Book:" << endl;
        string title, author, isbn;
        cout << ">> Enter the Title (0 to cancel): ";
        getline(cin, title);
        if (title == "0") return;

        cout << ">> Enter the Author (0 to cancel): ";
        getline(cin, author);
        if (author == "0") return;

        cout << ">> Enter the ISBN (0 to cancel): ";
        getline(cin, isbn);
        if (isbn == "0") return;

        // directly create a new book
        Book* newBook = new Book(nextBookId++, title, author, isbn);
        books.push_back(newBook);
        cout << "> Book Added with ID " << newBook->id << endl;
    }

    // author, title search
    void searchBooks() {
        cout << "\n>> Search Books:" << endl;
        cout << ">> Enter search term (Title or Author, 0 to cancel): ";
        string term;
        getline(cin, term);
        if (term == "0") return;

        bool found = false;
        for (auto book : books) {
            if (book->title.find(term) != string::npos || book->author.find(term) != string::npos) {
                cout << ">> Book ID: " << book->id << " | Title: " << book->title
                    << " | Author: " << book->author << " | ISBN: " << book->ISBN;
                cout << " | Available: " << (book->available ? "Yes" : "No") << endl;
                found = true;
            }
        }
        if (!found) {
            cout << ">> No books found matching \"" << term << "\"." << endl;
        }
    }


    // list all books in the library.

    void listAllBooks() {
        cout << "\n>> List All Books:" << endl;
        if (books.empty()) {
            cout << ">> No books in the library." << endl;
            return;
        }
        for (auto book : books) {
            cout << ">> Book ID: " << book->id << " | Title: " << book->title
                << " | Author: " << book->author << " | ISBN: " << book->ISBN;
            cout << " | Available: " << (book->available ? "Yes" : "No") << endl;
        }
    }


    // new user registered
    // user's name prompted

    void registerUser() {
        cout << "\n>> Register a New User:" << endl;
        int type;
        while (true) {
            cout << ">> Enter 1 for Student or 2 for Faculty (0 to cancel): ";
            cin >> type;
            if (type == 0) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return;
            }
            if (type == 1 || type == 2) break;
            cout << "!! ERROR: Only valid options are 1 or 2" << endl;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // clear newline

        string name;
        cout << ">> Enter name (0 to cancel): ";
        getline(cin, name);
        if (name == "0") return;

        User* newUser = nullptr;
        try {
            // directly create a new user object without a factory
            if (type == 1) {
                newUser = new Student(nextUserId++, name);
            }
            else if (type == 2) {
                newUser = new Faculty(nextUserId++, name);
            }
            users.push_back(newUser);
            cout << "> User Registered with ID " << newUser->id
                << " (" << newUser->getType() << ")" << endl;
        }
        catch (exception& e) {
            cout << "!! ERROR: " << e.what() << endl;
        }
    }


    // registered users all listed.

    void listAllUsers() {
        cout << "\n>> List All Users:" << endl;
        if (users.empty()) {
            cout << ">> No users registered." << endl;
            return;
        }
        for (auto user : users) {
            cout << ">> User ID: " << user->id << " | Name: " << user->name
                << " | Type: " << user->getType() << endl;
            if (user->borrowedBooks.empty())
                cout << "   >> No borrowed books." << endl;
            else {
                cout << "   >> Borrowed Book IDs: ";
                for (auto id : user->borrowedBooks)
                    cout << id << " ";
                cout << endl;
            }
        }
    }


    // borrow library book
    // check if book exits, available, if user exists. 

    void borrowBook() {
        cout << "\n>> Borrow a Book:" << endl;
        cout << ">> Enter Book Title (0 to cancel): ";
        string title;
        getline(cin, title);
        if (title == "0") return;

        int index = findBookIndexByTitle(title);
        if (index == -1) {
            cout << "!! Error: No book with that title exists !!" << endl;
            return;
        }
        Book* book = books[index];
        if (!book->available) {
            cout << "!! Error: Book is not available !!" << endl;
            return;
        }

        cout << ">> Enter User ID (or x to cancel): ";
        string userInput;
        getline(cin, userInput);
        if (userInput == "x" || userInput == "X") return;
        int userId = stoi(userInput);
        int userIndex = findUserIndexById(userId);
        if (userIndex == -1) {
            cout << "!! Error: No User with that ID Exists !!" << endl;
            return;
        }
        User* user = users[userIndex];
        // mark book as borrowed and record book ID in user's list
        book->available = false;
        user->borrowedBooks.push_back(book->id);
        cout << "> Book \"" << book->title << "\" borrowed by User " << user->id << endl;
    }

    // return borrowed book.
    // check if book exists, borrowed and if user borrowed it.

    void returnBook() {
        cout << "\n>> Return a Book:" << endl;
        cout << ">> Enter Book Title (0 to cancel): ";
        string title;
        getline(cin, title);
        if (title == "0") return;

        int index = findBookIndexByTitle(title);
        if (index == -1) {
            cout << "!! Error: No book with that title exists !!" << endl;
            return;
        }
        Book* book = books[index];
        if (book->available) {
            cout << "!! Error: Book was not borrowed !!" << endl;
            return;
        }

        cout << ">> Enter User ID (or x to cancel): ";
        string userInput;
        getline(cin, userInput);
        if (userInput == "x" || userInput == "X") return;
        int userId = stoi(userInput);
        int userIndex = findUserIndexById(userId);
        if (userIndex == -1) {
            cout << "!! Error: No User with that ID Exists !!" << endl;
            return;
        }
        User* user = users[userIndex];

        // verifying if user borrowed book
        bool found = false;
        for (size_t i = 0; i < user->borrowedBooks.size(); i++) {
            if (user->borrowedBooks[i] == book->id) {
                user->borrowedBooks.erase(user->borrowedBooks.begin() + i);
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "!! Error: This user did not borrow that book !!" << endl;
            return;
        }
        book->available = true;
        cout << "> Book \"" << book->title << "\" returned by User " << user->id << endl;
    }


    // manage books menu.
    // display options like adding books

    void manageBooks() {
        while (true) {
            cout << "\n> Manage Books:" << endl;
            cout << "1. Add a Book" << endl;
            cout << "2. Search Books" << endl;
            cout << "3. List All Books" << endl;
            cout << "4. Go Back" << endl;
            cout << "\nEnter your choice: ";
            int choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (choice == 1)
                addBook();
            else if (choice == 2)
                searchBooks();
            else if (choice == 3)
                listAllBooks();
            else if (choice == 4)
                break;
            else
                cout << "!! ERROR: Invalid choice. Try again." << endl;
        }
    }


    // manage users.
    // display options.

    void manageUsers() {
        while (true) {
            cout << "\n> Manage Users:" << endl;
            cout << "1. Register New User" << endl;
            cout << "2. List All Users" << endl;
            cout << "3. Go Back" << endl;
            cout << "\nEnter your choice: ";
            int choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (choice == 1)
                registerUser();
            else if (choice == 2)
                listAllUsers();
            else if (choice == 3)
                break;
            else
                cout << "!! ERROR: Invalid choice. Try again." << endl;
        }
    }


    // manage transactions (borrowing/returning books).

    void manageTransactions() {
        while (true) {
            cout << "\n> Manage Transactions:" << endl;
            cout << "1. Borrow a Book" << endl;
            cout << "2. Return a Book" << endl;
            cout << "3. Go Back" << endl;
            cout << "\nEnter your choice: ";
            int choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (choice == 1) {
                try {
                    borrowBook();
                }
                catch (exception& e) {
                    cout << "!! Exception: " << e.what() << endl;
                }
            }
            else if (choice == 2) {
                try {
                    returnBook();
                }
                catch (exception& e) {
                    cout << "!! Exception: " << e.what() << endl;
                }
            }
            else if (choice == 3)
                break;
            else
                cout << "!! ERROR: Invalid choice. Try again." << endl;
        }
    }


    // destructor for Library.
    // cleans up dynamically allocated memory for books and users.

    ~Library() {
        for (auto book : books)
            delete book;
        for (auto user : users)
            delete user;
    }
};


// initialize static instance pointer to nullptr.

Library* Library::instance = nullptr;


// main Function
// displays main menu, routes user to appropriate management menu.

int main() {
    Library* lib = Library::getInstance();
    int choice;
    while (true) {
        cout << "\nWelcome to the Library:" << endl;
        cout << "1. Manage Books" << endl;
        cout << "2. Manage Users" << endl;
        cout << "3. Manage Transactions" << endl;
        cout << "4. Exit" << endl;
        cout << "\nEnter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (choice == 1)
            lib->manageBooks();
        else if (choice == 2)
            lib->manageUsers();
        else if (choice == 3)
            lib->manageTransactions();
        else if (choice == 4) {
            cout << "\nThank you for using the Library System!" << endl;
            break;
        }
        else {
            cout << "!! ERROR: Invalid choice. Try again." << endl;
        }
    }
    return 0;
}
