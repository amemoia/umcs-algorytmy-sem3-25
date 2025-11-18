#include <iostream>
/*
IN
3               :: t <= 1000000
2 4 6 1 5 0     :: transport, 0 means ending
9 3 0           ::
10 9 8 0        ::

empty transports are possible

OUT
2 1 3 9 6 8 9 10 4 5

i used to write more comments but lowkey im too tired good luck chat
*/

// applies to individual elements within Tree
class Node
{
public:
    unsigned int value;
    int childCount = 0;
    Node* children[3] = {};
    Node(unsigned int value)
    {
        this->value = value;
        children[0] = children[1] = children[2] = nullptr;
    }
};

// applies to elements of the tree parent queue
struct QueueNode
{
    Node* node;
    QueueNode* next;
    QueueNode(Node* node) {this->node = node, next = nullptr;}
};

// FIFO queue that determines which parent to add elements to
class Queue
{
public:
    QueueNode* head = nullptr;
    QueueNode* tail = nullptr;

    void pop()
    {
        if (!head) return;
        QueueNode* temp = head;
        head = head->next;
        if (!head) tail = nullptr;
        delete temp;
    }
    void rm()
    {
        while (head) pop();
    }
    void push(Node* node)
    {
        QueueNode* queued = new QueueNode(node);
        if (!tail) head = tail = queued;
        else
        {
            tail->next = queued;
            tail = queued;
        }
    }
    Node* front() { return head ? head->node : nullptr; }
};

class Tree
{
public:
    Node* root = nullptr;
    Queue parent_queue;
    Node* selected_parent = nullptr;
    unsigned int curChildIndex = 0;

    void selectParent()
    {
        // the parent should be changed when we're maxed out on children OR if the transport ends
        // this means that we can end up with nodes with just one or two children
        if (selected_parent != nullptr) return;
        while (parent_queue.head != nullptr && parent_queue.front()->childCount != 0)
        {
            parent_queue.pop();
        }
        if (parent_queue.head != nullptr)
        {
            selected_parent = parent_queue.front();
            parent_queue.pop();
            curChildIndex = 0;
        }
    }

    void add(unsigned int value)
    {
        Node* node = new Node(value);
        if (root == nullptr)
        {
            root = node;
            parent_queue.push(root);
            return;
        }
        selectParent();
        if (selected_parent == nullptr) { parent_queue.push(node); return; }

        selected_parent->children[curChildIndex++] = node;
        selected_parent->childCount++;
        parent_queue.push(node);
        if (curChildIndex >= 3)
        {
            selected_parent = nullptr;
            curChildIndex = 0;
        }
    }
    void insertFromTransport(unsigned int *arr, unsigned int len)
    {
        // from the transport we insert in REVERSE order (starting from right side)
        selected_parent = nullptr;
        curChildIndex = 0;
        for (int i = 0; i < len; i++)
        {
            add(arr[len-i-1]);
        }
    }

    // display children (and grandchildren etc) before parent
    void postorder(Node* node)
    {
        if (node == nullptr) return;
        for (int i = 0; i < 3; i++) postorder(node->children[i]);
        std::cout << node->value << ' ';
    }
};

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    Tree tree;

    // 1 <= t <= 1000000
    int t;
    std::cin >> t;

    for (int i = 0; i < t; i++)
    {
        // the buffer is an array that is "expanded" if necessary to save memory
        unsigned int buf_cap = 32;
        unsigned int buf_size = 0;
        unsigned int *buf = new unsigned int[buf_cap];
        unsigned int val;
        std::cin >> val;
        while (val != 0)
        {
            if (buf_size >= buf_cap)
            {
                // double le size
                buf_cap *= 2;
                unsigned int *buf_temp = new unsigned int[buf_cap];
                for (int i = 0; i < buf_size; i++)
                {
                    buf_temp[i] = buf[i];
                }
                delete[] buf;
                buf = buf_temp;
            }
            buf[buf_size++] = val;
            std::cin >> val;
        }
        tree.insertFromTransport(buf, buf_size);
        delete[] buf;
    }

    tree.postorder(tree.root);
}