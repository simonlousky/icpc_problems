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

/* Floyd-Warshal */
void floyd(const vvl& g, vvl& d){
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
        vvl d, g;
        
        scanf("%d %d", &N, &R);
        for(int i=0; i<N; i++){
            string connections;
            cin >> connections;

            d.push_back(vl());
            g.push_back(vl());
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

        vvi ref(10005, vi(105, 0));
        for (int i=0; i<105 && i<d.size(); i++){
            for(int j=0; j<1002 && j<(i+1)*R; j++){
                if(i==0){
                    ref[i][j] = 1;
                    continue;
                }
                for(int k=1; k<=R; k++){
                    if(j-k < 0){
                        break;
                    }
                    ref[i][j] += ref[i-1][j-k];
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

            for(int k=0; k<m; k++){
                res += ref[dist-1][k] / pow(R,dist);
            }
            
            if(dist >= MAX_DISTANCE)
            {
                res = 0.0;
            }else if(a == b || m > pow(R, dist)){
                res = 1.0;
            }
            cout << setprecision(6) << fixed;
            cout << res << endl;
        }
        if(t != T-1){
            printf("\n");
        }
    }

    return 0;
}