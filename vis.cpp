#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <TCanvas.h>
#include <TGraph.h>
#include <TSystem.h>

void visualize() {
    std::ifstream file("heat_data.csv");
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    std::vector<std::string> filenames;
    std::string line;
    int timestep = 0;

    // Skip the header line
    if (std::getline(file, line)) {
        std::cout << "Skipping header: " << line << std::endl;
    }

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string token;
        std::vector<double> data;

        // Read the timestep
        if (!std::getline(ss, token, ',')) {
            std::cerr << "Error reading timestep at line " << timestep + 2 << std::endl;
            continue;
        }

        try {
            timestep = std::stoi(token);
        } catch (const std::invalid_argument &e) {
            std::cerr << "Invalid timestep: " << token << " at line " << timestep + 2 << std::endl;
            continue;
        }

        // Read the data
        while (std::getline(ss, token, ',')) {
            try {
                data.push_back(std::stod(token));
            } catch (const std::invalid_argument &e) {
                std::cerr << "Invalid number: " << token << " at line " << timestep + 2 << std::endl;
                data.clear();
                break;
            }
        }

        if (data.empty()) {
            continue; // Skip this line if it contains invalid data
        }

        TCanvas *c1 = new TCanvas("c1", "Heat Data", 800, 600);
        TGraph *graph = new TGraph(data.size());
        for (size_t i = 0; i < data.size(); ++i) {
            graph->SetPoint(i, i, data[i]);
        }

        // Set y-axis limits
        graph->SetMinimum(0.0);
        graph->SetMaximum(0.33);

        graph->Draw("AL");
        std::string filename = "heat_data_" + std::to_string(timestep) + ".png";
        c1->SaveAs(filename.c_str());
        filenames.push_back(filename);

        delete graph;
        delete c1;
    }

    // Create a GIF from the PNG images
    std::string command = "convert -delay 20 -loop 0 heat_data_*.png heat_data.gif";
    if (system(command.c_str()) != 0) {
        std::cerr << "Error creating GIF" << std::endl;
    } else {
        std::cout << "GIF created successfully!" << std::endl;
    }

    std::cout << "PNG images created successfully!" << std::endl;
}

int main() {
    visualize();
    return 0;
}