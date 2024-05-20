#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

struct Edge {
    int src, dest, weight;
};

class Camp {
public:
    int number;
    string name;
    string address;
    string contact;
    vector<pair<string, int>> resources;
    int needValue;

    Camp() : number(0), name(""), address(""), contact("") {} // default constructor

    Camp(int number, const string& name, const string& address, const string& contact)
        : number(number), name(name), address(address), contact(contact) {}
};

class Node {
public:
    int id;
    int dist;
    bool visited;
    vector<pair<int, int>> adj; // Adjacent nodes and their distances

    Node(int id) : id(id), dist(numeric_limits<int>::max()), visited(false) {}
};

class Graph {
public:
    map<int, Camp> camps;
    vector<Edge> edges;
    map<int, vector<pair<int, int>>> adjList;

    void addCamp(int number, const string& name, const string& address, const string& contact) {
        Camp camp(number, name, address, contact);
        camps[number] = camp;
    }

    void addEdge(int src, int dest, int weight = INT32_MAX, bool updateOnly = false) {
        if (camps.find(src) != camps.end() && camps.find(dest) != camps.end()) {
            for (auto& edge : edges) {
                if (edge.src == src && edge.dest == dest) {
                    edge.weight = weight;
                    adjList[src].push_back({dest, weight});
                    adjList[dest].push_back({src, weight});
                    return;
                }
            }
            if (!updateOnly) {
                Edge newEdge1 = {src, dest, weight};
                Edge newEdge2 = {dest, src, weight};
                edges.push_back(newEdge1);
                edges.push_back(newEdge2);
                adjList[src].push_back({dest, weight});
                adjList[dest].push_back({src, weight});
            } else {
                throw std::invalid_argument("Edge does not exist!");
            }
        } else {
            throw std::invalid_argument("One or both camp numbers are invalid!");
        }
    }

    set<int> getNearbyCamps(int campNumber) {
        set<int> nearbyCamps;
        for (auto& edge : edges) {
            if (edge.src == campNumber) {
                nearbyCamps.insert(edge.dest);
            } else if (edge.dest == campNumber) {
                nearbyCamps.insert(edge.src);
            }
        }
        return nearbyCamps;
    }

    int find(map<int, int>& parent, int i) {
        if (parent.at(i) == i) {
            return i;
        }
        return find(parent, parent.at(i));
    }

    void unionSets(map<int, int>& parent, int x, int y) {
        int xroot = find(parent, x);
        int yroot = find(parent, y);
        parent[xroot] = yroot;
    }

    vector<Edge> kruskalMST() {
        sort(edges.begin(), edges.end(), [](Edge a, Edge b) {
            return a.weight < b.weight;
        });

        map<int, int> parent;
        for (auto& camp : camps) {
            parent[camp.first] = camp.first;
        }

        vector<Edge> mst;
        for (Edge& edge : edges) {
            int x = find(parent, edge.src);
            int y = find(parent, edge.dest);

            if (x != y) {
                mst.push_back(edge);
                unionSets(parent, x, y);
            }
        }

        return mst;
    }

    vector<int> dijkstra(int start) {
        map<int, int> distances;
        for (const auto& camp : camps) {
            distances[camp.first] = numeric_limits<int>::max();
        }
        distances[start] = 0;

        using pii = pair<int, int>;
        priority_queue<pii, vector<pii>, greater<pii>> pq;
        pq.push({0, start});

        while (!pq.empty()) {
            int dist = pq.top().first;
            int node = pq.top().second;
            pq.pop();

            if (dist > distances[node]) continue;

            for (const auto& neighbor : adjList[node]) {
                int nextNode = neighbor.first;
                int weight = neighbor.second;
                if (distances[node] + weight < distances[nextNode]) {
                    distances[nextNode] = distances[node] + weight;
                    pq.push({distances[nextNode], nextNode});
                }
            }
        }

        vector<int> result;
        for (const auto& distance : distances) {
            result.push_back(distance.second);
        }

        return result;
    }
};

void addHardcodedNodesAndEdges(Graph& graph) {
    int userNode = 1001;
    int nodeA = 1002;
    int nodeB = 1003;
    int camp1 = 1;
    int camp2 = 2;

    graph.addCamp(userNode, "User Location", "Unknown", "Unknown");
    graph.addCamp(nodeA, "Node A", "Unknown", "Unknown");
    graph.addCamp(nodeB, "Node B", "Unknown", "Unknown");

    graph.addEdge(userNode, nodeA, 10);
    graph.addEdge(userNode, nodeB, 20);
    graph.addEdge(nodeA, camp1, 30);
    graph.addEdge(nodeB, camp2, 40);
}

int main() {
    Graph graph;
    int role;
    do {
        cout << "Select your role:\n";
        cout << "1. Admin\n";
        cout << "2. Camp Head\n";
        cout << "3. User\n";
        cout << "4. Exit\n";
        cin >> role;

        switch (role) {
            case 1: {
                // Admin login
                string password;
                cout << "Enter admin password: ";
                cin >> password;
                if (password == "0000") {
                    int choice;
                    do {
                        cout << "Menu:\n";
                        cout << "1) Add a camp\n";
                        cout << "2) View\n";
                        cout << "3) Add edges\n";
                        cout << "4) Exit\n";
                        cout << "Enter your choice: ";
                        cin >> choice;

                        switch (choice) {
                            case 1: {
                                int number;
                                string name;
                                string address;
                                string contact;

                                cout << "Enter camp number: ";
                                cin >> number;

                                cout << "Enter camp name: ";
                                cin >> name;

                                cout << "Enter camp address: ";
                                cin >> address;

                                cout << "Enter camp contact: ";
                                cin >> contact;

                                graph.addCamp(number, name, address, contact);
                                cout << "Camp added!\n";
                                break;
                            }
                            case 2: {
                                vector<Edge> mst = graph.kruskalMST();
                                cout << "Minimum spanning tree:\n";
                                for (Edge& edge : mst) {
                                    cout << "Camp " << edge.src << " - Camp " << edge.dest << " Cost: " << edge.weight << "\n";
                                }
                                break;
                            }
                            case 3: {
                                // Admin adds edges between camps
                                int srcCampNumber, destCampNumber, weight;
                                cout << "Enter the source camp number: ";
                                cin >> srcCampNumber;
                                cout << "Enter the destination camp number: ";
                                cin >> destCampNumber;
                                

                                try {
                                    graph.addEdge(srcCampNumber, destCampNumber);
                                    cout << "Edge added successfully between camp " << srcCampNumber << " and camp " << destCampNumber << "\n";
                                } catch (const std::invalid_argument& e) {
                                    cout << e.what() << '\n';
                                }
                                break;
                            }
                            case 4:
                                cout << "Exiting Admin...\n";
                                break;
                            default:
                                cout << "Invalid choice!\n";
                                break;
                        }
                    } while (choice != 4);
                } else {
                    cout << "Invalid password!\n";
                }
                break;
            }
            case 2: {
                // Camp Head login
                int campNumber;
                cout << "Enter your camp number: ";
                cin >> campNumber;
                if (graph.camps.find(campNumber) != graph.camps.end()) {
                    cout << "=====LOGGED IN=====\n";
                    int choice;
                    int needValue = 0;
                    do {
                        cout << "Menu:\n";
                        cout << "1) View nearby camps\n";
                        cout << "2) Add cost between camps\n";
                        cout << "3) Enter your needs \n";
                        cout << "4) Exit\n";
                        cout << "Enter your choice: ";
                        cin >> choice;

                        switch (choice) {
                            case 1: {
                                set<int> nearbyCamps = graph.getNearbyCamps(campNumber);
                                cout << "Nearby camps:\n";
                                for (int nearbyCampNumber : nearbyCamps) {
                                    Camp& nearbyCamp = graph.camps[nearbyCampNumber];
                                    cout << "\n==============================\n";
                                    cout << "Camp number: " << nearbyCamp.number << "\n";
                                    cout << "Camp name: " << nearbyCamp.name << "\n";
                                    cout << "Camp address: " << nearbyCamp.address << "\n";
                                    cout << "Camp contact: " << nearbyCamp.contact << "\n";
                                    cout << "\n==============================\n";
                                }
                                break;
                            }
                            case 2: {
                                // Camp Head adds cost between camps
                                int srcCampNumber, destCampNumber, distance, waterLevel, needValue;
                                cout << "Enter the source camp number: ";
                                cin >> srcCampNumber;
                                cout << "Enter the destination camp number: ";
                                cin >> destCampNumber;
                                cout << "Enter the inbetween distance: ";
                                cin >> distance;
                                cout << "Enter the inbetween water level: ";
                                cin >> waterLevel;

                                try {
                                    int needValueFactor = 1000;
                                    int waterLevelFactor = 100;
                                    int distanceFactor = 10;

                                     // Get the need values of the source and destination camps
                                    int srcNeedValue = graph.camps[srcCampNumber].needValue;
                                    int destNeedValue = graph.camps[destCampNumber].needValue;

                                    // Calculate the average need value
                                    int averageNeedValue = (srcNeedValue + destNeedValue) / 2.0;

                                    int cost = (INT32_MAX/2) - (needValue * averageNeedValue) + (waterLevel * waterLevelFactor) + (distance * distanceFactor);
                                    graph.addEdge(srcCampNumber, destCampNumber, cost, true);
                                    cout << "Cost added successfully between camp " << srcCampNumber << " and camp " << destCampNumber << "\n";
                                } catch (const std::invalid_argument& e) {
                                    cout << e.what() << '\n';
                                }
                                break;
                            }
                            case 3: {
                                cout << "Enter the need value: ";
                                cin >> needValue;
                                graph.camps[campNumber].needValue = needValue;
                                break;
                            }
                            case 4:
                                cout << "Exiting Camp Head...\n";
                                break;
                            default:
                                cout << "Invalid choice!\n";
                                break;
                        }
                    } while (choice != 4);
                } else {
                    cout << "Invalid camp number!\n";
                }
                break;
            }
            case 3: {
                // User login
                string password;
                cout << "Enter user password: ";
                cin >> password;
                if (password == "user_password") {
                    // User menu
                    addHardcodedNodesAndEdges(graph);
                    int userNode = 1001;
                    vector<int> distances = graph.dijkstra(userNode);
                    int minDistance = numeric_limits<int>::max();
                    int nearestCamp = -1;

                    for (const auto& camp : graph.camps) {
                        if (camp.first != userNode && distances[camp.first] < minDistance) {
                            minDistance = distances[camp.first];
                            nearestCamp = camp.first;
                        }
                    }

                    if (nearestCamp != -1) {
                        Camp& nearest = graph.camps[nearestCamp];
                        cout << "The nearest camp is:\n";
                        cout << "Camp number: " << nearest.number << "\n";
                        cout << "Camp name: " << nearest.name << "\n";
                        cout << "Camp address: " << nearest.address << "\n";
                        cout << "Camp contact: " << nearest.contact << "\n";
                    } else {
                        cout << "No nearby camps found.\n";
                    }
                } else {
                    cout << "Invalid password!\n";
                }
                break;
            }
            case 4:
                cout << "Exiting ...\n";
                break;
            default: {
                cout << "Invalid role!\n";
                break;
            }
        }
    } while (role != 4);

    return 0;
}