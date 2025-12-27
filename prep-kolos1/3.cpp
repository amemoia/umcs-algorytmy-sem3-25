#include <iostream>

class Stack
{
public:
    int top_idx;
    int capacity;
    char* entries;

     Stack(int size) { capacity = size; entries = new char[capacity]; top_idx = -1; }
    ~Stack()         { delete[] entries; }

    void push(char entry)
    {
        if (top_idx < capacity - 1) entries[++top_idx] = entry;
    }

    char pop()
    {
        if (top_idx >= 0) return entries[top_idx--];
        return '\0';
    }

    bool isEmpty() { return top_idx == -1; }
};

bool matches(char opening, char closing)
{
    return (opening == '(' && closing == ')') ||
           (opening == '[' && closing == ']') ||
           (opening == '{' && closing == '}');
}

bool isValid(const char* line, int len)
{
    Stack* stack = new Stack(len);
    for (int i = 0; line[i] != '.'; i++)
    {
        char c = line[i];
        if (c == '(' || c == '[' || c == '{') stack->push(c);
        else if (c == ')' || c == ']' || c == '}')
        {
            if (stack->isEmpty()) { delete stack; return false; }
            char o = stack->pop();
            if (!matches(o, c)) { delete stack; return false; }
        }
    }
    bool ret = stack->isEmpty();
    delete stack;
    return ret;
}

class ExtString
{
public:
    int len;
    int capacity;
    char* data;

     ExtString() { len = 0; capacity = 16; data = new char[capacity]; }
    ~ExtString() { delete[] data; }

    void resize()
    {
        capacity *= 2;
        char* temp = new char[capacity+1]; // add for /0
        for (int i = 0; i < len; i++) temp[i] = data[i];
        delete[] data;
        data = temp;
    }
    void add(char c)
    {
        if (len == capacity-1) resize();
        data[len++] = c;
    }
};

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    for (int i = 0; i < n; i++)
    {
        ExtString* string = new ExtString();
        char c = 0;
        while (c != '.')
        {
            std::cin >> c; // we'd have to use something else if we wanted to include spaces, tabs or newlines
            string->add(c);
        }

        if (isValid(string->data, string->len))
        {
            std::cout << "TAK\n";
            delete string;
        }
        else
        {
            std::cout << "NIE\n";
            delete string;
        }
    }
}
