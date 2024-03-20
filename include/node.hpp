#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <climits>  
#include <sstream>
#include <fstream>
#include <climits>
#include <limits>
#include <queue>
#include <utility>

/**
 * @brief Represents a node in a routing protocol.
 * 
 * This struct stores information about a node, including its destination,
 * next hop, and cost.
 */
struct Node{
    int destination; /**< The destination of the node. */
    int nextHop; /**< The next hop for reaching the destination. */
    int cost; /**< The cost associated with reaching the destination. */
};

using namespace std;
vector<vector<pair<int,int>>> adj_list; // pair of node and distance
vector<vector<Node>> dist_vector; // vector of vector of nodes
vector<pair<int,pair<int,string>>> msg;
vector<pair<int,pair<int,int>>> changes;

/**
 * @brief Initializes the graph with the given number of nodes.
 *
 * This function initializes the adjacency list of the graph with the given number of nodes.
 * The adjacency list is represented as a vector of vectors of pairs, where each pair represents
 * an edge between two nodes and its weight. Initially, the adjacency list is empty.
 *
 * @param n The number of nodes in the graph.
 */
void graph_init(int n){
    vector<vector<pair<int,int>>> temp(n+1, vector<pair<int,int>>(1,pair<int,int>(0,0)));
    adj_list = temp;
    return;
}

/**
 * Initializes the distance vector table with the given number of nodes.
 * 
 * @param n The number of nodes in the network.
 */
void dis_vec_init(int n){
    Node temp1;
    temp1.destination = 0;
    temp1.nextHop = 0;
    temp1.cost = 0;
    vector<vector<Node>> temp2(n+1, vector<Node>(1,temp1)); //will check later
    dist_vector = temp2;
    return;
}

/**
 * Adds an edge between two nodes in the graph.
 * 
 * @param n The index of the first node.
 * @param m The index of the second node.
 * @param weight The weight of the edge.
 */
void graph_edge(int n, int m, int weight){
    adj_list[n].push_back(pair<int,int>(m,weight));
    adj_list[m].push_back(pair<int,int>(n,weight));
}

/**
 * @brief Updates the weight of the edge between two nodes in the graph.
 *
 * This function updates the weight of the edge between node 'n' and node 'm' in the adjacency list.
 * If the edge does not exist, a new edge with the specified weight is added to the adjacency list.
 *
 * @param n The first node in the edge.
 * @param m The second node in the edge.
 * @param weight The new weight of the edge.
 */
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

/**
 * @brief Deletes an edge between two nodes in the adjacency list.
 *
 * This function removes the edge between nodes `n` and `m` in the adjacency list.
 * It searches for the edge in both `adj_list[n]` and `adj_list[m]` and removes it if found.
 *
 * @param n The first node of the edge.
 * @param m The second node of the edge.
 */
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

/**
 * @brief Prints the adjacency list representation of the graph.
 * 
 * This function iterates through the adjacency list and prints the connections of each node along with their weights.
 * 
 * @note This function does not print anything if the adjacency list is empty.
 * 
 * @see adj_list
 */
void print_graph(){
    for(int i = 1; i < adj_list.size(); i++){
        //cout << "Node " << i << " is connected to: ";
        for(int j = 0; j < adj_list[i].size(); j++){
            //cout << adj_list[i][j].first << " with weight " << adj_list[i][j].second << " ";
        }
        //cout << endl;
    }
}

/**
 * @brief Calculates the total hops between a source node and a destination node.
 *
 * This function takes in the source node and destination node as parameters and returns a vector of integers representing the sequence of nodes traversed from the source to the destination. The function uses the `dist_vector` data structure to determine the next hop for each node until the destination node is reached.
 *
 * @param src The source node.
 * @param dst The destination node.
 * @return A vector of integers representing the sequence of nodes traversed from the source to the destination.
 */
vector<int> tot_hops(int src, int dst){
    vector<int> hops;
    hops.push_back(src);
    while(dist_vector[src][dst].nextHop != dst){
        hops.push_back(dist_vector[src][dst].nextHop);
        src = dist_vector[src][dst].nextHop;
    }
    return hops;
}

/**
 * @brief Prints the distance vector to a file.
 *
 * This function prints the distance vector to the specified file. The distance vector contains information about the destination, next hop, and cost for each node in the network.
 *
 * @param file The name of the file to which the distance vector will be printed.
 */
void print_distance_vector(const char* file){
    ofstream outfile(file, std::ios_base::app);
    for(int i = 1; i < dist_vector.size(); i++){
        for(int j = 1; j < dist_vector[i].size(); j++){
            outfile << dist_vector[i][j].destination << " " << dist_vector[i][j].nextHop << " " << dist_vector[i][j].cost << endl;
        }
    }
    outfile.close();
}

/**
 * @brief Extracts messages from a file and stores them in a data structure.
 * 
 * This function reads a file line by line and extracts messages in the format of two integers followed by the rest of the line.
 * The extracted messages are stored in a data structure for further processing.
 * 
 * @param file The path to the file to be read.
 */
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

/**
 * @brief Extracts changes from a file and stores them in a vector.
 * 
 * This function reads the contents of the specified file and extracts the changes
 * represented by three consecutive strings (char1, char2, char3) in each line. 
 * The extracted changes are stored in a vector of pairs, where the first element 
 * of the pair is an integer (char1) and the second element is a pair of integers 
 * (char2, char3).
 * 
 * @param file The path to the file to be read.
 */
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

/**
 * @brief Appends a message to a file, indicating the source, destination, hops, and message content.
 * 
 * This function appends a message to the specified file, providing information about the source node,
 * destination node, hops taken, and the content of the message. The message is formatted as follows:
 * "from [source] to [destination] cost [cost] hops [hops] message [message]"
 * 
 * @param filename The name of the file to which the message will be appended.
 * @param src The source node ID.
 * @param dest The destination node ID.
 * @param hops A vector containing the IDs of the nodes traversed.
 * @param message The content of the message.
 */
void add_hops_message(const char* filename, int src, int dest, vector<int> hops, string message){
    ofstream outfile(filename, std::ios_base::app); // Open file in append mode

    outfile << "from " << src << " to " << dest << " cost " << dist_vector[src][dest].cost << " hops ";
    for(int i = 0; i < hops.size(); i++){
        outfile << hops[i] << " ";
    }
    outfile << "message" << message << std::endl;

    outfile.close();
}

/**
 * @file node.hpp
 * @brief Contains the definition of the compare struct used for comparing tuples.
 */

/**
 * @struct compare
 * @brief A struct used for comparing tuples based on the value of the first element.
 */
struct compare {
    /**
     * @brief Overloaded operator() to compare two tuples.
     * @param a The first tuple to compare.
     * @param b The second tuple to compare.
     * @return True if the first tuple has a greater value in the first element, false otherwise.
     */
    bool operator()(const tuple<int, int, int>& a, const tuple<int, int, int>& b) const {
        return get<0>(a) > get<0>(b);
    }
};


#endif // NODE_HPP