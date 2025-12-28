// mergesort: divide and conquer

#include <iostream>

// p = 0 => samica
// p = 1 => samiec
struct Dzik { int x; int y; int p; int idx; };

/*
    avoid using sqrt since we would square this later anyway
    use long long because the squared value can be (200 000)^2
    sum of which can reach 80 000 000 000
    which would go over the int limit
*/
long long dist_sq(const Dzik& a, const Dzik& b)
{
    long long dx = a.x - b.x;
    long long dy = a.y - b.y;
    return dx * dx + dy * dy;
}

class Stado
{
public:
    int cap = 0;
    int len = 0;
    Dzik* dziki = nullptr;
    
    // Pre-allocated work arrays (reused across calls)
    Dzik* sorted = nullptr;
    Dzik* temp = nullptr;
    Dzik* strip = nullptr;
    
    Stado() {}
    
    ~Stado() 
    { 
        delete[] dziki; 
        delete[] sorted;
        delete[] temp;
        delete[] strip;
    }
    
    void init(int c)
    {
        len = c;
        if (c > cap)
        {
            delete[] dziki;
            delete[] sorted;
            delete[] temp;
            delete[] strip;
            
            cap = c;
            dziki = new Dzik[cap];
            sorted = new Dzik[cap];
            temp = new Dzik[cap];
            strip = new Dzik[cap];
        }
    }
    
    void push(int i, int x, int y, int p)
    {
        dziki[i] = {x, y, p, i};
    }

    void merge_x(int left, int mid, int right)
    {
        int i = left, j = mid, k = left;
        while (i < mid && j < right)
        {
            if ( (sorted[i].x < sorted[j].x) || (sorted[i].x == sorted[j].x && sorted[i].y < sorted[j].y) ) temp[k++] = sorted[i++]; 
            else temp[k++] = sorted[j++];
        }

        while (i < mid) temp[k++] = sorted[i++];
        while (j < right) temp[k++] = sorted[j++];

        for (int i = left; i < right; i++) sorted[i] = temp[i];
    }

    void merge_sort_x(int left, int right)
    {
        if (right - left <= 1) return;
        
        int mid = left + (right - left) / 2;
        merge_sort_x(left, mid);  // left half
        merge_sort_x(mid, right);  // right half
        merge_x(left, mid, right);      // merge halves
    }

    void merge_y(int left, int mid, int right)
    {
        int i = left, j = mid, k = left;
        while (i < mid && j < right)
        {
            if (sorted[i].y < sorted[j].y || (sorted[i].y == sorted[j].y && sorted[i].x < sorted[j].x)) temp[k++] = sorted[i++];
            else temp[k++] = sorted[j++];
        }
        
        while (i < mid) temp[k++] = sorted[i++];
        while (j < right) temp[k++] = sorted[j++];
        
        for (int i = left; i < right; i++) sorted[i] = temp[i];
    }

    void sort_y_small(int left, int right) {
        for (int i = left + 1; i < right; i++) {
            Dzik key = sorted[i];
            int j = i - 1;
            while (j >= left && (sorted[j].y > key.y || (sorted[j].y == key.y && sorted[j].x > key.x))) {
                sorted[j + 1] = sorted[j];
                j--;
            }
            sorted[j + 1] = key;
        }
    }

    // for small arrays, bruteforce can be faster
    void brute_force(int left, int right, long long& min_dist, int& idx_female, int& idx_male)
    {
        for (int i = left; i < right; i++)
        {
            for (int j = i + 1; j < right; j++)
            {
                if (sorted[i].p != sorted[j].p)
                {
                    long long d = dist_sq(sorted[i], sorted[j]);
                    
                    if (min_dist == -1 || d < min_dist)
                    {
                        min_dist = d;
                        idx_female = (sorted[i].p == 0) ? sorted[i].idx : sorted[j].idx;
                        idx_male = (sorted[i].p == 0) ? sorted[j].idx : sorted[i].idx;
                    }
                }
            }
        }
        sort_y_small(left, right);
    }

    void closest_rec(int left, int right, long long& min_dist, int& idx_female, int& idx_male)
    {
        if (right - left <= 3)
        {
            brute_force(left, right, min_dist, idx_female, idx_male);
            return;
        }
        
        // divide - find midpoint
        int mid = left + (right - left) / 2;
        int mid_x = sorted[mid].x;
        
        // conquer - solve halves
        closest_rec(left, mid, min_dist, idx_female, idx_male);
        closest_rec(mid, right, min_dist, idx_female, idx_male);
        
        // merge halves by Y to maintain sorted order for strip processing
        merge_y(left, mid, right);

        // close to div line
        int strip_size = 0;
        for (int i = left; i < right; i++)
        {
            long long dx = sorted[i].x - mid_x;
            if (dx < 0) dx = -dx;
            if (min_dist == -1 || dx * dx < min_dist) strip[strip_size++] = sorted[i];
        }
        
        // check only nearby points in Y-sorted strip
        for (int i = 0; i < strip_size; i++)
        {
            for (int j = i + 1; j < strip_size && j < i + 8; j++)
            {
                if (strip[i].p != strip[j].p)
                {
                    long long dy = strip[j].y - strip[i].y;
                    if (min_dist != -1 && dy * dy >= min_dist) break;
                    
                    long long d = dist_sq(strip[i], strip[j]);
                    if (min_dist == -1 || d < min_dist)
                    {
                        min_dist = d;
                        idx_female = (strip[i].p == 0) ? strip[i].idx : strip[j].idx;
                        idx_male = (strip[i].p == 0) ? strip[j].idx : strip[i].idx;
                    }
                }
            }
        }
    }
    
    void find_closest(int& idx_female, int& idx_male)
    {
        idx_female = -1;
        idx_male = -1;
        
        bool has_female = false, has_male = false;
        for (int i = 0; i < len; i++)
        {
            sorted[i] = dziki[i];
            if (dziki[i].p == 0) has_female = true;
            else has_male = true;
        }
        
        if (!has_female || !has_male) return;

        merge_sort_x(0, len);
        
        long long min_dist = -1;
        closest_rec(0, len, min_dist, idx_female, idx_male);
    }
};

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);
    
    int s; // liczba stad
    if (!(std::cin >> s)) return 0;
    
    Stado stado; // Reuse one instance
    
    for (int i = 0; i < s; i++)
    {
        int n_i;
        std::cin >> n_i;
        stado.init(n_i);
        for (int j = 0; j < n_i; j++)
        {
            int x, y, p;
            std::cin >> x >> y >> p;
            stado.push(j, x, y, p);
        }

        int idx_female, idx_male;
        stado.find_closest(idx_female, idx_male);
        
        if (idx_female == -1) std::cout << "0\n";
        else std::cout << idx_female << " " << idx_male << "\n";

        // boar moves when called
        int m;
        std::cin >> m;
        for (int j = 0; j < m; ++j)
        {
            int idx, dx, dy;
            std::cin >> idx >> dx >> dy;
            
            stado.dziki[idx].x += dx;
            stado.dziki[idx].y += dy;
            
            stado.find_closest(idx_female, idx_male);
            
            if (idx_female == -1) std::cout << "0\n";
            else std::cout << idx_female << " " << idx_male << "\n";
        }
    }
}