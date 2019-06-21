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
#include <string>
#include <set>
#include <stack>
#include <string>
#include <vector>
#include <numeric>
#include <list>

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef vector<int> vi;
typedef pair<int, int> pii;


typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<vii> vvii;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef set<int> si;
typedef vector<si> vsi;

#define N_MAX 27


int main() {
    ll T;
//    char s[N_MAX];
    string s;
    queue<pair<char, int>> q;
    scanf("%lld", &T);
    for (int t=0; t<T; t++) {
        cin >> s;
        for (int i = 0; i < s.length(); i++) {
            q.push(pair<char, int>(s[i], i));
        }


        int r_idx = 0;
        int cycle = 0;
        int n = s.length();
        bool bad = false;


        while (!q.empty()) {
            pair<char, int> a = q.front(); q.pop();
            pair<char, int> last;
            // pair<char, int> b = q.front(); q.pop();

            if(a.first == q.front().first){
                while (!q.empty() && q.front().first == a.first) {
                    q.pop();
                }
            }else{
                q.push(a);
            }
            
            cycle++;
            if (cycle == 2 * n) break;
        }
        if (q.empty()) {
            cout << 1 << endl;
        }else{
            cout << 0 << endl;
        }
    }

    return 0;
}
 