#include <iostream>

/*
    N = <2, 2 000 000>
    INPUT
    3
    Julien Mort Maurice
    R 10
    R 3
    R 7
    R 9
    J Lexi
    J Rico
    R 4
    R 6
    L
    R 10
    R 0
    L
    R 4
    R 8
    X

    OUTPUT
    Julien 19

    COMMANDS
    R p - current contestant gets p = <0, 10> pts
    L - current contestant leaves
    J name - new contestant joins, is placed at the end of the queue, after the last graded or joined contestant
             name length is max 15 chars
    X - contest is cancelled (end)

    STEP BY STEP
    Julien +10 p
    Mort +3 p
    Maurice +7 p
    Julien +9 p
    Lexi joins after Julien
    Rico joins after Lexi
    Mort +4 p
    Maurice +6 p
    Julien exits
    Lexi +10 p
    Rico +0 p
    Mort exits
    Maurice +4 p
    Lexi +8 p
 */

void my_strcpy(char *dest, char *src)
{
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

class Contestant
{
public:
    char name[16] = {};
    int score = 0;
    // if next is null, that means we are last
    Contestant* next = nullptr;
    // constructor only needs name
    Contestant(char nm[16], int sc) { my_strcpy(name, nm); score=sc; }
};

class Queue
{
public:
    // queue type is first in, first out (FIFO)
    Contestant* first = nullptr;
    Contestant* last = nullptr;
    // add a contestant to the end of the queue
    // the J command adds after the last graded or last joined, which are always at the end anyways
    void push(Contestant* c)
    {
        c->next = nullptr;
        if (last != nullptr)
        {
            last->next = c;
            last = c;
        } else first = last = c;
    }
    void pop()
    {
        // removes the FIRST contestant from the queue
        if (first == nullptr) return;
        Contestant* temp = first;
        first = first->next;
        if (first == nullptr) last = nullptr;
        delete temp;
    }
    void deleteAll()
    {
        // delete all contestants to not leak memory
        // this is faster than calling pop() on every element
        Contestant* cur = last;
        while (cur != nullptr)
        {
            Contestant* prev = cur->next;
            delete cur;
            cur = prev;
        }
        first = last = nullptr;
    }
    void frontToBack()
    {
        // moves the current first to the end of the queue without allocating again
        if (first == nullptr || first == last) return;
        Contestant* cur = first;
        first = first->next;
        cur->next = nullptr;
        last->next = cur;
        last = cur;
    }
};

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    int N;
    std::cin >> N;
    std::cin.get();

    Queue queue{};
    char lastCommand = '\0';

    char highestName[16] = {};
    int highestScore = 0;

    for (int i = 0; i < N; ++i)
    {
        char name[16];
        std::cin >> name;
        Contestant* c = new Contestant(name, 0);
        // add the new contestant to the end of the queue
        queue.push(c);
    }
    while (lastCommand != 'X')
    {
        std::cin >> lastCommand;
        if (lastCommand == 'R')
        {
            // R: contestant is rated
            int score;
            std::cin >> score;
            queue.first->score += score;
            //std::cout << "Rated " << queue.first->name << " " << score << " (Total: " << *queue.first->score << ")" << '\n';
            if (queue.first->score > highestScore)
            {
                my_strcpy(highestName, queue.first->name);
                highestScore = queue.first->score;
                //std::cout << "New high score: " << highestName << " " << highestScore << '\n';
            }
            // add them to the end of the queue
            queue.frontToBack();
        }
        else if (lastCommand == 'J')
        {
            // join: create new contestant
            char name[16];
            std::cin >> name;
            Contestant* c = new Contestant(name, 0);
            queue.push(c);
            //std::cout << name << " joined the contest" << '\n';
        }
        else if (lastCommand == 'L')
        {
            // L: contestant leaves
            //char temp[16];
            //my_strcpy(temp, queue.first->name);
            queue.pop();
            //std::cout << temp << " left the contest" << '\n';
        }
        else if (lastCommand == 'X') break;
    }
    // cout the winner
    std::cout << highestName << ' ' << highestScore << '\n';
    queue.deleteAll();
}