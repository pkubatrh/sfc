#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include <unistd.h>

struct args {
    std::ifstream f;
    bool fflag;
    bool wolfram;
    int rmax;
};

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

void read_vector(std::vector<int> *vect, int vector_len) {
    int n;
    bool flag = true;
    std::string input;
    while (flag) {
        std::cout << ">";
        std::getline(std::cin, input);
        std::istringstream sstr(input);
        while(sstr >> n) {
            vect->push_back(n);
        }
        if (vect->size() != vector_len) {
            vect->clear();
            std::cout << "Wrong vector size." << std::endl;
        }
        else {
            flag = false;
        }
    }
}

int main(int argc, char **argv)
{
    args a;
    char c;
    bool modif = true;
    std::vector<hidden_neuron> net;
    std::vector<int> out_neurons;
    std::vector<std::vector<int> > v;
    // Default radius
    a.rmax = 3;
    a.wolfram = false;
    a.fflag = false;
    while ((c = getopt (argc, argv, "wr:f:")) != -1) {
        switch(c) {
            case 'r':
                a.rmax = atoi(optarg);
                break;
            case 'f':
                a.f.open(optarg);
                a.fflag = true;
                break;
            case 'w':
                a.wolfram = true;
                break;
            case '?':
                exit(1);
        }
    };
    if (!a.fflag) {
        exit(1);
    }

    std::string input;
    while(std::getline(a.f, input)) {
        std::vector<int> vect;
        std::istringstream sstr(input);
        int n;
        while(sstr >> n) {
            vect.push_back(n);
        }
        v.push_back(vect);
    }
    a.f.close();
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
                    if (v[i][vector_len] == out_neurons[net[j].out_neuron]) {
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
                new_neuron.cls = v[i][vector_len];
                new_neuron.r = a.rmax;
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
    if (a.wolfram && vector_len == 2) {
        std::cout << "WolframAlpha plot:" << std::endl;
        std::cout << "    plot ";
        int size = net.size();
        for (int i=0; i < size; i++) {
            std::cout << "(x-" << net[i].pv[0] << ")^2 + (y-" << net[i].pv[1] << ")^2 = " << net[i].r << "^2";
            if (i != size-1)
                std::cout << " and ";
        }
        std::cout << std::endl << std::endl;
    }
    std::cout << "Hidden neuron count: " << net.size() << std::endl;
    std::cout << "Resulting hidden neurons:" << std::endl;
    for (int i=0; i < net.size(); i++) {
        std::cout << "Centre: ";
        print_vector(net[i].pv, vector_len);
        std::cout << ", class: " << net[i].cls;
        std::cout << ", radius: " << net[i].r << std::endl;
    }

    while(true) {
        bool hit = false;
        std::cout << "Input vetor for classification (d=" << vector_len << "):" << std::endl;
        std::vector<int> inputv;
        read_vector(&inputv, vector_len);
        std::cout << "Comparing existing neurons to the vector:" << std::endl;
        for (int j=0; j<net.size(); j++) {
            float distance = dist(&inputv[0], net[j].pv, vector_len);
            std::cout << "Neuron: ";
            print_vector(net[j].pv, vector_len);
            std::cout << ", class: " << net[j].cls;
            std::cout << ", radius: " << net[j].r;
            std::cout << ", distance: " << distance << std::endl;
            if (distance <= net[j].r) {
                std::cout << "    Hit - class: " << net[j].cls << std::endl;
                hit = true;
                break;
            }
        }
        if (!hit) {
            std::cout << "Classification unsuccessful." << std::endl;
        }
    }
}
