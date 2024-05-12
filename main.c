#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef _WIN32
    #include <windows.h>
#endif

#include "debugmalloc.h"
#include "adatbazis.h"
#include "fajlkezeles.h"
#include "io.h"

void menu(Nevjegyzek* adatbazis);
void hozzaadas(Nevjegyzek* adatbazis);
void kereses(Nevjegyzek* adatbazis);
void importalas(Nevjegyzek* adatbazis);

int main(void) {
#ifdef _WIN32
    SetConsoleCP(1250);
    SetConsoleOutputCP(1250);
#endif

    Nevjegyzek adatbazis;
    bool sikeres_e = adatb_letrehoz(&adatbazis);
    if(!sikeres_e) {
        adatb_felszabadit(&adatbazis);
        exit(EXIT_FAILURE);
    }
    varakozas_bemenetre();
    menu(&adatbazis);
    adatb_felszabadit(&adatbazis);

    return 0;
}

void menu(Nevjegyzek* adatbazis){
    int valasztas = 0;

    do {
        // men� ki�r�sa a k�perny�re
        fomenu_kiiras(&valasztas);
        // megfelel� funkci� megh�v�sa
        switch(valasztas){
            case 1: hozzaadas(adatbazis); break;
            case 2: kereses(adatbazis); break;
            case 3: importalas(adatbazis); break;
            case 4: break; // kil�p�s
            default: break; // hib�s inputok itt kezel�dnek
        }
    }
    while(valasztas != 4);
}

// vez�rl�s a n�vjegy hozz�ad�s funkci�hoz
void hozzaadas(Nevjegyzek* adatbazis){
    Nevjegy ujnevjegy = {}; // �res stringre inicializ�l�s, az adatbeolvas f�ggv�nyhez
    adatbevitel(&ujnevjegy);
    if(mentie()) nevjegy_mentes(&ujnevjegy, adatbazis);
}

// vez�rl�s a keres�s funkci�hoz (f�lk�sz)
void kereses(Nevjegyzek* adatbazis){
    int kategoria_kod;
    kategoria_valasztas_menu(&kategoria_kod);
    if(kategoria_kod==6) return;
    if(!(kategoria_kod == 1 ||
         kategoria_kod == 2 ||
         kategoria_kod == 3 ||
         kategoria_kod == 4 ||
         kategoria_kod == 5)){
        printf("Hib�s kateg�ria k�d");
        varakozas_bemenetre();
        return;
    }
    char keresett_string[30+1]; // leghosszabb adatmez� hossza
    kereses_mezo(keresett_string);
    nevek_listaz(adatbazis, kategoria_kod, keresett_string);
    printf("V�lasztott n�vjegy (vagy visszal�p�s): ");
    getchar(); // (enter beolvas�sa)
    int valasztott_id;
    if(scanf(" %d", &valasztott_id)!=1) return;
    else{
        Nevjegy* nevjegyptr = nevjegy_keres(adatbazis, valasztott_id); // adatbazisbeli pointer
        if(nevjegyptr == NULL){
            printf("Nincs ilyen n�vjegy\n");
            return;
        }
        printf("\nKi�r (k) / m�dos�t (m) / t�r�l (t) / export�l (e) / vissza (v)? ");
        getchar(); // (enter beolvas�sa)
        char valasztas = getchar();
        getchar(); // (enter beolvas�sa)
        switch(valasztas){
            case 'k':{
                nevjegy_kiir(nevjegyptr);
                printf("\nVissza (v) ? ");
                if(getchar()=='v');
                break;
            }

            case 'm':{
                Nevjegy nevjegy;
                nevjegy = *nevjegyptr;
                adatbevitel(&nevjegy);
                if(mentie()){
                    if(nevjegy_modosit(adatbazis, &nevjegy, nevjegyptr)) printf("Sikeresen m�dos�tva");
                    else printf("Nem siker�lt m�dos�tani az adatb�zis f�jlban");
                }
                varakozas_bemenetre();
                break;
            }

            case 't':{
                printf("Biztosan t�rli? igen (i) / m�gse (m) ");
                if(getchar() == 'i'){
                    if(nevjegy_torles(adatbazis, nevjegyptr)) printf("T�r�lve");
                    else printf("Nem siker�lt t�r�lni az adatb�zis f�jlb�l");
                }
                varakozas_bemenetre();
                break;
            }

            case 'e':{
                if(vcard_export(nevjegyptr)==true) printf("Sikeres export�l�s");
                else printf("Nem siker�lt export�lni");
                varakozas_bemenetre();
                break;
            }

            case 'v': return;
            default: break; // hib�s inputok itt kezel�dnek
        }
    }
}

void importalas(Nevjegyzek* adatbazis){
    printf("F�jln�v: ");
    char fajlnev[50+4+1]; // .vcf 4 karakter
    scanf(" %[^\n]", fajlnev);
    getchar(); // (enter beolvas�sa)
    if(!vcard_import(fajlnev, adatbazis)) printf("Nem siker�lt beolvasni a f�jl");
    else printf("Sikeres import�l�s");
    varakozas_bemenetre();
}
