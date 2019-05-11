
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

using namespace std;

struct Clen
{
    string krestni;
    string prijmeni;
    string narozeni;
    string idotce;
    string idvlastni;
    string idmatky;
    string vyska;
    string hmotnost;
    Clen *dalsi, *predchozi;
};

struct Seznam{
    Clen *hlava, *zarazka;
};

bool prazdnyseznam (Seznam & s)
{
 return s.hlava == s.zarazka;
}


void vytvor_seznam (Seznam & s)
{ //vyska
    s.hlava = s.zarazka = new Clen {"","","","","","","","",nullptr,nullptr};
}

void pridejnazacatek (Seznam & s, string krestni , string idmatky, string prijmeni, string idotec, string vyska, string hmotnost, string narozeni, string idvlastni)
{
    s.hlava = s.hlava->predchozi = new Clen {krestni,idmatky,prijmeni,idotec,vyska,hmotnost,narozeni,idvlastni,s.hlava,nullptr};
}

void pridejnakonec (Seznam & s, string krestni , string idmatky, string prijmeni, string idotec, string vyska, string hmotnost, string narozeni, string idvlastni)
{

    if (prazdnyseznam(s))
    pridejnazacatek(s,krestni,idmatky,prijmeni,idotec,vyska,hmotnost,narozeni,idvlastni);

    else
    {
    s.zarazka -> predchozi = new Clen {krestni,idmatky,prijmeni,idotec,vyska,hmotnost,narozeni,idvlastni, s.zarazka, s.zarazka->predchozi};
    s.zarazka -> predchozi -> predchozi -> dalsi = s.zarazka -> predchozi;
    }
}

void nacist_ze_souboru (Seznam & s)
{
    string enter;
    ifstream soubor;
    soubor.open("ukol1.txt");
    while(soubor.fail())
    {
        cout << "Soubor nelze otevrit, zkontrolujte zda-li je ve slozce s programem ulozen soubor ukol1.txt (po kontole stisknete 0)" << endl;
        cin >> enter;
        soubor.open("ukol1.txt");
    }
    string vstup;
    while (getline(soubor,vstup))
    {
        if(vstup.substr(0,1)== "#" || vstup.length()== 0)
            continue;

        vstup= " " + vstup + " ";
        string data[8];
        for (int i = 0; i < 8; i++)
            {

                vstup = vstup.substr (1);
                int pracovn = vstup.find_first_of(" | ");
                data[i].assign(vstup.substr(0,pracovn));
                vstup = vstup.substr(pracovn);
            }
        pridejnakonec(s,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]);
        //cout << data [1] <<  data [6] <<endl;


    }
   soubor.close();
}

void vypis_od_zacatku (Seznam & s)
{
  Clen * pomocny = s.hlava;
  cout << "Takto vypada Vas seznam:" << endl;
  int pocetosob = 0;
  cout << "Poradi - Jmeno - ID matka - Prijmeni - ID otce - vyska - vaha - datum narozeni - ID osoby"<< endl;
  cout <<"------------------------------------------------------------------------"<<endl;
  while(pomocny != s.zarazka)
  {
      string idotce;
      string idmatky;
      if (pomocny -> idmatky == "-1")
        idmatky = "nezadano";
      else idmatky = pomocny -> idmatky;
      if (pomocny -> idotce == "-1") idotce = "nezadano";
      else idotce = pomocny -> idotce;

      cout<< pocetosob + 1 << ". " << pomocny -> krestni << pomocny -> idmatky << "   " << pomocny -> idmatky << "  " << pomocny ->prijmeni << "   " << pomocny -> idotce << "   " << pomocny -> vyska << "m   " << pomocny -> hmotnost <<"kg    " << pomocny -> narozeni << "   " << pomocny -> idvlastni << endl;

      pomocny = pomocny -> dalsi;
      pocetosob++;


  }

  cout << endl;
  cout << "Na Vasem seznamu se nachazi " << pocetosob << " osob." << endl;
  int datumnarozeni[pocetosob];
  if (pocetosob == 0)
    return;
  for (int i = 0;i < pocetosob; i++)
    datumnarozeni[i]=0;
  string rok_mesic_den[3];
  int pom = 0;
  pomocny = s.hlava;

  while (pomocny != s.zarazka)
    {
        string datumpracovni;
        datumpracovni.assign(pomocny -> narozeni);
        datumpracovni="-"+datumpracovni+"-";
        for (int j=0;j<3;j++)
        {
            datumpracovni=datumpracovni.substr(1);
            int pomoc = datumpracovni.find_first_of("-");
            rok_mesic_den[j].assign(datumpracovni.substr(0,pomoc));
            datumpracovni = datumpracovni.substr(pomoc);
        }

    int rok = stoi(rok_mesic_den[0]);
    int mesic = stoi(rok_mesic_den[1]);
    int den = stoi(rok_mesic_den[2]);
    datumnarozeni[pom]=rok*10000+mesic*100+den;
    pomocny=pomocny->dalsi;
    pom++;
    }

    int poradinejstarsiho = 0;
    for (int i = 0; i < pocetosob; i++)
    {
        if (datumnarozeni[i] < datumnarozeni[poradinejstarsiho])
            poradinejstarsiho = i;
    }

    int nejstarsipom = 0;
    pomocny = s.hlava;
    while (nejstarsipom != poradinejstarsiho)
    {
        pomocny=pomocny->dalsi;
        nejstarsipom++;
    }
    cout << "Nejstarsi osobou je osoba c." << poradinejstarsiho << " " << pomocny->krestni << " " << pomocny->prijmeni << " - " << pomocny->narozeni << " - " << pomocny->vyska << "m - " << pomocny->hmotnost << "kg" << endl;
}


int main()
{
    Seznam S;
    vytvor_seznam(S);
    string enter;
    cout << "Dobry den, tento program umozni praci se seznamem lidi." << endl;
    cout << "Na zacatek vlozte soubok ukol1.txt do slozky s timto zdrojovym kodem (az to bude, stisnkete napiste 0)" << endl;
    cin >> enter;
    nacist_ze_souboru(S);
    vypis_od_zacatku(S);
    return 0;
}
