#ifndef FAJLKEZELES_H_INCLUDED
#define FAJLKEZELES_H_INCLUDED

#define MAX_SOR_HOSSZ 112+4 // a Nevjegy strukt�r�b�l, illetve az elv�laszt� karakterb�l ad�d�an

bool van_adatb_fajl(void);
bool adatb_beolvas(Nevjegyzek* adatbazis);
bool fajl_inicializal_adatb(void);
bool fajlba_nevjegy_mentes(Nevjegy const* const nevjegy);
bool fajlba_adatbazis_mentes(Nevjegyzek const * const adatbazis);
bool vcard_import(char* fajlnev, Nevjegyzek* adatbazis);
bool vcard_export(Nevjegy const * const nevjegy);

#endif // FAJLKEZELES_H_INCLUDED
