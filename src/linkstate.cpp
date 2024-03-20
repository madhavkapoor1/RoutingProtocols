#include "linkstate.hpp"


using namespace std;
vector<vector<pair<int,int>>> adj_list; // pair of node and distance
vector<vector<Node>> dist_vector; // vector of vector of nodes
vector<pair<int,pair<int,string>>> msg;
vector<pair<int,pair<int,int>>> changes;

//since the graph is undirected we need to add the edge to both nodes
void graph_init(int n){
    vector<vector<pair<int,int>>> temp(n+1, vector<pair<int,int>>(1,pair<int,int>(0,0)));
    adj_list = temp;
    return;
}

void dis_vec_init(int n){
    Node temp1;
    temp1.destination = 0;
    temp1.nextHop = 0;
    temp1.cost = 0;
    vector<vector<Node>> temp2(n+1, vector<Node>(1,temp1)); //will check later
    dist_vector = temp2;
    return;
}

void graph_edge(int n, int m, int weight){
    adj_list[n].push_back(pair<int,int>(m,weight));
    adj_list[m].push_back(pair<int,int>(n,weight));
}

void update_weight(int n, int m, int weight){
    int i = 0;
    for(int count = 1; count < adj_list[n].size(); count++){
        if(adj_list[n][count].first == m){
            adj_list[n][count].second = weight;
            i++;
            break;
        }
    }
    for(int count = 1; count < adj_list[m].size(); count++){
        if(adj_list[m][count].first == n){
            adj_list[m][count].second = weight;
            i++;
            break;
        }
    }
    if(i != 2){
        graph_edge(n,m,weight);
    }
}

void delete_edge(int n, int m){
    for(int count = 1; count < adj_list[n].size(); count++){
        if(adj_list[n][count].first == m){
            adj_list[n].erase(adj_list[n].begin() + count);
            break;
        }
    }
    for(int count = 1; count < adj_list[m].size(); count++){
        if(adj_list[m][count].first == n){
            adj_list[m].erase(adj_list[m].begin() + count);
            break;
        }
    }
}

void print_graph(){
    for(int i = 1; i < adj_list.size(); i++){
        //cout << "Node " << i << " is connected to: ";
        for(int j = 0; j < adj_list[i].size(); j++){
            //cout << adj_list[i][j].first << " with weight " << adj_list[i][j].second << " ";
        }
        //cout << endl;
    }
}

struct compare {
    bool operator()(const tuple<int, int, int>& a, const tuple<int, int, int>& b) const {
        return get<0>(a) > get<0>(b);
    }
};


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

vector<int> tot_hops(int src, int dst){
    vector<int> hops;
    hops.push_back(src);
    while(dist_vector[src][dst].nextHop != dst){
        hops.push_back(dist_vector[src][dst].nextHop);
        src = dist_vector[src][dst].nextHop;
    }
    return hops;
}

void print_distance_vector(const char* file){
    ofstream outfile(file, std::ios_base::app);
    for(int i = 1; i < dist_vector.size(); i++){
        for(int j = 1; j < dist_vector[i].size(); j++){
            outfile << dist_vector[i][j].destination << " " << dist_vector[i][j].nextHop << " " << dist_vector[i][j].cost << endl;
        }
    }
    outfile.close();
}

void extract_message(char* file){
    ifstream file1(file);
    string line;
    while(getline(file1,line)){
        istringstream iss(line);
        int num1, num2;
        string rest_of_line;
        if(!(iss >> num1 >> num2)){
            break;
        }
        getline(iss, rest_of_line); // This will get the rest of the line
        msg.push_back(make_pair(num1, make_pair(num2, rest_of_line)));
    }
    file1.close();
}

void extract_changes(char* file){
    ifstream file1(file);
    string char1, char2, char3;
    while(file1 >> char1 >> char2 >> char3){
        if(char1 == "EOF" || char2 == "EOF" || char3 == "EOF"){
            break;
        }
        changes.push_back(make_pair(stoi(char1), make_pair(stoi(char2), stoi(char3))));
    }
    file1.close();
}

void add_hops_message(const char* filename, int src, int dest, vector<int> hops, string message){
    ofstream outfile(filename, std::ios_base::app); // Open file in append mode

    outfile << "from " << src << " to " << dest << " cost " << dist_vector[src][dest].cost << " hops ";
    for(int i = 0; i < hops.size(); i++){
        outfile << hops[i] << " ";
    }
    outfile << "message" << message << std::endl;

    outfile.close();
}

int main(int argc, char *argv[]){
    if(argc != 4){
        cout << "Usage: " << argv[0] << "./dvr <topologyFile> <messageFile> <changesFile>" << endl;
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