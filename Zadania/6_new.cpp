// bede szczery poprzednie rozwiazanie bylo mostly vibecoded
#include <iostream>
#include <vector>

// p=0 -> samica
// p=1 -> samiec
struct Dzik { int x; int y; int p; int idx; };
long long dist_sq(const Dzik &a, const Dzik &b) {
    long long dx = a.x - b.x;
    long long dy = a.y - b.y;
    return dx*dx + dy*dy;
}

long long global_min_dist;
int id_a, id_b;

std::vector<Dzik> work_copy;
std::vector<Dzik> merge_temp;
std::vector<Dzik> strip_temp;
int global_capacity = 0;

void update_min_dist(const Dzik &a, const Dzik &b) {
    long long dist = dist_sq(a, b);
    if (global_min_dist == -1 || dist < global_min_dist) {
        global_min_dist = dist;
        if (a.p == 0) {
            id_a = a.idx;
            id_b = b.idx;
        }
        else {
            id_a = b.idx;
            id_b = a.idx;
        }
    }
}

// merge sort X
void sort(std::vector<Dzik> &t, int left, int right, std::vector<Dzik> &temp) {
    if (left >= right) return;
    int mid = (left+right)/2;

    sort(t, left, mid, temp);
    sort(t, mid+1, right, temp);

    temp.clear();
    int i = left;
    int j = mid+1;
    while (i <= mid && j <= right) {
        if ( t[i].x < t[j].x || (t[i].x == t[j].x && t[i].y < t[j].y) ) temp.push_back(t[i++]);
        else temp.push_back(t[j++]);
    }
    while (i <= mid)   temp.push_back(t[i++]);
    while (j <= right) temp.push_back(t[j++]);
    for (int k = 0; k < temp.size(); k++) t[left+k] = temp[k];
}

void bubblesort_y(std::vector<Dzik> &t, int left, int right) {
    for (int i = left; i < right; i++) {
        for (int j = left; j < right-(i-left); j++) {
            if (t[j].y > t[j+1].y) {
                std::swap(t[j], t[j+1]);
            }
        }
    }
}

void dnc(std::vector<Dzik> &t, int left, int right, std::vector<Dzik> &temp, std::vector<Dzik> &strip) {
    if (right-left <= 3) {
        for (int i = left; i <= right; i++) {
            for (int j = i+1; j <= right; j++) {
                if (t[i].p != t[j].p) {
                    update_min_dist(t[i], t[j]);
                }
            }
        }
        bubblesort_y(t, left, right);
        return;
    }

    int mid = (left+right)/2;
    long long mid_x = t[mid].x;
    dnc(t, left, mid, temp, strip);
    dnc(t, mid+1, right, temp, strip);

    temp.clear();
    int i = left;
    int j = mid+1;
    while (i <= mid && j <= right) {
        if (t[i].y < t[j].y) temp.push_back(t[i++]);
        else temp.push_back(t[j++]);
    }
    while (i <= mid) temp.push_back(t[i++]);
    while (j <= right) temp.push_back(t[j++]);
    for (int k = 0; k < temp.size(); k++) t[left+k] = temp[k];

    strip.clear();
    for (int k = left; k <= right; k++) {
        long long dx = t[k].x - mid_x;
        if ( global_min_dist == -1 || dx * dx < global_min_dist ) strip.push_back(t[k]);
    }
    for (int k = 0; k < strip.size(); k++) {
        for (int l = k + 1; l < strip.size() && l < k + 8; ++l) {
            long long dy = strip[l].y - strip[k].y;
            if ( global_min_dist != -1 && dy * dy >= global_min_dist ) break;
            if (strip[k].p != strip[l].p) {
                update_min_dist(strip[k], strip[l]);
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    int s;
    std::cin >> s;
    
    while (s--) {
        int n;
        std::cin >> n;
        
        work_copy.resize(n);
        merge_temp.resize(n);
        strip_temp.resize(n);

        std::vector<Dzik> stado(n);
        for (int i = 0; i < n; i++) {
            std::cin >> stado[i].x >> stado[i].y >> stado[i].p;
            stado[i].idx = i;
        }

        int m;
        std::cin >> m;
        //std::vector<int> change_idx(m);
        //std::vector<long long> change_dx(m);
        //std::vector<long long> change_dy(m);
        //for (int j = 0; j < m; j++) std::cin >> change_idx[j] >> change_dx[j] >> change_dy[j];
        
        bool has_female = false, has_male = false;
        for (int i = 0; i < n; i++) {
            if (stado[i].p == 0) has_female = true;
            else has_male = true;
            if (has_female && has_male) break;
        }
        
        for (int i = -1; i < m; i++) {
            if (i >= 0) {
                int idx;
                long long dx, dy;
                std::cin >> idx >> dx >> dy;
                stado[idx].x += dx;
                stado[idx].y += dy;
            }
            
            if (!has_female || !has_male) {
                std::cout << "0\n";
                continue;
            }
            
            for (int k = 0; k < n; k++) work_copy[k] = stado[k];
            
            global_min_dist = -1;
            sort(work_copy, 0, n-1, merge_temp);
            dnc(work_copy, 0, n-1, merge_temp, strip_temp);
            
            if (global_min_dist == -1) std::cout << "0\n";
            else std::cout << id_a << " " << id_b << "\n";
        }
    }
}