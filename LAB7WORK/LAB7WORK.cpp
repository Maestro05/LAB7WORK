#include <iostream>
#include <vector>
#include <string>
#include <stdexcept> // Для исключений
#include <map>
#include <algorithm>  // Для сортировки и поиска
#include <windows.h>

using namespace std;

// Базовый класс для всех элементов меню
class MenuItem {
protected:
    double price; // Цена блюда
public:
    string name; // Название блюда

    MenuItem(const string& name, double price)
        : name(name), price(price) {}

    // Конструктор копирования
    MenuItem(const MenuItem& other)
        : name(name), price(other.price) {}

    // Перегрузка оператора присваивания
    MenuItem& operator=(const MenuItem& other) {
        if (this == &other) {  // Проверка на самоприсваивание
            return *this;
        }
        name = other.name;
        price = other.price;
        return *this;
    }

    // Перегрузка оператора сравнения
    bool operator==(const MenuItem& other) const {
        return name == other.name && price == other.price;
    }

    // Геттер для получения цены
    double getPrice() const {
        return price;
    }

    // Метод для отображения информации
    virtual void display() const {
        cout << name << " - " << price << " руб.\n";
    }

    virtual MenuItem* clone() const {
        return new MenuItem(*this);  // Создает копию объекта MenuItem
    }

    virtual ~MenuItem() {}

    // Дружественная функция для доступа к приватным данным
    friend void printMenuItemDetails(const MenuItem& item);
};

// Дружественная функция для доступа к приватным данным
void printMenuItemDetails(const MenuItem& item) {
    cout << "Детали блюда: " << item.name << ", Цена: " << item.price << endl;
}

// Класс для главного блюда
class MainDish : public MenuItem {
public:
    MainDish(const string& name, double price)
        : MenuItem(name, price) {}

    void display() const override {
        cout << "[Главное блюдо] ";
        MenuItem::display();
    }
    MenuItem* clone() const override {
        return new MainDish(name, price);  // Создаем новый объект MainDish с именем и ценой
    }
    MainDish& operator=(const MenuItem& other) {
        if (this != &other) {
            name = other.name;  // Присваиваем только поля базового класса
            price = other.getPrice();
        }
        return *this;
    }
};

// Класс для закусок
class Appetizer : public MenuItem {
public:
    Appetizer(const string& name, double price)
        : MenuItem(name, price) {}

    void display() const override {
        cout << "[Закуска] ";
        MenuItem::display();
    }
    MenuItem* clone() const override {
        return new Appetizer(name, price);  // Копируем имя и цену в новый объект
    }
};

// Класс для напитков
class Drink : public MenuItem {
public:
    Drink(const string& name, double price)
        : MenuItem(name, price) {} // Вызов конструктора базового класса

    void display() const override {
        cout << "[Напиток] ";
        MenuItem::display();
    }
    MenuItem* clone() const override {
        return new Drink(name, price);  // Копируем имя и цену
    }
};

// Класс для десертов
class Dessert : public MenuItem {
public:
    Dessert(const string& name, double price)
        : MenuItem(name, price) {}

    void display() const override {
        cout << "[Десерт] ";
        MenuItem::display();
    }
    MenuItem* clone() const override {
        return new Dessert(name, price);  // Копируем имя и цену
    }
};

// Класс для обработки заказа
class Order {
public:
    vector<MenuItem*> items;  // Список заказанных позиций
    static int totalOrders;    // Статическое поле для подсчета общего количества заказов

    Order() {
        totalOrders++;  // Увеличиваем счетчик заказов при создании нового заказа
    }

    // Добавить элемент в заказ
    void addItem(MenuItem* item) {
        items.push_back(item);

        // Сортируем элементы по цене
        sort(items.begin(), items.end(), [](MenuItem* a, MenuItem* b) {
            return a->getPrice() < b->getPrice();  // Сортировка по возрастанию цены
            });
    }

    // Поиск по названию блюда в текущем заказе
    void searchItemByName(const string& name) const {
        auto it = find_if(items.begin(), items.end(), [&name](MenuItem* item) {
            return item->name == name;  // Сравниваем название блюда
            });

        if (it != items.end()) {
            // Если элемент найден, выводим его
            cout << "Найдено блюдо:\n";
            (*it)->display();
        }
        else {
            cout << "Блюдо с таким названием не найдено.\n";
        }
    }

    // Вывести заказ
    void displayOrder(int orderNum) const {
        double total = 0;
        cout << "\nНомер заказа №" << orderNum << ":\n";  // Добавляем вывод номера заказа
        for (const auto& item : items) {
            item->display();
            total += item->getPrice();
        }
        cout << "Общая сумма: " << total << " руб.\n";
    }

    // Статический метод для получения количества заказов
    static int getTotalOrders() {
        return totalOrders;
    }

    // Конструктор копирования
    Order(const Order& other) {
        totalOrders = other.totalOrders;
        for (const auto& item : other.items) {
            // Создаем новые объекты на основе типа элемента
            items.push_back(item->clone());  // Будет использовать метод clone для копирования объектов
        }
    }


    // Перегрузка оператора присваивания
    Order& operator=(const Order& other) {
        if (this == &other) {
            return *this;
        }
        totalOrders = other.totalOrders;
        for (auto item : items) {
            delete item;  // Освобождаем старую память
        }
        items.clear();  // Очищаем текущий заказ

        for (const auto& item : other.items) {
            items.push_back(item->clone());  // Используем метод clone для глубокого копирования
        }
        return *this;
    }

    ~Order() {
        // Освобождаем память, если это необходимо
        for (auto item : items) {
            delete item;
        }
    }
};

// Инициализация статического поля
int Order::totalOrders = 0;

// Абстрактный класс для категорий еды
class FoodCategory {
public:
    virtual void displayCategory() const = 0;  // Чисто виртуальная функция

    virtual ~FoodCategory() {}
};

class MainDishCategory : public FoodCategory {
public:
    void displayCategory() const override {
        cout << "\nГлавные блюда:\n";
        cout << "1. Борщ - 150 руб.\n";
        cout << "2. Стейк - 300 руб.\n";
        cout << "3. Пельмени - 180 руб.\n";
        cout << "4. Ризотто - 220 руб.\n";
    }
};
// Функции отображения меню
void displayMenu() {
    cout << "\nВыберите категорию меню:\n";
    cout << "1. Главное блюдо\n";
    cout << "2. Закуски\n";
    cout << "3. Напитки\n";
    cout << "4. Десерты\n";
    cout << "5. Завершить заказ\n";
    cout << "6. Копировать предыдущий заказ\n";
    cout << "7. Копировать заказ по номеру\n";
    cout << "8. Поиск блюда по названию\n";
    cout << "9. Завершить работу\n";
    cout << "Введите номер категории: ";
}


void displayAppetizers() {
    cout << "\nЗакуски:\n";
    cout << "1. Салат Цезарь - 120 руб.\n";
    cout << "2. Оливье - 100 руб.\n";
    cout << "3. Блины с икрой - 150 руб.\n";
    cout << "4. Тосты с авокадо - 110 руб.\n";
}

void displayDrinks() {
    cout << "\nНапитки:\n";
    cout << "1. Кола - 50 руб.\n";
    cout << "2. Минеральная вода - 40 руб.\n";
    cout << "3. Сок апельсиновый - 70 руб.\n";
    cout << "4. Чай черный - 60 руб.\n";
}

void displayDesserts() {
    cout << "\nДесерты:\n";
    cout << "1. Торт Наполеон - 80 руб.\n";
    cout << "2. Мороженое - 60 руб.\n";
    cout << "3. Чизкейк - 120 руб.\n";
    cout << "4. Пирог с яблоками - 90 руб.\n";
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Создание объектов для меню
    vector<MenuItem*> mainDishes = {
        new MainDish("Борщ", 150),
        new MainDish("Стейк", 300),
        new MainDish("Пельмени", 180),
        new MainDish("Ризотто", 220)
    };

    vector<MenuItem*> appetizers = {
        new Appetizer("Салат Цезарь", 120),
        new Appetizer("Оливье", 100),
        new Appetizer("Блины с икрой", 150),
        new Appetizer("Тосты с авокадо", 110)
    };

    vector<MenuItem*> drinks = {
        new Drink("Кола", 50),
        new Drink("Минеральная вода", 40),
        new Drink("Сок апельсиновый", 70),
        new Drink("Чай черный", 60)
    };

    vector<MenuItem*> desserts = {
        new Dessert("Торт Наполеон", 80),
        new Dessert("Мороженое", 60),
        new Dessert("Чизкейк", 120),
        new Dessert("Пирог с яблоками", 90)
    };

    // История заказов
    vector<Order> orderHistory;

    // Создание объекта заказа
    Order currentOrder;
    bool ordering = true;
    int orderNumber = 1;

    while (ordering) {
        displayMenu();

        int category;
        cin >> category;

        switch (category) {
        case 1: {
            FoodCategory* category1 = new MainDishCategory();
            int choice;
            category1->displayCategory();  // Выведет "Главные блюда:
            cout << "Выберите главное блюдо (1-4): ";
            cin >> choice;
            delete category1;
            if (choice >= 1 && choice <= 4) {
                currentOrder.addItem(mainDishes[choice - 1]);
            }
            else {
                throw invalid_argument("Некорректный выбор!");
            }
            break;
        }
        case 2: {
            displayAppetizers();
            int choice;
            cout << "Выберите закуску (1-4): ";
            cin >> choice;
            if (choice >= 1 && choice <= 4) {
                currentOrder.addItem(appetizers[choice - 1]);
            }
            else {
                throw invalid_argument("Некорректный выбор!");
            }
            break;
        }
        case 3: {
            displayDrinks();
            int choice;
            cout << "Выберите напиток (1-4): ";
            cin >> choice;
            if (choice >= 1 && choice <= 4) {
                currentOrder.addItem(drinks[choice - 1]);
            }
            else {
                throw invalid_argument("Некорректный выбор!");
            }
            break;
        }
        case 4: {
            displayDesserts();
            int choice;
            cout << "Выберите десерт (1-4): ";
            cin >> choice;
            if (choice >= 1 && choice <= 4) {
                currentOrder.addItem(desserts[choice - 1]);
            }
            else {
                throw invalid_argument("Некорректный выбор!");
            }
            break;
        }
        case 5:
            currentOrder.displayOrder(orderNumber); // Выводим номер заказа
            orderHistory.push_back(currentOrder);  // Добавляем заказ в историю
            currentOrder = Order(); // Сбрасываем текущий заказ для нового
            orderNumber++;
            break;
        case 6:
            if (!orderHistory.empty()) {
                currentOrder = Order(orderHistory.back()); // Копируем предыдущий заказ
                cout << "Копирован предыдущий заказ.\n";
            }
            else {
                cout << "Нет предыдущих заказов для копирования.\n";
            }
            break;
        case 7:
            if (orderHistory.empty()) {
                cout << "Нет заказов для копирования.\n";  // Если история заказов пуста
            }
            else {
                // Вывод всей истории заказов с номерами
                cout << "\nИстория заказов:\n";
                for (int i = 0; i < orderHistory.size(); ++i) {
                    cout << "Номер заказа №" << (i + 1) << ":\n";
                    orderHistory[i].displayOrder(i + 1);  // Отображаем каждый заказ
                    cout << "-------------------------\n";
                }

                // Запрос номера для копирования
                int orderId;
                cout << "Введите номер заказа для копирования: ";
                cin >> orderId;

                // Проверка корректности введенного номера
                if (orderId > 0 && orderId <= orderHistory.size()) {
                    currentOrder = Order(orderHistory[orderId - 1]);  // Копируем заказ по номеру
                    cout << "Копирован заказ номер " << orderId << ".\n";
                }
                else {
                    cout << "Неверный номер заказа. Попробуйте снова.\n";  // Обработка ошибки
                }
            }
            break;
        case 8:
            {
            cout << "Введите название блюда для поиска: ";
            string name;
            cin.ignore(); // Чтобы очистить буфер от лишнего символа новой строки
            getline(cin, name);
            currentOrder.searchItemByName(name); // Поиск блюда в текущем заказе
            break;
            }
        case 9:
            cout << "Завершение работы.\n";
            cout << "Общее количество сделанных заказов: " << Order::getTotalOrders() - 1 << endl;
            ordering = false;
            break;
        default:
            cout << "Некорректный выбор. Попробуйте снова.\n";
            break;
        }
    }

    return 0;
}
