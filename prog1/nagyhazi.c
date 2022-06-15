#include "debugmalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h.>
#include <string.h>
//-------------------------------------------------Csapatkeret-Összeállítóhoz szelektáló program-------------------------------------------------------------//
// **A program egy csapatösszállító program, amely képes 3 sportágat és 3 korosztályt befogadni külső fájlokból,
// amelyekből aztán a beérkező paraméterek hatására előállítja az alkalmas játékosokat és megkeresi a legtöbbet
// játszott játékost, és kiírja az adatait.**
// A kódot és a külső txt fájlokat: Dudás Tamás Alex csinálta.
// Előfordul a programban, hogy ihletett merítettem/segítséget kértem a következő oldalakról illetve személytől: InfoC, programozás alapjai1. villanyos pptk/gyakorlatok anyaga, laborvezetőm tanácsai, segítségei, meglátásai.


// HIBAKÓD A 42
typedef enum sport{FOCI,KOSAR,ROPLABDA}sport;//sportághoz enum
typedef enum szabade{IGEN,NEM}szabade; // Ráér e?
//focicsapat 11+4 ---kosárlabda 6+2 ---röpladba 6+2

//struktúrák 
typedef struct l //Játékos struktúra 
{ 
    int azon;
    char nev[50];
    char korosztaly[5];
    szabade hetvege;
    sport sportag;
    int meccsek_szama;
    struct l*next;
}jatekos;
typedef struct m // Meccs struktúra
{

    sport sportag;
    char helyszin[50+1];
    int korosztaly_meccs;
    int sportolok[3][15];
    struct m*next;
}meccsek;

//Console felülelethez
int korosztaly_teszt(char *tomb)//konzol felületről bejövő adatok ellenőrzéséhez
{
    int back = 42;
    if( strcmp(tomb,"u7")==0 || strcmp(tomb,"u9")==0 || strcmp(tomb,"u14")==0) back = 0;
    return back;
}
int sportag_teszt(char *tomb)//konzol felületről bejövő adatok ellenőrzéséhez
{
    int back = 42;
    if( strcmp(tomb,"foci")==0 || strcmp(tomb,"kosar")==0 || strcmp(tomb,"roplabda")==0) back = 0;
    return back;
}
char* konzole_sport_convert(char *bejovo_sportag)//konzol felületről bejövő adatok feldolgozáshoz való konvertálásához
{
        char *sportok="";

    if(strcmp(bejovo_sportag,"foci")==0)sportok="foci";
        else if(strcmp(bejovo_sportag,"kosar")==0)sportok="kosár";
            else if(strcmp(bejovo_sportag,"roplabda")==0)sportok="röplabda";

    return sportok;
}
char* konzole_kor_convert(char *bejovo_korosztaly)//konzol felületről bejövő adatok feldolgozáshoz való konvertálásához
{
    char *korosztalyok="";
    if(strcmp(bejovo_korosztaly,"u7")==0) korosztalyok="u7";
        else if(strcmp(bejovo_korosztaly,"u9")==0) korosztalyok="u9";
            else if(strcmp(bejovo_korosztaly,"u14")==0)korosztalyok="u14";
    
    return korosztalyok;
}

//Játékos txt feldolgozásáháoz
int weeekend(char *valami)//Játékos hétvégéjének ellőrzésére szolgáló fgv IGEN==0 NEM==1
{
    int a=42;
    if(strcmp(valami,"igen")==0) a=IGEN;
    else if(strcmp(valami,"nem")==0)a = NEM;

    return a;
}
int sport_converter(char *valamit)//Játékos sportágának megállapítása a fent található enum sport alapján vannak az értékek
{
    int a=42;
    if(strcmp(valamit,"foci")==0) a =FOCI;
    else if(strcmp(valamit,"kosár")==0) a= KOSAR;
    else if(strcmp(valamit,"röplabda")==0) a = ROPLABDA;

    return a;
}
int korosztaly_convert(char *valamit)//Játkos korosztáylának meghatározása pld u14->14
{
    int a = 42;
    if(strcmp(valamit,"u9")==0) a =9;
    else if(strcmp(valamit,"u7")==0) a = 7;
        else if(strcmp(valamit,"u14")==0)a = 14;
    
    return a;
}
void printlist_jatekos(jatekos * head)//Kiíró függvény
{
    jatekos *current = head;
    for(current; current!=NULL; current = current->next)
    {
        printf("%d %s %s %d %d %d\n", current->azon, current->nev, current->korosztaly, current->hetvege, current->sportag,current->meccsek_szama);
    }
}
jatekos* jatekos_txt_to_list(jatekos*head,FILE *pp)// Egy nagyobb beolvasó függvény mely egyszeresen láncolt strázsa nélüli listába olvassa be a játékosokat és adataikat
{
    jatekos* newhead=NULL;
    if(pp==NULL) { printf("\nNem található ilyen nevezetű fájl! ");return NULL;};
    //adatok
    //int b = rows_in_file(fp);
    //
    int szam;
    char nevt[50];
        char knev[25];
        char vnev[25];
    char koroszt[5];
    char szabad[5];
    char sportag[10];

    head=NULL;
    jatekos*current;
    while(fscanf(pp,"%d %s %s %s %s %s",&szam,vnev,knev,koroszt,szabad,sportag)!=EOF)
    {
        if(head == NULL)//Ures volt a lista, uj elem +
        {
            head=(jatekos*)malloc(sizeof(jatekos));
            if(head==NULL)return NULL;
            current=head;
            current->next=NULL;
        }
        else
        {
            current->next=(jatekos*)malloc(sizeof(jatekos));
            if(current->next == NULL) return NULL;
            current->next->next=NULL; // Lista vége
            current=current->next; // Az uj ele legyen a curr
            
        }
        current->azon=szam;
        strcpy(current->nev,vnev);
        strcat(current->nev," ");
        strcat(current->nev,knev);
        strcpy(current->korosztaly,koroszt);
        current->hetvege = (szabade)weeekend(szabad);
        current->sportag=(sport)sport_converter(sportag);
    }
    newhead=head;
    fclose(pp);
    return newhead;
}
//Meccsek txt feldolgozása
int sport_converter_meccs(char *valamit)//Meccs sportágának konvertálása megfelelő formátumba szintén a fent látható enum sportban szereplő elemek a vissaztérési értékek
{
    int a=42;
    if(strcmp(valamit,"focilabda")==0) a =FOCI;
    else if(strcmp(valamit,"kosárlabda")==0) a= KOSAR;
    else if(strcmp(valamit,"röplabda")==0) a = ROPLABDA;

    return a;
}
void printlist_meccs(meccsek * head)//Meccseket kilistázó föggvény, figyelebe veszi, hogy a meccs milyen sportág ezalapján 15 vagy 8 főt listáz
{
    meccsek *current = head;
    for(current; current!=NULL; current = current->next)
    {
        printf("%d %s %d ",current->sportag,current->helyszin,current->korosztaly_meccs);

        if(current->sportag==FOCI)
        {
                            for(int i = 0; i<3; i++) 
                            { 
                        for(int j = 0; j<15; j++) 
                        { 
                        if(current->sportolok[i][j]!=0)printf("%d ", current->sportolok[i][j]); 
                        } 
                            }
                            printf("\n") ;
        }
        else if(current->sportag==KOSAR || current->sportag==ROPLABDA)
        {
            for(int i = 0; i<3; i++) 
                            { 
                        for(int j = 0; j<8; j++) 
                        { 
                        if(current->sportolok[i][j]!=0)printf("%d ", current->sportolok[i][j]); 
                        } 
                            }
                            printf("\n") ;
        }
    }

}
meccsek*txt_to_meccs(meccsek * head,FILE *fp)// Egy nagyobb beolvasó függvény mely egy egyszeresen láncolt strázsa nélküli listába olvassa be a meccseket és adataikat
{
        if(fp==NULL) {printf("\nNem található ilyen nevezetű fájl!");return NULL;}

    meccsek* newhead=NULL;
    meccsek* current;

    sport s;
    char sportag[15]="";//ok
    char helyszin2[25]="";//ok
    char korosztaly[5]="";//ok
    int jatekosok[15]={0};
    //olvasás
    head=NULL;

  while(fscanf(fp,"%s %s %s",sportag,helyszin2,korosztaly)==3)
{
    if(strcmp(sportag,"focilabda")==0)
    {
        int i = 0;
        while(i!=15)
        {
            if(fscanf(fp,"%d",&jatekosok[i]))
            i++;
        }
    }
    else if(strcmp(sportag,"focilabda")!=0)
    {
        int i =0;
        while(i!=8)
        {
        fscanf(fp,"%d ",&jatekosok[i]);
        ++i;
        }
    }
    //beolv vége

    //adatok
    if(head == NULL)//Ures volt a lista, uj elem +
        {
            head=(meccsek*)malloc(sizeof(meccsek));
            if(head==NULL)return NULL;
            current=head;
            current->next=NULL;
        }
        else
        {
            current->next=(meccsek*)malloc(sizeof(meccsek));
            if(current->next==NULL) return NULL;
            current->next->next=NULL; // Lista vége
            current=current->next; // Az uj ele legyen a curr
        }
        strcpy(current->helyszin,helyszin2);
            int forkor=korosztaly_convert(korosztaly);
            current->korosztaly_meccs=forkor;

            sport b = (sport)sport_converter_meccs(sportag);
            current->sportag=b;
            int nullazo[15]={0};
            switch (b)
            {
                case FOCI:     memcpy(current->sportolok[0],jatekosok,15*sizeof(int));memcpy(current->sportolok[1],nullazo,15*sizeof(int));memcpy(current->sportolok[2],nullazo,15*sizeof(int));break;
                case KOSAR:    memcpy(current->sportolok[1],jatekosok,8*sizeof(int));memcpy(current->sportolok[0],nullazo,15*sizeof(int));memcpy(current->sportolok[2],nullazo,15*sizeof(int));break;
                case ROPLABDA: memcpy(current->sportolok[2],jatekosok,8*sizeof(int));memcpy(current->sportolok[0],nullazo,15*sizeof(int));memcpy(current->sportolok[1],nullazo,15*sizeof(int));break;
                default:        b=(sport)42;    break;
            }
}
    newhead = head;
    fclose(fp);
    return newhead;
}

//MAIN FÜGGVÉNYEK
void felszabadit_players(jatekos *head)//Felszabadító függvény játékos listához
{
    jatekos *iter=head;
    while(iter !=NULL)
    {
        jatekos *kov=iter->next;
        free(iter);
        iter = kov;
    }
}
void preset0(jatekos* head)// A játkosok meccsei számának nevezetű egész típusú adatát állítja 0-ra
{   
    if(head==NULL)return ;
    jatekos* current=head;
    for(current; current!=NULL;current=current->next)
    {
        current->meccsek_szama=0;
    }
}
jatekos* push_front(jatekos*head,jatekos *t)// Egy játékos beszúrása előre egy egyszeresen láncolt stárzsa nélküli játékos listába
{
    if(t==NULL)return NULL;
    jatekos * newelem=(jatekos*)malloc(sizeof(jatekos));
    newelem->azon=t->azon;
    newelem->hetvege=t->hetvege;
    strcpy(newelem->korosztaly,t->korosztaly);
    strcpy(newelem->nev,t->nev);
    newelem->sportag=t->sportag;
    newelem->next=head;
    head=newelem;
    return head;
}
void szamolo(meccsek *head, jatekos *vizsgalando)//Adott vizsgálandó játékos meccsei számának meghatározása
{
    //korosztály és sportág ellenőrző
    if(vizsgalando==NULL)return ;
    int sp = vizsgalando->sportag;
    int koroszt = korosztaly_convert(vizsgalando->korosztaly);
    
    meccsek* mozgo;
    jatekos *temp = vizsgalando;

    for(temp;temp!=NULL; temp=temp->next)
    {
        for(mozgo=head; mozgo !=NULL; mozgo=mozgo->next)
        {
            if(mozgo->korosztaly_meccs == koroszt && mozgo->sportag == sp)
            {
                int b = temp->azon;
                for(int j =0; j <15; j++)
                {
                    if(mozgo->sportolok[sp][j]==b){temp->meccsek_szama++;}/*printf("%d ", temp->azon);*/
                   
                }
            }
        }
    }
}
int compare_name(jatekos *a, jatekos *b) //Játékosok neveinek összehasonlítása az ABC sorbarendezéshez
{
     return strcmp(a->nev, b->nev);
}
jatekos *insert_sorted(jatekos *head, char *name, int azon) //Játékosok listájába való beszúrás ABC sorrend alapján
{
    jatekos *ptr = (jatekos*)malloc(sizeof(jatekos));
    if (ptr == NULL) {
        return NULL;
    }

    strcpy(ptr->nev,name);
    ptr->azon = azon;
    ptr->next = NULL;

    jatekos **pp = &head;
    while (*pp != NULL && compare_name(ptr, *pp) >= 0) {
        pp = &(*pp)->next;
    }
    ptr->next = *pp;
    *pp = ptr;

    return head;
}
jatekos* szortirozo(jatekos*head, jatekos*fu7, char*sportag, char*korosztaly)//A játékosok különböző korosztály és sport szerinti szétválasztásában játszik szerepet
{
   
    jatekos*current=head;
    int sp=sport_converter(sportag);
    for(current;current!=NULL; current=current->next)
    {
        if(strcmp(current->korosztaly,korosztaly)==0 && current->sportag==sp && current->hetvege==0)
        {
           fu7= push_front(fu7,current);
        }
    }
    return fu7;
}
jatekos*nagy_szamolo(jatekos *head, meccsek*feje,char *korosztaly, char *sportag)//Egy nagy függvény, amely sok kicsi lőbb fennt megismert függvényt alkalmaz, azonban fő feladata, hogy előállítsa a kereset paraméterek alapján a megfelelő játékosok listáját, amelyekből aztán lehet tovább dolgozni
{   
    if(head==NULL)return NULL;
    if(feje==NULL)return NULL;


    jatekos *f_u7_head=NULL; char focilabda[]="foci";   
    jatekos*f_u9_head=NULL;  char under_seven[]="u7"; //U7
    jatekos*f_u14_head=NULL;

    jatekos *k_u7_head=NULL;    char kosarlabda[]="kosár";
    jatekos*k_u9_head=NULL;     char under_nine[]="u9"; //U9
    jatekos*k_u14_head=NULL;

    jatekos *r_u7_head=NULL;    char roplabda[]="röplabda";
    jatekos*r_u9_head=NULL;     char under_fourteen[]="u14"; //U14
    jatekos*r_u14_head=NULL;
    //Ráérő játékosok
    f_u7_head=szortirozo(head,f_u7_head,focilabda,under_seven); f_u9_head=szortirozo(head,f_u9_head,focilabda,under_nine); f_u14_head=szortirozo(head,f_u14_head,focilabda,under_fourteen);
    k_u7_head=szortirozo(head,k_u7_head,kosarlabda,under_seven); k_u9_head=szortirozo(head,k_u9_head,kosarlabda,under_nine); k_u14_head=szortirozo(head,k_u14_head,kosarlabda,under_fourteen);
    r_u7_head=szortirozo(head,r_u7_head,roplabda,under_seven); r_u9_head=szortirozo(head,r_u9_head,roplabda,under_nine); r_u14_head=szortirozo(head,r_u14_head,roplabda,under_fourteen);

    int sp=sport_converter(sportag);//bejövő sportág
    int kor=korosztaly_convert(korosztaly);//bejövő korosztály
    jatekos* back_them=NULL; // A kérdezett játékosok listája, amelyt visszaadjuk.
   
    switch(sp) // meccsek kinullázva
    {
        case FOCI:  if(kor==7){preset0(f_u7_head);szamolo(feje,f_u7_head);back_them=f_u7_head;

                                    felszabadit_players(f_u9_head);felszabadit_players(f_u14_head);
                                    felszabadit_players(r_u7_head);felszabadit_players(r_u9_head);felszabadit_players(r_u14_head);
                                    felszabadit_players(k_u7_head);felszabadit_players(k_u9_head);felszabadit_players(k_u14_head);
                             }
                    else if(kor==9){preset0(f_u9_head);szamolo(feje,f_u9_head);back_them=f_u9_head;
                                    felszabadit_players(f_u7_head);felszabadit_players(f_u14_head);
                                    felszabadit_players(r_u7_head);felszabadit_players(r_u9_head);felszabadit_players(r_u14_head);
                                    felszabadit_players(k_u7_head);felszabadit_players(k_u9_head);felszabadit_players(k_u14_head);                   
                                   }
                    else if(kor==14){preset0(f_u14_head);szamolo(feje,f_u14_head);back_them=f_u14_head;                  
                                    felszabadit_players(f_u7_head);felszabadit_players(f_u9_head);
                                    felszabadit_players(r_u7_head);felszabadit_players(r_u9_head);felszabadit_players(r_u14_head);
                                    felszabadit_players(k_u7_head);felszabadit_players(k_u9_head);felszabadit_players(k_u14_head);                                                   
                                    }
                    break;

        case ROPLABDA:  if(kor==7){preset0(r_u7_head);szamolo(feje,r_u7_head);back_them=r_u7_head;
                                    felszabadit_players(f_u7_head);felszabadit_players(f_u9_head);felszabadit_players(f_u14_head);
                                    felszabadit_players(r_u9_head);felszabadit_players(r_u14_head);
                                    felszabadit_players(k_u7_head);felszabadit_players(k_u9_head);felszabadit_players(k_u14_head);                                               
                                  }
                    else if(kor==9){preset0(r_u9_head);szamolo(feje,r_u9_head);back_them=r_u9_head;
                    
                                    felszabadit_players(f_u7_head);felszabadit_players(f_u9_head);felszabadit_players(f_u14_head);
                                    felszabadit_players(r_u7_head);felszabadit_players(r_u14_head);
                                    felszabadit_players(k_u7_head);felszabadit_players(k_u9_head);felszabadit_players(k_u14_head);
                    
                                   }
                    else if(kor==14){preset0(r_u14_head);szamolo(feje,r_u14_head);back_them=r_u14_head;
                    
                                    felszabadit_players(f_u7_head);felszabadit_players(f_u9_head);felszabadit_players(f_u14_head);
                                    felszabadit_players(r_u7_head);felszabadit_players(r_u9_head);
                                    felszabadit_players(k_u7_head);felszabadit_players(k_u9_head);felszabadit_players(k_u14_head);                                       
                                    }
                    break;

        case KOSAR: if(kor==7){preset0(k_u7_head);szamolo(feje,k_u7_head);back_them=k_u7_head;      
                                    felszabadit_players(f_u7_head);felszabadit_players(f_u9_head);felszabadit_players(f_u14_head);
                                    felszabadit_players(r_u7_head);felszabadit_players(r_u9_head);felszabadit_players(r_u14_head);
                                    felszabadit_players(k_u9_head);felszabadit_players(k_u14_head); 
                              }
                    else if(kor==9){preset0(k_u9_head);szamolo(feje,k_u9_head);back_them=k_u9_head;
                                    felszabadit_players(f_u7_head);felszabadit_players(f_u9_head);felszabadit_players(f_u14_head);
                                    felszabadit_players(r_u7_head);felszabadit_players(r_u9_head);felszabadit_players(r_u14_head);
                                    felszabadit_players(k_u7_head);felszabadit_players(k_u14_head);                                      
                                   }
                    else if(kor==14){preset0(k_u14_head);szamolo(feje,k_u14_head);back_them=k_u14_head;
                    
                                    felszabadit_players(f_u7_head);felszabadit_players(f_u9_head);felszabadit_players(f_u14_head);
                                    felszabadit_players(r_u7_head);felszabadit_players(r_u9_head);felszabadit_players(r_u14_head);
                                    felszabadit_players(k_u7_head);felszabadit_players(k_u9_head);
                                    }
                    break;
        default: perror("ERROR");
    }
return back_them;

}
jatekos* delete_the_upperaveragepalyers(jatekos* head)// A "jó" játékosokat nem szelektálja egy másik listába, csak, akik az átlag alapján kevesebbet vagy ugyannyit játszottat, mint az átlag
{   
    if(head==NULL)return NULL;
    int atlag=0;
    int db;
    jatekos* back=NULL;
    jatekos *current = head;
    for(current,db=0; current!=NULL; current=current->next)
    {
        atlag+=current->meccsek_szama;
        db++;
    }
    atlag =round(atlag/(db+0.0));
    for(jatekos *temp = head; temp!=NULL; temp = temp->next)
    {
        if(temp->meccsek_szama<=atlag)back=insert_sorted(back,temp->nev,temp->azon);
    }
    return back;
}
void printlegjobb(jatekos* head,meccsek *feje)// A legjobb játékos megkereseséért felelős illetve azon adatainak kiírásáért
{
    if(feje==NULL)return ;
    if(head==NULL)return ;
    int max =0;
    int azon_max = 0; // itt van a legtöbbet játszott játékos azonja
    char name[50]="";
    int sp=head->sportag;
    int kor=korosztaly_convert(head->korosztaly);

    jatekos * current = head;
    for(current; current!=NULL ; current=current->next)
    {
        if(current->meccsek_szama>max){max=current->meccsek_szama; azon_max=current->azon;}
    }
    jatekos*current2=head;
    for(current2=head; current2!=NULL; current2=current2->next)
    {
        if(current2->azon==azon_max)strcpy(name,current2->nev);
    }
    meccsek* mozgo=feje;
    int hanyszor=0;
    int c =0;
    char varos_tmp[50]="Default";
    for(mozgo; mozgo!=NULL; mozgo=mozgo->next)
    {
        if(mozgo->sportag==sp && mozgo->korosztaly_meccs==kor)
        {   c= 0;
                for(int j = 0; j <15; ++j)
                { 
                    if(mozgo->sportolok[sp][j]==azon_max)
                    {
                        c++;
                        if(hanyszor==0){hanyszor=c;strcpy(varos_tmp,mozgo->helyszin);}
                        else if(c>hanyszor) {hanyszor=c;strcpy(varos_tmp,mozgo->helyszin);}
                    }
                }
        }
    }
    printf("\nA legtöbbet játszott játékos neve: %s, azonosítója: %d\nLegtöbbet e helyen játszotta: %s, méghozzá ennyiszer: %d-(szer/szor)",name,azon_max,varos_tmp,hanyszor);
}
void printlist_jatekos_vegleges(jatekos * head) //Kiírja az alkalmas játékosokat, akikből lehet válogatni az utolsó meccsre
{
    jatekos *current = head;
    if(current ==NULL)printf("Nincsenek az adatbázisban alkalmas játékosok!\n");   
    
    for(current; current!=NULL; current = current->next)
    {
        printf("Játékos neve: %s \t \t \t   || azonositója: %d\n", current->nev,current->azon);
    }
}
void felszabadit_meccsek(meccsek *head) //Felszabadító függvény meccsek listához
{
    meccsek *iter=head;
    while(iter !=NULL)
    {
        meccsek *kov=iter->next;
        free(iter);
        iter = kov;
    }
}

int main()
{
    /*--------------------------------*/
     printf("Üdvözlöm a csapatösszeállító programban!\nKérem adja meg a kívánt korosztályt [u7;u9;u14], illetve a sportág nevét [foci,kosar,roplabda]!\n");
    char bejovo_korosztaly[5];
    char bejovo_sportag[10];
    int i =10;
   do {
    scanf("%s %s",bejovo_korosztaly,bejovo_sportag);
    if(strcmp(bejovo_sportag, "vege")==0 || strcmp(bejovo_korosztaly,"vege")==0)return 0;
        int hibakoroszt = korosztaly_teszt(bejovo_korosztaly);
        int hibasportag = sportag_teszt(bejovo_sportag);
        if(hibakoroszt !=0 || hibasportag != 0) {printf("\033[0;31m");printf("Hiba az adatok beolvasása közben!\nKérem adja meg újra a kívánt adatokat (kívánt korosztályt [u7;u9;u14]), (kívánt sportág nevét [foci,kosar,roplabda] )megfelelő formátumban, amennyiben kiszeretne lépni írja be, hogy 'vege vege'!\n");printf("\033[0m");}
        else
        {
             i = 0; 
        }

   } while(i!=0);
    //Console bekérés,  Console felület vége
    //------------------------------------------------------------------------//

    /*--------------------------------*/
    jatekos* head;
    // játékosok listájának beolvasása
    FILE *pp=fopen("1.file.txt", "r");
    head=jatekos_txt_to_list(head,pp);
    //printlist_jatekos(head);  //Betöltés teszteléshez

    meccsek*feje;                   
    FILE *fp=fopen("2file.txt", "r"); //meccsek listájának elkészítése
    feje=txt_to_meccs(feje,fp);
    //printlist_meccs(feje); // Betöltés teszteléshez

    char *korosztalyok=konzole_kor_convert(bejovo_korosztaly); // Konzolról bejövő adat átalakítása
    char *sportok=konzole_sport_convert(bejovo_sportag);   //Konzolról bejövő adat átalakítása

    jatekos* megkapott_lista=nagy_szamolo(head,feje,korosztalyok,sportok);// Szortirozza a játékosokat és előállítja a megfelelő játékosok l.listáját
    //printlist_jatekos(megkapott_lista);// TESZTHEZ
    printlegjobb(megkapott_lista,feje);// Legtöbbet játszó játékos megkeresése és adatainak kiirása
    jatekos* vegleges=delete_the_upperaveragepalyers(megkapott_lista); // Az alkalmas játékosokat szelektáló függvény, melyből előáll az alkalmas játékosok l.listája
    printf("\n\nA program szerint ráérő/alkalmas játékosok listája a következő:\n\n");
    printlist_jatekos_vegleges(vegleges);// Ők akik alkalmasak

    //------------------------------------------------------------------------//

    /*--------------------------------*/

    //szivárogtatás ellen való felszabadítások
    felszabadit_players(head);
    felszabadit_players(megkapott_lista);
    felszabadit_players(vegleges);
    felszabadit_meccsek(feje);
    debugmalloc_dump();
    //------------------------------------------------------------------------//

    return 0; 
}