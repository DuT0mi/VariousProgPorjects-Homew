#ifndef WORDCLASS_H
#define WORDCLASS_H
//LISTAADAT
#include <iostream>
#include <cstdio>
#include <string.h>
#include <cassert>
#include <cstring>
//Lehetséges kódok !1 !2 !3 !4 !5 -> !1 leggyakoribb
//  Alapértelemezett (Default) állapot jelentése :=
//      szo = "default"
//      code = "00"
//      wordcounter = 0
static const int MAX_STRING_LENGHT = 22; //Maximum szo méret
static const int MAX_WORD_COUNT = 2000;  //Maximum szó szám a fileban
static const int WORD_COUNT_DEFAULT = 0; //A szó számláló alapértelmezett értékei
static const int MIN_CHAR = 3;           //A szó, minimum karakterszáma a számláláshoz
static const int BASIC_COUNT = 1;        //A számolásnál, ez az alapértelemezett hozzáadandó
class Word
{
private:
    std::string szo;  //Maga a szó
    int wordcounter;  //Szó gyakorisága
    std::string code; //Kód
public:
    //Alapértelemezett/Többváltozós konstruktor
    //Balról jobbra
    //1. szó            
    //2. kód            
    //3. szó_gyakoriság 
    Word(const std::string & = "default", const std::string & = "00", int = WORD_COUNT_DEFAULT); //Alapértelemezett/Többváltozós konstruktor
    //Másoló konstruktor
    Word(const Word &);                                                                          //Másoló konstruktor
    //Word1 = Word2
    Word &operator=(const Word &);                                                               //Word1 = Word2
    //Megadja a szó hosszát a nulla terminál nélkül
    int GetwordLenght() const;                                                                   //Megadja a szó hosszát a nulla terminál nélkül
    //Visszaadja, hogy a szó gyakoriságát
    int GetwordCount() const;                                                                    //Visszaadja, hogy a szó gyakoriságát
    //Visszaadja a kódot
    const std::string& getCode() const;                                                          //Visszaadja a kódot
    //Visszaadja a szót
    const std::string& getWord() const;                                                          //Visszaadja a szót
    //Beállíthatja, hogy a szó hányszor szerepelt
    //Validál
    void setwordCounter(const int& =BASIC_COUNT);                                                //Beállíthatja, hogy a szó hányszor szerepelt,, validálás mellett
    //Alapértelmetett (Default) állapotba kerül a szó
    //szo = "default"
    //code = "00"
    //wordcounter = 0
    void SetwordDefault();                                                                       //Alapértelmetett (Default) állapotba kerül a szó
    //Beállítja a kódot
    //Validál
    bool setCode(const std::string &);                                                           //Beállítja a kódot, validálás mellett
    //Beállít egy kívánt szót
    //Validál
    void setWord(const std::string &);                                                           //Validálás mellett beállít egy kívánt szót
    //Eldönti 2 szóról, hogy egyenlőek-e
    bool IswordsEqual(const Word &) const;                                                       //Eldönti 2 szóról, hogy egyenlőek-e
    //Eldönti, hogy kód e a string
    // Paraméterek:
    // 1. std::string 
    bool isCode(const std::string &) const;                                                      //Eldönti, hogy kód-e a string
    // Paraméterek:
    // 1. A szó, amiről tudni szeretnénk
    // 2. A Word (Limit elemű) tömb
    // 3. Limit
    bool IsArrayElement(const Word&, Word*, const int )const;                                    //Eleme-e a tömbnek az elem
    // Paraméterek:
    // 1. A Word, aminek szeretnénk a kódját megtudni
    // 2. A Word (Limit elemű) tömb
    // 3. Limit
    static std::string GetCodeFromArray(const Word&, Word*, const int );                         //Visszaadja a codot a tömbből
    //Visszaadja a szót a tömbből
    // Paraméterek:
    // 1. szó
    // 2. Tömb
    // 3. Tömb méret
    static std::string GetWordFromArray(const std::string&, Word*,const int);                    //Visszaadja a szót a tömbből
};
    std::istream &operator>>(std::istream &,  Word &);
    std::ostream &operator<<(std::ostream &, const Word &);
#endif