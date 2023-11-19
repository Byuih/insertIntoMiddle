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

    // ����� ��� ������� ���� � �������� ��� ����� ������
    void insertIntoMiddle(int value, int pos)
    {
        Node* newNode = new Node(value);

        // ����������� ����� ���������� ��� ����������� ������
        std::unique_lock<std::mutex> queueLock(*queue_mutex);

        // ���������� ��������� temp �� ������ ������
        Node* temp = head;

        // ���� �� ������ ������� ��� ���������� ��������
        for (int i = 1; i < pos && temp->next; ++i)
        {
            // ����������� ���������� �������� ����
            std::unique_lock<std::mutex> currentLock(*temp->node_mutex);

            // ��������� � ���������� ����
            temp = temp->next;
        }

        // ����������� ���������� ������ ����
        std::unique_lock<std::mutex> newNodeLock(*newNode->node_mutex);

        // ��������� ����� ���� � �������� ��� ����� ������
        newNode->next = temp->next;
        temp->next = newNode;
    }
};