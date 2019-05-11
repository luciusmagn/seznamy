#include <fstream>
#include <iostream>
#include <stdlib.h>
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

    unsigned int vyska;
    float hmotnost;

    Clen *dalsi, *predchozi;
};

struct Seznam {
    Clen *hlava, *zarazka;
};

bool prazdny_seznam(Seznam& s)
{
    return s.hlava == s.zarazka;
}

struct Seznam* vytvor_seznam()
{
    return new Seznam { nullptr, nullptr };
}

void pridej_na_zacatek(Seznam& s, Clen* novy)
{
    novy->dalsi = s.hlava;
    s.hlava = s.hlava->predchozi = novy;
}

void pridej_na_konec(Seznam& s, Clen *novy)
{
    if (prazdny_seznam(s))
        pridej_na_zacatek(s, novy);
    else {
        novy->dalsi = s.zarazka;
        novy->predchozi = s.zarazka->predchozi;

        s.zarazka->predchozi = novy;
        s.zarazka->predchozi->predchozi->dalsi = s.zarazka->predchozi;
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
        if (vstup.substr[0] == '#' || vstup.empty())
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
            .birthday = data[6],
            .id_otce = stoi(data[3]),
            .id_matky = stoi(data[1]),
            .id = stoi(data[7]),
            .vyska = stoul(data[4]),
            .hmotnost = stof(data[5]),

            .dalsi = nullptr,
            .zarazka = nullptr,
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

        if (pomocny->idmatky == "-1")
            id_matky = "nezadano";
        else
            idmatky = pomocny->idmatky;

        if (pomocny->idotce == "-1")
            id_otce = "nezadano";
        else
            id_otce = pomocny->idotce;

        cout
            << pocetosob + 1 << ". "
            << pomocny->krestni << "   "
            << id_matky << "   "
            << pomocny->prijmeni << "   "
            << id_otce << "   "
            << pomocny->vyska << "m   "
            << pomocny->hmotnost << "kg   "
            << pomocny->narozeni << "   "
            << pomocny->idvlastni
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
    cout << "Nejstarsi osobou je osoba c." << poradinejstarsiho << " " << pomocny->krestni << " " << pomocny->prijmeni << " - " << pomocny->narozeni << " - " << pomocny->vyska << "m - " << pomocny->hmotnost << "kg" << endl;
}

int main()
{
    Seznam S = vytvor_seznam();
    string enter;
    cout << "Dobry den, tento program umozni praci se seznamem lidi." << endl;
    cout << "Na zacatek vlozte soubok ukol1.txt do slozky s timto zdrojovym kodem a stisknete Enter" << endl;
    cin >> enter;
    nacist_ze_souboru(S);
    vypis_od_zacatku(S);
    return 0;
}
