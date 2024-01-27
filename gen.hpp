#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <random>
#include <set>

using rand_dist = std::uniform_int_distribution<std::mt19937_64::result_type>;

std::vector<std::string> part_S, part_M, part_E, pairs;
int stateN = 0;
std::string stateW;
int loopN = 0;
std::string loopW;

char stop;

void loadParticlesFromFile(const std::string& filename, std::vector<std::string>& particles);
void loop();
bool hasPairs(const std::string& input, const std::vector<std::string>& pairsToCheck);
void clearConsole();
void printParticles(const std::vector<std::string>& particles);
void saveWordsToFile(const std::set<std::string>& words);
void generate_W();

void saveWordsToFile(const std::set<std::string>& words) {
    int i = 1;
    std::ofstream outputFile("words.txt");
    if (outputFile.is_open()) {
        for (const auto& word : words) {
            if (stateN == 1) {
                outputFile << i << ". " << word << std::endl;
                i++;
            } else if (stateN == 0) {
                outputFile << word << std::endl;
            }
        }
        outputFile.close();
        std::cout << std::endl;
        std::cout << "Words saved to words.txt\n";
    } else {
        std::cerr << "Unable to open file: words.txt\n";
    }
}

void loadParticlesFromFile(const std::string& filename, std::vector<std::string>& particles) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string particle;
        while (file >> particle) {
            particles.push_back(particle);
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

bool hasPairs(const std::string& input, const std::vector<std::string>& pairsToCheck) {
    for (const auto& pair : pairsToCheck) {
        if (input.find(pair) != std::string::npos) {
            return true;
        }
    }
    return false;
}

void clearConsole() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void printParticles(const std::vector<std::string>& particles) {
    for (const auto& pair : particles) {
        std::cout << pair << std::endl;
    }
}

void menu() {
    int choice;

    if (stateN == 0) {
        stateW = "OFF";
    } else {
        stateW = "ON";
    }

    if (loopN == 0) {
        loopW = "OFF";
    } else {
        loopW = "ON";
    }

    clearConsole();
    std::cout << std::endl;
    std::cout << "1. Generate W" << std::endl;
    std::cout << "2. Word Count(" << stateW << ")" << std::endl;
    std::cout << "3. looped(" << loopW << ")" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << std::endl;
    std::cout << "> ";
    std::cin >> choice;

    switch (choice) {
        case 1:
            if (loopN == 0) {
                generate_W();
            } else if (loopN == 1) {
                loop();
            }
            break;
        case 2:
            if (stateN == 0) {
                stateN = 1;
                clearConsole();
                menu();
            } else {
                stateN = 0;
                clearConsole();
                menu();
            }
            break;
        case 3:
            if (loopN == 0) {
                loopN = 1;
                clearConsole();
                menu();
            } else {
                loopN = 0;
                clearConsole();
                menu();
            }
            break;
        case 4:
            clearConsole();
            exit(0);
            break;
        default:
            clearConsole();
            exit(0);
            break;
    }
}

void loop() {
    generate_W();
}

void generate_W() {
    clearConsole();

    srand(time(nullptr));
    std::set<std::string> uniqueWords;
    auto device = std::mt19937_64(rand());

    int amount;
    std::string word;

    loadParticlesFromFile("part_S.txt", part_S);
    loadParticlesFromFile("part_M.txt", part_M);
    loadParticlesFromFile("part_E.txt", part_E);

    rand_dist distribution_S(0, part_S.size() - 1);
    rand_dist distribution_M(0, part_M.size() - 1);
    rand_dist distribution_E(0, part_E.size() - 1);

    std::cout << std::endl;
    std::cout << "How many words do you want to generate? (Enter 0 to quit, 1 to go menu): ";
        
    std::cin >> amount;

    if (amount == 0) {
        clearConsole();
        exit(0);
    } else if (amount == 1) {
        clearConsole();
        menu();
    }

    clearConsole();

    while (uniqueWords.size() < static_cast<std::size_t>(amount)) {
        std::string partO = part_S.at(distribution_S(device));
        std::string partT = part_M.at(distribution_M(device));
        std::string partTT = part_E.at(distribution_E(device));

        word = partO + partT + partTT;

        loadParticlesFromFile("pair.txt", pairs);

        if (!hasPairs(word, pairs)) {
            uniqueWords.insert(word);
        }
    }

    int j = 1;
    std::cout << '\n';
    std::cout << "Words:" << std::endl << std::endl;
    for (const auto& word : uniqueWords) {
        if (stateN == 1) {
            std::cout << j << ". " << word << std::endl;
            j++;
        } else if (stateN == 0) {
            std::cout << word << std::endl;
        }
    }

    saveWordsToFile(uniqueWords);

    uniqueWords.clear();

    std::cout << "Enter something to continue: ";
    std::cin >> stop;

    if (loopN == 0){
        menu();
    } else if (loopN == 1){
        loop();
    }
}

