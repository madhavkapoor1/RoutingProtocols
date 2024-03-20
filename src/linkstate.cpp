#include "common.hpp"

/**
 * @brief Performs Dijkstra's algorithm to find the shortest path from each node to all other nodes in the graph.
 *
 * This function uses Dijkstra's algorithm to calculate the shortest path from each node to all other nodes in the graph.
 * It initializes the distance and next hop arrays, and uses a priority queue to process the nodes in order of their distances.
 * The function updates the distance and next hop arrays as it explores the graph, and stores the results in the distance vector.
 *
 * @note This function assumes that the adjacency list and other necessary data structures have been initialized before calling it.
 */
void dijkstra() {
    for(int i = 1; i< adj_list.size(); i++){
        int src = i;
        priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, compare> pq;        vector<int> dist(adj_list.size(), INT_MAX);
        vector<int> nextHops(adj_list.size(), -1);
        dist[i] = 0;
        nextHops[i] = i;
        pq.push(make_tuple(0, i, i));
        while(!pq.empty()){
            int dis = get<0>(pq.top());
            int node = get<1>(pq.top());
            int prev = get<2>(pq.top());

            pq.pop();
            for(auto it : adj_list[node]){
                int edgeweight = it.second;
                int adjNode = it.first;
                if(node == i){
                    prev = adjNode;
                }
            
                if(dis + edgeweight < dist[adjNode]){
                    dist[adjNode] = dis + edgeweight;
                    nextHops[adjNode] = prev;
                    pq.push(make_tuple(dist[adjNode], adjNode, prev));
                }
            }
        }
        for(int j = 1; j < adj_list.size(); j++){
            Node temp;
            temp.destination = j;
            temp.nextHop = nextHops[j];
            temp.cost = dist[j];
            dist_vector[src].push_back(temp);
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

    dijkstra();
    const char *op = argv[4] == NULL ? "output.txt" : argv[4];
    print_distance_vector(op);
    for(int count = 0; count < msg.size() ; count++){
       // cout<<"here"<<endl;
        vector<int> hops = tot_hops(msg[count].first, msg[count].second.first);        
        add_hops_message(op, msg[count].first, msg[count].second.first, hops, msg[count].second.second);
    }

    dist_vector.clear();

    for(int count = 0; count < changes.size(); count++){
        if(changes[count].second.second == -999){
            delete_edge(changes[count].first, changes[count].second.first);
        }else{
           // cout << "Updating weight of edge " << changes[count].first << " " << changes[count].second.first << " to " << changes[count].second.second << endl;
            update_weight(changes[count].first, changes[count].second.first, changes[count].second.second);
           // print_graph();
        }

        if(changes[count].first > vertices || changes[count].second.first > vertices){
            vertices = max(changes[count].first, changes[count].second.first);
        } 
        
        dist_vector.clear();
        dis_vec_init(vertices);
        
        dijkstra();
        print_distance_vector(op);
        
        for(int count = 0; count < msg.size() ; count++){
           // cout<<"here"<<endl;
            vector<int> hops = tot_hops(msg[count].first, msg[count].second.first);  
            add_hops_message(op, msg[count].first, msg[count].second.first, hops,msg[count].second.second);
        }
    }

    return 0;
}