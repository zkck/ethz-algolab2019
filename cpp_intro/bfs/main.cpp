#include <iostream>
#include <queue>
#include <set>
#include <map>
#include <algorithm> 
using namespace std;

void routine1();
void routine2();

int main(int argc, char const *argv[])
{
    routine2();
    return 0;
}

void routine2() {
    int num_tests;
    cin >> num_tests;

    for (int t = 0; t < num_tests; t++) {

        int num_vertices, num_edges, vstart;
        cin >> num_vertices >> num_edges >> vstart;

        vector<int> edges[num_vertices];

        for (int e = 0; e < num_edges; ++e) {
            int v1, v2;
            cin >> v1 >> v2;

            edges[v1].push_back(v2);
            edges[v2].push_back(v1);
        }

        int distances[num_vertices];
        for (int i = 0; i < num_vertices; ++i) distances[i] = -1;

        // initialize variables
        queue<int> q;

        q.push(vstart);
        distances[vstart] = 0;

        while (!q.empty()) {
            int v = q.front(); q.pop();
            int d = distances[v];

            vector<int> neighbors = edges[v];
            //sort(neighbors.begin(), neighbors.end());

            for (vector<int>::iterator it = edges[v].begin(); it != edges[v].end(); it++) {
                int w = * it;
                if (distances[w] == -1) {
                    distances[w] = d + 1;
                    q.push(w);
                }
            }
        }

        for (int v = 0; v < num_vertices; ++v)
            cout << distances[v] << " ";
        cout << endl;
    }
}

void routine1() {
    int num_tests;
    cin >> num_tests;

    for (int t = 0; t < num_tests; t++) {

        int num_vertices, num_edges, vstart;
        cin >> num_vertices >> num_edges >> vstart;

        int edges[num_vertices][num_vertices] = {0};

        for (int e = 0; e < num_edges; e++) {
            int v1, v2;
            cin >> v1 >> v2;
            edges[v2][v1] = 1;
            edges[v1][v2] = 1;
        }

        // initialize BFS data structures
        queue<int> q;
        int discovered[num_vertices];

        // set all vertices to undiscovered
        for (int i = 0; i < num_vertices; ++i) discovered[i] = -1;

        // starting vertex
        q.push(vstart);
        discovered[vstart] = 0;

        while (!q.empty()) {

            // take vertex from queue and get its distance
            int v = q.front(); q.pop();
            int d = discovered[v];

            // iterate through other vertices
            for (int w = 0; w < num_vertices; ++w) {

                // if edge is available and undiscovered
                if ((edges[w][v] == 1) && (discovered[w] == -1)) {

                    // discover the vertex with a certain distance and add to queue
                    discovered[w] = d + 1;
                    q.push(w);
                }
            }
        }

        for (int v = 0; v < num_vertices; ++v)
            cout << discovered[v] << " ";
        cout << endl;
    }
}