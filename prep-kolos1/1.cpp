#include <iostream>

class Node
{
    public:
    long value;
    Node* next;
    Node* prev;
    Node(long v)
    {
        value = v;
        next = nullptr;
        prev = nullptr;
    }
};
class Queue
{
    public:
    unsigned long len = 0;
    Node* start = nullptr;
    Node* end = nullptr;

    ~Queue()
    {
        while (start != nullptr)
        {
            rm_start();
        }
    }

    long get_start() { return start!=nullptr ? start->value : -1; }
    long get_end() { return end!=nullptr ? end->value : -1; }
    unsigned long get_len() { return len; }
    void insert_start(long value)
    {
        if (start == nullptr)
        {
            start = new Node(value);
            end = start;
        }
        else
        {
            Node* temp = start;
            start = new Node(value);
            start->next = temp;
            temp->prev = start;
        }
        len++;
    }
    void insert_end(long value)
    {
        if (end == nullptr)
        {
            end = new Node(value);
            start = end;
        }
        else
        {
            Node* temp = end;
            end = new Node(value);
            end->prev = temp;
            temp->next = end;
        }
        len++;
    }
    int rm_start()
    {
        if (start == nullptr) return -1;
        if (start == end)
        {
            delete start;
            start = nullptr;
            end = nullptr;
            len= 0;
            return 1;
        }
        if (start != nullptr)
        {
            Node* temp = start;
            start = temp->next;
            start->prev = nullptr;
            delete temp;
            len--;
            return 0;
        }
    }
    int rm_end()
    {
        if (end == nullptr) return -1;
        if (start == end)
        {
            delete start;
            start = nullptr;
            end = nullptr;
            len = 0;
            return 1;
        }
        if (end != nullptr)
        {
            Node* temp = end;
            end = temp->prev;
            end->next = nullptr;
            delete temp;
            len--;
            return 0;
        }
    };
};

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    unsigned long n;
    std::cin >> n;

    Queue* queue = new Queue();

    for (unsigned long i = 0; i < n; i++)
    {
        int op;
        std::cin >> op;
        if (op == 1)
        {
            long out = queue->get_start();
            if (out == -1) std::cout << "PUSTA" << '\n';
            else std::cout << out << '\n';
        }
        else if (op == 2)
        {
            long out = queue->get_end();
            if (out == -1) std::cout << "PUSTA" << '\n';
            else std::cout << out << '\n';
        }
        else if (op == 3) std::cout << queue->get_len() << '\n';
        else if (op == 4)
        {
            long add;
            std::cin >> add;
            queue->insert_start(add);
        }
        else if (op == 5)
        {
            long add;
            std::cin >> add;
            queue->insert_end(add);
        }
        else if (op == 6)
        {
            int out = queue->rm_start();
            if (out == -1) std::cout << "PUSTA" << '\n';
        }
        else if (op == 7)
        {
            int out = queue->rm_end();
            if (out == -1) std::cout << "PUSTA" << '\n';
        }
    }

    delete queue;
}