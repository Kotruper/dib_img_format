#include <exception>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <string>
#include <unistd.h>
using namespace std;

SDL_Window* window = NULL;
SDL_Surface* screen = NULL;

#define szerokosc 512
#define wysokosc 340
#define KOLORY 8

#define tytul "GKiM2021 - Projekt - Projekt"



void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
void setPixel(int x, int y, SDL_Color kolor);
SDL_Color getPixel (int x, int y);

void czyscEkran(Uint8 R, Uint8 G, Uint8 B);

int dodajKolor(SDL_Color kolor);
bool porownajKolory(SDL_Color k1, SDL_Color k2);
int sprawdzKolor(SDL_Color kolor);
void ladujBMP(char const*,int,int);
void Sorting();
void zapiszObrazek(char metoda, bool dithering, string nazwaPliku);
void medianCut(int start, int koniec, int iter);
char biggestRange(int,int);
void sortSegment(int,int,char);
void debug();
int dither(SDL_Color kolor,int x,int y, float**,float**,float**);
void obliczPalete();
void wczytajObrazek(string nazwaPliku);
int kompresjaRLE(int wejscie[], int dlugosc, int tablicaRLE[]);

void zapiszUbite(ofstream& wyjscie, int* obrazek, int n);
void zapiszRLE(ofstream& wyjscie, int* obrazek, int n);
void wczytajUbite(ifstream& wejscie,int wysokoscObrazka, int szerokoscObrazka);
void wczytajRLE(ifstream& wejscie, Uint16 dlugoscRLE, Uint16 wysokoscObrazka, Uint16 szerokoscObrazka);

void Funkcja1();
void Funkcja2();
void Funkcja3();
void Funkcja4();
void Funkcja5();
void Funkcja6();
void Funkcja7();
void Funkcja8();
void Funkcja9();

void setPixel(int x, int y, SDL_Color kolor);
SDL_Color* paletaGlowna = NULL;
SDL_Color paletaKolorow[KOLORY]{{0,0,0},{0,0,255},{0,255,0},{0,255,255},{255,0,0},{255,0,255},{255,255,0},{255,255,255}};
//SDL_Color paletaSzarosci[KOLORY]{{0,0,0}, {29,29,29}, {76,76,76}, {105,105,105}, {149,149,149}, {178,178,178}, {225,225,225}, {255,255,255}}; //chciał inne szarości
SDL_Color paletaSzarosci[KOLORY]{{0,0,0}, {36,36,36}, {73,73,73}, {109,109,109}, {146,146,146}, {182,182,182}, {218,218,218}, {255,255,255}};
SDL_Color paletaNarzucona[KOLORY];
int znajdzSomsiada(SDL_Color kolor){
    int min = 765;
    int indexMin;
    SDL_Color temp;
    for(int i = 0; i < KOLORY; i++){
        temp = paletaGlowna[i];
        int odleglosc = abs(kolor.r - temp.r) + abs(kolor.g - temp.g) + abs(kolor.b - temp.b);
        if(odleglosc < min){
            indexMin = i;
            min = odleglosc;
        }
    }
    return indexMin;
}

int znajdzSomsiada2(SDL_Color kolor){
    int min = 999999;
    int indexMin;
    SDL_Color temp;
    for(int i = 0; i < KOLORY; i++){
        temp = paletaGlowna[i];
        float odleglosc = sqrt((kolor.r - temp.r)*(kolor.r - temp.r) + (kolor.g - temp.g)*(kolor.g - temp.g) + (kolor.b - temp.b)*(kolor.b - temp.b));
        if(odleglosc < min){
            indexMin = i;
            min = odleglosc;
        }
    }
    return indexMin;
}
int znajdzSomsiada2(float R,float G,float B){
    int min = 999999;
    int indexMin;
    SDL_Color temp;
    float odleglosc;
    for(int i = 0; i < KOLORY; i++){
        temp = paletaGlowna[i];
        odleglosc = sqrt((R - temp.r)*(R - temp.r) + (G - temp.g)*(G - temp.g) + (B - temp.b)*(B - temp.b));
        if(odleglosc < min){
            indexMin = i;
            min = odleglosc;
        }
    }
    return indexMin;
}

SDL_Color paleta[100000];
int ileKolorow = 0;

SDL_Color pixele[200000];
int ilePixeli = 0;
int ileKolorowGlowna=0;

void wypiszInfo(bool ditherIMG, char paletteIMG, string plikIMG,bool conDir){
    cout << "\n/////////////////////////////////////////////////////\n";
    //cout << "Funckje 1 do 6 automatycznie wykonuja zapis do pliku!\n";
    //cout << "1.Stala paleta\n2.Stala paleta + dithering\n3.8Kolorow szarosci\n4.8Kolorow szarosci + dithering\n5.Dostosowana paleta\n6.Dostosowana paleta + dithering\n7.Odczyt z pliku\n8.Wyswietl palete\n9.Wyswietl menu";
    cout<<"1 - Dithering: ["<<((ditherIMG)?'x':' ')<<"]\n";
    //cout<<"2 - Kompresja RLE: ["<<((comressIMG)?'x':' ')<<"]\n";
    cout<<"2 - Paleta: ";
    switch(paletteIMG){
        case 's': cout<<"Szarosci\n"; break;
        case 'n': cout<<"Narzucona\n"; break;
        default: cout<<"Domyslna\n"; break;
    }
    cout<<"3 - Wybierz plik: "<<plikIMG<<"\n";
    cout<<"4 - Zapisz obrazek\n";
    cout<<"5 - Wczytaj obrazek\n";
    cout<<"6 - Kierunek konwersji: DIB "<<((conDir)?"<-":"->")<<" BMP\n";
    cout<<"7 - Przekonwertuj obrazek\n";
    cout << "\n/////////////////////////////////////////////////////\n\n";
}

void Funkcja1() {
    //zapiszObrazek('k',false);
    SDL_UpdateWindowSurface(window);
}

void Funkcja2(){
    //zapiszObrazek('k',true);
    SDL_UpdateWindowSurface(window);
}

void Funkcja3() {
    //zapiszObrazek('s',false);
    SDL_UpdateWindowSurface(window);
}

void Funkcja4(){
    //zapiszObrazek('s',true);
    SDL_UpdateWindowSurface(window);
}

void Funkcja5() {
    //zapiszObrazek('n',false);
    SDL_UpdateWindowSurface(window);
}

void Funkcja6() {
    //zapiszObrazek('n',true);
    SDL_UpdateWindowSurface(window);
}

void Funkcja7() {
    //wczytajObrazek();
    SDL_UpdateWindowSurface(window);
}

void Funkcja8() {
    if(!paletaGlowna)
        paletaGlowna = paletaKolorow;

    for(int y = wysokosc/2; y < wysokosc/2+40; y++){
        for(int x = 0; x < szerokosc/2; x++){
            if(y-wysokosc/2 < 5)
                setPixel(x,y,paletaGlowna[0]);
            else if(y-wysokosc/2 < 10)
                setPixel(x,y,paletaGlowna[1]);
            else if(y-wysokosc/2 < 15)
                setPixel(x,y,paletaGlowna[2]);
            else if(y-wysokosc/2 < 20)
                setPixel(x,y,paletaGlowna[3]);
            else if(y-wysokosc/2 < 25)
                setPixel(x,y,paletaGlowna[4]);
            else if(y-wysokosc/2 < 30)
                setPixel(x,y,paletaGlowna[5]);
            else if(y-wysokosc/2 < 35)
                setPixel(x,y,paletaGlowna[6]);
            else
                setPixel(x,y,paletaGlowna[7]);
        }
    }

    SDL_UpdateWindowSurface(window);
}

void Funkcja9() {
    //wypiszInfo();
    SDL_UpdateWindowSurface(window);
}

void obliczPalete() {
    SDL_Color temp;
    int wartosc;
    ileKolorowGlowna=0;
    ilePixeli=0;

    for (int y=0; y<wysokosc/2; y++){
        for (int x=0; x<szerokosc/2; x++){
            temp=getPixel(x,y);
            pixele[ilePixeli]=temp;
            ilePixeli++;
        }
    }
    medianCut(0,ilePixeli,3);

    SDL_UpdateWindowSurface(window);
}

char biggestRange(int start, int koniec){
    int minB=0,minG=0,minR=0,maxB=0,maxG=0,maxR=0;
    for(int i=start;i<koniec;i++){
        if(pixele[i].b<pixele[minB].b)
            minB=i;
        if(pixele[i].g<pixele[minG].g)
            minG=i;
        if(pixele[i].r<pixele[minR].r)
            minR=i;
        if(pixele[i].b>pixele[maxB].b)
            maxB=i;
        if(pixele[i].g>pixele[maxG].g)
            maxG=i;
        if(pixele[i].g>pixele[maxR].g)
            maxR=i;
    }
    int rangeB=pixele[maxB].b-pixele[minB].b;
    int rangeG=pixele[maxG].g-pixele[minG].g;
    int rangeR=pixele[maxR].r-pixele[minR].r;
    //cout<<"ranges: ("<<rangeR<<","<<rangeG<<","<<rangeB<<")\n";
    char biggest='B';
    if(rangeR>rangeB)
        biggest='R';
    if(rangeG>rangeR)
        biggest='G';
    if(rangeG>rangeB)
        biggest='G';
    return biggest;
}

void sortSegment(int start, int koniec, char sortBy){
    for (size_t i = start; i < koniec; i++){
        int maxI=i;
        for (size_t ii = i; ii < koniec; ii++){
            switch(sortBy){
                case 'R':
                    if(pixele[ii].r>pixele[maxI].r)
                        maxI=ii;
                break;
                case 'G':
                    if(pixele[ii].g>pixele[maxI].g)
                        maxI=ii;
                break;
                case 'B':
                    if(pixele[ii].b>pixele[maxI].b)
                        maxI=ii;
                break;
            }
        }
        swap(pixele[i],pixele[maxI]);
    }
}

void medianCut(int start, int koniec, int iter){
    if (iter>0){
        char sortBy=biggestRange(start,koniec);
        //cout<<"sorting by "<<sortBy<<"\n";
        //debug();
        sortSegment(start,koniec,sortBy);
        //debug();
        int mid=(start+koniec)/2; //?
        medianCut(start,mid,iter-1);
        medianCut(mid+1,koniec,iter-1);
    }
    else{
        int sumR=0,sumG=0,sumB=0;
        for(int i=start;i<koniec;i++){
            sumR+=pixele[i].r;
            sumG+=pixele[i].g;
            sumB+=pixele[i].b;
        }
        SDL_Color temp{sumR/(koniec-start+1),sumG/(koniec-start+1),sumB/(koniec-start+1)};
        paletaNarzucona[ileKolorowGlowna]=temp;
        cout<<ileKolorowGlowna<<": ("<<(int)temp.r<<" "<<(int)temp.g<<" "<<(int)temp.b<<")\n";
        ileKolorowGlowna++;
    }
}

void debug(){
    SDL_Color temp;
    for (int y=0; y<wysokosc/2; y++){
        for (int x=0; x<szerokosc/2; x++){
            temp=pixele[(y*szerokosc/2)+x];
            setPixel(x+szerokosc/2,y+wysokosc/2,temp);
        }
    }
    SDL_UpdateWindowSurface(window);
    sleep(1);
}

int kompresjaRLE(int wejscie[], int dlugosc, int tablicaRLE[]){
    int poz = 0;
    int i = 0;

    //dopóki bajty nie skompresowane
    while(i < dlugosc){
        //sekwaencja powtarzania sie co najmniej 2 bajtow
        if( (i < dlugosc-1) && (wejscie[i] == wejscie[i+1])){
            //mierzymy dlugosc sekwencji
            int j = 0;
            while((i+j < dlugosc-1) && (wejscie[i+j] == wejscie[i+j+1]) && (j<254)){
                j++;
            }
            //wypisujemy spakowana sekwencje
            tablicaRLE[poz++] = j+1;
            tablicaRLE[poz++] = wejscie[i+j];
            //cout << j+1 << ", " << wejscie[i+j] << ", ";
            //przesuwamy wskaznik o dlugosc sekwencji
            i += (j+1);
        }else{ //sekwaencja roznych bajtow
            //mierzymy dlugosc sekwencji
            int j = 0;
            while((i+j < dlugosc-1) && (wejscie[i+j] != wejscie[i+j+1]) && (j<254)){
                j++;
            }

            //koncowka
            if((i+j == dlugosc-1) && (j<254)){
                j++;
            }

            //wypisujemy spakowana sekwencje
            tablicaRLE[poz++] = 0;
            tablicaRLE[poz++] = j;
            //cout << (int)0 << ", " << j << ", ";
            for(int k = 0; k < j; k++){
                tablicaRLE[poz++] = wejscie[i+k];
                //cout << wejscie[i+k] << ", ";
            }
            if(j%2 != 0){
                tablicaRLE[poz++] = 0;
                //cout << (int)0 << ", ";
            }

            //przesuwamy wskaznik
            i += (j);
        }
    }
    return poz;
}

void dekompresjaRLE(int wejscie[], int dlugosc, int wyjscie[]){
    int i = 0;
    int ile = 0;
    int poz = 0;

    //dopoki wszystki bajty nie sa zdekompresowane
    while(i < dlugosc){
        //sekwencaja powatrzajacych się bajtow
        if(wejscie[i] > 0){
            for(int j = 0; j < wejscie[i]; j++){
                //cout << wejscie[i+1] << ", ";
                wyjscie[poz++] = wejscie[i+1];
            }
            i += 2;
        }
        else{ //sekwencja roznych bajtiow
            ile = wejscie[i+1];
            for(int j = 0; j < ile; j++){
                //cout << wejscie[i+1+j+1] << ", ";
                wyjscie[poz++] = wejscie[i+1+j+1];
            }
            i += ile + 2;
            if(ile % 2 != 0){
                i++;
            }
        }
    }
}


void zapiszUbite(ofstream& wyjscie, int* obrazek, int n){
    Uint8 zapisInt=0;
    int zapisPrzes=8;
    for (size_t i = 0; i < n; i++){
        zapisPrzes-=3;
            if (zapisPrzes>=0)
                zapisInt|=(obrazek[i]<<zapisPrzes);
            else
                zapisInt|=(obrazek[i]>>-zapisPrzes);
            if (zapisPrzes<=0){
                wyjscie.write((char*)&zapisInt, sizeof(Uint8));
                zapisInt=0;
                zapisPrzes+=8;
                zapisInt|=(obrazek[i]<<zapisPrzes);
            }
    }
    if (zapisInt>0){
        wyjscie.write((char*)&zapisInt, sizeof(Uint8));
    }
}

void zapiszRLE(ofstream& wyjscie, int* obrazek, int n){
    for(int i = 0; i < n; i++){
        wyjscie.write((char*)&obrazek[i], sizeof(Uint8));
    }
}

void wczytajRLE(ifstream& wejscie, Uint16 dlugoscRLE, Uint16 wysokoscObrazka, Uint16 szerokoscObrazka){
    Uint8 wartosc;
    int * tab1;
    tab1 = new int[dlugoscRLE];
    int i;
    for(int i = 0; i < dlugoscRLE; i++){
        wejscie.read((char*)&wartosc, sizeof(Uint8));
        tab1[i] = wartosc;
    }

    int * tab2;
    tab2 = new int[wysokoscObrazka*szerokoscObrazka];
    dekompresjaRLE(tab1, dlugoscRLE, tab2);

    i = 0;
    for (int y=0; y<wysokoscObrazka; y++){
        for (int x=0; x<szerokoscObrazka; x++){
            setPixel(x+szerokoscObrazka,y,paletaGlowna[tab2[i++]]);
        }
    }
}

void wczytajUbite(ifstream& wejscie,int wysokoscObrazka, int szerokoscObrazka){
    int wczytInt=0;
    int wczytPrzes=8;
    int wartosc;
    wejscie.read((char*)&wartosc, sizeof(Uint8));

    for (int y=0; y<wysokoscObrazka; y++){
        for (int x=0; x<szerokoscObrazka; x++){
            wczytInt=0;
            wczytPrzes-=3;
            if (wczytPrzes>=0)
                wczytInt|=(wartosc>>wczytPrzes);
            else
                wczytInt|=(wartosc<<-wczytPrzes);
            if (wczytPrzes<=0){
                wejscie.read((char*)&wartosc, sizeof(Uint8));
                wczytPrzes+=8;
                wczytInt|=(wartosc>>wczytPrzes);
            }
            wczytInt&=7;
            setPixel(x+szerokoscObrazka,y,paletaGlowna[wczytInt]);
        }
    }
}

void zapiszObrazek(char metoda, bool dithering, string nazwaPliku){
    SDL_Color kolor,kolorZPalety;
    Uint16 szerokoscObrazka = szerokosc/2;
    Uint16 wysokoscObrazka = wysokosc/2;
    char identyfikator[] = "T10";
    int R,G,B,wartosc;

    cout<<"Zapisujemy plik '"<<nazwaPliku<<".dib' za pomoca metody write() uzywajac palety ";
    paletaGlowna == NULL;
    switch (metoda){
    case 'n':
        paletaGlowna=paletaNarzucona;
        cout<<"narzuconej.\n";
        break;
    case 's':
        paletaGlowna=paletaSzarosci;
        cout<<"szarosci\n";
        break;
    default:
        paletaGlowna=paletaKolorow;
        cout<<"domyslnej\n";
        break;
    }


    ofstream wyjscie((nazwaPliku+".dib"),ios::binary);

    wyjscie.write((char*)&metoda, sizeof(char)); //d-domyślne,s-szarości,n-narzucona
    wyjscie.write((char*)&identyfikator, sizeof(char)*3);
    wyjscie.write((char*)&szerokoscObrazka, sizeof(Uint16));
    wyjscie.write((char*)&wysokoscObrazka, sizeof(Uint16));
    if (metoda=='n'){
        obliczPalete();
        for (size_t i = 0; i < KOLORY; i++){
            wyjscie.write((char*)&paletaGlowna[i].r, sizeof(uint8_t));
            wyjscie.write((char*)&paletaGlowna[i].g, sizeof(uint8_t));
            wyjscie.write((char*)&paletaGlowna[i].b, sizeof(uint8_t));
        }
    }

    //Uint8 zapisInt=0;
    //int zapisPrzes=8;

    int bladR,bladG,bladB;
    float uncR,uncG,uncB;
    float bledyR[(szerokosc/2)+2][(wysokosc/2)+1],bledyG[(szerokosc/2)+2][(wysokosc/2)+1],bledyB[(szerokosc/2)+2][(wysokosc/2)+1];
    memset(bledyR,0,sizeof(bledyR));
    memset(bledyG,0,sizeof(bledyG));
    memset(bledyB,0,sizeof(bledyB));
    int przesuniecie=1; //aby nie wyjsc poniżej -1 tabeli błędów

    int obrazek [szerokosc*wysokosc];
    int iterator = 0;

    for (int y=0; y<wysokoscObrazka; y++){
        for (int x=0; x<szerokoscObrazka; x++){
            kolor=getPixel(x,y);
            if(metoda == 's'){
                int BW=0.299*kolor.r+0.587*kolor.g+0.114*kolor.b;
                kolor.r=BW;
                kolor.g=BW;
                kolor.b=BW;
            }
            wartosc=znajdzSomsiada2(kolor);
            kolorZPalety=paletaGlowna[wartosc];

            if (dithering){

                    //Wprowadzic normalizacje!!!!
                uncR=kolor.r+bledyR[x+przesuniecie][y];//update koloru na pod. błędu
                uncG=kolor.g+bledyG[x+przesuniecie][y];//update koloru na pod. błędu
                uncB=kolor.b+bledyB[x+przesuniecie][y];//update koloru na pod. błędu

                if(uncR < 0)
                    uncR = 0;
                if(uncR > 255)
                    uncR = 255;
                if(uncG < 0)
                    uncG = 0;
                if(uncG > 255)
                    uncG = 255;
                if(uncB < 0)
                    uncB = 0;
                if(uncB > 255)
                    uncB = 255;

                wartosc=znajdzSomsiada2(uncR,uncG,uncB);//znalezienie najbliszego koloru z palety
                kolorZPalety=paletaGlowna[wartosc];

                //bladR = kolor.r - kolorZPalety.r;//obliczenie bledu
                //bladG = kolor.g - kolorZPalety.g;
                //bladB = kolor.b - kolorZPalety.b;
                bladR = uncR - kolorZPalety.r;//obliczenie bledu
                bladG = uncG - kolorZPalety.g;
                bladB = uncB - kolorZPalety.b;

                bledyR[x+przesuniecie+1][y]+=(bladR*7.0/16.0);//update tablic bledow
                bledyR[x+przesuniecie-1][y+1]+=(bladR*3.0/16.0);
                bledyR[x+przesuniecie][y+1]+=(bladR*5.0/16.0);
                bledyR[x+przesuniecie+1][y+1]+=(bladR*1.0/16.0);

                bledyG[x+przesuniecie+1][y]+=(bladG*7.0/16.0);
                bledyG[x+przesuniecie-1][y+1]+=(bladG*3.0/16.0);
                bledyG[x+przesuniecie][y+1]+=(bladG*5.0/16.0);
                bledyG[x+przesuniecie+1][y+1]+=(bladG*1.0/16.0);

                bledyB[x+przesuniecie+1][y]+=(bladB*7.0/16.0);
                bledyB[x+przesuniecie-1][y+1]+=(bladB*3.0/16.0);
                bledyB[x+przesuniecie][y+1]+=(bladB*5.0/16.0);
                bledyB[x+przesuniecie+1][y+1]+=(bladB*1.0/16.0);

                //wartosc=dither(kolor,x,y,bledyR,bledyG,bledyB);
            }

            obrazek[iterator] = wartosc;
            setPixel(x + szerokosc / 2, y + wysokosc / 2, kolorZPalety);
            iterator++;

        }
    }

    int * tablicaRLE;
    tablicaRLE = new int[szerokosc*wysokosc];


    int dlugoscTabRLE = kompresjaRLE(obrazek, iterator, tablicaRLE);
    bool czyRLE = false;


    if(dlugoscTabRLE < ceil(3.0/8.0*float(iterator))){
        czyRLE = true;
        wyjscie.write((char*)&czyRLE, sizeof(Uint8));
        wyjscie.write((char*)&dlugoscTabRLE, sizeof(Uint16));
        cout << "Zapis przy uzyciu kompresji RLE" << endl;
        zapiszRLE(wyjscie, tablicaRLE, dlugoscTabRLE);
    }else{
        cout << "Zapis przy uzyciu upychania 3bit do 8 bit" << endl;
        wyjscie.write((char*)&czyRLE, sizeof(Uint8));
        zapiszUbite(wyjscie, obrazek, iterator);
    }

    cout <<"RLE: "<<dlugoscTabRLE << " b    Ubite: " << ceil(3.0/8.0*float(iterator)) <<" b\n";

    wyjscie.close();
    SDL_UpdateWindowSurface(window);
}

void wczytajObrazek(string nazwaPliku){
    SDL_Color kolor,kolorZPalety;
    Uint16 szerokoscObrazka;
    Uint16 wysokoscObrazka;
    char identyfikator[] = "   ";
    char metoda='0';
    int wartosc;

    cout<<"\n\nOdczytujemy plik '"<<nazwaPliku<<".dib' za pomoca metody write()"<<endl;
    ifstream wejscie(nazwaPliku+".dib",ios::binary);

    wejscie.read((char*)&metoda, sizeof(char)); //d-domyślne,s-szarości,n-narzucona
    wejscie.read((char*)&identyfikator, sizeof(char)*3);
    wejscie.read((char*)&szerokoscObrazka, sizeof(Uint16));
    wejscie.read((char*)&wysokoscObrazka, sizeof(Uint16));
    cout<<"Paleta: ";
    switch (metoda){
    case 'n':
        paletaGlowna=paletaNarzucona;
        cout<<"narzucona\n";
        break;
    case 's':
        paletaGlowna=paletaSzarosci;
        cout<<"szarosci\n";
        break;
    default:
        paletaGlowna=paletaKolorow;
        cout<<"domyslna\n";
        break;
    }
    cout<<"Sygnatura: "<<identyfikator<<endl;
    cout<<"Wysokosc obrazka: "<<wysokoscObrazka<<endl;
    cout<<"Szerokosc obrazka: "<<szerokoscObrazka<<endl;
    if (metoda=='n'){
        for (size_t i = 0; i < KOLORY; i++){
            wejscie.read((char*)&paletaGlowna[i].r, sizeof(uint8_t));
            wejscie.read((char*)&paletaGlowna[i].g, sizeof(uint8_t));
            wejscie.read((char*)&paletaGlowna[i].b, sizeof(uint8_t));
        }
    }

    bool czyRLE;
    uint16_t dlugoscRLE;
    wejscie.read((char*)&czyRLE, sizeof(Uint8));
    if(czyRLE){
        cout<<"Kompresja: RLE"<<endl;
        wejscie.read((char*)&dlugoscRLE, sizeof(Uint16));
        wczytajRLE(wejscie,dlugoscRLE, wysokoscObrazka,szerokoscObrazka);
    }else{
        cout<<"Kompresja: Upychanie 3bit do 8 bit"<<endl;
        wczytajUbite(wejscie,wysokoscObrazka,szerokoscObrazka);
    }

    wejscie.close();
    SDL_UpdateWindowSurface(window);
}


int dodajKolor(SDL_Color kolor) {
	int aktualnyKolor = ileKolorow;
	paleta[aktualnyKolor] = kolor;
	ileKolorow++;
	return (aktualnyKolor);
}

bool porownajKolory(SDL_Color k1, SDL_Color k2) {
	bool wynik = true;
	if (k1.r != k2.r) wynik = false;
	if (k1.g != k2.g) wynik = false;
	if (k1.b != k2.b) wynik = false;
	return wynik;
}

int sprawdzKolor(SDL_Color kolor) {
	int wynik = -1;
	if (ileKolorow>0) {
		for (int k=0; k<ileKolorow; k++) {
			if (porownajKolory(kolor, paleta[k])) {
				wynik = k;
				//licznik[k] += 1;
			}
		}
	}
	if (wynik<0) {
		wynik = dodajKolor(kolor);
	}
	return wynik;
}

void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
  if ((x>=0) && (x<szerokosc) && (y>=0) && (y<wysokosc))
  {
    /* Zamieniamy poszczególne składowe koloru na format koloru piksela */
    Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

    /* Pobieramy informację ile bajtów zajmuje jeden piksel */
    int bpp = screen->format->BytesPerPixel;

    /* Obliczamy adres piksela */
    Uint8 *p1 = (Uint8 *)screen->pixels + (y*2) * screen->pitch + (x*2) * bpp;
    Uint8 *p2 = (Uint8 *)screen->pixels + (y*2+1) * screen->pitch + (x*2) * bpp;
    Uint8 *p3 = (Uint8 *)screen->pixels + (y*2) * screen->pitch + (x*2+1) * bpp;
    Uint8 *p4 = (Uint8 *)screen->pixels + (y*2+1) * screen->pitch + (x*2+1) * bpp;

    /* Ustawiamy wartość piksela, w zależnoœci od formatu powierzchni*/
    switch(bpp)
    {
        case 1: //8-bit
            *p1 = pixel;
            *p2 = pixel;
            *p3 = pixel;
            *p4 = pixel;
            break;

        case 2: //16-bit
            *(Uint16 *)p1 = pixel;
            *(Uint16 *)p2 = pixel;
            *(Uint16 *)p3 = pixel;
            *(Uint16 *)p4 = pixel;
            break;

        case 3: //24-bit
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p1[0] = (pixel >> 16) & 0xff;
                p1[1] = (pixel >> 8) & 0xff;
                p1[2] = pixel & 0xff;
                p2[0] = (pixel >> 16) & 0xff;
                p2[1] = (pixel >> 8) & 0xff;
                p2[2] = pixel & 0xff;
                p3[0] = (pixel >> 16) & 0xff;
                p3[1] = (pixel >> 8) & 0xff;
                p3[2] = pixel & 0xff;
                p4[0] = (pixel >> 16) & 0xff;
                p4[1] = (pixel >> 8) & 0xff;
                p4[2] = pixel & 0xff;
            } else {
                p1[0] = pixel & 0xff;
                p1[1] = (pixel >> 8) & 0xff;
                p1[2] = (pixel >> 16) & 0xff;
                p2[0] = pixel & 0xff;
                p2[1] = (pixel >> 8) & 0xff;
                p2[2] = (pixel >> 16) & 0xff;
                p3[0] = pixel & 0xff;
                p3[1] = (pixel >> 8) & 0xff;
                p3[2] = (pixel >> 16) & 0xff;
                p4[0] = pixel & 0xff;
                p4[1] = (pixel >> 8) & 0xff;
                p4[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4: //32-bit
            *(Uint32 *)p1 = pixel;
            *(Uint32 *)p2 = pixel;
            *(Uint32 *)p3 = pixel;
            *(Uint32 *)p4 = pixel;
            break;

        }
    }
}

void setPixelSurface(int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
  if ((x>=0) && (x<szerokosc*2) && (y>=0) && (y<wysokosc*2))
  {
    /* Zamieniamy poszczególne składowe koloru na format koloru piksela */
    Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

    /* Pobieramy informację ile bajtów zajmuje jeden piksel */
    int bpp = screen->format->BytesPerPixel;

    /* Obliczamy adres piksela */
    Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

    /* Ustawiamy wartość piksela, w zależności od formatu powierzchni*/
    switch(bpp)
    {
        case 1: //8-bit
            *p = pixel;
            break;

        case 2: //16-bit
            *(Uint16 *)p = pixel;
            break;

        case 3: //24-bit
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4: //32-bit
            *(Uint32 *)p = pixel;
            break;
        }
    }
}

SDL_Color getPixel(int x, int y) {
    SDL_Color color ;
    Uint32 col = 0 ;
    if ((x>=0) && (x<szerokosc) && (y>=0) && (y<wysokosc)) {
        //określamy pozycję
        char* pPosition=(char*)screen->pixels ;

        //przesunięcie względem y
        pPosition+=(screen->pitch*y*2) ;

        //przesunięcie względem x
        pPosition+=(screen->format->BytesPerPixel*x*2);

        //kopiujemy dane piksela
        memcpy(&col, pPosition, screen->format->BytesPerPixel);

        //konwertujemy kolor
        SDL_GetRGB(col, screen->format, &color.r, &color.g, &color.b);
    }
    return ( color ) ;
}

SDL_Color getPixelSurface(int x, int y, SDL_Surface *surface) {
    SDL_Color color ;
    Uint32 col = 0 ;
    if ((x>=0) && (x<szerokosc) && (y>=0) && (y<wysokosc)) {
        //określamy pozycję
        char* pPosition=(char*)surface->pixels ;

        //przesunięcie względem y
        pPosition+=(surface->pitch*y) ;

        //przesunięcie względem x
        pPosition+=(surface->format->BytesPerPixel*x);

        //kopiujemy dane piksela
        memcpy(&col, pPosition, surface->format->BytesPerPixel);

        //konwertujemy kolor
        SDL_GetRGB(col, surface->format, &color.r, &color.g, &color.b);
    }
    return ( color ) ;
}


void ladujBMP(char const* nazwa, int x, int y)
{
    SDL_Surface* bmp = SDL_LoadBMP(nazwa);
    if (!bmp)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
    }
    else
    {
        cout<<"Zaladowano plik '"<<nazwa<<"'\n\n";
        SDL_Color kolor;
        for (int yy=0; yy<bmp->h; yy++) {
			for (int xx=0; xx<bmp->w; xx++) {
				kolor = getPixelSurface(xx, yy, bmp);
				setPixel(xx, yy, kolor.r, kolor.g, kolor.b);
			}
        }
		SDL_FreeSurface(bmp);
        SDL_UpdateWindowSurface(window);
    }

}

void zapiszBMP(char const* plik){
    SDL_Surface* bmp = SDL_CreateRGBSurface(SDL_SWSURFACE,szerokosc/2,wysokosc/2,24,0x00ff0000,0x0000ff00, 0x000000ff, 0x00000000);
    const SDL_Rect obszar{szerokosc,0,szerokosc,wysokosc};
    SDL_BlitScaled(screen,&obszar,bmp,NULL);
    SDL_SaveBMP(bmp,plik);
}


void czyscEkran(Uint8 R, Uint8 G, Uint8 B)
{
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, R, G, B));
    SDL_UpdateWindowSurface(window);
}

/*
Początek nowego kodu
*/

void setPixel(int x, int y, SDL_Color kolor){
    Uint8 R = kolor.r; Uint8 G = kolor.g; Uint8 B = kolor.b;
    return setPixel(x,y,R,G,B);
}

/*
Koniec nowego kodu
*/


int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
    }

    window = SDL_CreateWindow(tytul, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szerokosc*2, wysokosc*2, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    screen = SDL_GetWindowSurface(window);
    if (screen == NULL) {
        fprintf(stderr, "SDL_GetWindowSurface Error: %s\n", SDL_GetError());
    return false;
    }
    SDL_UpdateWindowSurface(window);


    bool done = false;
    SDL_Event event;
    // główna pętla programu


    bool ditherIMG=0;
    char paletteIMG='d';
    string plikIMG="ImageGKIM";
    bool conDir=0;
    wypiszInfo(ditherIMG,paletteIMG,plikIMG,conDir);

    while (SDL_WaitEvent(&event)) {
        // sprawdzamy czy pojawiło się zdarzenie
        switch (event.type) {
            case SDL_QUIT:
                done = true;
                break;
            // sprawdzamy czy został wciśnięty klawisz
            case SDL_KEYDOWN: {
                // wychodzimy, gdy wciśnięto ESC
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    done = true;
                if (event.key.keysym.sym == SDLK_1){
                    //Funkcja1();
                    ditherIMG=!ditherIMG;
                    system("CLS");
                    wypiszInfo(ditherIMG,paletteIMG,plikIMG,conDir);
                }
                if (event.key.keysym.sym == SDLK_2){
                    //Funkcja2();
                    switch(paletteIMG){
                        case 's': paletteIMG='n'; break;
                        case 'n': paletteIMG='d'; break;
                        default : paletteIMG='s'; break;
                    }
                    system("CLS");
                    wypiszInfo(ditherIMG,paletteIMG,plikIMG,conDir);
                }
                if (event.key.keysym.sym == SDLK_3){
                    //Funkcja3();
                    cout<<"Wprowadz nazwe pliku: ";
                    do{
                        cin>>plikIMG;
                    }while(plikIMG=="");
                    system("CLS");
                    wypiszInfo(ditherIMG,paletteIMG,plikIMG,conDir);
                }
                if (event.key.keysym.sym == SDLK_6){
                    //Funkcja4();
                    conDir=!conDir;
                    system("CLS");
                    wypiszInfo(ditherIMG,paletteIMG,plikIMG,conDir);
                }
                if (event.key.keysym.sym == SDLK_4)
                    zapiszObrazek(paletteIMG,ditherIMG,plikIMG);
                    //Funkcja5();
                if (event.key.keysym.sym == SDLK_5)
                    //Funkcja6();
                    wczytajObrazek(plikIMG);
                if (event.key.keysym.sym == SDLK_7){
                    system("CLS");
                    wypiszInfo(ditherIMG,paletteIMG,plikIMG,conDir);

                    string temp=plikIMG+".bmp";
                    if(conDir){
                        ladujBMP(temp.c_str(),0,0);
                        zapiszObrazek(paletteIMG,ditherIMG,plikIMG);
                    }else{
                        wczytajObrazek(plikIMG);
                        zapiszBMP(temp.c_str());
                    }
                }
                    Funkcja7();
                if (event.key.keysym.sym == SDLK_8)
                    Funkcja8();
                if (event.key.keysym.sym == SDLK_9)
                    Funkcja9();
                if (event.key.keysym.sym == SDLK_a)
                    ladujBMP("obrazek1.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_s)
                    ladujBMP("obrazek2.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_d)
                    ladujBMP("obrazek3.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_f)
                    ladujBMP("obrazek4.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_g)
                    ladujBMP("obrazek5.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_h)
                    ladujBMP("obrazek6.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_j)
                    ladujBMP("obrazek7.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_k)
                    ladujBMP("obrazek8.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_b)
                    czyscEkran(0, 0, 10);

                else
                    break;
               }

        }
        if (done) break;
    }

    if (screen) {
        SDL_FreeSurface(screen);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }


    SDL_Quit();
    return 0;
}
