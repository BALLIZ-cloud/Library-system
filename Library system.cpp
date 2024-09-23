
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Struct to manage book data
struct add_book {
    vector<int> id;
    vector<string> name;
    vector<int> quantity;
    vector<int> total_quantity;  // Track the total quantity for borrowed count

    // Function to add a book
    void add() {
        int k;
        string g;
        int l;
        cout << "Enter book ID & name & quantity: ";
        cin >> k >> g >> l;
        id.push_back(k);
        name.push_back(g);
        quantity.push_back(l);
        total_quantity.push_back(l);  // Initialize total quantity with the same value
    }

    // Function to display all books
    void list_books() {
        for (size_t i = 0; i < id.size(); ++i) {
            int borrowed = total_quantity[i] - quantity[i];  // Calculate total borrowed
            cout << "ID: " << id[i] << ", Name: " << name[i]
                 << ", Quantity: " << quantity[i]
                 << ", Borrowed: " << borrowed << endl;
        }
    }

    // Find a book by name
    int find_book(string book_name) {
        for (size_t i = 0; i < name.size(); ++i) {
            if (name[i] == book_name) {
                return i;  // Return the index of the book if found
            }
        }
        return -1;  // Return -1 if not found
    }
};

// Struct to manage user data
struct add_user {
    int id;
    string name, e_mail, address;
    vector<string> borrowed_books;  // Keep track of borrowed books

    // Function to borrow a book
    void borrow_book(add_book &library) {
        string book_name;
        cout << "Enter the name of the book to borrow: ";
        cin >> book_name;

        // Find the book in the library
        int book_index = library.find_book(book_name);
        if (book_index != -1 && library.quantity[book_index] > 0) {
            borrowed_books.push_back(book_name);
            library.quantity[book_index]--;  // Decrease the quantity
            cout << "Book '" << book_name << "' borrowed successfully!" << endl;
        } else if (book_index != -1) {
            cout << "Sorry, the book '" << book_name << "' is out of stock." << endl;
        } else {
            cout << "Book not found in the library." << endl;
        }
    }

    // Function to return a book
    void return_book(add_book &library) {
        string book_name;
        cout << "Enter the name of the book to return: ";
        cin >> book_name;

        // Check if the user has borrowed this book
        auto it = find(borrowed_books.begin(), borrowed_books.end(), book_name);
        if (it != borrowed_books.end()) {
            // Find the book in the library
            int book_index = library.find_book(book_name);
            if (book_index != -1) {
                borrowed_books.erase(it);  // Remove the book from the user's borrowed list
                library.quantity[book_index]++;  // Increase the quantity in the library
                cout << "Book '" << book_name << "' returned successfully!" << endl;
            }
        } else {
            cout << "You haven't borrowed this book." << endl;
        }
    }

    // Print the user's borrowed books
    void print_borrowed_books() {
        cout << "Books borrowed by " << name << ":" << endl;
        for (const auto &book : borrowed_books) {
            cout << "- " << book << endl;
        }
    }
};

// Global objects for storing books and users
add_book library;
vector<add_user> users;

// Function to add a user
void add_new_user() {
    add_user user;
    cout << "Enter user ID: "; cin >> user.id;
    cout << "Enter user name: "; cin >> user.name;
    cout << "Enter user email: "; cin >> user.e_mail;
    cout << "Enter user address: "; cin >> user.address;
    users.push_back(user);
}

// Function to print all users
void print_users() {
    for (auto &user : users) {
        cout << "User ID: " << user.id << ", Name: " << user.name
             << ", Email: " << user.e_mail << ", Address: " << user.address << endl;
        user.print_borrowed_books();
    }
}

// Menu system
void menu() {
    int choice = 0;
    while (true) {
        cout << "\nLibrary Menu:\n";
        cout << "1) Add book\n";
        cout << "2) Search books by prefix\n";
        cout << "3) Print who borrowed books by name\n";
        cout << "4) Print library by ID\n";
        cout << "5) Print library by name\n";
        cout << "6) Add user\n";
        cout << "7) User borrow book\n";
        cout << "8) User return book\n";
        cout << "9) Print users\n";
        cout << "10) Exit\n";
        cout << "Select your menu choice: ";
        cin >> choice;

        if (choice == 1) {
            library.add();
        }
        else if (choice == 2) {
            string prefix;
            cout << "Enter book name prefix: ";
            cin >> prefix;
            for (const auto &book : library.name) {
                if (book.rfind(prefix, 0) == 0) {
                    cout << "Book: " << book << endl;
                }
            }
        }
        else if (choice == 3) {
            string book_name;
            cout << "Enter book name: ";
            cin >> book_name;

            cout << "Users who borrowed '" << book_name << "':" << endl;
            for (const auto &user : users) {
                if (find(user.borrowed_books.begin(), user.borrowed_books.end(), book_name) != user.borrowed_books.end()) {
                    cout << user.name << endl;
                }
            }
        }
        else if (choice == 4) {
            cout << "Library sorted by ID:" << endl;
            vector<pair<int, int>> books;
            for (size_t i = 0; i < library.id.size(); ++i) {
                int borrowed = library.total_quantity[i] - library.quantity[i];
                books.push_back({library.id[i], i});  // Store index for later access
            }
            sort(books.begin(), books.end());

            for (const auto &book : books) {
                int idx = book.second;
                int borrowed = library.total_quantity[idx] - library.quantity[idx];
                cout << "ID: " << library.id[idx] << ", Name: " << library.name[idx]
                     << ", Quantity: " << library.quantity[idx]
                     << ", Borrowed: " << borrowed << endl;
            }
        }
        else if (choice == 5) {
            library.list_books();
        }
        else if (choice == 6) {
            add_new_user();
        }
        else if (choice == 7) {
            if (users.empty()) {
                cout << "No users available. Add a user first." << endl;
            } else {
                int user_id;
                cout << "Enter user ID to borrow a book: ";
                cin >> user_id;
                auto it = find_if(users.begin(), users.end(), [&user_id](const add_user &user) {
                    return user.id == user_id;
                });

                if (it != users.end()) {
                    it->borrow_book(library);
                } else {
                    cout << "User not found." << endl;
                }
            }
        }
        else if (choice == 8) {
            if (users.empty()) {
                cout << "No users available. Add a user first." << endl;
            } else {
                int user_id;
                cout << "Enter user ID to return a book: ";
                cin >> user_id;
                auto it = find_if(users.begin(), users.end(), [&user_id](const add_user &user) {
                    return user.id == user_id;
                });

                if (it != users.end()) {
                    it->return_book(library);
                } else {
                    cout << "User not found." << endl;
                }
            }
        }
        else if (choice == 9) {
            print_users();
        }
        else if (choice == 10) {
            cout << "Exiting..." << endl;
            break;
        }
        else {
            cout << "Invalid choice, try again." << endl;
        }
    }
}

int main() {
    menu();
    return 0;
}


