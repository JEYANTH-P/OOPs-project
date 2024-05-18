#include <bits/stdc++.h>
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

    Camp() : number(0), name(""), address(""), contact("") {} // default constructor

    Camp(int number, const string& name, const string& address, const string& contact)
        : number(number), name(name), address(address), contact(contact) {}
};

class Graph {
public:
    map<int, Camp> camps;
    vector<Edge> edges;

    void addCamp(int number, const string& name, const string& address, const string& contact) {
        Camp camp(number, name, address, contact);
        camps[number] = camp;
    }

    void addEdge(int src, int dest, int weight = INT32_MAX, bool updateOnly = false) {
        if (camps.find(src) != camps.end() && camps.find(dest) != camps.end()) {
            for (auto& edge : edges) {
                if (edge.src == src && edge.dest == dest) {
                    edge.weight = weight;
                    return;
                }
            }
            if (!updateOnly) {
                Edge newEdge1 = {src, dest, weight};
                Edge newEdge2 = {dest, src, weight};
                edges.push_back(newEdge1);
                edges.push_back(newEdge2);
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
};

int main() {
    Graph graph;
    int role;
    do{
    
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
                            int srcCampNumber, destCampNumber;
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
                cout<<"=====LOGGED IN=====\n";
                int choice;
                do {
                    cout << "Menu:\n";
                    cout << "1) View nearby camps\n";
                    cout << "2) Add cost between camps\n";
                    cout << "3) Exit\n";
                    cout << "Enter your choice: ";
                    cin >> choice;

                    switch (choice) {
                        case 1: {
                            set<int> nearbyCamps = graph.getNearbyCamps(campNumber);
                            cout << "Nearby camps:\n";
                            for (int nearbyCampNumber : nearbyCamps) {
                                Camp& nearbyCamp = graph.camps[nearbyCampNumber];
                                cout<<"\n==============================\n";
                                cout << "Camp number: " << nearbyCamp.number << "\n";
                                cout << "Camp name: " << nearbyCamp.name << "\n";
                                cout << "Camp address: " << nearbyCamp.address << "\n";
                                cout << "Camp contact: " << nearbyCamp.contact << "\n";
                                cout<<"\n==============================\n";
                            }
                            break;
                        }
                        case 2: {
                            // Camp Head adds cost between camps
                            int srcCampNumber, destCampNumber, cost;
                            cout << "Enter the source camp number: ";
                            cin >> srcCampNumber;
                            cout << "Enter the destination camp number: ";
                            cin >> destCampNumber;
                            cout << "Enter the cost: ";
                            cin >> cost;

                            try {
                                graph.addEdge(srcCampNumber, destCampNumber, cost, true);
                                cout << "Cost added successfully between camp " << srcCampNumber << " and camp " << destCampNumber << "\n";
                            } catch (const std::invalid_argument& e) {
                                cout << e.what() << '\n';
                            }
                            break;
                        }
                        case 3:
                            cout << "Exiting Camp Head...\n";
                            break;
                        default:
                            cout << "Invalid choice!\n";
                            break;
                    }
                } while (choice != 3);
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
    }while(role != 4);

    return 0;
}

