#include "../include/node.hpp"

void distance_vector(){
    for(int count = 1; count < adj_list.size(); count++){
        vector<pair<int,int>> temp = adj_list[count];
        for(int dst = 1; dst < adj_list.size(); dst++){
            if(count == dst){
                Node vec;
                vec.destination = dst;
                vec.nextHop = dst;
                vec.cost = 0;
                if(dist_vector[count].size() < dst + 1 ){
                    dist_vector[count].push_back(vec);
                }
            }
            else{
                Node vec;
                for(int i = 1; i < temp.size(); i++){
                    vec.destination = dst;
                    vec.cost = INT_MAX;
                    pair<int,int> neighbour = temp[i];
                    int distance = 0;
                    if(dist_vector[neighbour.first].size() - 1 >= dst && dist_vector[neighbour.first][dst].cost != INT_MAX){
                        distance = neighbour.second + dist_vector[neighbour.first][dst].cost;
                        if(distance < vec.cost){
                            vec.cost = distance;
                            vec.nextHop = neighbour.first;
                        }
                    }else if(neighbour.first == dst){
                        if(vec.cost > neighbour.second){
                            vec.cost = neighbour.second;
                            vec.nextHop = neighbour.first;
                        }
                    }
                    if(dist_vector[count].size() < dst + 1){
                        dist_vector[count].push_back(vec);
                    }
                    else if(dist_vector[count][dst].cost > vec.cost){
                        dist_vector[count][dst] = vec;
                    }
                }
            }
        }
    }
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

    for(int count = 0; count < vertices; count++) distance_vector();
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
        
        for(int count = 0; count < vertices; count++) distance_vector();
        print_distance_vector(op);
        
        for(int count = 0; count < msg.size() ; count++){
           // cout<<"here"<<endl;
            vector<int> hops = tot_hops(msg[count].first, msg[count].second.first);  
            add_hops_message(op, msg[count].first, msg[count].second.first, hops,msg[count].second.second);
        }
    }

    return 0;
}