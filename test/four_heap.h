//
// Created by 12096 on 2022/10/28.
//

#ifndef MYMUDUO_FOUR_HEAP_H
#define MYMUDUO_FOUR_HEAP_H

#include <vector>
#include <algorithm>
#include <deque>
#include <cassert>
#include <memory>
using namespace std;
template <typename T, typename Comparer = less<T>>
class Four_heap
{
public:
    Four_heap();
    ~Four_heap();
    bool empty();
    T top();
    void pop();
    void push(T Node);
private:
    void fixdown();
    void fixup();
//    int getsonIdx(int &cur,int &time);//获取子节点中最小值对应的下标。
private:
    vector<T> vec;
};

template <typename T, typename Comparer>
Four_heap<T, Comparer>::Four_heap()
{
    vec.reserve(100);
}

template <typename T, typename Comparer>
Four_heap<T, Comparer>::~Four_heap()
{
//    for (auto i : vec)
//        delete i;
}

template <typename T, typename Comparer>
bool Four_heap<T, Comparer>::empty()
{
    return vec.empty();
}

template <typename T, typename Comparer>
T Four_heap<T, Comparer>::top()
{
    assert(!vec.empty());
    return vec[0];
}

template <typename T, typename Comparer>
void Four_heap<T, Comparer>::pop()
{
    swap(vec[0], vec.back());
    T p = vec.back();
    vec.pop_back();
    // delete p;
    if (vec.size() > 1)
        fixdown();
}

template <typename T, typename Comparer>
void Four_heap<T, Comparer>::push(T Node)
{
    vec.emplace_back(Node);
    fixup();
}
//#ifdef 	getsondix
//void Four_heap::fixdown()//用于pop操作，交换首位元素，弹出尾部元素，然后fixdown()
//{
//	int curIdx = 0;
//	TimerNode*key = vec[curIdx];
//	int key_time = key->getExpTime();
//	int sIdx = getsonIdx(curIdx, key_time);
//	while (sIdx != -1)
//	{
//		if (vec[sIdx]->getExpTime() < key_time)
//		{
//			vec[curIdx] = vec[sIdx];
//			curIdx = sIdx;
//			sIdx = getsonIdx(curIdx, key_time);
//		}
//		else
//			break;
//	}
//	vec[curIdx] = key;
//}
//#else
template <typename T, typename Comparer>
void Four_heap<T, Comparer>::fixdown()//用于pop操作，交换首位元素，弹出尾部元素，然后fixdown()
{
    Comparer c;
    int curIdx = 0;
    T key_time = vec[curIdx];
    int sIdx = -1;
    while ((curIdx<<2) + 1 < vec.size())
    {
        auto mintime = key_time;
        for (int i = (curIdx << 2) + 1; i <= (curIdx << 2) + 4 && i < vec.size(); i++)
        {
            auto curtime = vec[i];
            mintime = min(curtime, mintime, c);
            if (curtime == mintime)
                sIdx = i;
        }
        if (sIdx == -1)
            break;
        vec[curIdx] = vec[sIdx];
        curIdx = sIdx;
        sIdx = -1;
    }
    vec[curIdx] = key_time;
}
//#endif

template <typename T, typename Comparer>
void Four_heap<T, Comparer>::fixup()
{
    Comparer c;
    int curIdx = vec.size() - 1;
    if (curIdx == 0)
        return;
    int fIdx = (curIdx - 1) / 4;
    T key_time = vec[curIdx];
    while (curIdx)
    {
        if (key_time == min(key_time, vec[fIdx], c))
        {
            vec[curIdx] = vec[fIdx];
            curIdx = fIdx;
            fIdx = (curIdx - 1) / 4;
        }
        else
            break;
    }
    vec[curIdx] = key_time;
}
//
//template <typename T>
//int Four_heap<T>::getsonIdx(int &cur,int &time)
//{
//    int i = 4 * cur + 1, mintime = time, idx = -1;
//    while (i <= 4*cur + 4 && i < vec.size())
//    {
//        int curtime = vec[i]->getExpTime();
//        mintime = min(curtime, mintime);
//        if (curtime == mintime)
//            idx = i;
//        i++;
//    }
//    return idx;
//}

#endif //MYMUDUO_FOUR_HEAP_H
