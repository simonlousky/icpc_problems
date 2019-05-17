#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <numeric>
#include <stdio.h>

#define MAX_N 10000
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef vector<int> vi;
typedef vector<pair<int,int>> vpi;
typedef pair<int, int> pii;
int8_t lut[MAX_N][MAX_N];
int8_t update_lut(ll i, ll j)
{
    if(i == 0 || j == 0)
    {
        return 1;
    }
    if(i == j)
    {
        return lut[i][j];
    }
    lut[i][j] = update_lut(i, j-1) * update_lut(i+1, j);
    return lut[i][j];
}

int main()
{
    ll N, K;
    while(!cin.eof())
    {

        cin >> N >> K;
        // build lut

        for (ll i=1; i<=N; i++)
        {
            int x;
            cin >> x;
            if (x<0)
            {
                lut[i][i] = -1;
            }else if (x==0)
            {
                lut[i][i] = 0;
            }else
            {
                lut[i][i] = 1;
            }
        }
        update_lut(1, N+1);

        // iterate over commands
        for (ll i=0; i<K; i++)
        {
            string command;
            cin >> command;
            if(command == "C")//change
            {
                ll I, V;
                cin >> I >> V;
                if (V < 0)
                {
                    lut[I][I] = -1;
                }else if(V == 0)
                {
                    lut[I][I] = 0;
                }else
                {
                    lut[I][I] = 1;
                }
                update_lut(1, N+1);
                
            }else
            {
                ll I, J;
                cin >> I >> J;
                if(lut[I][J] == -1)
                {
                    cout << "-";
                }else if(lut[I][J] == 1)
                {
                    cout << "+";
                }else
                {
                    cout << 0;
                }
                
            }
            
        }

        cout << endl;
    }
    return 0;
}

