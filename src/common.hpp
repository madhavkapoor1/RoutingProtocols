#ifndef COMMON_HPP
#define COMMON_HPP

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
extern vector<vector<pair<int,int>>> adj_list; // pair of node and distance
extern vector<vector<Node>> dist_vector; // vector of vector of nodes
extern vector<pair<int,pair<int,string>>> msg;
extern vector<pair<int,pair<int,int>>> changes;

/**
 * @brief Initializes the graph with the given number of nodes.
 *
 * This function initializes the adjacency list of the graph with the given number of nodes.
 * The adjacency list is represented as a vector of vectors of pairs, where each pair represents
 * an edge between two nodes and its weight. Initially, the adjacency list is empty.
 *
 * @param n The number of nodes in the graph.
 */
void graph_init(int n);

/**
 * Initializes the distance vector table with the given number of nodes.
 * 
 * @param n The number of nodes in the network.
 */
void dis_vec_init(int n);

/**
 * Adds an edge between two nodes in the graph.
 * 
 * @param n The index of the first node.
 * @param m The index of the second node.
 * @param weight The weight of the edge.
 */
void graph_edge(int n, int m, int weight);

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
void update_weight(int n, int m, int weight);

/**
 * @brief Deletes an edge between two nodes in the adjacency list.
 *
 * This function removes the edge between nodes `n` and `m` in the adjacency list.
 * It searches for the edge in both `adj_list[n]` and `adj_list[m]` and removes it if found.
 *
 * @param n The first node of the edge.
 * @param m The second node of the edge.
 */
void delete_edge(int n, int m);

/**
 * @brief Prints the adjacency list representation of the graph.
 * 
 * This function iterates through the adjacency list and prints the connections of each node along with their weights.
 * 
 * @note This function does not print anything if the adjacency list is empty.
 * 
 * @see adj_list
 */
void print_graph();

/**
 * @brief Calculates the total hops between a source node and a destination node.
 *
 * This function takes in the source node and destination node as parameters and returns a vector of integers representing the sequence of nodes traversed from the source to the destination. The function uses the `dist_vector` data structure to determine the next hop for each node until the destination node is reached.
 *
 * @param src The source node.
 * @param dst The destination node.
 * @return A vector of integers representing the sequence of nodes traversed from the source to the destination.
 */
vector<int> tot_hops(int src, int dst);

/**
 * @brief Prints the distance vector to a file.
 *
 * This function prints the distance vector to the specified file. The distance vector contains information about the destination, next hop, and cost for each node in the network.
 *
 * @param file The name of the file to which the distance vector will be printed.
 */
void print_distance_vector(const char* file);

/**
 * @brief Extracts messages from a file and stores them in a data structure.
 * 
 * This function reads a file line by line and extracts messages in the format of two integers followed by the rest of the line.
 * The extracted messages are stored in a data structure for further processing.
 * 
 * @param file The path to the file to be read.
 */
void extract_message(char* file);

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
void extract_changes(char* file);

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
void add_hops_message(const char* filename, int src, int dest, vector<int> hops, string message);


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
        if(get<0>(a) == get<0>(b)){
            return get<1>(a) > get<1>(b);
        }
        return get<0>(a) > get<0>(b);
    }
};

#endif // NODE_HPP