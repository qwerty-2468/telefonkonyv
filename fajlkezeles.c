#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include "adatbazis.h"
#include "fajlkezeles.h"
#include "io.h"

//#define TESZT

// visszaadja, hogy l�tezik-e m�r adatab�zis.txt f�jl
bool van_adatb_fajl(void){
    FILE* fp = fopen("adatbazis.txt", "r");
    if(fp != NULL){
        fclose(fp);
        return true;
    }
    return false;
}

// beolvassa az adatb�zis f�jl tartalm�t a param�ter�l kapott adatb�zisba
bool adatb_beolvas(Nevjegyzek* adatbazis){
    FILE* fp = fopen("adatbazis.txt", "r");
    if(fp != NULL){
        printf("Bet�lt�s...\n");
        //fejl�c �tugr�sa
        char tmp[45+1] ="#"; //ilyen hossz� a leghosszabb sor az adatb�zis f�jl fejl�c�ben
        while(tmp[0] == '#' && fgets(tmp, sizeof(tmp), fp)!=NULL){
            ;
        }

        // beolvas�s soronk�nt, majd azokb�l az adatok elt�rol�sa az ideiglenes nevjegy mem�ri�ba
        char sor[MAX_SOR_HOSSZ];
        while (fgets(sor, sizeof(sor), fp) != NULL){
            Nevjegy ideiglenes;
            sscanf(sor, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%s", // | jellel vannak elv�lasztva az adatok a f�jlban
                    &ideiglenes.id,
                    ideiglenes.nev,
                    ideiglenes.foglalkozas,
                    ideiglenes.cim,
                    ideiglenes.telefonszam,
                    ideiglenes.email);

            // csak tesztel�skor fut le
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

// l�trehoz egy adatb�zis f�jlt a program mapp�j�ban
bool fajl_inicializal_adatb(){
    FILE* fp = fopen("adatbazis.txt", "w");
    if(fp != NULL){
        fprintf(fp, "#N�vjegyz�k adatb�zis f�jl   v1\n"
                    "#\n"
                    "#ID|N�v|Foglalkoz�s|C�m|Telefonsz�m|Email\n");
        fclose(fp);
        return true;
    }
    return false;
}

// ki�rja az adatb�zis f�jlba a param�terk�nt kapott n�vjegyet
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

// ki�rja a f�jlba a teljes adatb�zist (fel�l�rja az el�z� f�jlt)
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

// a param�ter�nt kapott f�jlnev� vCard f�jl �ltal tartalmazott n�vjegyet import�lja az adatb�zisba
bool vcard_import(char* fajlnev, Nevjegyzek* adatbazis){
    if(strstr(fajlnev, ".vcf") == NULL) strcat(fajlnev, ".vcf");
    printf("\n%s\n", fajlnev);
    varakozas_bemenetre();
    FILE* fp = fopen(fajlnev, "r");
    if(fp != NULL){
        printf("Import�l�s...\n");

        // beolvas�s soronk�nt, majd azokb�l az adatok elt�rol�sa az ideiglenes nevjegy mem�ri�ba
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
    printf("Nem siker�lt megnyitni a f�jt.\n");
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
