#include <iostream>

struct animal
{
    short weight;
    char name[11];
    int index;
};
struct speciesList
{
    animal* animals;
    int count;
    int capacity; // only expand when necessary
};

void initAnimals(speciesList& list) {
    list.capacity = 100;
    list.animals = new animal[list.capacity];
    list.count = 0;
}

void my_strcpy(char *dest, char *src)
{
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void addAnimal(speciesList& list, short weight, char* name)
{
    // this function keeps the list sorted when adding new animals
    // if this isnt faster im going to kill myself

    // first, the animals array is limited to 100 by default to use less memory
    // it migth be necessary to expand it - this makes a new larger array and copies everything over
    if (list.count == list.capacity) {
        int newCapacity = list.capacity + 100;
        animal* newArr = new animal[newCapacity];
        for (int i = 0; i < list.count; ++i) {
            newArr[i] = list.animals[i];
        }
        delete[] list.animals;
        list.animals = newArr;
        list.capacity = newCapacity;
    }

    // we look where to insert the new animal (ascending order)
    // when the weight is equal, the animal that was there before comes first
    int pos = 0;
    while (pos < list.count && list.animals[pos].weight <= weight) {
        pos++;
    }
    // then every element after the insertion needs to be shifted to the right
    for (int i = list.count; i > pos; --i) {
        list.animals[i] = list.animals[i - 1];
        list.animals[i].index = i;
    }
    // finally we insert the new animal in its place
    list.animals[pos].weight = weight;
    //std::strcpy(list.animals[pos].name, name); // surely this doesnt count as stl right...
    // update: it counted as stl
    my_strcpy(list.animals[pos].name, name);
    list.animals[pos].index = pos;
    list.count++;
}

int getSpeciesIndex(char c) {
    switch (c) {
    case 'b': return 0;
    case 'a': return 1;
    case 'n': return 2;
    case 'h': return 3;
    case 's': return 4;
    case 'g': return 5;
    case 'p': return 6;
    case 'z': return 7;
    default: return -1;
    }
}

void ros(speciesList& list)
{
    if (list.count==0) { std::cout << "-\n"; return; }
    for (int i = 0; i < list.count; i++)
    {
        std::cout << list.animals[i].name << " ";
    }
    std::cout << '\n';
}
void mal(speciesList& list)
{
    if (list.count==0) { std::cout << "-\n"; return; }
    for (int i = list.count-1; i >= 0; i--)
    {
        std::cout << list.animals[i].name << " ";
    }
    std::cout << '\n';
}

int main()
{
    // evil info page io hack
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    int N;
    std::cin >> N;
    std::cin.get();

    // 8 different species
    speciesList species[8];
    for (int i = 0; i < 8; ++i)
    {
        initAnimals(species[i]);
    }

    // N = <1, 250 000>

    // INPUT               OUTPUT
    // 11
    // ara 2 coco
    // ile ara             1
    // ros ara             coco
    // ara 1 lola
    // ara 2 bobo
    // ile ara             3
    // ros ara             lola coco bobo
    // mal ara             bobo coco lola
    // ros zebra           -
    // ros nosorozec       -
    // ile paw             0

    for (int i = 0; i < N; ++i)
    {
        // since the values are split by spaces but we dont need the spaces,
        // we can avoid using cin.getline
        // cin will take one of those values at a time

        // technically the commands are all a length of 3, unless we are adding an animal
        // in which case the longest possible starter is hipopotam at 9 characters
        char command[10];
        std::cin >> command;
        // ile
        if (command[0] == 'i')
        {
            char arg[11]; // the species to list
            std::cin >> arg;
            int index = getSpeciesIndex(arg[0]);
            if (index == -1) { std::cout << "no such animal exists \n"; continue; }
            std::cout << species[index].count << std::endl;
        }
        // ros
        else if (command[0] == 'r')
        {
            char arg[11]; // the species to list
            std::cin >> arg;
            int index = getSpeciesIndex(arg[0]);
            if (index == -1) { std::cout << "no such animal exists \n"; continue; }
            ros(species[index]);
        }
        // mal
        else if (command[0] == 'm')
        {
            char arg[11]; // the species to list
            std::cin >> arg;
            int index = getSpeciesIndex(arg[0]);
            if (index == -1) { std::cout << "no such animal exists \n"; continue; }
            mal(species[index]);
        }
        // add animal
        else
        {
            short weight; // the max value for mass is 2500, it cannot be negative
            char name[11]; // the max length of a name is 10 (+1 for \0)
            // we treat the command as the animal species
            std::cin >> weight >> name;
            int index = getSpeciesIndex(command[0]);
            if (index == -1) { std::cout << "no such animal exists \n"; continue; }
            // addAnimal keeps the list ordered ascending by default
            // this way we don't have to sort on every call of ros() or mal()
            addAnimal(species[index], weight, name);
        }
    }
}