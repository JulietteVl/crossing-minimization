#include <iostream>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>

using namespace std;

bool compare_first(const pair<int, double> &a, const pair<int, double> &b);
bool compare_second(const pair<int, double> &a, const pair<int, double> &b);

bool compare_first(const pair<int, double> &a, const pair<int, double> &b)
{
    if (a.first == b.first){
        return a.second < b.second;
    }
    return a.first < b.first;
}

bool compare_second(const pair<int, double> &a, const pair<int, double> &b)
{
    return a.second < b.second;
}