
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
using namespace std;



struct Book {
    string title;
    string author;
    int year;
};

class IBookCollection {
public:
    virtual void addBook(Book book) = 0;
    virtual vector<Book> findByAuthor(string author) = 0;
    virtual bool removeBook(string title) = 0;
    virtual void showAll() = 0;

    virtual ~IBookCollection() {}
};


class BookCollection : public IBookCollection {
private:
    vector<Book> books;

public:
    void addBook(Book book) override {
        books.push_back(book);
        cout << "Книгу додано!\n";
    }

    vector<Book> findByAuthor(string author) override {
        vector<Book> result;

        for (const Book& book : books) {
            if (book.author == author) {
                result.push_back(book);
            }
        }

        return result;
    }

    bool removeBook(string title) override {
        for (auto it = books.begin(); it != books.end(); ++it) {
            if (it->title == title) {
                books.erase(it);
                return true;
            }
        }

        return false;
    }

    void showAll() override {
        if (books.empty()) {
            cout << "Список книг порожній.\n";
            return;
        }

        for (const Book& book : books) {
            cout << "Назва: " << book.title
                << " | Автор: " << book.author
                << " | Рік: " << book.year << endl;
        }
    }
};

class BookCollectionProxy : public IBookCollection {
private:
    BookCollection collection;
    bool accessAllowed;

public:
    BookCollectionProxy(bool access) {
        accessAllowed = access;
    }

    void addBook(Book book) override {
        if (accessAllowed) {
            cout << "[Proxy] Доступ дозволено.\n";
            collection.addBook(book);
        }
        else {
            cout << "[Proxy] Доступ заборонено!\n";
        }
    }

    vector<Book> findByAuthor(string author) override {
        if (accessAllowed) {
            cout << "[Proxy] Пошук книги...\n";
            return collection.findByAuthor(author);
        }

        cout << "[Proxy] Доступ заборонено!\n";
        return {};
    }

    bool removeBook(string title) override {
        if (accessAllowed) {
            cout << "[Proxy] Видалення книги...\n";
            return collection.removeBook(title);
        }

        cout << "[Proxy] Доступ заборонено!\n";
        return false;
    }

    void showAll() override {
        if (accessAllowed) {
            collection.showAll();
        }
        else {
            cout << "[Proxy] Доступ заборонено!\n";
        }
    }
};


int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    setlocale(LC_ALL, "uk_UA");

    BookCollectionProxy library(true);
    int choice;

    do {
        cout << "\n===== БІБЛІОТЕКА =====\n";
        cout << "1. Додати книгу\n";
        cout << "2. Знайти книгу за автором\n";
        cout << "3. Видалити книгу\n";
        cout << "4. Показати всі книги\n";
        cout << "0. Вихід\n";
        cout << "Ваш вибір: ";

        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            Book book;

            cout << "Назва книги: ";
            getline(cin, book.title);

            cout << "Автор: ";
            getline(cin, book.author);

            cout << "Рік видання: ";
            cin >> book.year;
            cin.ignore();

            library.addBook(book);
        }
        else if (choice == 2) {
            string author;

            cout << "Введіть автора: ";
            getline(cin, author);

            vector<Book> result = library.findByAuthor(author);

            if (result.empty()) {
                cout << "Книг цього автора не знайдено.\n";
            }
            else {
                for (const Book& book : result) {
                    cout << book.title << " | "
                        << book.author << " | "
                        << book.year << endl;
                }
            }
        }
        else if (choice == 3) {
            string title;

            cout << "Введіть назву книги для видалення: ";
            getline(cin, title);

            if (library.removeBook(title)) {
                cout << "Книгу видалено!\n";
            }
            else {
                cout << "Книгу не знайдено.\n";
            }
        }
        else if (choice == 4) {
            library.showAll();
        }

    } while (choice != 0);

    cout << "Програму завершено.\n";

    return 0;
}
