#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

// Osnovna klasa Racun
class Racun {
protected:
    string nazivBanke;
    string tipKartice;
    string brojRacuna;
    int godinaVazenja;

public:
    Racun(string nb, string tk, string br, int gv = 2023)
        : nazivBanke(nb), tipKartice(tk), brojRacuna(br), godinaVazenja(gv) {}

    virtual bool sifraRacunaValidna() const = 0;

    friend ostream& operator<<(ostream& os, const Racun& racun) {
        os << " " << racun.nazivBanke << " " << racun.tipKartice
            << " " << racun.brojRacuna << " " << racun.godinaVazenja;
        return os;
    }

    int getGodinaVazenja() const {
        return godinaVazenja;
    }

    string getBrojRacuna() const {
        return brojRacuna;
    }
};

// Klasa DevizniRacun
class DevizniRacun : public Racun {
public:
    DevizniRacun(string nb, string tk, string br, int gv = 2023)
        : Racun(nb, tk, br, gv) {}

    bool sifraRacunaValidna() const override {
        return all_of(brojRacuna.begin(), brojRacuna.end(), [](char c) { return isdigit(c) || islower(c); });
    }
};

// Klasa DinarskiRacun
class DinarskiRacun : public Racun {
public:
    DinarskiRacun(string nb, string tk, string br, int gv = 2023)
        : Racun(nb, tk, br, gv) {}

    bool sifraRacunaValidna() const override {
        return none_of(brojRacuna.begin(), brojRacuna.end(), [](char c) { return isdigit(c) || isupper(c); });
    }
};

int main() {
    vector<Racun*> racuni;
    ifstream inputFile("input.txt");
    ofstream outputFile("output.txt");

    if (!inputFile.is_open()) {
        cerr << "Error opening input file!" << endl;
        return 1;
    }

    char oznaka;
    string nazivBanke, tipKartice, brojRacuna;
    int godinaVazenja;

    while (inputFile >> oznaka >> nazivBanke >> tipKartice >> brojRacuna >> godinaVazenja) {
        if (godinaVazenja == 0) {
            godinaVazenja = 2024;
        }
        if (oznaka == 'I') {
            racuni.push_back(new DevizniRacun(nazivBanke, tipKartice, brojRacuna, godinaVazenja));
        }
        else if (oznaka == 'D') {
            racuni.push_back(new DinarskiRacun(nazivBanke, tipKartice, brojRacuna, godinaVazenja));
        }
    }

    inputFile.close();

    stable_sort(racuni.begin(), racuni.end(), [](Racun* a, Racun* b) {
        return a->getGodinaVazenja() > b->getGodinaVazenja();
        });

    auto minElement = min_element(racuni.begin(), racuni.end(), [](Racun* a, Racun* b) {
        return a->getGodinaVazenja() < b->getGodinaVazenja();
        });

    if (minElement != racuni.end()) {
        Racun* minRacun = *minElement;
        outputFile << *minRacun << endl;

        if (!minRacun->sifraRacunaValidna()) {
            outputFile << "Racun sa najmanjom godinom vazenja ima nevalidan broj!" << endl;
        }
    }

    for (Racun* racun : racuni) {
        delete racun;
    }

    outputFile.close();

    return 0;
}
