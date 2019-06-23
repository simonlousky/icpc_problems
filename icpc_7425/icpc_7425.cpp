#include<iostream>
#include<vector>
#include<stdio.h>
#include<math.h>
#include<queue>
#include<stack>
#include<vector>
#include<set>
#include<map>

#define EPS 1e-9
#define endl '\n'

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef long long ll;

/* =========== CG functions ============ */

struct point { double x, y;   // only used if more precision is needed
    point() { x = y = 0.0; }                      // default constructor
    point(double _x, double _y) : x(_x), y(_y) {}        // user-defined
    bool operator < (point other) const { // override less than operator
        if (fabs(x - other.x) > EPS)                 // useful for sorting
            return x < other.x;          // first criteria , by x-coordinate
        return y < other.y; }          // second criteria, by y-coordinate
    // use EPS (1e-9) when testing equality of two floating points
    bool operator == (point other) const {
        return (fabs(x - other.x) < EPS && (fabs(y - other.y) < EPS)); } };

struct line { double a, b, c; };          // a way to represent a line

struct segment { 
    point a, b; 

    point get_left(){
        if (a.x < b.x || fabs(a.x - b.x) < EPS){
            return a;
        }
        return b;
    }
    point get_right(){
        if (a.x < b.x){
            return b;
        }
        return a;
    }
    point get_down(){
        if (a.y < b.y || fabs(a.y - b.y) < EPS){
            return a;
        }
        return b;
    }
    point get_up(){
        if (a.y < b.y){
            return b;
        }
        return a;
    }

    bool operator < (const segment& other) const { // override less than operator
        if (fabs(a.x - other.a.x) > EPS)                 // useful for sorting
            return a.x < other.a.x;          // first criteria , by x-coordinate
        return a.y < other.a.y; }   
    bool operator == (const segment& other) const{
        return a == other.a && b == other.b;
    }
};    

struct inverseCompare{
    bool operator() (const pair<int, segment>& a, const pair<int, segment>& b){
        return a > b;
    }
};

// the answer is stored in the third parameter (pass by reference)
void pointsToLine(point p1, point p2, line &l) {
    if (fabs(p1.x - p2.x) < EPS) {              // vertical line is fine
        l.a = 1.0;   l.b = 0.0;   l.c = -p1.x;           // default values
    } else {
        l.a = -(double)(p1.y - p2.y) / (p1.x - p2.x);
        l.b = 1.0;              // IMPORTANT: we fix the value of b to 1.0
        l.c = -(double)(l.a * p1.x) - p1.y;
    } 
}

bool areParallel(line l1, line l2) {       // check coefficients a & b
    return (fabs(l1.a-l2.a) < EPS) && (fabs(l1.b-l2.b) < EPS); }

// returns true (+ intersection point) if two lines are intersect
bool areIntersect(line l1, line l2, point &p) {
    if (areParallel(l1, l2)) {
        return false;      
    }
    // solve system of 2 linear algebraic equations with 2 unknowns
    p.x = (l2.b * l1.c - l1.b * l2.c) / (l2.a * l1.b - l1.a * l2.b);
    // special case: test for vertical line to avoid division by zero
    if (fabs(l1.b) > EPS) p.y = -(l1.a * p.x + l1.c);
    else                  p.y = -(l2.a * p.x + l2.c);
    return true; }

bool segmentIntersect(segment& a, segment& b, point &p) {
    line l1, l2;
    pointsToLine(a.a, a.b, l1);
    pointsToLine(b.a, b.b, l2);
    
    /* check first if have common points otherwise segments on the same line
    won't be recognized as intersecting */
    if(a.a == b.a || a.a == b.b || a.b == b.a || a.b == b.b){
        return true;
    }

    if(areIntersect(l1, l2, p)){
        double left = a.get_left().x < b.get_left().x ? b.get_left().x : a.get_left().x;
        double top = a.get_up().y < b.get_up().y ? a.get_up().y : b.get_up().y;
        double down = a.get_down().y < b.get_down().y ? b.get_down().y : a.get_down().y;
        double right = a.get_right().x < b.get_right().x ? a.get_right().x : b.get_right().x;
        if( (p.x > right && fabs(p.x - right) > EPS)
        ||  (p.x < left && fabs(p.x - left) > EPS)
        ||  (p.y < down && fabs(p.y - down) > EPS)
        ||  (p.y > top && fabs(p.y - top) > EPS) )
        {
            return false;
        }
    
        return true;
    }
    return false;
}

/* exists a cycle of odd length */
bool dfs(vvi &g, vector<int> &col, int node, int c) {
  if (col[node] != -1)
    return col[node] == c;

  col[node] = c;
  for (auto &to : g[node]) {
    if (!dfs(g, col, to, c ^ 1))
      return false;
  }
  return true;
}

/* ================================================= */

int main(){
    int w, p;
    
    while(cin >> w >> p){

        vector<point> wells;
        vector<segment> pipes;
        vvi g;
        bool impossible = false;

        for(int i=0; i<w; i++){
            point well;
            scanf("%lf %lf", &well.x, &well.y);
            /* accumulate wells */
            wells.push_back(well);
        }

        for(int i=0; i<p; i++){
            segment pipe;
            int well_idx;
            cin >> well_idx >> pipe.b.x >> pipe.b.y;
            pipe.a = wells[well_idx-1];
            /* accumulate pipes */
            pipes.push_back(pipe);
        }
        
        /* Create intersection graph */
        g.assign(p, vi());
        for(size_t i=0; i<pipes.size(); i++){
            for(size_t j=i+1; j<pipes.size(); j++){
                point p;
                if(pipes[i].a == pipes[j].a){
                    continue;
                }
                if(segmentIntersect(pipes[i], pipes[j], p)){
                    g[i].push_back(j);
                    g[j].push_back(i);
                }
            }
        }

        vector<int> col(p, -1);
        for (int i = 0; i < p; ++i) {
            if (col[i] == -1){
                if(!dfs(g, col, i, 0)){
                    impossible = true;
                    break;
                }
            }
        }

        if(impossible){
            cout << "impossible" << endl;
        }else{
            cout << "possible" << endl; 
        }

    }

    return 0;
}