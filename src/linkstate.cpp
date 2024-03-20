#include "../include/node.hpp"

//function dijkstra(G, S)
    // for each vertex V in G
    //     dist[V] <- infinite
    //     prev[V] <- NULL
    //     If V != S, add V to Priority Queue Q
    // dist[S] <- 0
    
    // while Q IS NOT EMPTY
    //     U <- Extract MIN from Q
    //     for each unvisited neighbour V of U
    //         temperoryDist <- dist[U] + edgeWeight(U, V)
    //         if temperoryDist < dist[V]
    //             dist[V] <- temperoryDist
    //             prev[V] <- U
    // return dist[], prev[]

void dijkstra(int source) {
    int n = adj_list.size(); // Number of vertices
    dist_vector.clear();
    dist_vector.resize(n, vector<Node>(n, {0, 0, INT_MAX})); // Initialize distances as infinite, except for the source
    
    // Custom comparator for the priority queue
    auto compare = [](const pair<int, int>& left, const pair<int, int>& right) {
        return left.second > right.second;
    };
    
    priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(compare)> pq(compare);

    pq.push({source, 0});
    dist_vector[source][source] = {source, source, 0}; // Distance to itself is 0

    vector<bool> visited(n, false); // Keep track of visited nodes

    while (!pq.empty()) {
        int u = pq.top().first;
        pq.pop();

        if (visited[u]) continue; // If already visited, skip
        visited[u] = true;

        for (auto& edge : adj_list[u]) {
            int v = edge.first, weight = edge.second;

            if (dist_vector[source][v].cost > dist_vector[source][u].cost + weight) {
                // Update the distance vector for the better path
                dist_vector[source][v].destination = v;
                dist_vector[source][v].nextHop = (u == source) ? v : dist_vector[source][u].nextHop;
                dist_vector[source][v].cost = dist_vector[source][u].cost + weight;

                pq.push({v, dist_vector[source][v].cost});
            }
        }
    }
}

int main(int argc, char *argv[]){
    if(argc != 4){
        cout << "Usage: " << argv[0] << "./lsr <topologyFile> <messageFile> <changesFile>" << endl;
        return -1;
    }

    int vertices = 0;
    int edges = 0; 
    ifstream file1(argv[1]);
    string char1, char2, char3;
    while(file1 >> char1 >> char2 >> char3){
        if(char1 == "EOF" || char2 == "EOF" || char3 == "EOF"){
            break;
        }
        vertices = max(vertices,max(stoi(char1),stoi(char2)));
        edges++;
    }
    file1.close();
    graph_init(vertices);
    dis_vec_init(vertices);
    //adding edges to the graph
    ifstream file2(argv[1]);

    while(file2 >> char1 >> char2 >> char3){
        if(char1 == "EOF" || char2 == "EOF" || char3 == "EOF"){
            break;
        }
        if(stoi(char3) != -999) graph_edge(stoi(char1),stoi(char2),stoi(char3));
    }
    file2.close();
    extract_message(argv[2]);
    extract_changes(argv[3]);

    const char *op = argv[4] == NULL ? "output.txt" : argv[4];

    for(int count = 0; count < msg.size() ; count++){
        int source = msg[count].first;
        int destination = msg[count].second.first;
        dijkstra(source);
        print_distance_vector(op);
        vector<int> hops = tot_hops(source, destination);
        add_hops_message(op, source, destination, hops, msg[count].second.second);
    }

    dist_vector.clear();

    for(int count = 0; count < changes.size(); count++){
        if(changes[count].second.second == -999){
            delete_edge(changes[count].first, changes[count].second.first);
        }else{
            update_weight(changes[count].first, changes[count].second.first, changes[count].second.second);
        }

        dist_vector.clear();
        dis_vec_init(vertices);
        
        for(int count = 0; count < vertices; count++) dijkstra(count);
        print_distance_vector(op);
        
        for(int count = 0; count < msg.size() ; count++){
            int source = msg[count].first;
            int destination = msg[count].second.first;
            vector<int> hops = tot_hops(source, destination);  
            add_hops_message(op, source, destination, hops, msg[count].second.second);
        }
    }

    return 0;
}

void run_dijkstra_for_each_line(const string& messageFile) {
    ifstream file(messageFile);
    string line;
    while (getline(file, line)) {
        if (line.length() >= 3) {
            int source = line[0] - '0';
            int destination = line[2] - '0';
            dijkstra(source);
            // Perform operations with the obtained shortest path from source to destination
            // ...
        }
    }
    file.close();
}
