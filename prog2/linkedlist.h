#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "node.h"
#include <fstream>
#include <algorithm>
static const int CODES_PRINT_LIMIT = 5;
enum choice
{
    BUILD,
    COMPRESS,
    NOTHINGTODO
};
class LinkedList
{
public:
    //Lista feje
    Node *head;                                             //Lista feje
    //Konstruktor
    LinkedList();                                           //Konstruktor
    //Destruktor
    ~LinkedList() {}                                        //Destruktor
    //Visszaadja, hogy üres e a lista
    bool isEmpty() const;                                   //Visszaadja, hogy üres-e a lista
    //Kilistázza a listát
    //Használatához -> kikommentezni Node.h és Node.cpp-ben meg ezt is
    //void displayList() const;                               //Kilistázza a listát

    //Hozzáad egy elemet a listához
    void addTolist(const Word &);                           //Hozzáad egy elemet a listához
    //Törli a listát
    void deleteList(); //Törli a listát
    //*******************************************************************************************************
    //                                          Function 1 - kitömörít

    //BuildList-hez megkeresi a leggyakoribb szavakat a listába és növeli a wordcountereiket
    void ListAlgorithm_Count();                                 //BuildList-hez megkeresi a leggyakoribb szavakat a listába és növeli a wordcountereiket
    //Alapértelemezett rakja, azon elemeket melyek feleslegesek a művelethez
    void ListAlgorithm_delete();                                //Alapértelemezett (Default) állapotba  rakja, azon elemeket melyek feleslegesek a művelethez
    //Eldönti, hogy tömörítsen vagy dekódoljon
    //Ha == BUILD (0)  listafelépítés a megadott fileból és kiírás a megadott file-ba
    //Ha == COMPRESS (1)  Nincs listaépítés csak feldolgozás
    //Ha == NOTHINGTODO (2) Valami gond van
    int WhatToDo(char const *) const;                           //Eldönti, hogy tömörítsen vagy dekódoljon
    //Listát épít a megadott file-ból és kiírja a megadottba a tömörített veriót
    // Paraméterek:
    //1. Forrás
    void BuildList(char const *);                               //Listát épít a megadott file-ból 
    //Töröl a listából egy megadott elemet
    //void deletefromList(const Word &);                        //Töröl a listából egy megadott elemet

    //Rendezve szúr be egy elemet
    //Csökkenő sorrendben
    void sortedInsert( Node** ,  Node* );                       //Rendezve szúr be egy elemet a listába
    //Rendezi a listát
    //Csökkenő sorrendbe
    void insertionSort(Node **);                                //Wordcounter szerint csökkenőbe rendezi a listát
    //Megfordítja a listát
    void reverse();                                             //Megfordítja a listát
    //Kiírja a megadott file-ba a listát
    // Paraméterek:
    // 1. Cél
    // 2. Kódokat tartalmazó tömb
    void WriteList_FUNCTION1(char const* ,  Word* )const;       //Kiírja a megadott file-ba a listát
    //Hozzárendeli az 5 leggyakoribb szóhoz a kódokat
    void SetCodes();                                            //Hozzárendeli az 5 leggyakoribb szóhoz a megfelelő  kódokat
    // Tömbbe teszi az 5 leggyakoribb szót
    // Paraméterek:
    // 0. visszatérési értékhez egy 5 elemű tömb
    // 1. Egy ötelemű tömb
    Word* GoToArray5(Word* )const;                              //Tömbbe teszi az 5 leggyakoribb szót   
    //*******************************************************************************************************
    //|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    //*******************************************************************************************************

    //                                         Function 2 - Visszatömörítéshez        
    //Dekódol és a megadott fileba kiírja a dekódolt elemeket
    // Paraméterek:
    // 1. Forrás
    // 2. Cél
    void WriteList_FUNCTION2(char const *, char const *);        //Dekódol(vissza-tömörít) és a megadott file-ba kiírja a dekódolt elemeket
    //*******************************************************************************************************
};

#endif