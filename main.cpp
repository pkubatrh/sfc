#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>

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

void print_vector(int *v, int len) {
    std::cout << "(";
    for (int j=0; j<len; j++){
        if (j == len-1)
            std::cout << v[j];
        else
            std::cout << v[j] << ", ";
    }
    std::cout << ")";
}

int main()
{
    std::ifstream f;
    bool modif = true;
    std::vector<hidden_neuron> net;
    std::vector<int> out_neurons;
    std::vector<std::vector<int> > v;
    f.open("input");
    std::string input;
    while(std::getline(f, input)) {
        std::vector<int> vect;
        std::istringstream sstr(input);
        int n;
        while(sstr >> n) {
            vect.push_back(n);
        }
        v.push_back(vect);
    }
    f.close();
    if (v.size() == 0) exit(1);
    int vector_num = v.size();
    int vector_len = v[0].size() -1;
    std::cout << std::endl << "--- Start of learning phase ---" << std::endl;
    while(modif) {
        modif = false;
        for (int i=0; i<vector_num; i++) {
            bool hit = false;
            std::cout << std::endl << "Vector: ";
            print_vector(&v[i][0], vector_len);
            std::cout << ", class: " << v[i][vector_len+1] << std::endl;
            std::cout << "Comparing existing neurons to the vector" << std::endl;
            for (int j=0; j<net.size(); j++) {
                float distance = dist(&v[i][0], net[j].pv, vector_len);
                std::cout << "Neuron: ";
                print_vector(net[j].pv, vector_len);
                std::cout << ", class: " << net[j].cls;
                std::cout << ", distance: " << distance << std::endl;
                if (distance <= net[j].r) {
                    if (v[i][2] == out_neurons[net[j].out_neuron]) {
                        std::cout << "    Hit - matching class" << std::endl;
                        hit = true;
                    } else {
                        std::cout << "    Different classes - resize sphere to half of the distance" << std::endl;
                        std::cout << "    Old r: " << net[j].r;
                        net[j].r = distance / 2;
                        std::cout << ", new r: " << net[j].r << std::endl;
                    }
                }
            }
            if (!hit) {
                std::cout << "Creating new hidden neuron" << std::endl;
                hidden_neuron new_neuron;
                new_neuron.name = "Neuron_" + std::to_string(i+1);
                new_neuron.pv = &v[i][0];
                new_neuron.cls = v[i][2];
                new_neuron.r = rmax;
                modif = true;
                int pos = std::find(out_neurons.begin(), out_neurons.end(), new_neuron.cls) - out_neurons.begin();
                if (pos >= out_neurons.size() ) {
                    // Not found, create new out neuron
                    std::cout << "Output neuron for the class not found, creating new output neuron" << std::endl;
                    out_neurons.push_back(new_neuron.cls);
                    new_neuron.out_neuron = out_neurons.size()-1;
                } else {
                    // Connect new neuron to existing out neuron
                    std::cout << "Output neuron found at pos: " << pos << std::endl;
                    new_neuron.out_neuron = pos;
                }
                net.push_back(new_neuron);
                std::cout << "Output neurons: " << out_neurons.size() << std::endl;
            }
        }
    }
    std::cout << std::endl << "--- End of learning phase ---" <<std::endl << std::endl;
    if (vector_len == 2) {
        std::cout << "WolframAlpha plot:" << std::endl;
        std::cout << "    plot ";
        int size = net.size();
        for (int i=0; i < size; i++) {
            std::cout << "(x-" << net[i].pv[0] << ")^2 + (y-" << net[i].pv[1] << ")^2 = " << net[i].r << "^2";
            if (i != size-1)
                std::cout << " and ";
        }
        std::cout << std::endl;
    }
    for (int i=0; i < net.size(); i++) {
        std::cout << "Class: " << out_neurons[net[i].out_neuron] << std::endl;
    }
    std::cout << "Net length: " << net.size() << std::endl;
}
