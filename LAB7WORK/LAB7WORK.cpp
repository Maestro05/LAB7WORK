﻿#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>  // Для алгоритмов сортировки и поиска

// Базовый класс Author
class Author {
private:
    std::string name;
    std::string surname;
    std::string birthdate;

public:
    Author() : name(""), surname(""), birthdate("") {}

    Author(const Author& other)
        : name(other.name), surname(other.surname), birthdate(other.birthdate) {
        std::cout << "Конструктор копии автора: " << name << " " << surname << std::endl;
    }

    void input() {
        std::cout << "Введите имя автора: ";
        std::getline(std::cin, name);
        std::cout << "Введите фамилию автора: ";
        std::getline(std::cin, surname);
        std::cout << "Введите дату рождения (DD.MM.YYYY): ";
        std::getline(std::cin, birthdate);
    }

    const std::string& getName() const { return name; }
    const std::string& getSurname() const { return surname; }
    const std::string& getBirthdate() const { return birthdate; }

    void print() const {
        std::cout << "Автор: " << name << " " << surname << ", Дата рождения: " << birthdate << std::endl;
    }
};

// Класс категории
class Category {
private:
    std::string name;
    std::string description;

public:
    Category() : name(""), description("") {}

    void input() {
        std::cout << "Введите название категории: ";
        std::getline(std::cin, name);
        std::cout << "Введите описание категории: ";
        std::getline(std::cin, description);
    }

    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }

    void print() const {
        std::cout << "Категория: " << name << ", Описание: " << description << std::endl;
    }
};

// Абстрактный базовый класс для объектов, которые можно выдать
class Lendable {
public:
    virtual void decreaseCopies() = 0;  // Чисто виртуальная функция для уменьшения доступных копий
    virtual void print() const = 0;     // Чисто виртуальная функция для вывода информации
    virtual ~Lendable() = default;
};

// Базовый класс Book, реализующий Lendable
class Book : public Lendable {
protected:
    std::string title;
    Author author;
    Category category;
    int year;
    int copiesAvailable;

public:
    Book() : title(""), year(0), copiesAvailable(0) {}

    virtual void input() {
        std::cout << "Введите название книги: ";
        std::getline(std::cin, title);
        author.input();
        category.input();

        std::cout << "Введите год издания: ";
        std::cin >> year;
        std::cout << "Введите количество доступных копий: ";
        std::cin >> copiesAvailable;
        std::cin.ignore();  // Очищаем буфер после ввода чисел
    }

    const std::string& getTitle() const { return title; }
    const Author& getAuthor() const { return author; }
    const Category& getCategory() const { return category; }
    int getYear() const { return year; }
    int getCopiesAvailable() const { return copiesAvailable; }

    virtual void decreaseCopies() override {
        if (copiesAvailable > 0) {
            --copiesAvailable;
        }
        else {
            throw std::runtime_error("Нет доступных копий для выдачи!");
        }
    }

    virtual void print() const override {
        std::cout << "Книга: " << title << ", Год: " << year << ", Доступных копий: " << copiesAvailable << std::endl;
        author.print();
        category.print();
    }

    // Перегрузка оператора присваивания
    Book& operator=(const Book& other) {
        if (this != &other) {
            title = other.title;
            author = other.author;
            category = other.category;
            year = other.year;
            copiesAvailable = other.copiesAvailable;
        }
        return *this;
    }
};

// Производный класс для научных книг
class ScienceBook : public Book {
private:
    std::string researchField;  // Научная область

public:
    ScienceBook() : Book(), researchField("") {}

    ScienceBook(const ScienceBook& other) : Book(other), researchField(other.researchField) {}

    void input() override {
        Book::input();
        std::cout << "Введите научную область: ";
        std::getline(std::cin, researchField);
    }

    void print() const override {
        Book::print();
        std::cout << "Научная область: " << researchField << std::endl;
    }
};

// Производный класс для художественных книг
class FictionBook : public Book {
private:
    std::string genre;  // Жанр

public:
    FictionBook() : Book(), genre("") {}

    FictionBook(const FictionBook& other) : Book(other), genre(other.genre) {}

    void input() override {
        Book::input();
        std::cout << "Введите жанр книги: ";
        std::getline(std::cin, genre);
    }

    void print() const override {
        Book::print();
        std::cout << "Жанр: " << genre << std::endl;
    }
};

// Класс читателя
class Reader {
private:
    std::string name;
    std::string surname;
    std::string cardNumber;

public:
    Reader() : name(""), surname(""), cardNumber("") {}

    void input() {
        std::cout << "Введите имя читателя: ";
        std::getline(std::cin, name);
        std::cout << "Введите фамилию читателя: ";
        std::getline(std::cin, surname);
        std::cout << "Введите номер читательского билета: ";
        std::getline(std::cin, cardNumber);
    }

    void print() const {
        std::cout << "Читатель: " << name << " " << surname << ", Номер карты: " << cardNumber << std::endl;
    }
};

// Класс библиотеки
class LibrarySystem {
private:
    std::vector<Book*> books;  // Список книг в библиотеке

public:
    ~LibrarySystem() {
        for (auto book : books) {
            delete book;  // Освобождаем память
        }
    }

    void addBook() {
        int choice;
        std::cout << "Выберите тип книги:\n1. Научная\n2. Художественная\n";
        std::cin >> choice;
        std::cin.ignore();

        Book* newBook = nullptr;
        if (choice == 1) {
            newBook = new ScienceBook();
        }
        else if (choice == 2) {
            newBook = new FictionBook();
        }
        else {
            std::cout << "Неверный выбор.\n";
            return;
        }

        newBook->input();
        books.push_back(newBook);
        std::cout << "Книга успешно добавлена в библиотеку!" << std::endl;
    }

    // Алгоритм сортировки: сортируем книги по названию (title)
    void sortBooks() {
        std::sort(books.begin(), books.end(), [](Book* a, Book* b) {
            return a->getTitle() < b->getTitle();
            });
        std::cout << "Книги отсортированы по названию!" << std::endl;
    }

    // Алгоритм поиска: находим книгу по названию
    void findBookByTitle() {
        std::string title;
        std::cout << "Введите название книги для поиска: ";
        std::getline(std::cin, title);

        auto it = std::find_if(books.begin(), books.end(), [&title](Book* book) {
            return book->getTitle() == title;
            });

        if (it != books.end()) {
            std::cout << "Книга найдена:\n";
            (*it)->print();
        }
        else {
            std::cout << "Книга не найдена.\n";
        }
    }

    void showBooks() const {
        if (books.empty()) {
            std::cout << "Нет доступных книг в библиотеке." << std::endl;
        }
        else {
            std::cout << "\nСписок книг в библиотеке:" << std::endl;
            for (const auto& book : books) {
                book->print();
            }
        }
    }

    void showMenu() {
        int choice;
        do {
            try {
                std::cout << "\n--- Меню библиотеки ---" << std::endl;
                std::cout << "1. Добавить книгу" << std::endl;
                std::cout << "2. Выдать книгу" << std::endl;
                std::cout << "3. Показать все книги" << std::endl;
                std::cout << "4. Сортировать книги" << std::endl;
                std::cout << "5. Найти книгу по названию" << std::endl;
                std::cout << "6. Выход" << std::endl;
                std::cout << "Введите ваш выбор: ";
                std::cin >> choice;
                std::cin.ignore();  // Очищаем буфер после ввода числа

                if (choice < 1 || choice > 6) {
                    throw std::out_of_range("Неверный выбор. Попробуйте снова.");
                }

                switch (choice) {
                case 1:
                    addBook();
                    break;
                case 2:
                    issueBook();
                    break;
                case 3:
                    showBooks();
                    break;
                case 4:
                    sortBooks();
                    break;
                case 5:
                    findBookByTitle();
                    break;
                case 6:
                    std::cout << "Выход из программы." << std::endl;
                    break;
                default:
                    throw std::out_of_range("Неверный выбор. Попробуйте снова.");
                }
            }
            catch (const std::exception& e) {
                std::cout << "Ошибка: " << e.what() << std::endl;
            }
        } while (choice != 6);
    }
};

int main() {
    setlocale(LC_ALL, "Rus");

    LibrarySystem library;
    library.showMenu();

    return 0;
}
