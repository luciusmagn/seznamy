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

void pridej_na_konec(Seznam& s, Clen* novy)
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
    cout << "------------------------------------------------------------------------------------------" << endl;

    while (pomocny != s.zarazka && pomocny != nullptr) {
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
    << endl << endl;
}

void smaz_prvniho(Seznam& s)
{
    if (prazdny_seznam(s))
        return;
    else if (s.hlava->dalsi == nullptr)
    {
        delete s.hlava;
        s.hlava = nullptr;
    }
    else
    {
        Clen* pomocny = s.hlava;
        s.hlava = s.hlava->dalsi;
        s.hlava->predchozi = nullptr;
        delete pomocny;
    }
}

Clen* na_pozici(Seznam& s, unsigned int poradi)
{
    Clen* pomocny = s.hlava;
    for (int i = 1; i < poradi; i++)
        pomocny = pomocny->dalsi;
    return pomocny;
}

void smaz_posledniho(Seznam& s)
{
    if (prazdny_seznam(s))
        return;
    else if (s.hlava->dalsi == nullptr)
        smaz_prvniho(s);
    else
    {
        Clen* pomocny = s.zarazka;
        s.zarazka = s.zarazka->predchozi;
        s.zarazka->dalsi = nullptr;
        delete pomocny;
    }
}

void smaz_na(Seznam& s, Clen* mazany)
{
    if (mazany == s.hlava)
        smaz_prvniho(s);
    else if (mazany == s.zarazka)
        smaz_posledniho(s);
    else
    {
        Clen* pomocny = mazany;
        mazany->predchozi->dalsi = mazany->dalsi;
        mazany->dalsi->predchozi = mazany->predchozi;
        delete pomocny;
    }
}

void smaz_vsechny(Seznam& s)
{
    while (!prazdny_seznam(s))
        smaz_prvniho(s);
}

unsigned int delka_seznamu(Seznam& s)
{
    unsigned int delka = 0;
    Clen* pomocny = s.hlava;
    while ( (pomocny = pomocny->dalsi) )
        delka++;
    cout << "[WOWSO] délka: " << delka << endl;
    return delka;
}

void pridej_na_misto(Seznam& s, Clen* novy, unsigned int index)
{
    if (index == 0)
        pridej_na_zacatek(s, novy);
    else if (index == delka_seznamu(s))
        pridej_na_konec(s, novy);
    else
    {
        Clen* pomocny = s.hlava;
        for (int i = 1; i < index; i++) {
            pomocny = pomocny->dalsi;
        }

        novy->dalsi = pomocny->dalsi;
        novy->predchozi = pomocny;
        pomocny->dalsi = novy;
    }
}

char vol_operaci(Seznam& s)
{
    char volba;

    cout<<"Co si prejete s Vasim seznamem delat?"<< endl;

    if (prazdny_seznam(s))
    {
        cout<<"Pridat osobu ...... a" << endl;
        cout << "Zadejte: ";
        cin >> volba;
        cout << endl;
        while (volba != 97 && volba != 103)
        {
            cout << "Zadali jste neplatne pismeno.";
            cin >> volba;
        }
    }
    else
    {
        cout<<"Pridat osobu na zacatek ...... a" << endl;
        cout<<"Pridat osobu na konec ...... b" << endl;
        cout<<"Pridat osobu na konkretni misto ...... c" << endl;
        cout<<"Smazat prvni osobu ...... d" << endl;
        cout<<"Smazat posledni osobu ...... e" << endl;
        cout<<"Smazat osobu na kokretnim miste ...... f" << endl;
        cout<<"Smazat cely seznam ...... g" << endl;
        cout << "Zadejte Vasi volbu: ";
        cin >> volba;
        cout << endl;
        while (volba < 97 || volba > 103)
            {
                cout << "Zadali jste neplatne pismeno." << endl;
                cin >> volba;
                cout << endl;
            }
    }

    return volba;
}

char proved_operaci(Seznam & s, char volba)
{
    string udaj [8];
    if (volba=='a')
    {
            cout << "Napiste krestni jmeno osoby: ";
            cin >> udaj [0];
            cout << "Napiste prijmeni osoby: ";
            cin >> udaj [1];
            cout << "Napiste ID osoby: ";
            cin >> udaj [2];
            cout << "Napiste ID matky: ";
            cin >> udaj [3];
            cout << "Napiste ID otce: ";
            cin >> udaj [4];
            cout << "Napiste vysku osoby: ";
            cin >> udaj [5];
            cout << "Napiste vahu osoby: ";
            cin >> udaj [6];
            cout << "Napiste datum narozeni osoby (ve formatu YYYY-M-D): ";
            cin >> udaj [7];

            Clen* muj_novy = new Clen {
                .krestni = udaj [0],
                .prijmeni = udaj [1],
                .narozeni = udaj [7],
                .id_otce = stoi(udaj [4]),
                .id_vlastni = stoi(udaj [2]),
                .id_matky = stoi(udaj [3]),
                .vyska = stoul(udaj [5]),
                .hmotnost = stof(udaj [6]),

                .dalsi = nullptr,
                .predchozi = nullptr,
            };

            pridej_na_zacatek(s, muj_novy);
    }
    else if (volba=='b')
    {
            cout << "Napiste krestni jmeno osoby: ";
            cin >> udaj [0];
            cout << "Napiste prijmeni osoby: ";
            cin >> udaj [1];
            cout << "Napiste ID osoby: ";
            cin >> udaj [2];
            cout << "Napiste ID matky: ";
            cin >> udaj [3];
            cout << "Napiste ID otce: ";
            cin >> udaj [4];
            cout << "Napiste vysku osoby: ";
            cin >> udaj [5];
            cout << "Napiste vahu osoby: ";
            cin >> udaj [6];
            cout << "Napiste datum narozeni osoby (ve formatu YYYY-M-D): ";
            cin >> udaj [7];

           Clen* muj_novy = new Clen {
                .krestni = udaj [0],
                .prijmeni = udaj [1],
                .narozeni = udaj [7],
                .id_otce = stoi(udaj [4]),
                .id_vlastni = stoi(udaj [2]),
                .id_matky = stoi(udaj [3]),
                .vyska = stoul(udaj [5]),
                .hmotnost = stof(udaj [6]),

                .dalsi = nullptr,
                .predchozi = nullptr,
            };

            pridej_na_konec(s,muj_novy);


    }
    else if (volba=='c')
    {
        unsigned int misto;
        cout << "Napiste na jake misto chcete pridat osobu: ";
        cin >> misto;

        cout << "Napiste krestni jmeno osoby: ";
        cin >> udaj [0];
        cout << "Napiste prijmeni osoby: ";
        cin >> udaj [1];
        cout << "Napiste ID osoby: ";
        cin >> udaj [2];
        cout << "Napiste ID matky: ";
        cin >> udaj [3];
        cout << "Napiste ID otce: ";
        cin >> udaj [4];
        cout << "Napiste vysku osoby: ";
        cin >> udaj [5];
        cout << "Napiste vahu osoby: ";
        cin >> udaj [6];
        cout << "Napiste datum narozeni osoby (ve formatu YYYY-M-D): ";
        cin >> udaj [7];

        Clen* muj_novy = new Clen {
            .krestni = udaj [0],
            .prijmeni = udaj [1],
            .narozeni = udaj [7],
            .id_otce = stoi(udaj [4]),
            .id_vlastni = stoi(udaj [2]),
            .id_matky = stoi(udaj [3]),
            .vyska = stoul(udaj [5]),
            .hmotnost = stof(udaj [6]),

            .dalsi = nullptr,
            .predchozi = nullptr,
        };

        pridej_na_misto(s, muj_novy, misto - 1);
    }
    else if (volba=='d')
    {
        smaz_prvniho(s);
    }
    else if (volba=='e')
    {
        smaz_posledniho(s);
    }
    else if (volba=='f')
    {
        unsigned int misto;
        cout << "Napiste poradi osoby, kterou chcete smazat: ";
        cin >> misto;
        smaz_na(s,na_pozici(s,misto));
    }
    else if (volba=='g')
    {
       smaz_vsechny(s);
    }

    vypis_od_zacatku(s);
    return proved_operaci(s,vol_operaci(s));
}

int main()
{
    Seznam S = vytvor_seznam();

    cout << "Vlozte soubor ukol1.txt do slozky s timto zdrojovym kodem a stisknete Enter" << endl;
    cin.get();
    nacist_ze_souboru(S);
    vypis_od_zacatku(S);
    proved_operaci(S, vol_operaci(S));

    return 0;
}
