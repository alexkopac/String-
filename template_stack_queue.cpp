#include <iostream>
#include <string>
#include <stdexcept>
#include <windows.h>

using namespace std;

// Користувацький клас із лекції для перевірки сумісності шаблонів
class MyInt {
private:
    int value;
public:
    MyInt(int value = 0) : value(value) {}

    void SetData(int value) {
        this->value = value;
    }

    int GetData() const {
        return value;
    }
};

// =========================================================
// Шаблонний клас «Стек» (LIFO — Last In, First Out)
// =========================================================
template <typename T>
class Stack {
private:
    T* data;
    size_t size;
    size_t capacity;

public:
    Stack(size_t capacity = 4) : capacity(capacity > 0 ? capacity : 4), size(0) {
        data = new T[this->capacity];
    }

    // Конструктор копіювання
    Stack(const Stack<T>& other) : capacity(other.capacity), size(other.size) {
        data = new T[capacity];
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    // Оператор присвоєння копіюванням
    Stack<T>& operator=(const Stack<T>& other) {
        if (this == &other) {
            return *this;
        }
        delete[] data;
        capacity = other.capacity;
        size = other.size;
        data = new T[capacity];
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        return *this;
    }

    ~Stack() {
        delete[] data;
    }

    void Resize(size_t newCapacity) {
        if (newCapacity > capacity) {
            T* newData = new T[newCapacity];
            for (size_t i = 0; i < size; i++) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
            capacity = newCapacity;
        } else {
            throw invalid_argument("Нова місткість має бути більшою за поточну.");
        }
    }

    // Додавання елемента на верхівку стека
    void Push(const T& item) {
        if (size == capacity) {
            Resize(capacity * 2);
        }
        data[size++] = item;
    }

    // Отримання верхнього елемента без видалення
    T& Peek() {
        if (IsEmpty()) {
            throw out_of_range("Стек порожній: неможливо виконати Peek().");
        }
        return data[size - 1];
    }

    const T& Peek() const {
        if (IsEmpty()) {
            throw out_of_range("Стек порожній: неможливо виконати Peek().");
        }
        return data[size - 1];
    }

    // Видалення верхнього елемента
    void Pop() {
        if (IsEmpty()) {
            throw out_of_range("Стек порожній: неможливо виконати Pop().");
        }
        size--;
    }

    bool IsEmpty() const {
        return size == 0;
    }

    size_t GetSize() const {
        return size;
    }

    size_t GetCapacity() const {
        return capacity;
    }
};

// =========================================================
// Шаблонний клас «Черга» (FIFO — First In, First Out)
// =========================================================
template <typename T>
class Queue {
private:
    T* data;
    size_t capacity;
    size_t size;
    size_t frontIndex;
    size_t backIndex;

public:
    Queue(size_t capacity = 4)
        : capacity(capacity > 0 ? capacity : 4), size(0), frontIndex(0), backIndex(0) {
        data = new T[this->capacity];
    }

    // Конструктор копіювання
    Queue(const Queue<T>& other)
        : capacity(other.capacity), size(other.size), frontIndex(0), backIndex(other.size) {
        data = new T[capacity];
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[(other.frontIndex + i) % other.capacity];
        }
    }

    // Оператор присвоєння копіюванням
    Queue<T>& operator=(const Queue<T>& other) {
        if (this == &other) {
            return *this;
        }
        delete[] data;
        capacity = other.capacity;
        size = other.size;
        frontIndex = 0;
        backIndex = size;
        data = new T[capacity];
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[(other.frontIndex + i) % other.capacity];
        }
        return *this;
    }

    ~Queue() {
        delete[] data;
    }

    void Resize(size_t newCapacity) {
        if (newCapacity <= capacity) {
            throw invalid_argument("Нова місткість черги має бути більшою за поточну.");
        }
        T* newData = new T[newCapacity];
        for (size_t i = 0; i < size; i++) {
            newData[i] = data[(frontIndex + i) % capacity];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
        frontIndex = 0;
        backIndex = size;
    }

    // Додавання елемента в кінець черги
    void Enqueue(const T& item) {
        if (size == capacity) {
            Resize(capacity * 2);
        }
        data[backIndex] = item;
        backIndex = (backIndex + 1) % capacity;
        size++;
    }

    // Доступ до першого елемента в черзі
    T& Front() {
        if (IsEmpty()) {
            throw out_of_range("Черга порожня: неможливо виконати Front().");
        }
        return data[frontIndex];
    }

    const T& Front() const {
        if (IsEmpty()) {
            throw out_of_range("Черга порожня: неможливо виконати Front().");
        }
        return data[frontIndex];
    }

    // Видалення першого елемента з черги
    void Dequeue() {
        if (IsEmpty()) {
            throw out_of_range("Черга порожня: неможливо виконати Dequeue().");
        }
        frontIndex = (frontIndex + 1) % capacity;
        size--;
    }

    bool IsEmpty() const {
        return size == 0;
    }

    size_t GetSize() const {
        return size;
    }

    size_t GetCapacity() const {
        return capacity;
    }
};

// =========================================================
// Тестування роботи шаблонних класів Stack<T> та Queue<T>
// =========================================================
int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    try {
        // 1. Тестування Stack<int>
        cout << "=== 1. Тестування Stack<int> ===\n";
        Stack<int> intStack;
        intStack.Push(1);
        intStack.Push(2);
        intStack.Push(3);
        intStack.Push(63);
        intStack.Push(12);
        intStack.Push(32);

        cout << "Розмір: " << intStack.GetSize() << ", Місткість: " << intStack.GetCapacity() << "\nЕлементи (LIFO): ";
        while (!intStack.IsEmpty()) {
            cout << intStack.Peek() << " ";
            intStack.Pop();
        }
        cout << "\n\n";

        // 2. Тестування Stack<string>
        cout << "=== 2. Тестування Stack<string> ===\n";
        Stack<string> strStack;
        strStack.Push("Перший рядок");
        strStack.Push("Другий рядок");
        strStack.Push("Третій рядок");

        while (!strStack.IsEmpty()) {
            cout << "  -> " << strStack.Peek() << '\n';
            strStack.Pop();
        }
        cout << '\n';

        // 3. Тестування Queue<int>
        cout << "=== 3. Тестування Queue<int> ===\n";
        Queue<int> intQueue;
        intQueue.Enqueue(10);
        intQueue.Enqueue(12);
        intQueue.Enqueue(15);
        intQueue.Enqueue(1);
        intQueue.Enqueue(1099);

        cout << "Розмір: " << intQueue.GetSize() << ", Місткість: " << intQueue.GetCapacity() << "\nЕлементи (FIFO): ";
        while (!intQueue.IsEmpty()) {
            cout << intQueue.Front() << " ";
            intQueue.Dequeue();
        }
        cout << "\n\n";

        // 4. Тестування Queue<string>
        cout << "=== 4. Тестування Queue<string> ===\n";
        Queue<string> strQueue(2);
        strQueue.Enqueue("Запит №1");
        strQueue.Enqueue("Запит №2");
        strQueue.Dequeue(); // Видаляємо перший елемент, щоб перевірити циклічний зсув перед Resize
        strQueue.Enqueue("Запит №3");
        strQueue.Enqueue("Запит №4");

        while (!strQueue.IsEmpty()) {
            cout << "  -> " << strQueue.Front() << '\n';
            strQueue.Dequeue();
        }
        cout << '\n';

        // 5. Тестування Queue<MyInt> (сумісність із класом MyInt із лекції)
        cout << "=== 5. Тестування Queue<MyInt> ===\n";
        Queue<MyInt> myIntQueue;
        myIntQueue.Enqueue(MyInt(100));
        myIntQueue.Enqueue(MyInt(200));
        while (!myIntQueue.IsEmpty()) {
            cout << "  MyInt value: " << myIntQueue.Front().GetData() << '\n';
            myIntQueue.Dequeue();
        }

    } catch (const exception& ex) {
        cout << "Помилка: " << ex.what() << endl;
    }

    return 0;
}