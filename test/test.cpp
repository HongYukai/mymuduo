////
//// Created by 12096 on 2022/10/16.
////

#include <iostream>
#include <string.h>
#include <set>
#include <queue>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include "four_heap.h"


using namespace std;

int main() {
    multiset<int> s;
    priority_queue<int, vector<int>, greater<int>> pq;
    Four_heap<int> fh;
    for (auto i = 0; i < 2000000; i++) {
        auto x = rand() % 800000;
        s.insert(x);
        pq.push(x);
        fh.push(x);
    }
    struct timeval t1, t2;
    int x = 200000;
    vector<int> v;
    gettimeofday(&t1, NULL);
    auto it = s.lower_bound(x);
    copy(s.begin(), it, back_inserter(v));
    s.erase(s.begin(), it);
    for (int i = 0; i < v.size(); i++) {
        s.insert(v[i]);
    }
    gettimeofday(&t2, NULL);
    cout << "The time of getting k nums by rb-tree is " << 1000000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) << endl;
    v.clear();
    gettimeofday(&t1, NULL);
    while (pq.top() < x) {
        v.push_back(pq.top());
        pq.pop();
    }
    for (int i = 0; i < v.size(); i++) {
        pq.push(v[i]);
    }
    gettimeofday(&t2, NULL);
    cout << "The time of getting k nums by two_heap is " << 1000000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) << endl;
    v.clear();
    gettimeofday(&t1, NULL);
    while (fh.top() < x) {
        v.push_back(fh.top());
        fh.pop();
    }
    for (int i = 0; i < v.size(); i++) {
        fh.push(v[i]);
    }
    gettimeofday(&t2, NULL);
    cout << "The time of getting k nums by four_heap is " << 1000000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) << endl;

}

