#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include "adatbazis.h"
#include "fajlkezeles.h"
#include "io.h"

//#define TESZT

// visszaadja, hogy létezik-e már adatabázis.txt fájl
bool van_adatb_fajl(void){
    FILE* fp = fopen("adatbazis.txt", "r");
    if(fp != NULL){
        fclose(fp);
        return true;
    }
    return false;
}

// beolvassa az adatbázis fájl tartalmát a paraméterül kapott adatbázisba
bool adatb_beolvas(Nevjegyzek* adatbazis){
    FILE* fp = fopen("adatbazis.txt", "r");
    if(fp != NULL){
        printf("Betöltés...\n");
        //fejléc átugrása
        char tmp[45+1] ="#"; //ilyen hosszú a leghosszabb sor az adatbázis fájl fejlécében
        while(tmp[0] == '#' && fgets(tmp, sizeof(tmp), fp)!=NULL){
            ;
        }

        // beolvasás soronként, majd azokból az adatok eltárolása az ideiglenes nevjegy memóriába
        char sor[MAX_SOR_HOSSZ];
        while (fgets(sor, sizeof(sor), fp) != NULL){
            Nevjegy ideiglenes;
            sscanf(sor, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%s", // | jellel vannak elválasztva az adatok a fájlban
                    &ideiglenes.id,
                    ideiglenes.nev,
                    ideiglenes.foglalkozas,
                    ideiglenes.cim,
                    ideiglenes.telefonszam,
                    ideiglenes.email);

            // csak teszteléskor fut le
            #ifdef TESZT
                nevjegy_kiir(&ideiglenes);
                varakozas_bemenetre();
            #endif

            memoriaba_iras(&ideiglenes, adatbazis);

        }
        fclose(fp);
        return true;
    }
    return false;
}

// létrehoz egy adatbázis fájlt a program mappájában
bool fajl_inicializal_adatb(){
    FILE* fp = fopen("adatbazis.txt", "w");
    if(fp != NULL){
        fprintf(fp, "#Névjegyzék adatbázis fájl   v1\n"
                    "#\n"
                    "#ID|Név|Foglalkozás|Cím|Telefonszám|Email\n");
        fclose(fp);
        return true;
    }
    return false;
}

// kiírja az adatbázis fájlba a paraméterként kapott névjegyet
bool fajlba_nevjegy_mentes(Nevjegy const* const nevjegy){
    FILE* fp = fopen("adatbazis.txt", "a");
    if(fp != NULL){
        fprintf(fp, "\n%d|%s|%s|%s|%s|%s",
                    nevjegy->id,
                    nevjegy->nev,
                    nevjegy->foglalkozas,
                    nevjegy->cim,
                    nevjegy->telefonszam,
                    nevjegy->email);
        fclose(fp);
        return true;
    }
    return false;
}

// kiírja a fájlba a teljes adatbázist (felülírja az elõzõ fájlt)
bool fajlba_adatbazis_mentes(Nevjegyzek const * const adatbazis){
    if(fajl_inicializal_adatb()){
        Nevjegy* mozgo = adatbazis->elso->kov;
        while(mozgo != adatbazis->utolso){
            bool sikerult_e = fajlba_nevjegy_mentes(mozgo);
            if(!sikerult_e) return false;
            mozgo = mozgo->kov;
        }
        return true;
    }
    return false;
}

// a paraméterént kapott fájlnevû vCard fájl által tartalmazott névjegyet importálja az adatbázisba
bool vcard_import(char* fajlnev, Nevjegyzek* adatbazis){
    if(strstr(fajlnev, ".vcf") == NULL) strcat(fajlnev, ".vcf");
    printf("\n%s\n", fajlnev);
    varakozas_bemenetre();
    FILE* fp = fopen(fajlnev, "r");
    if(fp != NULL){
        printf("Importálás...\n");

        // beolvasás soronként, majd azokból az adatok eltárolása az ideiglenes nevjegy memóriába
        char sor[MAX_SOR_HOSSZ];
        Nevjegy ideiglenes = {};
        while (fgets(sor, sizeof(sor), fp) != NULL){
            if((strstr(sor, "FN") != NULL)){sscanf(sor, "FN:%[^\n]", ideiglenes.nev);}
            if((strstr(sor, "TITLE") != NULL)){sscanf(sor, "TITLE:%[^\n]", ideiglenes.foglalkozas);}
            if((strstr(sor, "ADR") != NULL)){sscanf(sor, "ADR%*[^:]:%[^\n]", ideiglenes.cim);}
            if((strstr(sor, "TEL") != NULL)){sscanf(sor, "TEL%*[^:]:%[^\n]", ideiglenes.telefonszam);}
            if((strstr(sor, "EMAIL") != NULL)){sscanf(sor, "EMAIL%*[^:]:%[^\n]", ideiglenes.email);}
        }
        adat_fixalas(&ideiglenes);
        nevjegy_kiir(&ideiglenes);
        if(mentie()) nevjegy_mentes(&ideiglenes, adatbazis);
        fclose(fp);
        return true;
    }
    printf("Nem sikerült megnyitni a fájt.\n");
    varakozas_bemenetre();
    return false;
}

bool vcard_export(Nevjegy const * const nevjegy){
    char fajlnev[30+4+1];
    strcpy(fajlnev, nevjegy->nev);
    strcat(fajlnev, ".vcf");
    FILE* fp = fopen(fajlnev, "w");
    if(fp != NULL){
        fprintf(fp, "BEGIN:VCARD\n"
                    "VERSION:4.0\n"
                    "FN:%s\n"
                    "TITLE:%s\n"
                    "ADR:%s\n"
                    "TEL:%s\n"
                    "EMAIL:%s\n"
                    "END:VCARD",
                    nevjegy->nev,
                    nevjegy->foglalkozas,
                    nevjegy->cim,
                    nevjegy->telefonszam,
                    nevjegy->email);
        fclose(fp);
        return true;
    }
    return false;
}
