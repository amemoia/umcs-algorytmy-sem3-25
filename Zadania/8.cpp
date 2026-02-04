#include <iostream>
#include <string>
#include <vector>

// it only gets worse
struct Animal
{
    int slots_min, slots_max;
    std::string name;
    int conflicts;     // bit i = 1 if conflicts with animal i
    int scenes;        // A=1, B=2, C=4
    int slot_mask;
    int assigned_slot;
    char assigned_scene;
    // needs to be a counter so we dont unblock too early
    //int blocked_count[20];

    Animal(std::string& name, int slots_min, int slots_max, std::string& scenes_str)
    {
        this->slots_min = slots_min;
        this->slots_max = slots_max;
        this->name = name;
        this->assigned_slot = -1;
        this->assigned_scene = '\0';
        this->conflicts = 0;
        this->scenes = 0;
        this->slot_mask = 0;

        // A=1, B=2, C=4
        if (scenes_str.find('A') != -1) scenes |= 1;
        if (scenes_str.find('B') != -1) scenes |= 2;
        if (scenes_str.find('C') != -1) scenes |= 4;
    }
};

//int slot_occupants[20]; // bitmask of which animals are in each slot
int slot_scenes[20]; // bitmask of which scenes are occupied in each slot

char get_scene_char(int bit)
{
    if (bit == 1) return 'A';
    if (bit == 2) return 'B';
    return 'C';
}

bool backtrack(int animal_idx, int n, Animal** animals, int s, std::vector<int>& domains)
{
    if (animal_idx == n) return true;

    Animal* current = animals[animal_idx];
    int valid_slots = domains[animal_idx];  // forward checking domain

    for (int slot = current->slots_min; slot <= s && slot <= current->slots_max; slot++)
    {
        if (!((valid_slots >> slot) & 1)) continue;
        int available = current->scenes & ~slot_scenes[slot];
        if (!available) continue;

        while (available)
        {
            // lowest bit first (A=1, B=2, C=4)
            int scene_bit = available & -available;
            available ^= scene_bit;

            slot_scenes[slot] |= scene_bit;

            // forward checking, avoids to future animls
            std::vector<int> next_domains = domains;
            bool ok = true;

            for (int next = animal_idx + 1; next < n; next++)
            {
                if ((current->conflicts >> next) & 1)
                {
                    next_domains[next] &= ~(1 << slot);
                    // impossible
                    if (next_domains[next] == 0)
                    {
                        ok = false;
                        break;
                    }
                }
            }

            if (ok)
            {
                current->assigned_slot = slot;
                current->assigned_scene = get_scene_char(scene_bit);
                if (backtrack(animal_idx + 1, n, animals, s, next_domains)) return true;
            }

            slot_scenes[slot] &= ~scene_bit;
        }
    }

    return false;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // animals, restrictions, slots
    int n, m, s;
    std::cin >> n >> m >> s;

    Animal** animals = new Animal*[n];

    for (int i = 0; i < 22; i++) slot_scenes[i] = 0;

    for (int i = 0; i < n; i++)
    {
        std::string name, scenes;
        int a, b; // timeslot range (incl)
        std::cin >> name >> a >> b >> scenes;
        animals[i] = new Animal(name, a, b, scenes);

        // bit mask of valid slots based on range
        for (int k = a; k <= b && k <= s; k++) animals[i]->slot_mask |= (1 << k);
    }

    for (int i = 0; i < m; i++)
    {
        // X ! Y
        // animals X and Y cannot be active in the same timeslot
        std::string x_name, y_name, excl;
        std::cin >> x_name >> excl >> y_name;

        int x_i = -1, y_i = -1;
        for (int j = 0; j < n && (x_i == -1 || y_i == -1); j++)
        {
            if (x_i == -1 && animals[j]->name == x_name) x_i = j;
            if (y_i == -1 && animals[j]->name == y_name) y_i = j;
        }
        animals[x_i]->conflicts |= (1 << y_i);
        animals[y_i]->conflicts |= (1 << x_i);
    }

    // all valid slots per animal
    std::vector<int> domains(n);
    for (int i = 0; i < n; i++) domains[i] = animals[i]->slot_mask;

    bool found = backtrack(0, n, animals, s, domains);

    if (found)
    {
        for (int i = 0; i < n; i++)
        {
            std::cout << animals[i]->name << " "
                << animals[i]->assigned_slot << " "
                << animals[i]->assigned_scene << "\n";
        }
    }
    else std::cout << "NIE\n";

    for (int i = 0; i < n; ++i) delete animals[i];
    delete[] animals;
}

