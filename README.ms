# Routing Protocols Implementation

This project implements various routing protocols in C++, demonstrating the application of different algorithms used in network routing mechanisms. Adhering strictly to the guidelines provided for the assignment, the project is meticulously tested across various topologies, ensuring the correct implementation of tie-breaking conditions and the dynamic nature of networks through the addition and deletion of nodes.

## Description

The Routing Protocols project aims to provide a comprehensive, hands-on experience with the core algorithms that drive routing in networks. It includes detailed implementations of widely-used protocols:

- **Distance Vector Protocol**: Utilizing the Bellman-Ford algorithm, this protocol calculates the best path to each node in the network.
- **Link State Protocol**: Based on Dijkstra's algorithm, it constructs a map of the network and computes the shortest path to each node.


## Testing

The project includes extensive testing, simulating various network topologies to validate the correctness of the routing algorithms. Specific scenarios ensure that tie-breaking conditions are met when multiple paths of identical cost are available.

Modifying topologies by adding or deleting nodes can be done to observe the dynamic adaptation of the routing tables, providing insights into the convergence properties and resilience of the implemented protocols.
