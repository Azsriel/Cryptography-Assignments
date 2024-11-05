#include <iostream>
#include <vector>

#define infinity 9999
#define n 8

class Node {
public:
    std::vector<int> nextHop;
    std::vector<int> cost;

    Node() {
        nextHop.resize(n+1, -1);
        cost.resize(n+1, infinity);
    }
};

void distanceVector(std::vector<std::vector<int>> costMatrix, std::vector<Node> nodes) {
    bool flag = true;
    while (flag) {
        flag = false;
        for (int i = 1; i<=n; ++i) {
            for (int j = 1; j<=n; ++j) {
                for (int k = 1; k<=n; ++k) {
                    if (nodes[i].cost[j] > costMatrix[i][k] + nodes[k].cost[j]) {
                        nodes[i].cost[j] = nodes[i].cost[k] + nodes[k].cost[j];
                        std::cout << "Path of " << i << " to " << j << " updated to " << nodes[i].cost[j] << " using next hop " << k << "\n";
                        nodes[i].nextHop[j] = k;
                        flag = true;
                    }
                }
            } 
        }
    }

    // Print routing tables
    std::cout << std::endl;
    for (int i = 1; i<=n; ++i) {
        std::cout << "Routing table for router " << i << "\n";
        std::cout << "Node\tCost\tNext Hop\n";
        for (int j = 1; j<=n; ++j) {
            std::cout << j << "\t" << nodes[i].cost[j] << "\t" << nodes[i].nextHop[j] << "\n";
        }
    }
    


}

int main() {
    std::vector<Node> routers(n+1);
    for (int i = 1; i<=n; ++i) {
        routers[i].cost[i] = 0;
        routers[i].nextHop[i] = i;
    }

    // Input: row 0 and column 0 are padding so that we can start indexing with 1 
    std::vector<std::vector<int>> costMatrix = 
    {
        {infinity, infinity, infinity, infinity, infinity, infinity, infinity, infinity, infinity},
        {infinity, 0, 2, 5, 1, infinity, infinity, infinity, infinity},
        {infinity, 2, 0, infinity, 3, 4, infinity, infinity, infinity},
        {infinity, 5, infinity, 0, infinity, infinity, 2, 3, infinity},
        {infinity, 1, 3, infinity, 0, 1, 2, infinity, infinity},       
        {infinity, infinity, 4, infinity, 1, 0, infinity, infinity, 3},
        {infinity, infinity, infinity, 2, 2, infinity, 0, 1, infinity},
        {infinity, infinity, infinity, 3, infinity, infinity, 1, 0, 2},
        {infinity, infinity, infinity, infinity, infinity, 3, infinity, 2, 0}
    };

    for (int i = 1; i<=n; ++i) {
        for (int j = 1; j<=n; ++j) {
            if (costMatrix[i][j] != infinity) {
                routers[i].cost[j] = costMatrix[i][j];
                routers[i].nextHop[j] = j;
            }
        }
    }

    distanceVector(costMatrix, routers);

    return 0;
}