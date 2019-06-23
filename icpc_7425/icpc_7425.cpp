#include<iostream>
#include<vector>
#include<stdio.h>
#include<math.h>
#include<queue>
#include<stack>
#include<vector>
#include<set>
#include<map>

#define EPS 1E-9

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

struct segment { point a, b; 
    bool operator < (const segment& other) const { // override less than operator
        if (fabs(a.x - other.a.x) > EPS)                 // useful for sorting
            return a.x < other.a.x;          // first criteria , by x-coordinate
        return a.y < other.a.y; }   
    bool operator == (const segment& other) const{
        return a == other.a && b == other.b;
    }
};    

bool operator<(const segment& a, const segment& b){
    return a.operator<(b);
}

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
    if (areParallel(l1, l2)) return false;            // no intersection
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

    if(areIntersect(l1, l2, p)){
        double x_left, x_right, y_down, y_up;
        if(a.a.x < a.b.x){
            x_left = a.a.x;
            x_right = a.b.x;
        }
        if(a.a.y < a.b.y){
            y_down = a.a.y;
            y_up = a.b.y;
        }
        if( (x_left < p.x || fabs(x_left-p.x) < EPS)
        && (x_right > p.x || fabs(x_right-p.x) < EPS) 
        && (y_down < p.y || fabs(y_down-p.y) > EPS)
        && (y_up > p.y || fabs(y_up-p.x) > EPS) ){
            return true;
        }
    }
    p.x = INT16_MAX;
    p.y = INT16_MAX;
    return false;
}

bool segmentIntersect(segment& a, double x_line, point& p){
    segment line_segment;
    line_segment.a.x = x_line;
    line_segment.a.y = INT16_MAX;
    line_segment.b.x = x_line;
    line_segment.b.y = INT16_MIN;
    return segmentIntersect(a, line_segment, p);
}

double segment_to_x(const string& s, const segment& a){
    double p;
    if(s == "start"){
        p = a.a.x < a.b.x ? a.a.x : a.b.x;
    }else if(s == "end"){
        p = a.a.x < a.b.x ? a.b.x : a.a.x;
    }else{
        p = -1;
    }
    return p;
}

/* ========= End of CG functions ========= */

/* Floyd-Warshal */
void floyd(const vvi& g, vvi& d){
    for(int k=0; k<g.size(); k++){
        for(int u=0; u<g.size(); u++){
            for(int v=0; v<g.size(); v++){
                d[u][v] = min(d[u][v], d[u][k]+d[k][v]);
            }
        }
    }
}

void dfs(vvi&g, int s, int limit, vector<bool>& visible, bool& hit, int o){
    if(limit == 0 && s == o){
        hit = true;
        return;
    }
    for(int u:g[s]) {
        if (!visible[u]) {
            // cout << "limit: " << limit << " " << s << "-->" << u << endl;
            visible[u] = true;
            dfs(g, u, limit-1, visible, hit, o);
            visible[u] = false;
        }
    }
}

bool limited_cycle(vvi& g, int s, int limit){
    vector<bool> visible;
    bool hit = false;
    visible.assign(g.size(), false);   

    dfs(g, s, limit, visible, hit, s);
    // cout << "limited cycle: " << hit << endl;
    return hit;
}

/* ========== sweep algorithm =========== */

struct Event{
    static vector<segment>* segments;
    string type;
    int segment_id;
    pair<int, int> intersection;
    point intersection_point;
    
    Event(string type, int segment_id){
        type = type;
        segment_id = segment_id;
    }

    Event(string type, int intersect_s1, int intersect_s2, point p){
        type = type;
        intersection = make_pair(intersect_s1, intersect_s2);
        intersection_point = p;
    }

    static void set_segments(vector<segment>& segments){
        segments = segments;
    }

    bool operator<(const Event& other) const{
        double a,b;
        a = (type == "intersection") ? intersection_point.x : segment_to_x(type, segments->at(segment_id));
        b = (other.type == "intersection") ? other.intersection_point.x : segment_to_x(other.type, segments->at(other.segment_id));
        return a < b;
    }
};


struct Status{
    int segment_id;
    double y_location;

    Status(int id){
        segment_id = id;
    }

    Status(int segment_id, double y_location){
        segment_id = segment_id;
        y_location = y_location;
    }

    void set_y(double y){
        y_location = y;
    }

    bool operator<(const Status& other){
        return y_location < other.y_location;
    }

    bool operator==(const Status& other){
        return segment_id == other.segment_id;
    }

    bool operator!=(const Status& other){
        return !operator==(other);
    }
};

int check_intersect_up(set<Status>& status, vector<segment>& segments, int a, point& p){
    auto it = status.find(a);
    if(next(it) != status.end()){
        /* check if they intersect */
        if(segmentIntersect(segments[it->segment_id], segments[next(it)->segment_id], p)){
            return next(it)->segment_id;
        }
    }
    return -1;
}

int check_intersect_down(set<Status>& status, vector<segment>& segments, int a, point& p){
    auto it = status.find(a);
    if(prev(it) != status.end()){
        /* check if they intersect */
        if(segmentIntersect(segments[it->segment_id], segments[prev(it)->segment_id], p)){
            return prev(it)->segment_id;
        }
    }
    return -1;
}

void find_intersections(vector<segment>& pipes, vector<vector<bool>>& result){

    priority_queue<Event> events;
    set<Status> status;

    for (int i=0; i<pipes.size(); i++){
        events.push(Event("start", i));
        events.push(Event("end", i));
    }

    /* Sweep */
    while(!events.empty()){
        point p;
        auto event = events.top(); events.pop();

        if(event.type != "intersection"){ /* start or end point */

            if(event.type == "end"){
                status.erase(Status(event.segment_id));
                continue;
            }

            double x = segment_to_x(event.type, pipes[event.segment_id]);
            segmentIntersect(pipes[event.segment_id], x, p);
            status.insert(Status(event.segment_id, p.y));

            /* check up and down */
            int b = check_intersect_up(status, pipes, event.segment_id, p);
            if(b > -1 && result[event.segment_id][b] == false){
                events.push(Event("intersection", event.segment_id, b, p));
                result[event.segment_id][b] = true;
            }
            b = check_intersect_down(status, pipes, event.segment_id, p);
            if(b > -1 && result[event.segment_id][b] == false){
                events.push(Event("intersection", event.segment_id, b, p));
                result[event.segment_id][b] = true;
            }
            
        }else{ /* is intersection */
            int a,b;
            a = event.intersection.first;
            b = event.intersection.second;

            segmentIntersect(pipes[a], event.intersection_point.x+0.01, p);
            status.erase(Status(a));
            status.insert(Status(a, p.y));

            segmentIntersect(pipes[b], event.intersection_point.x+0.01, p);
            status.erase(Status(b));
            status.insert(Status(b, p.y));

            /* check up and down for a*/
            int c = check_intersect_up(status, pipes, a, p);
            if(c > -1 && result[a][c] == false){
                events.push(Event("intersection", a, c, p));
                result[a][c] = true;
            }
            c = check_intersect_down(status, pipes, a, p);
            if(c > -1 && result[a][c] == false){
                events.push(Event("intersection", a, c, p));
                result[a][c] = true;
            }

            /* check up and down for b */
            c = check_intersect_up(status, pipes, b, p);
            if(c > -1 && result[b][c] == false){
                events.push(Event("intersection", b, c, p));
                result[b][c] = true;
            }
            c = check_intersect_down(status, pipes, b, p);
            if(c > -1 && result[b][c] == false){
                events.push(Event("intersection", b, c, p));
                result[b][c] = true;
            }

        }

    }
}



/* ================================================= */

int main(){
    int w, p;
    
    while(scanf("%d", &w) != EOF){
        scanf("%d", &p);

        vector<point> wells;
        vector<segment> pipes;
        vvi g;
        bool impossible = false;

        for(int i=0; i<w; i++){
            point well;
            scanf("%lf %lf", &well.x, &well.y);
            // cout << "well: " << well.x << "," << well.y << endl;
            /* accumulate wells */
            wells.push_back(well);
        }

        for(int i=0; i<p; i++){
            segment pipe;
            int well_idx;
            scanf("%d %lf %lf", &well_idx, &pipe.b.x, &pipe.b.y);
            pipe.a = wells[well_idx-1];
            /* accumulate pipes */
            pipes.push_back(pipe);
        }
        
        /* Create intersection graph */
        // vvi d(p, vi(p, INT16_MAX));
        vector<vector<bool>> intersection_map(p, vector<bool>(p, false));
        find_intersections(pipes, intersection_map);
        
        /* ====== */ 

        g.assign(p, vi());
        for(int i=0; i<pipes.size(); i++){
            for(int j=i; j<pipes.size(); j++){
                point p;
                if(segmentIntersect(pipes[i], pipes[j], p)
                && !(p == pipes[i].a)){
                    g[i].push_back(j);
                    g[j].push_back(i);
                }
            }
        }

        // floyd(g, d);

        // for(int i=0; i<p; i++){
        //     for(int j=0; j<p; j++){
        //         cout << d[i][j] << "\t";
                
        //     }
        //     cout << endl;
        // }

        for(int i=0; i<p; i++){
            if (limited_cycle(g, i, 3)){
                impossible = true;
                break;
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