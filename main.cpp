#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <string>

using namespace std;

struct Clen {
    string krestni;
    string prijmeni;
    string narozeni;

    ssize_t
        id_otce,
        id_vlastni,
        id_matky;

    unsigned long int vyska;
    float hmotnost;

    Clen *dalsi, *predchozi;
};

struct Seznam {
    Clen *hlava, *zarazka;
};

bool prazdny_seznam(Seznam& s)
{
    return s.hlava == nullptr;
}

Seznam vytvor_seznam()
{
    return Seznam { nullptr, nullptr };
}

void pridej_na_zacatek(Seznam& s, Clen* novy)
{
    if (prazdny_seznam(s)) {
        s.hlava = novy;
        s.zarazka = novy;
    }
    else {
        novy->dalsi = s.hlava;
        s.hlava = s.hlava->predchozi = novy;
    }
}

void pridej_na_konec(Seznam& s, Clen *novy)
{
    if (prazdny_seznam(s))
        pridej_na_zacatek(s, novy);
    else if (s.hlava == s.zarazka) {
        s.zarazka = novy;
        s.hlava->dalsi = novy;
        s.zarazka->predchozi = s.hlava;
    }
    else {
        novy->predchozi = s.zarazka;
        s.zarazka->dalsi = novy;
        s.zarazka = novy;
    }
}

void nacist_ze_souboru(Seznam& s)
{
    string vstup;
    ifstream soubor;

    soubor.open("ukol1.txt");

    while (soubor.fail()) {
        cout << "Soubor nelze otevrit, zkontrolujte zda je ve slozce s programem ulozen soubor ukol1.txt a stisknete Enter" << endl;
        cin.get();
        soubor.open("ukol1.txt");
    }

    while (getline(soubor, vstup)) {
        if (vstup[0] == '#' || vstup.empty())
            continue;

        vstup = " " + vstup + " ";

        string data[8];
        for (int i = 0; i < 8; i++) {
            vstup = vstup.substr(1);
            int pracovn = vstup.find_first_of(" | ");
            data[i].assign(vstup.substr(0, pracovn));
            vstup = vstup.substr(pracovn);
        }

        Clen* novy = new Clen {
            .krestni = data[0],
            .prijmeni = data[2],
            .narozeni = data[6],
            .id_otce = stoi(data[3]),
            .id_vlastni = stoi(data[7]),
            .id_matky = stoi(data[1]),
            .vyska = stoul(data[4]),
            .hmotnost = stof(data[5]),

            .dalsi = nullptr,
            .predchozi = nullptr,
        };

        pridej_na_konec(s, novy);
    }

    soubor.close();
}

void vypis_od_zacatku(Seznam& s)
{
    Clen* pomocny = s.hlava;
    int pocetosob = 0;

    cout << "Takto vypada Vas seznam:" << endl;
    cout << "Poradi - Jmeno - ID matka - Prijmeni - ID otce - vyska - vaha - datum narozeni - ID osoby" << endl;
    cout << "------------------------------------------------------------------------" << endl;

    while (pomocny != s.zarazka) {
        string id_otce;
        string id_matky;

        if (pomocny->id_matky == -1)
            id_matky = "nezadano";
        else
            id_matky = std::to_string(pomocny->id_matky);

        if (pomocny->id_otce == -1)
            id_otce = "nezadano";
        else
            id_otce = std::to_string(pomocny->id_otce);

        cout
            << pocetosob + 1 << ". "
            << pomocny->krestni << "   "
            << id_matky << "   "
            << pomocny->prijmeni << "   "
            << id_otce << "   "
            << pomocny->vyska << "m   "
            << pomocny->hmotnost << "kg   "
            << pomocny->narozeni << "   "
            << pomocny->id_vlastni
        << endl;

        pomocny = pomocny->dalsi;
        pocetosob++;
    }

    cout << endl;
    cout << "Na Vasem seznamu se nachazi " << pocetosob << " osob." << endl;

    int datumnarozeni[pocetosob];

    if (pocetosob == 0)
        return;
    for (int i = 0; i < pocetosob; i++)
        datumnarozeni[i] = 0;
    string rok_mesic_den[3];
    int pom = 0;
    pomocny = s.hlava;

    while (pomocny != s.zarazka) {
        string datumpracovni;
        datumpracovni.assign(pomocny->narozeni);
        datumpracovni = "-" + datumpracovni + "-";

        for (int j = 0; j < 3; j++) {
            datumpracovni = datumpracovni.substr(1);
            int pomoc = datumpracovni.find_first_of("-");
            rok_mesic_den[j].assign(datumpracovni.substr(0, pomoc));
            datumpracovni = datumpracovni.substr(pomoc);
        }

        int rok = stoi(rok_mesic_den[0]);
        int mesic = stoi(rok_mesic_den[1]);
        int den = stoi(rok_mesic_den[2]);

        datumnarozeni[pom] = rok * 10000 + mesic * 100 + den;
        pomocny = pomocny->dalsi;
        pom++;
    }

    int poradinejstarsiho = 0;

    for (int i = 0; i < pocetosob; i++) {
        if (datumnarozeni[i] < datumnarozeni[poradinejstarsiho])
            poradinejstarsiho = i;
    }

    int nejstarsipom = 0;
    pomocny = s.hlava;

    while (nejstarsipom != poradinejstarsiho) {
        pomocny = pomocny->dalsi;
        nejstarsipom++;
    }

    cout
        << "Nejstarsi osobou je osoba c."
        << poradinejstarsiho << " "
        << pomocny->krestni << " "
        << pomocny->prijmeni << " - "
        << pomocny->narozeni << " - "
        << pomocny->vyska << "m - "
        << pomocny->hmotnost
        << "kg"
    << endl;
}

void count_list(Clen* c) {
    cout << c->krestni << endl;
    if (c->dalsi != nullptr)
        count_list(c->dalsi);
}

int main()
{
    Seznam S = vytvor_seznam();

    cout << "Dobry den, tento program umozni praci se seznamem lidi." << endl;
    cout << "Na zacatek vlozte soubok ukol1.txt do slozky s timto zdrojovym kodem a stisknete Enter" << endl;
    cin.get();
    nacist_ze_souboru(S);
    vypis_od_zacatku(S);
    return 0;
}
