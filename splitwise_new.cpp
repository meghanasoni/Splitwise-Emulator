#include <bits/stdc++.h>
using namespace std;
int n = 0;
struct Edge
{
    int v; // Vertex v (or "to" vertex) of a directed edge u-v. "From" vertex u can be obtained using index in adjacent array.

    int flow; // flow of data in edge

    int C; // capacity

    int rev; // To store index of reverse edge in adjacency list so that we can quickly find it.
};

// Residual Graph
class Graph
{
    int V; // number of vertex
    int* level; // stores level of a node
    vector<Edge>* adj;
    vector<pair<int, int>> edge_set;
public:
    Graph(int V)
    {
        adj = new vector<Edge>[V];
        this->V = V;
        level = new int[V];
    }
    vector<pair<int, int>> getEdges()
    {
        return edge_set;
    }
    // add edge to the graph
    void addEdge(int u, int v, int C)
    {
        // Forward edge : 0 flow and C capacity
        Edge a{ v, 0, C, (int)adj[v].size() };

        // Back edge : 0 flow and 0 capacity
        Edge b{ u, 0, 0, (int)adj[u].size() };

        adj[u].push_back(a);
        adj[v].push_back(b); // reverse edge
        n++;
        edge_set.push_back(make_pair(u, v));
    }
    void printEdges();
    bool BFS(int s, int t);
    int sendFlow(int s, int flow, int t, int ptr[]);
    int DinicMaxflow(int s, int t);
    Graph residualGraph(int s, int t);
    void printDebt();
};

// Finds if more flow can be sent from s to t.
// Also assigns levels to nodes.
bool Graph::BFS(int s, int t)
{
    for (int i = 0; i < V; i++)
        level[i] = -1;

    level[s] = 0; // Level of source vertex

    // Create a queue, enqueue source vertex
    // and mark source vertex as visited here
    // level[] array works as visited array also.
    list<int> q;
    q.push_back(s);

    vector<Edge>::iterator i;
    while (!q.empty())
    {
        int u = q.front();
        q.pop_front();
        for (i = adj[u].begin(); i != adj[u].end(); i++)
        {
            Edge& e = *i;
            if (level[e.v] < 0 && e.flow < e.C)
            {
                // Level of current vertex is,
                // level of parent + 1
                level[e.v] = level[u] + 1;

                q.push_back(e.v);
            }
        }
    }

    // IF we can not reach to the sink we
    // return false else true
    return level[t] < 0 ? false : true;
}

// A DFS based function to send flow after BFS has
// figured out that there is a possible flow and
// constructed levels. This function called multiple
// times for a single call of BFS.
// flow : Current flow send by parent function call
// start[] : To keep track of next edge to be explored.
//           start[i] stores  count of edges explored
//           from i.
//  u : Current vertex
//  t : Sink
int Graph::sendFlow(int u, int flow, int t, int start[])
{
    // Sink reached
    if (u == t)
        return flow;

    // Traverse all adjacent edges one -by - one.
    for (; start[u] < adj[u].size(); start[u]++)
    {
        // Pick next edge from adjacency list of u
        Edge& e = adj[u][start[u]];

        if (level[e.v] == level[u] + 1 && e.flow < e.C)
        {
            // find minimum flow from u to t
            int curr_flow = min(flow, e.C - e.flow);

            int temp_flow
                = sendFlow(e.v, curr_flow, t, start);

            // flow is greater than zero
            if (temp_flow > 0)
            {
                // add flow  to current edge
                e.flow += temp_flow;

                // subtract flow from reverse edge
                // of current edge
                adj[e.v][e.rev].flow -= temp_flow;
                return temp_flow;
            }
        }
    }

    return 0;
}
void Graph::printEdges()
{
    int n_transactions = 0;
    for (int i = 0; i < V; i++)
    {
        for(auto x: adj[i])

        {
            if(x.C!=0){
                n_transactions++;
                cout<<i<<" owes "<<x.v<<" an amount of "<<x.C<<endl;
            }
        }
    }
    cout<<"Total number of transactions are "<<n_transactions<<endl;
}
Graph Graph::residualGraph(int s, int t)
{
    Graph rg(8);
    if (s == t)
        return rg;

   /* int total = 0; // Initialize result

    // Augment the flow while there is path
    // from source to sink
    while (BFS(s, t) == true)
    {
        // store how many edges are visited
        // from V { 0 to V }
        int* start = new int[V + 1] { 0 };

        // while flow is not zero in graph from S to D
        while (int flow = sendFlow(s, INT_MAX, t, start))

            // Add path flow to overall flow
            total += flow;
    }*/
    for (int i = 0; i < V; i++)
    {
        for(auto edge: adj[i])
        {
            {
                //cout<<"Checking the edge "<<i<<" "<<edge.v<<" "<<edge.C<<" "<<edge.flow<<endl;
                int remainingCap;
                if(edge.flow < 0){
                    remainingCap = edge.C;
                }
                else{
                    remainingCap = edge.C - edge.flow;
                }
                //  If there is capacity remaining in the graph, then add the remaining capacity as an edge
                //  so that it can be used for optimizing other debts within the graph
                if(remainingCap > 0)
                {
                    rg.addEdge(i, edge.v, remainingCap);
                }
            }
        }
    }
    return rg;
}
void Graph::printDebt(){
    int debt[V] = {0};
    for (int i = 0; i < V; i++)
    {
        for(auto x: adj[i])

        {
            if(x.C!=0) {debt[i]+=x.C; debt[x.v]-=x.C;}
           // cout<<i<<" "<<x.v<<" capacity = "<<x.C<<" flow = "<<x.flow<<endl;
        }
    }
    cout<<"The debts are : ";
    for(int i=1; i<V; i++){
        cout<<i<<" -> "<<debt[i]<<"  ";
    }
    cout<<endl;
}
// Returns maximum flow in graph
        int Graph::DinicMaxflow(int s, int t)
        {
            // Corner case
            if (s == t)
                return -1;

            int total = 0; // Initialize result

            // Augment the flow while there is path
            // from source to sink
            while (BFS(s, t) == true)
            {
                // store how many edges are visited
                // from V { 0 to V }
                int* start = new int[V + 1] { 0 };

                // while flow is not zero in graph from S to D
                while (int flow = sendFlow(s, INT_MAX, t, start))

                    // Add path flow to overall flow
                    total += flow;
            }
            // return maximum flow
            return total;
        }
        map<pair<int, int>, bool> visited;
        Graph solve(Graph g, int u, int v)
        {
            int maxflow = g.DinicMaxflow(u, v);
            //cout<<"Maximum flow = "<<maxflow<<endl;
            visited[make_pair(u, v)] = true;
            Graph rg = g.residualGraph(u, v);
            if(maxflow > 0)
            {
                rg.addEdge(u, v, maxflow);
            }

            //cout<<"The residual graph is:"<<endl;
            //rg.printEdges();
            //cout<<endl;
            return rg;
        }
// Driver Code
int main()
{
    int V;
    cout<<"Enter the number of members in the group : ";
    cin>>V;
    Graph g(V+1);
    cout<<"Enter A to add an transaction as (a, b, c) where (a owes b c dollars) OR -1 to end."<<endl;
    string s;
    cin>>s;
    while(s=="A"){
        int a, b, c;
        cin>>a>>b>>c;
        g.addEdge(a, b, c);
        cin>>s;
    }
    cout<<"Initially :" <<endl;
    g.printEdges();
    //g.printDebt();
    vector<pair<int, int>> edge_set = g.getEdges();
    for(int i=0; i<n; i++)
    {
        for(auto x: edge_set)
        {
            int u = x.first;
            int v = x.second;
            if(!visited[x])
            {
               // cout<<"Checking edge from "<<u<<" to "<<v<<endl;
                g = solve(g, u, v);
                break;
            }
        }
    }
    //final ans stored in g
   cout<<endl<<"Finally :" <<endl;
    g.printEdges();
    //g.printDebt();
    return 0;
}


/*Graph g(8);
    //Alice = 1 Bob = 2 Charlie = 3 David = 4 Ema = 5 Fred = 6 Gabe = 7
    g.addEdge(6, 5, 10);
    g.addEdge(7, 2, 30);
    g.addEdge(7, 4, 10);
    g.addEdge(6, 2, 10);
    g.addEdge(6, 3, 30);
    g.addEdge(6, 4, 10);
    g.addEdge(2, 3, 40);
    g.addEdge(3, 4, 20);
    g.addEdge(4, 5, 50);*/

    /*A 2 4 10
    A 3 4 10
    A 2 4 20
    -1*/

