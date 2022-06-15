#ifndef NODE_H
#define NODE_H
#include "wordclass.h"
//LISTAELEM
//Alapértelmezett kivonás
static const int DEFAULT_SUB = 1;                              //Alapértelmezett kivonás
//A kezdőérték az összeghez
static const int START_SUM = 0;                               //A kezdőérték az összeghez
class Node
{
private:
    //Az összes Node darabszáma
    static unsigned sumofNodes;                              //Az összes Node darabszáma
public:
    //A szó
    Word word;                                               //A szó
    //A következő node-ra mutató pointer
    Node *next;                                             //A következő node-ra mutató pointer
    //Visszaadja a darabszámot
    static unsigned getNodesum();                           //Visszaadja a darabszámot
    //Kivon x-t a darabszám összegéből
    static void subfromNodeSum(const int & = DEFAULT_SUB);  //Kivon x-t az összegből
    //Konverziós konstruktor
    Node(const Word &);                                     //Konverziós konstruktor
    //Sima konstruktor
    Node();                                                 //Sima konstruktor
    //Destruktor
    ~Node();                                                //Destruktor
};
//std::ostream &operator<<(std::ostream &, Node *);         //teszthez (displaylist)
#endif