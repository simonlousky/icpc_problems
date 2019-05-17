#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
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
    ll N, K;
    while(!cin.eof())
    {
        set<int> zeros;
        set<int> minus;

        cin >> N >> K;
        for (ll i=1; i<=N; i++)
        {
            int x;
            cin >> x;
            if (x<0)
            {
                minus.insert(i);
            }else if (x==0)
            {
                zeros.insert(i);
            }
        }

        // iterate over commands
        for (ll i=0; i<K; i++)
        {
            string command;
            cin >> command;
            if(command == "C")//change
            {
                int I, V;
                cin >> I >> V;
                if (V < 0)
                {
                    minus.insert(I);
                    zeros.erase(I);
                }else if(V == 0)
                {
                    zeros.insert(I);
                    minus.erase(I);
                }else
                {
                    zeros.erase(I);
                    minus.erase(I);
                }
                
            }else
            {
                int I, J;
                cin >> I >> J;
                bool zero = false;
                bool negative = false;
                for (auto idx: zeros)
                {
                    if (I <= idx && idx <= J)
                    {
                        zero = true;
                        break;
                    }
                }
                if(zero){
                    cout << 0;
                }else{
                    
                    for (auto idx : minus)
                    {
                        if (I <= idx && idx <= J)
                        {
                            negative = !negative;
                        }
                    }
                    cout << (negative ? "-" : "+");
                }
            }
            
        }

        cout << endl;
    }
    return 0;
}

