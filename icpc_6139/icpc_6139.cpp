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
#include <sstream>

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef vector<int> vi;
typedef vector<pair<int,int>> vpi;
typedef pair<int, int> pii;

// fenwick tree
int8_t fen_sum(int8_t* fen_arr, ll idx)
{
    int result = 0;
    while(idx)
    {
        result += fen_arr[idx];
        idx -= idx & -idx;
    }
    return result;
}

// fenwick tree
void fen_add(int8_t* fen_arr, ll idx, ll N)
{
    while(idx < N)
    {
        fen_arr[idx] += 1;
        idx += (idx & -idx);
    }
}

// fenwick tree
void fen_sub(int8_t* fen_arr, ll idx, ll N)
{
    while(idx < N)
    {
        fen_arr[idx] -= 1;
        idx += (idx & -idx);
    }
}

int main()
{
    ll N, K;
    while(scanf("%lld %lld", &N, &K) == 2)
    {
        set<int> zeros;

        // cin >> N >> K;
        int8_t* fen_minus = new int8_t[N+1];
        int8_t* fen_zeros = new int8_t[N+1];
        int8_t* orig = new int8_t[N+1];
        for(ll i=0; i<=N; ++i)
        {
            fen_minus[i] = 0;
            fen_zeros[i] = 0;
        }

        for(ll i=1; i<=N; i++)
        {
            int x;
            cin >> x;
            orig[i] = (x == 0) ? 0 : x/abs(x);

            if(x < 0)
            {
                fen_add(fen_minus, i, N+1);
            }
            if(x == 0)
            {
                fen_add(fen_zeros, i, N+1);
            }
        }

        for (ll i=0; i<K; i++)
        {
            string command;
            cin >> command;
            if(command == "C")//change
            {
                int I, V;
                cin >> I >> V;
                V = (V == 0) ? 0 : V/abs(V);
                int8_t tmp = orig[I];
                orig[I] = V;

                if(V == tmp)
                {
                    continue;
                }else if(V < 0)
                {
                    fen_add(fen_minus, I, N+1);

                    // if previous was zero remove zero
                    if(tmp == 0)
                    {
                        fen_sub(fen_zeros, I, N+1);
                    }
                }else if(V == 0)
                {
                    fen_add(fen_zeros, I, N+1);

                    // if previous was minus
                    if(tmp == -1)
                    {
                        fen_sub(fen_minus, I, N+1);
                    }
                }else
                {
                    if(tmp == -1)
                    {
                        fen_sub(fen_minus, I, N+1);
                    }else
                    {
                        fen_sub(fen_zeros, I, N+1);
                    }
                }
            }else
            {
                int I, J;
                cin >> I >> J;
                int8_t res = fen_sum(fen_minus, J) - fen_sum(fen_minus, I) + (orig[I]==-1);
                int8_t zero = fen_sum(fen_zeros, J) - fen_sum(fen_zeros, I) + (orig[I]==0);
                if(!zero) // handle zeros
                {
                    cout << ((res % 2) ? "-" : "+");
                }
                else{
                    cout << 0;
                }
            }
            
        }

        cout << endl;
    }
    return 0;
}

