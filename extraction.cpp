#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>
#include <cmath>
#include <regex>
#include <cctype> 
#include <algorithm>
namespace fs = std::filesystem;

// Para rodar na titânio g++ -std=c++17 -o extraction extraction.cpp -lstdc++fs


// Função para processar o arquivo
void process_file(const std::string& input_file, const std::string& output_file) {
    std::ifstream infile(input_file);
    std::ofstream outfile(output_file);
    
    // Checa se o arquivo foi aberto corretamente
    if (!infile.is_open() || !outfile.is_open()) {
        std::cerr << "Erro ao abrir arquivo." << std::endl;
        return;
    }

    std::string line;
    int row_num = 0;
    std::vector<std::string> exclude = {"EVTH", "RUNE", "EVTE"};
    
    // Pular as primeiras 6 linhas
    while (row_num < 6 && std::getline(infile, line)) {
        row_num++;
    }

    // Processar o arquivo linha por linha
    while (std::getline(infile, line)) {
        // Dividir a linha em palavras
        std::stringstream ss(line);
        std::string word;
        std::vector<std::string> fields;

        while (ss >> word) {
            // Substituir "  " por um espaço único
            std::regex space_pattern("\\s{2,}");
            word = std::regex_replace(word, space_pattern, " ");
            fields.push_back(word);
        }

        // Excluir linhas com palavras não desejadas
        bool exclude_line = false;
        for (const auto& ex : exclude) {
            if (std::find(fields.begin(), fields.end(), ex) != fields.end()) {
                exclude_line = true;
                break;
            }
        }

        if (exclude_line) {
            continue;
        }

        // Renomear os campos conforme seu código
        if (fields.size() > 5) {
            double m = std::stod(fields[0]);
            double x = std::stod(fields[4]);
            double y = std::stod(fields[5]);

            // Filtrar valores de "m" entre 2000 e 7000
            if (m >= 2000 && m <= 7000) {
                // Calcular r
                double r = std::sqrt(x * x + y * y);

                // Escrever no arquivo de saída
                outfile << m << "\t" << x << "\t" << y << "\t" << r << std::endl;
            }
        }
    }

    infile.close();
    outfile.close();
}

// Função para iterar pelos arquivos e processar
void process_all_files(const std::string& directory) {
    // Usar a biblioteca filesystem para iterar pelos arquivos no diretório
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        std::cout << "Arquivo: " << entry.path() << std::endl;
        std::string filename = entry.path().filename().string();

        // Verificar se o nome do arquivo começa com "OUT" e é seguido por números
        if (filename.rfind("OUT", 0) == 0 && std::all_of(filename.begin() + 3, filename.end(), ::isdigit)) {
            std::cout << "Processando o arquivo: " << filename << std::endl;
            std::string input_file = entry.path().string();
            std::string output_file = entry.path().parent_path().string() + "/OUT" + filename.substr(3) + "_tratado.csv";

            // Processar o arquivo
            process_file(input_file, output_file);
        }
    }
}

int main() {
    // Diretório onde os arquivos estão localizados
    std::string directory = "./"; 

    process_all_files(directory);

    return 0;
}
