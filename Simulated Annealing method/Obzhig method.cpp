#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <cmath>
#include <limits>
#include <algorithm> 
#include <random>
#include <time.h> 
#include <cstdlib>

using namespace std;

vector<int> initial_tour;
std::mt19937 rng(std::random_device{}());

double getRandomDouble(double min, double max) {
    std::uniform_real_distribution<double> dist(min, max);
    return dist(rng);
}
int getRandomInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}


double distance(double x1, double y1, double x2, double y2) {
    return sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
}

double calculateTotalCost(vector<tuple<int, double, double>>& data, vector<int>& tour) {
    double totalCost = 0.0;

    for (int i = 0; i < tour.size() - 1; ++i) {
        int currentIndex = tour[i] - 1;
        int nextIndex = tour[i + 1] - 1;
        totalCost += distance(get<1>(data[currentIndex]), get<2>(data[currentIndex]),
                              get<1>(data[nextIndex]), get<2>(data[nextIndex]));
    }

    int lastIndex = tour.back() - 1;
    int firstIndex = tour.front() - 1;
    totalCost += distance(get<1>(data[lastIndex]), get<2>(data[lastIndex]),
                          get<1>(data[firstIndex]), get<2>(data[firstIndex]));

    return totalCost;
}


vector<int> inverse(vector<int>& state) {
    int node_one_index = getRandomInt(0, state.size() - 1);
    int node_one = state[node_one_index];

    vector<int> reduced_state = state;
    reduced_state.erase(reduced_state.begin() + node_one_index);

    int node_two = reduced_state[getRandomInt(0, reduced_state.size() - 1)];

    int index_one = node_one_index;
    int index_two = find(state.begin(), state.end(), node_two) - state.begin();

    int start = min(index_one, index_two);
    int end = max(index_one, index_two);

    reverse(state.begin() + start, state.begin() + end + 1);
    return state;
}




vector<int> insert(vector<int>& state) {
    int node_j_index = getRandomInt(0, state.size() - 1);
    int node_j = state[node_j_index];

    state.erase(state.begin() + node_j_index);

    int node_i_index = getRandomInt(0, state.size());
    state.insert(state.begin() + node_i_index, node_j);

    return state;
}


vector<int> swap(vector<int>& state) {
    int pos_one = getRandomInt(0, state.size() - 1);
    int pos_two = getRandomInt(0, state.size() - 1);

    std::swap(state[pos_one], state[pos_two]);
    return state;
}


vector<int> swap_routes(vector<int>& state) {
    int subroute_a = getRandomInt(0, state.size() - 1);
    int subroute_b = getRandomInt(0, state.size() - 1);

    int start = min(subroute_a, subroute_b);
    int end = max(subroute_a, subroute_b);

    vector<int> subroute(state.begin() + start, state.begin() + end);

    state.erase(state.begin() + start, state.begin() + end);

    int insert_pos = getRandomInt(0, state.size());

    state.insert(state.begin() + insert_pos, subroute.begin(), subroute.end());

    return state;
}


vector<int> get_neighbor(const vector<int>& tour) {
    vector<int> modified_tour = tour;
    int mutation_type = getRandomInt(0, 3);

    switch (mutation_type) {
        case 0:
            return insert(modified_tour);
        case 1:
            return inverse(modified_tour);
        case 2:
            return swap_routes(modified_tour);
        case 3:
            return swap(modified_tour);
        default:
            return modified_tour;
    }
}




void obzhig(vector<tuple<int, double, double>>& data) {
    double temperature = 10;  
    double alpha = 0.999999;   
    int max_iterations = 10000000;

    vector<int> cur_tour = initial_tour;
    double cur_cost = calculateTotalCost(data, cur_tour);
    
    vector<int> best_tour = cur_tour;
    double best_cost = cur_cost;

    for (int i = 0; i < max_iterations; i++) {
        if (i % 2000000 == 0) {
            cout << "Iteration " << i << ", Current Cost: " << cur_cost << ", Best Cost: " << best_cost << "\n";
            cout << "Temperature - " << temperature << "\n";
        }

        vector<int> neighbor = get_neighbor(cur_tour);
        double neighbor_cost = calculateTotalCost(data, neighbor);
        double delta = neighbor_cost - cur_cost;

        if (delta < 0 || exp(-delta / temperature) > getRandomDouble(0.0, 1.0)) {
            cur_tour = neighbor;
            cur_cost = neighbor_cost;
        }

        if (cur_cost < best_cost) {
            best_cost = cur_cost;
            best_tour = cur_tour;
        }

        temperature *= alpha;
        
    }
    cout << "Best tour cost: " << best_cost << "\n";
    cout << "Best tour path: ";
    for (int city : best_tour) {
        cout << city << " ";
    }
    // cout << best_tour[0];
    cout << "\n";
}


vector<int> generateRandomTour(int numCities) {
    vector<int> tour(numCities);
    
    for (int i = 0; i < numCities; ++i) {
        tour[i] = i + 1; 
    }

    shuffle(tour.begin(), tour.end(), rng);

    tour.push_back(tour[0]);

    return tour;
}



int main() {
    ifstream inputFile("qa194.txt");
    if (!inputFile) {
        cerr << "Error: Unable to open the file.\n";
        return 1;
    }

    vector<tuple<int, double, double>> data;
    int id;
    double x, y;

    while (inputFile >> id >> x >> y) {
        data.emplace_back(id, x, y);
    }
    inputFile.close();

    // initial_tour = generateRandomTour(data.size());
    // initial_tour = {137, 134, 132, 126, 125, 127, 130, 111, 104, 101, 99, 94, 89, 90, 98, 86, 85, 65, 20, 63, 36, 59, 62, 82, 109, 113, 114, 119, 122, 118, 106, 105, 107, 108, 97, 92, 95, 96, 93, 88, 91, 103, 102, 87, 80, 71, 76, 75, 78, 72, 74, 69, 60, 33, 18, 21, 24, 26, 17, 7, 11, 14, 25, 23, 13, 16, 8, 6, 1, 4, 2, 3, 5, 9, 10, 12, 15, 19, 30, 32, 35, 31, 27, 22, 28, 29, 37, 51, 47, 39, 34, 40, 43, 38, 41, 48, 46, 44, 42, 50, 49, 55, 54, 52, 53, 56, 58, 61, 67, 73, 66, 68, 45, 57, 64, 70, 77, 79, 81, 83, 84, 100, 110, 112, 115, 116, 117, 121, 120, 123, 124, 128, 133, 135, 129, 131, 136, 143, 148, 160, 166, 171, 185, 180, 178, 170, 167, 168, 165, 159, 158, 162, 155, 151, 147, 141, 152, 153, 157, 154, 150, 144, 139, 138, 142, 146, 149, 164, 172, 179, 174, 173, 175, 177, 181, 193, 188, 191, 192, 189, 184, 183, 183, 186, 187, 190, 194, 182, 176, 169, 163, 161, 156, 145, 140, 137};
    // initial_tour = {174, 173, 175, 184, 177, 181, 178, 180, 170, 167, 168, 165, 159, 158, 162, 155, 151, 147, 141, 152, 153, 157, 154, 150, 144, 139, 138, 142, 146, 149, 156, 145, 140, 137, 134, 132, 126, 125, 127, 130, 111, 104, 101, 99, 94, 89, 90, 98, 86, 85, 65, 20, 63, 36, 59, 62, 82, 109, 113, 114, 119, 122, 118, 103, 102, 87, 80, 71, 76, 75, 78, 72, 74, 69, 60, 57, 45, 37, 39, 51, 47, 43, 48, 46, 41, 38, 40, 34, 27, 22, 29, 28, 33, 18, 21, 24, 26, 17, 7, 11, 14, 25, 23, 13, 16, 8, 6, 1, 4, 2, 3, 5, 9, 10, 12, 15, 19, 30, 32, 31, 35, 44, 42, 50, 49, 55, 54, 52, 53, 56, 58, 61, 67, 73, 66, 68, 64, 70, 77, 84, 81, 79, 83, 88, 91, 93, 96, 95, 92, 97, 106, 105, 107, 108, 100, 110, 112, 115, 116, 117, 121, 120, 123, 124, 128, 133, 135, 129, 131, 136, 143, 148, 160, 166, 171, 185, 193, 188, 189, 191, 192, 190, 194, 182, 176, 169, 161, 163, 164, 172, 179, 186, 187, 183, 174};
    // initial_tour = {32, 30, 19, 15, 12, 10, 9, 5, 3, 2, 4, 1, 6, 8, 16, 13, 23, 25, 14, 11, 7, 17, 26, 24, 21, 18, 33, 28, 29, 22, 27, 34, 40, 43, 47, 51, 39, 37, 45, 57, 60, 69, 74, 72, 78, 75, 76, 71, 80, 87, 102, 103, 118, 122, 119, 114, 113, 109, 82, 62, 59, 36, 63, 20, 65, 85, 86, 98, 90, 89, 94, 99, 101, 104, 111, 130, 127, 125, 126, 132, 134, 137, 140, 145, 156, 149, 146, 142, 138, 139, 144, 150, 154, 157, 153, 152, 141, 147, 151, 155, 162, 158, 159, 165, 168, 167, 170, 180, 178, 181, 177, 184, 175, 173, 174, 174, 183, 187, 186, 179, 172, 164, 163, 161, 169, 176, 182, 194, 190, 192, 191, 189, 188, 193, 185, 171, 166, 160, 148, 143, 136, 131, 129, 135, 133, 128, 124, 123, 120, 121, 117, 116, 115, 112, 110, 100, 108, 107, 105, 106, 97, 92, 95, 96, 93, 91, 88, 83, 79, 81, 84, 77, 70, 64, 68, 66, 73, 67, 61, 58, 56, 53, 52, 48, 46, 54, 55, 49, 50, 42, 44, 41, 38, 35, 31, 32};
    initial_tour = {161, 163, 164, 172, 179, 174, 173, 175, 184, 177, 181, 178, 180, 170, 167, 168, 165, 159, 158, 162, 155, 151, 147, 152, 141, 144, 150, 153, 157, 154, 139, 138, 142, 146, 149, 156, 145, 140, 137, 134, 132, 126, 125, 127, 130, 111, 104, 101, 99, 94, 89, 90, 98, 86, 85, 65, 20, 63, 36, 59, 62, 82, 71, 80, 87, 76, 75, 78, 72, 74, 69, 60, 57, 45, 37, 27, 22, 29, 28, 33, 18, 21, 24, 26, 17, 7, 11, 14, 25, 23, 13, 16, 8, 6, 1, 4, 2, 3, 5, 9, 10, 12, 15, 19, 30, 32, 31, 35, 38, 41, 44, 42, 50, 49, 55, 54, 46, 48, 52, 53, 56, 58, 43, 40, 34, 39, 47, 51, 61, 67, 73, 66, 68, 64, 70, 77, 84, 81, 79, 83, 88, 92, 97, 95, 96, 93, 91, 103, 102, 109, 113, 114, 119, 122, 118, 106, 105, 107, 108, 100, 110, 112, 115, 116, 117, 121, 120, 123, 124, 128, 133, 135, 129, 131, 136, 143, 148, 160, 166, 171, 185, 193, 188, 189, 191, 192, 190, 187, 183, 186, 194, 182, 176, 169, 161};
    // sort(initial_tour.begin(), initial_tour.end());
    // for (int i = 0; i < initial_tour.size(); i++){
    //     if (initial_tour[i] == initial_tour[i + 1]) cout << initial_tour[i] << " ";
    // }
    // cout << initial_tour.size()<<"\n";
    cout << "Random Initial Tour:\n";
    for (int city : initial_tour) {
        cout << city << " ";
    }
    cout << "\n";

    obzhig(data);

    return 0;
}


