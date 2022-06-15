#include "linkedlist.h"
LinkedList::LinkedList()
{
    this->head = nullptr;
}
inline bool LinkedList::isEmpty() const
{
    return head == nullptr;
}
/* //Teszteléshez hasznos
void LinkedList::displayList() const
{
    Node *current;
    for (current = head; current != NULL; current = current->next)
        std::cout << current << std::endl;
}
*/
void LinkedList::addTolist(const Word &word)
{
    if (head->getNodesum() >= MAX_WORD_COUNT)
    {
        //std::cerr << "Nem adható több szó hozzá!"<< std::endl;
        throw std::length_error("Nem adható hozzá több szó!");
    }
    Node *new_node = new Node(word);
    new_node->next = NULL;
    if (head == NULL)
        head = new_node;
    else
    {
        Node *temp = head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new_node;
    }
}
void LinkedList::deleteList()
{
    Node *next = this->head;
    Node *current = nullptr;
    while (next != nullptr)
    {
        current = next;
        next = next->next;
        delete current;
    }
}
int LinkedList::WhatToDo(char const *filename) const
{
    std::fstream file;
    std::string choose = "default";
    file.open(filename, std::ios::in);
    if (!file)
    {
        std::cerr << "Hiba a következő fájl megnyitásakor: " << filename << std::endl;
        throw "Sikertelen file megnyitás";
    }
    else
    {
        int i = 0;
        while (i != 1)
        {
            file >> choose;
            if (file.eof())
                break;
            i++;
        }
    }
    file.close();
    if (file.good())
    {
        bool logic = this->head->word.isCode(choose);
        //Ha igaz akkor az első string kód volt -> dekódolni kell
        //Ha hamis akkor az első string nem kód volt -> tömörítünk
        if (logic)
            return COMPRESS; //Dekódolás
        return BUILD;        //Tömörítés
    }
    else
    {
        std::cerr << "Hiba a file bezárásakor: " << filename << std::endl;
        return -1;
    }
    return NOTHINGTODO; //Hiba volt valahol
}
void LinkedList::BuildList(char const *source)
{
    std::string temp = "";
    Word word_temp;
    std::fstream file_honnan;
    file_honnan.open(source);
    if (!file_honnan)
    {
       // std::cerr << "Hiba a file megnyitásakor: " << source << std::endl;
        throw std::invalid_argument("Hiba a file megnyitásakor");
    }
    else
    {
        while (1)
        {
            file_honnan >> temp;
            word_temp.setWord(temp);
            addTolist(word_temp);
            word_temp.SetwordDefault();
            if (file_honnan.eof())
                break;
        }
    }
    file_honnan.close();
    if (!file_honnan)
    {
        std::cerr << "Hiba a file bezárásakor: " << source << std::endl;
    }
}
void LinkedList::ListAlgorithm_delete()
{
    reverse();
    Node *temp;
    Node *current;
    for (temp = head; temp != NULL; temp = temp->next)
        for (current = temp->next; current != NULL; current = current->next)
            if (temp->word.IswordsEqual(current->word.getWord()))
                current->word.SetwordDefault();
}
/* //Teszteléshez hasznos
void LinkedList::deletefromList(const Word &param)
{
    //Fej mentése
    Node *temp = this->head;
    Node *prev = NULL;

    //Ha a fejben van a törlendő
    if (temp != NULL && temp->word.IswordsEqual(param))
    {
        this->head = temp->next;
        delete temp;
        return;
    }
    //Különben keresni kell
    else
    {
        while (temp != NULL && !temp->word.IswordsEqual(param))
        {
            prev = temp;
            temp = temp->next;
        }
        if (temp == NULL)
            return;

        prev->next = temp->next;
        delete temp;
    }
}
*/
void LinkedList::ListAlgorithm_Count()
{
    Node *temp;
    Node *current;
    int plus;
    //Count meghatározása
    for (temp = head; temp != NULL; temp = temp->next)
        for (current = temp; current != NULL; current = current->next)
        {
            if (temp->word.IswordsEqual(current->word) && temp->word.GetwordLenght() >= MIN_CHAR)
            {
                plus = temp->word.GetwordCount();
                temp->word.setwordCounter((plus + 1));
            }
        }
}
void LinkedList::sortedInsert(Node **head_ref, Node *new_node)
{
    Node *current;
    if (*head_ref == NULL || (*head_ref)->word.GetwordCount() >= new_node->word.GetwordCount())
    {
        new_node->next = *head_ref;
        *head_ref = new_node;
    }
    else
    {
        current = *head_ref;
        while (current->next != NULL && current->next->word.GetwordCount() < new_node->word.GetwordCount())
        {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}
void LinkedList::insertionSort(Node **head_ref)
{
    // Inicializálni egy sorted listát
    Node *sorted = NULL;

    // Bejárni a listát és hozzáadni rendezve
    Node *current = *head_ref;
    while (current != NULL)
    {
        // Következő elmentése a következő iterációhoz
        Node *next = current->next;

        // Beszúrni az aktuálisat a rendezett listába
        sortedInsert(&sorted, current);
        // Frissíteni az aktuálisat
        current = next;
    }

    // Frissíteni a fejet az újra
    *head_ref = sorted;
}
void LinkedList::reverse()
{
    // Inicalizálni az atkuálisat, előzöt és a következő pointereket
    Node *current = head;
    Node *prev = NULL, *next = NULL;

    while (current != NULL)
    {
        // Következő elmentése
        next = current->next;

        // Fordítani az atkuálisat
        current->next = prev;

        // Pointerek mozgatása
        prev = current;
        current = next;
    }
    head = prev;
}
void LinkedList::SetCodes()
{
    Node *temp;
    int iter;
    for (temp = this->head, iter = 0; temp != NULL; temp = temp->next)
    {
        if (iter < 5)
        {
            switch (iter)
            {
            case 0:
                temp->word.setCode("!1");
                break; //1.Leggyakoribb kód
            case 1:
                temp->word.setCode("!2");
                break; //2.Leggyakoribb kód
            case 2:
                temp->word.setCode("!3");
                break; //3.Leggyakoribb kód
            case 3:
                temp->word.setCode("!4");
                break; //4.Leggyakoribb kód
            case 4:
                temp->word.setCode("!5");
                break; //5.Leggyakoribb kód

            default:
                temp->word.setCode("00");
                break; //Default kód
            }
            iter++;
        }
        else
            return;
    }
}
Word *LinkedList::GoToArray5(Word *destinition) const
{
    Node *temp;
    int count;
    for (temp = this->head, count = 0; temp != NULL; temp = temp->next, count++)
        if (count < CODES_PRINT_LIMIT)
            destinition[count] = temp->word;

    return destinition;
}
void LinkedList::WriteList_FUNCTION1(const char *filename_ki, Word *codes) const
{
    // létezik vagy nem létezik, ahova írni kell, Fstream tehát, ha nem létezik létrehozza
    std::fstream file;
    file.open(filename_ki, std::ios::out);
    if (!file)
    {
        //std::cerr << "Sikertelen file megnyitás" << std::endl;
        throw std::invalid_argument("Hiba a file megnyitásakor");
    }
    else
    {
        //Elsőnek a kódokat és a jelentésüket
        for (int i = 0; i < CODES_PRINT_LIMIT; i++)
            file << codes[i].getCode() << " " << codes[i].getWord() << " ";

        Node *current;
        for (current = this->head; current != NULL; current = current->next)
        {
            //Ha eleme ez az elem annak a tömbnek
            if (this->head->word.IsArrayElement(current->word, codes, CODES_PRINT_LIMIT))
            {
                std::string code_ki = this->head->word.GetCodeFromArray(current->word.getWord(), codes, CODES_PRINT_LIMIT);
                if (code_ki == "00")
                {
                    //std::cerr << "Érvénytelen vagy alapértelmezett kód, hiba a filekezelésnél" << std::endl;
                    throw std::invalid_argument("Érvénytelen vagy alapértelmezett kód!");
                }
                file << code_ki << " ";
            }

            else
            {
                file << current->word.getWord() << " ";
            }
        }
    }
    file.close();
    if (!file)
    {
        std::cerr << "Sikertelen file bezárás" << filename_ki << std::endl;
    }
}
void LinkedList::WriteList_FUNCTION2(char const *source, char const *destinition)
{
    //Beolvassuk a kódokat és a jelentéseket
    std::string code_temp = "";
    std::string string_temp = "";
    std::string whatisit = "";
    Word word_temp;

    Word *words = new Word[CODES_PRINT_LIMIT];
    std::fstream file_honnan;
    std::fstream file_hova;
    file_honnan.open(source, std::ios::in);
    //
    if (!file_honnan)
    {
        std::cerr << "Sikertelen file megnyitás: " << source << std::endl;
        throw std::invalid_argument("Sikertelen file megnyitás");
    }
    else
    {
        //Kódok és  a hozzájuk tartozó szavak eltárolása
        for (int i = 0; i < CODES_PRINT_LIMIT; i++)
        {
            file_honnan >> code_temp;
            file_honnan >> string_temp;
            word_temp.setCode(code_temp);
            word_temp.setWord(string_temp);
            words[i] = word_temp;
        }
        //Helyettesítés
        word_temp.SetwordDefault();
        while (file_honnan >> string_temp)
        {
            if (this->head->word.isCode(string_temp))
            {
                word_temp.setCode(string_temp);
                word_temp.setWord(string_temp);
            }
            else
            {
                word_temp.setWord(string_temp);
                word_temp.setCode("00");
            }

            this->addTolist(word_temp);
        }
        file_honnan.close();
        if (file_honnan)
        {
            std::cerr << "Sikertelen file bezárás!" << std::endl;
        }
    }
    file_hova.open(destinition, std::ios::out);
    Node *current;
    for (current = this->head; current != NULL; current = current->next)
    {
        if (this->head->word.isCode(current->word.getCode()))
        {
            string_temp = this->head->word.GetWordFromArray(current->word.getCode(), words, CODES_PRINT_LIMIT);
            if (string_temp == "default")
            {
                //std::cerr << "Érvénytelen vagy alapértelmezett szó!" << std::endl;
                throw std::invalid_argument("Érvénytelen vagy alapértelmezett szó!");
            }
            file_hova << string_temp << " ";
        }
        else
        {
            file_hova << current->word.getWord() << " ";
        }
    }

    this->deleteList();
    delete[] words;
}