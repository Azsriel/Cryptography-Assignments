#include <iostream>
#include <vector>
#include <algorithm>

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

struct pair {
    int cost;
    int node;
    int next;
};

void LinkState(int root, Node current, std::vector<std::vector<int>> costMatrix) {
    std::cout << "Link State Routing for Router " << root << "\n"; 
    std::vector<pair> tentativeList;
    std::vector<int> permanent;
    tentativeList.push_back({0, root, root});
    
    while (tentativeList.size() != 0) {
        // Find smallest
        pair smallest = {infinity, -1, -1};
        int index = -1;
        for (int p = 0; p<tentativeList.size(); ++p) {
            pair i = tentativeList[p];
            if (i.cost < smallest.cost) {
                smallest = i;
                index = p;
            }
        }

        // Add to permanent List;
        permanent.push_back(smallest.node);
        current.cost[smallest.node] = smallest.cost;
        current.nextHop[smallest.node] = smallest.next;
        tentativeList.erase(tentativeList.begin() + index);
        std::cout << "Added " << smallest.node << " to Permanent List\n";

        // Add unprocessed
        for (int i = 1; i<=n; ++i) {
            if (costMatrix[smallest.node][i] != infinity && std::count(permanent.begin(), permanent.end(), i) == 0) {
                bool present = false;
                for (pair j: tentativeList) {
                    if (j.node == i) {
                        present = true;
                        if (j.cost > (smallest.cost + costMatrix[smallest.node][i])) {
                            j.cost = smallest.cost + costMatrix[smallest.node][i];
                            j.next = smallest.node;
                        }
                    }
                }
                if (!present) {
                    tentativeList.push_back({smallest.cost + costMatrix[smallest.node][i], i, smallest.node});
                }
            }
        }
    }

    // Print routing table;
    std::cout << std::endl;
    std::cout << "Routing table for router " << root << "\n";
    std::cout << "Node\tCost\tNext Hop\n";
    for (int i = 1; i<=n; ++i) {
        std::cout << i << "\t" << current.cost[i] << "\t" << current.nextHop[i] << "\n";
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

    for(int i = 1; i<=n; ++i) {
        LinkState(i, routers[i], costMatrix);
    }

    return 0;
}