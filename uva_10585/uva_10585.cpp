#include<vector>
#include<iostream>
#include<stdio.h>
#include<set>
#include<algorithm>
#include<math.h>
#include<tuple>
#include<queue>
#include<stack>

#define EPS 1e-9

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef vector<int> vi;
typedef vector<pair<int,int>> vpi;
typedef pair<int, int> pii;

#define EPS 1e-9
#define MAX_SIZE 100

typedef struct PT
{
    double x,y;
    
    // compare the lexicographic order of 2 points
    // first compare x, if 2 x's are equal, compare y
    bool operator < (const PT& _p) const
    {
        if (fabs(x - _p.x) > EPS) {
            return (x < _p.x);
        } 
        return (y < _p.y);
    }
    bool operator==(const PT& _p) const
    {
        return ( (fabs(x - _p.x) < EPS) && (fabs(y - _p.y) < EPS) );
    }
}PT;

// check p3 (leftmost) -> p2 -> p1 (rightmost) left turn (not right turn)
bool left(PT& p1, PT& p2, PT& p3)
{
    double cross = (p2.x - p3.x) * (p1.y - p3.y) - (p2.y - p3.y) * (p1.x - p3.x);
    
    if (fabs(cross) <= EPS)
        return true;    // to delete p2
    if (cross > 0)
        return true;
    return false;
}

vector<PT> get_convex_hull(vector<PT>& Points) {
    
    PT p;
    //-------------- incremental alg. ---------
    // upper hull
    sort(Points.begin(), Points.end());
    stack<PT> stk_up;
    stk_up.push(Points[0]);
    stk_up.push(Points[1]);
    for (int i=2; i<Points.size(); i++) {
        while ((stk_up.size() >= 2)) {
            PT p2, p3;
            p2 = stk_up.top();
            stk_up.pop();
            p3 = stk_up.top();
            if (!left(Points[i], p2, p3)){
                stk_up.push(p2);
                break;
            }
        }
        stk_up.push(Points[i]);
    }
    // lower hull
    for (int i=0; i<Points.size(); i++) {
        Points[i].x = -Points[i].x;
        Points[i].y = -Points[i].y;
    }
    sort(Points.begin(), Points.end());
    stack<PT> stk_low;
    stk_low.push(Points[0]);
    stk_low.push(Points[1]);
    for (int i=2; i<Points.size(); i++) {
        while ((stk_low.size() >= 2)) {
            PT p2, p3;
            p2 = stk_low.top();
            stk_low.pop();
            p3 = stk_low.top();
            if (!left(Points[i], p2, p3)){
                stk_low.push(p2);
                break;
            }
        }
        stk_low.push(Points[i]);
    }
    
    // print convex hull - cw order from leftmost point
    vector<PT> CH;
    stk_low.pop();
    while (!stk_low.empty()) {
        p = stk_low.top();
        p.x = -p.x;
        p.y = -p.y;
        CH.push_back(p);
        stk_low.pop();
    }
    stk_up.pop();
    while (!stk_up.empty()) {
        CH.push_back(stk_up.top());
        stk_up.pop();
    }
    reverse(CH.begin(), CH.end());  // ccw -> cw
    // for (vector<PT>::const_iterator iter = CH.begin(); iter != CH.end(); iter++)
    //     cout << (*iter).x << ' ' << (*iter).y << endl;
    
    return CH;
}

int main()
{
    int cases;
    bool bad = false;
    scanf("%d", &cases);
    for(int i=0; i<cases; ++i){
        vector<PT> Points;
        vector<PT> orig;
        vector<PT> convex_hull;
        int n;
        scanf("%d", &n);
        for (int i=0; i<n; i++){
            PT tmp;
            scanf("%lf %lf", &tmp.x, &tmp.y);
            Points.push_back(tmp);
            orig.push_back(tmp);
            // cout << tmp.x << ", " << tmp.y << endl;
        }
        // find convex hull
        convex_hull = get_convex_hull(Points);
        if (convex_hull.size() % 2)
        {
            cout << "no" << endl;
            continue;
        }
        // there are 2n vertices in hull
        PT a = convex_hull[0]; 
        PT b = convex_hull[convex_hull.size()/2];
        // get center between 1 and n
        PT s;
        s.x = (b.x + a.x) / 2; s.y = (b.y + a.y) / 2;
        // check for every point that there exist a symetrical q in S at the opposite side of the center
        for(auto pt : orig)
        {
            // cout << "pt: " << pt.x << "," << pt.y << endl;
            double x = 2*s.x - pt.x; double y = 2*s.y - pt.y;
            PT tmp; tmp.x=x; tmp.y=y;
            // cout << "checking if " << x << ", " << y << " is in vector" << endl;
            if(find(orig.begin(), orig.end(), tmp) == orig.end())
            {
                cout << "no" << endl;
                bad = true;
                break;
            }
        }
        if(!bad)
        {
            cout << "yes" << endl;
        }
        bad = false;
    }
    // cout << endl;
    return 0;
}