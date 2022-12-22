//
// Created by 12096 on 2022/12/8.
//

#include "HttpRequest.h"

void ltrim(std::string& s) {
    int n = s.size();
    int i = 0;
    while (i < n && s[i] == ' ') {
        ++i;
    }
    if (i == n) return;
    s = s.substr(i, s.size() - i);
}

void rtrim(std::string& s) {
    if (s.empty()) return;
    int i = s.size() - 1;
    while (i >= 0 && s[i] == ' ') {
        --i;
    }
    if (i == -1) return;
    s = s.substr(i, s.size() - i);
}

void trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}