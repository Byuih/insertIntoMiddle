#include <mutex>

struct Node
{
    int value;
    Node* next;
    std::mutex* node_mutex;

    Node(int val) : value(val), next(nullptr), node_mutex(new std::mutex) {}
};

class FineGrainedQueue
{
private:
    Node* head;
    std::mutex* queue_mutex;

public:
    FineGrainedQueue() : head(nullptr), queue_mutex(new std::mutex) {}

    // Метод для вставки узла в середину или конец списка
    void insertIntoMiddle(int value, int pos)
    {
        Node* newNode = new Node(value);

        // Захватываем общую блокировку для модификации списка
        std::unique_lock<std::mutex> queueLock(*queue_mutex);

        // Перемещаем указатель temp на начало списка
        Node* temp = head;

        // Идем до нужной позиции или последнего элемента
        for (int i = 1; i < pos && temp->next; ++i)
        {
            // Захватываем блокировку текущего узла
            std::unique_lock<std::mutex> currentLock(*temp->node_mutex);

            // Переходим к следующему узлу
            temp = temp->next;
        }

        // Захватываем блокировку нового узла
        std::unique_lock<std::mutex> newNodeLock(*newNode->node_mutex);

        // Вставляем новый узел в середину или конец списка
        newNode->next = temp->next;
        temp->next = newNode;
    }
};