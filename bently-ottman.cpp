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

    point get_left(){
        if (a.x < b.x){
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
        if (a.y < b.y){
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
        
        if ( (a.get_left().x <= p.x)
        && (a.get_right().x >= p.x)
        && (a.get_down().y <= p.y) 
        && (a.get_up().y >= p.y) )
        {
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

vector<segment>* Event::segments = nullptr;

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

    bool operator<(const Status& other) const{
        if(segment_id == other.segment_id){
            return false;
        }
        if(fabs(y_location-other.y_location) > EPS)
        {
            return y_location < other.y_location;
        }
        return false;
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

    Event::set_segments(pipes);

    priority_queue<Event> events;
    set<Status> status;
    // set<pair<int,int>> intersection_toss;

    for (size_t i=0; i<pipes.size(); i++){
        events.push(Event("start", i));
        events.push(Event("end", i));
    }

    /* Sweep */
    while(!events.empty()){
        point p;
        int a,b;
        auto event = events.top(); events.pop();

        if(event.type == "start"){
            segment seg = pipes[event.segment_id];
            status.insert(Status(event.segment_id, pipes[event.segment_id].get_left().y));
            
            /* check up and down */
            b = check_intersect_up(status, pipes, event.segment_id, p);
            if(b > -1 && result[event.segment_id][b] == false){
                events.push(Event("intersection", event.segment_id, b, p));
                result[event.segment_id][b] = true;
            }
            b = check_intersect_down(status, pipes, event.segment_id, p);
            if(b > -1 && result[event.segment_id][b] == false){
                events.push(Event("intersection", event.segment_id, b, p));
                result[event.segment_id][b] = true;
            }
        }

        else if(event.type == "end"){
            auto it = status.find(Status(event.segment_id));
            b = check_intersect_up(status, pipes, prev(it)->segment_id, p);
            if(b > -1 && result[prev(it)->segment_id][b] == false){
                events.push(Event("intersection", prev(it)->segment_id, b, p));
                result[prev(it)->segment_id][b] = true;
            }
            b = check_intersect_up(status, pipes, next(it)->segment_id, p);
            if(b > -1 && result[next(it)->segment_id][b] == false){
                events.push(Event("intersection", next(it)->segment_id, b, p));
                result[next(it)->segment_id][b] = true;
            }

            status.erase(Status(event.segment_id));
            continue;

        }else if(event.type == "intersection"){
            int s,t,r,u;
            s = event.intersection.first;
            t = event.intersection.second;

            /*  u --------------------
                t   ____     _____
                        \   /
                          X
                s   ____/   \_____
                r --------------------
            */

            // todo: here
            /* remove intersection rs intersection */

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


