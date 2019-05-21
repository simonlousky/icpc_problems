#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>
#include <numeric>
#include <stdio.h>

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef vector<int> vi;
typedef vector<pair<int,int>> vpi;
typedef pair<int, int> pii;

int main()
{
    string word;
    int Q_num, Period, K;
    vpi registered;
    vpi Queue;
    while (cin >> word)
    {
        if(word == "#")
        {
            cin >> K;

            for(auto entry : registered)
            {
                for(ll i=1; i<=K; i++)
                {
                    Queue.push_back(pii(entry.first * i, entry.second));
                }
            }

            sort(Queue.begin(), Queue.end());
            for(int i = 0; i < K; i++)
            {
                cout << Queue[i].second << endl;
            }
            return 0;
        }

        cin >> Q_num >> Period;
        registered.push_back(pii(Period, Q_num));
    }
    return 0;
}

