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
        // menü kiírása a képernyõre
        fomenu_kiiras(&valasztas);
        // megfelelõ funkció meghívása
        switch(valasztas){
            case 1: hozzaadas(adatbazis); break;
            case 2: kereses(adatbazis); break;
            case 3: importalas(adatbazis); break;
            case 4: break; // kilépés
            default: break; // hibás inputok itt kezelõdnek
        }
    }
    while(valasztas != 4);
}

// vezérlés a névjegy hozzáadás funkcióhoz
void hozzaadas(Nevjegyzek* adatbazis){
    Nevjegy ujnevjegy = {}; // üres stringre inicializálás, az adatbeolvas függvényhez
    adatbevitel(&ujnevjegy);
    if(mentie()) nevjegy_mentes(&ujnevjegy, adatbazis);
}

// vezérlés a keresés funkcióhoz (félkész)
void kereses(Nevjegyzek* adatbazis){
    int kategoria_kod;
    kategoria_valasztas_menu(&kategoria_kod);
    if(kategoria_kod==6) return;
    if(!(kategoria_kod == 1 ||
         kategoria_kod == 2 ||
         kategoria_kod == 3 ||
         kategoria_kod == 4 ||
         kategoria_kod == 5)){
        printf("Hibás kategória kód");
        varakozas_bemenetre();
        return;
    }
    char keresett_string[30+1]; // leghosszabb adatmezõ hossza
    kereses_mezo(keresett_string);
    nevek_listaz(adatbazis, kategoria_kod, keresett_string);
    printf("Választott névjegy (vagy visszalépés): ");
    getchar(); // (enter beolvasása)
    int valasztott_id;
    if(scanf(" %d", &valasztott_id)!=1) return;
    else{
        Nevjegy* nevjegyptr = nevjegy_keres(adatbazis, valasztott_id); // adatbazisbeli pointer
        if(nevjegyptr == NULL){
            printf("Nincs ilyen névjegy\n");
            return;
        }
        printf("\nKiír (k) / módosít (m) / töröl (t) / exportál (e) / vissza (v)? ");
        getchar(); // (enter beolvasása)
        char valasztas = getchar();
        getchar(); // (enter beolvasása)
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
                    if(nevjegy_modosit(adatbazis, &nevjegy, nevjegyptr)) printf("Sikeresen módosítva");
                    else printf("Nem sikerült módosítani az adatbázis fájlban");
                }
                varakozas_bemenetre();
                break;
            }

            case 't':{
                printf("Biztosan törli? igen (i) / mégse (m) ");
                if(getchar() == 'i'){
                    if(nevjegy_torles(adatbazis, nevjegyptr)) printf("Törölve");
                    else printf("Nem sikerült törölni az adatbázis fájlból");
                }
                varakozas_bemenetre();
                break;
            }

            case 'e':{
                if(vcard_export(nevjegyptr)==true) printf("Sikeres exportálás");
                else printf("Nem sikerült exportálni");
                varakozas_bemenetre();
                break;
            }

            case 'v': return;
            default: break; // hibás inputok itt kezelõdnek
        }
    }
}

void importalas(Nevjegyzek* adatbazis){
    printf("Fájlnév: ");
    char fajlnev[50+4+1]; // .vcf 4 karakter
    scanf(" %[^\n]", fajlnev);
    getchar(); // (enter beolvasása)
    if(!vcard_import(fajlnev, adatbazis)) printf("Nem sikerült beolvasni a fájl");
    else printf("Sikeres importálás");
    varakozas_bemenetre();
}
