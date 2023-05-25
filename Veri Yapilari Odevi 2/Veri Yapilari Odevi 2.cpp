#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//Dugum yapisi
struct Dugum {
    int plaka;
    Dugum* sol;
    Dugum* sag;

    Dugum(int plaka) {
        this->plaka = plaka;
        sol = sag = NULL;
    }
};

struct Agac {
    Dugum* kok;
    Agac() {
        kok = NULL;
    }
};

//AgacDerinligi Olcme
int AgacDerinligi(Dugum* kok) {
    if (kok == NULL) {
        return 0;
    }
    return max(AgacDerinligi(kok->sol), AgacDerinligi(kok->sag)) + 1;
}

//Binary Tree'yi yazdirmak icin gerekli sutunlari bulma
int sutunBul(int h) {
    if (h == 1)
        return 1;
    return sutunBul(h - 1) + sutunBul(h - 1) + 1;
}

//Agac matrisi olusturarak binary tree yazdirmayi saglama
void AgacMatrisiOlustur(int** M, Dugum* kok, int sutun, int satir, int yukseklik) {
    if (kok == NULL)
        return;
    M[satir][sutun] = kok->plaka;
    AgacMatrisiOlustur(M, kok->sol, sutun - pow(2, yukseklik - 2), satir + 1, yukseklik - 1);
    AgacMatrisiOlustur(M, kok->sag, sutun + pow(2, yukseklik - 2), satir + 1, yukseklik - 1);
}

//Binary Tree'yi yazdirma
void AgacYazdir(Dugum* kok) {
    int derinlik = AgacDerinligi(kok);
    int sutun = sutunBul(derinlik);
    int** M = new int* [derinlik];
    for (int i = 0; i < derinlik; i++) {
        M[i] = new int[sutun];
    }
    AgacMatrisiOlustur(M, kok, sutun / 2, 0, derinlik);
    for (int i = 0; i < derinlik; i++) {
        for (int j = 0; j < sutun; j++) {
            if (M[i][j] == 0)
                cout << " " << " ";
            else if (M[i][j] == -842150451) // Bos dugumleri yazdirmayi engelle
                cout << " " << " "; // Bos dugumleri bosluk olarak yazdir
            else
                cout << M[i][j] << " ";
        }
        cout << endl;
    }
}

//Agacin dengeli olup olmadigini olcer
bool DengeliMi(Dugum* dugum) {
    if (dugum == NULL) {
        return true;
    }

    int solYukseklik = AgacDerinligi(dugum->sol);
    int sagYukseklik = AgacDerinligi(dugum->sag);

    if (abs(solYukseklik - sagYukseklik) <= 1 && DengeliMi(dugum->sol) && DengeliMi(dugum->sag)) {
        return true;
    }

    return false;
}

//Plakalar arasindaki agac duzey farki hesaplama
int plakaDuzeyFarki(Dugum* kok, int plaka1, int plaka2) {
    if (kok == NULL) {
        return -1; // Agac bossa -1 dondurur
    }
    Dugum* ortakAta = kok; // Ortak ata dugumunu tutan degisken
    while (ortakAta != NULL) {
        if (plaka1 < ortakAta->plaka && plaka2 < ortakAta->plaka) { // Iki plaka da ortak atanin solunda ise
            ortakAta = ortakAta->sol; 
        }
        else if (plaka1 > ortakAta->plaka && plaka2 > ortakAta->plaka) { // Iki plaka da ortak atanin sagindaysa
            ortakAta = ortakAta->sag; 
        }
        else { // Iki plakanin ortak atasi bulunduysa
            break; 
        }
    }
    if (ortakAta == NULL) {
        return -1; // Iki plakanin ortak atasi yoksa -1 dondur
    }
    int yukseklik1 = 0; int yukseklik2 = 0; 

    Dugum* gecici = ortakAta;
    while (gecici != NULL && gecici->plaka != plaka1) {
        if (plaka1 < gecici->plaka) { 
            gecici = gecici->sol;
        }

        else { 
            gecici = gecici->sag; 
        }
        yukseklik1++;
    }
    if (gecici == NULL) {
        return -1; 
    }
    gecici = ortakAta;
    while (gecici != NULL && gecici->plaka != plaka2) {
        if (plaka2 < gecici->plaka) {
            gecici = gecici->sol;
        }
        else { 
            gecici = gecici->sag; 
        }
        yukseklik2++; 
    }
    if (gecici == NULL) {
        return -1; 
    }
    return abs(yukseklik1 - yukseklik2); // Iki yukseklik arasindaki mutlak degeri dondur
}

//Agaca dugum ekleme
Dugum* ekle(Dugum* dugum, int plaka) {
    if (dugum == NULL) {
        dugum = new Dugum(plaka);
        dugum->plaka = plaka;
        dugum->sol = NULL;
        dugum->sag = NULL;
    }
    else if (plaka < dugum->plaka) {
        dugum->sol = ekle(dugum->sol, plaka);
    }
    else {
        dugum->sag = ekle(dugum->sag, plaka);
    }
    return dugum;
}

//Inorder siralamasi
void inorder(Dugum* dugum) {
    if (dugum != NULL) {
        inorder(dugum->sol);
        cout << dugum->plaka << "-";
        inorder(dugum->sag);
    }
}

//Preorder siralamasi
void preorder(Dugum* dugum) {
    if (dugum != NULL) {
        cout << dugum->plaka << "-";
        preorder(dugum->sol);
        preorder(dugum->sag);
    }
}

//Postorder siralamasi
void postorder(Dugum* dugum) {
    if (dugum != NULL) {
        postorder(dugum->sol);
        postorder(dugum->sag);
        cout << dugum->plaka << "-";
    }
}

//Girdisi yapilan plakalarla eslesen sehirleri yazdirma
void SehirYazdir(string (*sehirBilgileri)[18], int *plaka1, int *plaka2) {
    int eslesenSehir=0;
    for (int j = 0; j < 18; j++) {
        if (to_string(*plaka1) == sehirBilgileri[0][j] || to_string(*plaka2) == sehirBilgileri[0][j] ) {
            eslesenSehir++;
        }
    }
    if (eslesenSehir < 1) {
        cout << "Girdiginiz plakayla eslesen sehir bulunamadi." << endl;    //Eslesen sehir bulunmazsa yazdirilan uyari mesaji
        return;
    }
    cout << "Sehirler : ";
    for (int j = 0; j < 18; j++) {
        if (to_string(*plaka1) == sehirBilgileri[0][j]) {
            cout << sehirBilgileri[1][j] << " ";
        }
        else if (to_string(*plaka2) == sehirBilgileri[0][j]) {
            cout << sehirBilgileri[1][j] << " ";
        }
    }
}

int main() {
    Dugum* kok = NULL;
    string sehirBilgileri[2][18];
    int i = 0;
    int plaka1, plaka2;

    // 1.1) Dosya okuma
    ifstream plakaDosyasi("C:/Users/daggu/Desktop/sehir_plaka_kodlari.txt");

    // 1.1) Dosyadan iki boyutlu diziye aktarma
    if (plakaDosyasi.is_open()) {
        string satir;
        while (getline(plakaDosyasi, satir)) {
            size_t virgulKonum = satir.find(",");
            sehirBilgileri[0][i] = satir.substr(0, virgulKonum);
            sehirBilgileri[1][i] = satir.substr(virgulKonum + 1);
            i++;
        }
        plakaDosyasi.close();
    }
    //Uyari mesaji
    else {
        cout << "Dosya acilamadi";
    }

    // 1.2) Binary tree olusturma
    for (int j = 0; j < i; j++) {
        kok = ekle(kok, stoi(sehirBilgileri[0][j]));
    }

    // 1.3) AgacDerinligi ciktisi
    int derinlik = AgacDerinligi(kok);
    cout << "Agacin derinligi: " << derinlik <<", ";

    // 1.3) Dengeli mi ciktisi
    bool dengeli = DengeliMi(kok);
    if (dengeli) {
        cout << "Dengeli";
    }
    else {
        cout << "Dengeli degil";
    }

    // 1.3) Binary tree'nin goruntusunu ekrana yazdirma
    cout << endl << "Binary Tree:" << endl;
    AgacYazdir(kok);

    // 1.4) Plaka girdisi
    cout << endl <<"1. ve 2. sehir icin plaka kodu giriniz: ";
    cin >> plaka1; cin>> plaka2;

    // 1.4) Plakaya denk gelen sehirleri yazdirma
    SehirYazdir(sehirBilgileri, &plaka1, &plaka2);

    // 1.4) Plakalar arasi agac duzey farki yazdirma
    cout << endl <<"Agactaki duzey farki : " << plakaDuzeyFarki(kok, plaka1, plaka2);

    // 1.5) Preorder siralamayi ekrana yazdirma
    cout << endl << "Preorder: ";
    preorder(kok);

    // 1.5) Inorder siralamayi ekrana yazdirma
    cout << endl << "Inorder: ";
    inorder(kok);

    // 1.5) Postorder siralamayi ekrana yazdirma
    cout << endl << "Postorder: ";
    postorder(kok);

    return 0;
}
