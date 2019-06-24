#include<iostream>
#include<vector>
#include<numeric>
#include<math.h>
#include<string>
#include<iomanip>
#include<algorithm>
#include<stdio.h>

using namespace std;

#define MAX_M 10005
#define MAX_DISTANCE 51234
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef long long ll;
typedef vector<ll> vl;
typedef vector<vl> vvl;
typedef vector<double> vd;
typedef vector<vd> vvd;

/* Floyd-Warshal */
void floyd(const vvi& g, vvi& d){
    for(size_t k=0; k<g.size(); k++){
        for(size_t u=0; u<g.size(); u++){
            for(size_t v=0; v<g.size(); v++){
                d[u][v] = min(d[u][v], d[u][k]+d[k][v]);
            }
        }
    }
}

int main(){
    int T;
    scanf("%d", &T);
    for (int t = 0; t < T; t++){
        int N, R;
        vvi d, g;
        
        scanf("%d %d", &N, &R);
        for(int i=0; i<N; i++){
            string connections;
            cin >> connections;

            d.push_back(vi());
            g.push_back(vi());
            for(int j=0; j<N; j++){
                int w = connections[j] == 'N' ? MAX_DISTANCE : 1;
                
                if(i == j){
                    g[i].push_back(0);
                    d[i].push_back(0);
                }else{
                    g[i].push_back(w);
                    d[i].push_back(w);
                }
            }
        }
        /* obtain min stations between every point */
        floyd(d, d);

/* test with doubles */
        vvd ref(105, vd(10005, 0));
        for (int i=0; i<105 && i<d.size(); i++){
            for(int j=0; j<10005 && j<(i+1)*R; j++){
                if(i==0){
                    ref[i][j] = 1.0/R;
                    continue;
                }
                for(int k=1; k<=R; k++){
                    if(j-k < 0){
                        break;
                    }
                    ref[i][j] += ref[i-1][j-k] / R;
                }
            }
        }

        int C;
        scanf("%d", &C);
        printf("Case %d\n", t+1);
        for(int i=0; i<C; i++){
            int a,b,m,dist;
            double res = 0.0;
            scanf("%d %d %d", &a, &b, &m);
            dist = d[a-1][b-1];
            
            if (dist < 100 && m <= 10005){
                for(int k=0; k<m; k++){
                    res += ref[dist-1][k];
                }
            }

            cout << setprecision(6) << fixed;
            cout << res << endl;
        }
        printf("\n");
    }

    return 0;
}