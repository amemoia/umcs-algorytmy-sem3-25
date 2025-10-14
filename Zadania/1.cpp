#include <string>
#include <iostream>
#define TOTAL_HOURS_WASTED 8

const std::string Animals[] = {
    "malpa", "lew", "osiol", "bizon", "kon",
    "slon", "tygrys", "pingwin", "sowa", "owca", "kaczka"
};
const std::string Sounds[] = {
    "ua", "rar", "io", "mu", "iha",
    "tru", "grr", "pi", "hu", "be", "kwa"
};

inline bool isVowel(char c)
{
    switch (c) {
    case 'a': case 'e': case 'i': case 'o': case 'u':
        return true;
    default:
        return false;
    }
}
inline bool isAlpha(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}
inline char toLower(char c)
{
    return (c >= 'A' && c <= 'Z') ? (c + 32) : c;
}

inline void removeTrailingSpaces(char* buffer, int& cur)
{
    while (cur > 0 && buffer[cur-1] == ' ') cur--;
}

std::string cleanup(const std::string& str)
{
    const int len = str.length();

    // skip leading spaces
    int start = 0;
    while (start < len && str[start] == ' ') start++;

    char buffer[len + 1];
    int cur = 0;

    char prev = 0;
    for (int i = start; i < len; ++i)
    {
        char c = str[i];
        if (c == ' ')
        {
            if (cur > 0 && buffer[cur-1] == ' ') continue;
            buffer[cur++] = ' ';
            prev = ' ';
        }
        else if (isAlpha(c))
        {
            c = toLower(c);
            if (isVowel(c) && prev == c) continue;
            buffer[cur++] = c;
            prev = c;
        }
    }

    removeTrailingSpaces(buffer, cur);

    buffer[cur] = '\0';
    return std::string(buffer, cur);
}
int compareSegments(const std::string& str)
{
    int len = str.length();
    if (len == 0) return -1;
    int firstSegLen = 0;
    while (firstSegLen < len && str[firstSegLen] != ' ') ++firstSegLen;
    if (firstSegLen == 0) return -1;

    int pos = firstSegLen + 1;
    while (pos < len)
    {
        for (int i = 0; i < firstSegLen; ++i)
        {
            if (pos + i >= len || str[i] != str[pos + i]) return -1;
        }
        pos += firstSegLen + 1;
        if (pos < len && str[pos-1] != ' ') return -1;
    }
    return firstSegLen;
}
int handleSound(const std::string &snd)
{
    if (!snd.empty())
    {
        for (int i = 0; i < snd.length(); ++i)
        {
            char c = snd[i];
            if (isAlpha(c))
            {
                char firstChar = toLower(c);
                if (firstChar != 'u' && firstChar != 'r' && firstChar != 'i' &&
                    firstChar != 'm' && firstChar != 't' && firstChar != 'g' &&
                    firstChar != 'p' && firstChar != 'h' && firstChar != 'b' &&
                    firstChar != 'k') {
                    return -1;
                }
                break;
            }
        }
    }

    std::string filtered = cleanup(snd);
    if (filtered.empty()) return -1;
    int firstSpace = compareSegments(filtered);
    if (firstSpace == -1) return -1;

    char firstChar = filtered[0];
    for (int i = 0; i < 11; ++i) {
        if (Sounds[i].length() != firstSpace) continue;
        if (Sounds[i][0] != firstChar) continue;

        int j;
        for (j = 1; j < firstSpace; ++j) {
            if (Sounds[i][j] != filtered[j]) break;
        }
        if (j == firstSpace) return i;
    }
    return -1;
}

int main()
{
    // this disables sync between printf/scanf and cout/cin
    // we dont use these so its free perf
    // cin.tie disables autoclear of cout before every cin
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int N;
    std::cin >> N;
    std::cin.ignore();
    for (int i = 0; i < N; ++i)
    {
        std::string in;
        std::getline(std::cin, in);
        int index = handleSound(in);
        if (index == -1) std::cout << "nie wiem\n";
        else std::cout << Animals[index] << '\n';
    }
}