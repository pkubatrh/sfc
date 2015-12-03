#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>

int vector_num = 7;
int vector_len = 2;
int rmax = 3;


struct hidden_neuron {
    std::string name;
    float r;
    int cls;
    int *pv;
    int out_neuron;
};

// Computes n-dimensional distance between 2 vectors
// sqrt(sum((v1n - v2n)^2))
float dist(int *v1, int *v2, int size) {
    float result = 0;
    for (int i = 0; i < size; i++) {
        result += pow((v1[i] - v2[i]), 2);
    }
    return sqrt(result);
}

int main()
{
    bool modif = false;
    std::vector<hidden_neuron> net;
    std::vector<int> out_neurons;
    int v[vector_num][vector_len+1] = {
        {0, 1, 1},
        {1, 0, 1},
        {2, 0, 3},
        {0, 0, 2},
        {1, 1, 2},
        {2, 1, 2},
        {1, 2, 2}
    };
    for (int i=0; i<vector_num; i++) {
        bool hit = false;
        std::cout << "Comparing existing neurons to new vector" << std::endl;
        for (int j=0; j<net.size(); j++) {
            float distance = dist(v[i], net[j].pv, vector_len);
            std::cout << "Distance: " << distance << std::endl;
            if (distance <= net[j].r) {
                std::cout << "Class1: " << v[i][2] << std::endl;
                std::cout << "Class2: " << out_neurons[net[j].out_neuron] << std::endl;
                if (v[i][2] == out_neurons[net[j].out_neuron]) {
                    std::cout << "Hit - continue with next vector" << std::endl;
                    hit = true;
                } else {
                    std::cout << "Different classes - resize sphere" << std::endl;
                    std::cout << "Old r: " << net[j].r;
                    net[j].r = distance / 2;
                    std::cout << ", new r: " << net[j].r << std::endl;
                }
            }
        }
        /*std::cout << "Vector: " << i << std::endl;
        std::cout << "Inside: (" << v[i][0] <<", " << v[i][1] << ")" << std::endl;
        std::cout << "Class: " << v[i][2] << std::endl;
        std::cout << "Diff: " << difference(v[0], v[i], vector_len) << std::endl;
        */
        if (!hit) {
            std::cout << "Creating new hidden neuron" << std::endl;
            hidden_neuron new_neuron;
            new_neuron.name = "Neuron_" + std::to_string(i+1);
            new_neuron.pv = v[i];
            new_neuron.cls = v[i][2];
            new_neuron.r = rmax;
            modif = true;
            int pos = std::find(out_neurons.begin(), out_neurons.end(), new_neuron.cls) - out_neurons.begin();
            if (pos >= out_neurons.size() ) {
                // Not found, create new out neuron
                std::cout << "Not found - new out neuron" << std::endl;
                out_neurons.push_back(new_neuron.cls);
                new_neuron.out_neuron = out_neurons.size()-1;
            } else {
                // Connect new neuron to existing out neuron
                std::cout << "Neuron found at pos: " << pos << std::endl;
                new_neuron.out_neuron = pos;
            }
            net.push_back(new_neuron);
            std::cout << "Out neuron: " << new_neuron.out_neuron << std::endl;
        }
    }
    std::cout << "Net length: " << net.size() << std::endl;
}
