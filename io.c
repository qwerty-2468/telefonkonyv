#ifdef _WIN32
    #include <windows.h>
#endif
#include <stdio.h>
#include <string.h>
#include "adatbazis.h"
#include "io.h"


typedef enum Kategoria{
    nev = 1,
    foglalkozas = 2,
    cim = 3,
    telefonszam = 4,
    email = 5
} Kategoria;

void fomenu_kiiras(int* valasztas){
    system("cls");
    printf("N�VJEGYZ�K\n\n"
            "(1) �j n�vjegy hozz�ad�sa\n"
            "(2) Keres�s\n"
            "(3) Import�l�s\n"
            "(4) Kil�p�s\n"
            "\nKiv�lasztott men�pont: ");
    // men�pont kiv�laszt�sa
    scanf(" %d", valasztas);
    getchar(); // (enter beolvas�sa)
}

// megl�v� n�vjegy eset�n ki�rja a r�gi adatokat
void adatbevitel(Nevjegy* nevjegy){
    system("cls");
    printf("N�v: ");
    printf("%s ", nevjegy->nev);
    gets(nevjegy->nev);
    printf("\nFoglalkoz�s: ");
    printf("%s ", nevjegy->foglalkozas);
    gets(nevjegy->foglalkozas);
    printf("\nC�m: ");
    printf("%s ", nevjegy->cim);
    gets(nevjegy->cim);
    printf("\nTelefonsz�m: ");
    printf("%s ", nevjegy->telefonszam);
    gets(nevjegy->telefonszam);
    printf("\nEmail: ");
    printf("%s ", nevjegy->email);
    gets(nevjegy->email);
    adat_fixalas(nevjegy);
    nevjegy_kiir(nevjegy);
}

// megk�rdezi, hogy szeretn�-e menteni �s ezt visszaadja
bool mentie(){
    char input;
    do{
    printf("\nMenti a n�vjegyet? ( igen(i) / nem(n) ) ");
    input = getchar();
    } while (input != 'i' && input != 'n');
    if(input=='i') return true;
    else return false;
}

// egy n�vjegy adatmez�inek ki�r�sa
void nevjegy_kiir(Nevjegy const *const nevjegy){
    printf("\nN�v: %s\n"
           "Foglalkoz�s: %s\n"
           "C�m: %s\n"
           "Telefonsz�m: %s\n"
           "Email c�m: %s\n",
           nevjegy->nev,
           nevjegy->foglalkozas,
           nevjegy->cim,
           nevjegy->telefonszam,
           nevjegy->email);
}

// a v�lasztott kateg�ria alapj�n keres �s kilist�zza a megfelel� neveket
void nevek_listaz(Nevjegyzek const *const adatbazis, const int kategoria, const char* keresett_string){
    system("cls");
    printf("Az adatb�zisban szerepl� nevek:\n");
    Nevjegy* mozgo = adatbazis->elso->kov;
    while (mozgo != adatbazis->utolso) {
        switch(kategoria){
            case nev:{
                    if(wildcard_keres(keresett_string, mozgo->nev))
                        printf("(%d) %s\n", mozgo->id, mozgo->nev);
                    break;
            }

            case foglalkozas:
                    if(strcmp(mozgo->foglalkozas, keresett_string)==0)
                        printf("(%d) %s\n", mozgo->id, mozgo->nev);
                    break;

            case cim:
                    if(strcmp(mozgo->cim, keresett_string)==0)
                        printf("(%d) %s\n", mozgo->id, mozgo->nev);
                    break;

            case telefonszam:
                    if(strcmp(mozgo->telefonszam, keresett_string)==0)
                        printf("(%d) %s\n", mozgo->id, mozgo->nev);
                    break;

            case email:
                    if(strcmp(mozgo->email, keresett_string)==0)
                        printf("(%d) %s\n", mozgo->id, mozgo->nev);
                    break;
            default: break;

        }
        mozgo = mozgo->kov;
    }
}

void kategoria_valasztas_menu(int* kategoria_kod){
    system("cls");
    printf("Milyen adatra szeretne keresni?\n\n"
           "(1) N�v\n"
           "(2) Foglalkoz�s\n"
           "(3) C�m\n"
           "(4) Telefonsz�m\n"
           "(5) Email c�m\n"
           "(6) Vissza a f�men�be\n\n"
           "Kateg�ria: ");
    scanf(" %d", kategoria_kod);
}

void kereses_mezo(char* keresett_string){
    system("cls");
    printf("Keresett kifejez�s: ");
    scanf(" %[^\n]", keresett_string);
}

void varakozas_bemenetre(void){
    printf("\nEnter a tov�bbl�p�shez...");
    while(getchar() != '\n')
    ;
}
