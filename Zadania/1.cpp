#include <string>
#include <iostream>
#define TOTAL_HOURS_WASTED 8

// wstawiam to na nowo bo ciezko mi zrozumiec kod i chce miec kod z komentarzami do kazdego zadania na tichym

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

    // +1 to account for \0
    char buffer[len + 1];
    int cur = 0;

    // store the previous char for checking dupes etc
    char prev = 0;
    for (int i = start; i < len; ++i)
    {
        char c = str[i];
        if (c == ' ')
        {
            // dupes
            if (cur > 0 && buffer[cur-1] == ' ') continue;
            buffer[cur++] = ' ';
            prev = ' ';
        }
        else if (isAlpha(c))
        {
            c = toLower(c);
            // dupes
            if (isVowel(c) && prev == c) continue;
            buffer[cur++] = c;
            prev = c;
        }
    }

    removeTrailingSpaces(buffer, cur);

    buffer[cur] = '\0';
    // here cur determines the length of the string to return, so we dont have a bunch of empty space after \0
    return std::string(buffer, cur);
}
int compareSegments(const std::string& str)
{
    // this function is intended to save space by not making a new variable for every segment
    // a segment is basically a word, so split by a space etc
    // it should be called AFTER duplicate spaces are removed by cleanup()
    // it measures the length of the first segment (x) and checks str[i] == str[i+x]
    // after encountering a space it simply moves onto the next word, i still represents the first segment
    // whereas i+x will be whatever the current word is
    // this returns the length of the first segment (aka the position of the first space) to be used later
    int len = str.length();
    if (len == 0) return -1;
    int firstSegLen = 0;
    while (firstSegLen < len && str[firstSegLen] != ' ') ++firstSegLen;
    if (firstSegLen == 0) return -1;

    // this moves our "cursor" position to the second word of the string, which we'll be comparing
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
                // this is absolutely horrible and a time waster and we do this later anyway so it's worthless BUT
                // the code passed all checks on tichy so im keeping this in case this handles
                // some weird edge case scenario that i dont remember right now
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

    // remove specials, spaces, dupes allat
    std::string filtered = cleanup(snd);
    if (filtered.empty()) return -1;
    // compareSegments returns the position of the first space character
    // or -1 if any segment doesn't match the rest
    int firstSpace = compareSegments(filtered);
    if (firstSpace == -1) return -1;

    char firstChar = filtered[0];
    for (int i = 0; i < 11; ++i) {
        // if the length or first letter dont match we dont need to check anything else since its already wrong
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
    // cout.tie is also a thing and does the reverse
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int N;
    std::cin >> N;
    // skips the enter after inputting N
    std::cin.ignore();
    for (int i = 0; i < N; ++i)
    {
        std::string in;
        std::getline(std::cin, in);
        // handlesound does all the work on the sound and returns the animal index or -1 if it cant find it
        int index = handleSound(in);
        if (index == -1) std::cout << "nie wiem\n";
        else std::cout << Animals[index] << '\n';
    }
}