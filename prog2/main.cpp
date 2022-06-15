#include "linkedlist.h"
#include <stdexcept>
//  *************************************************************************************************
//                                  FILE tömörítő program II.
//  *************************************************************************************************
//  Hivatkozások:
//  https://stackoverflow.com/questions/347949/how-to-convert-a-stdstring-to-const-char-or-char
//  https://www.guru99.com/cpp-file-read-write-open.html
//  BME HIT programozás alapjai 1 Láncolt Listás adatszerkezetek ppt-k   (  Reminderként  )
//  https://www.geeksforgeeks.org/insertion-sort-for-singly-linked-list/ (Listarendezéshez)
// *************************************************************************************************
int main(int argc, char *argv[])
{
    //**************************************
    std::cout << std::endl;
    std::cout << "Üdvözlöm kérem ajda meg a bemeneti file nevét (.txt kiterjesztéssel) illetve a helyével kiegészítve, ha nem a programmal azonos mappában van!" << std::endl;
label:
    std::cout << "Kérem adja meg a tömörítendő fájlt: " << std::endl;
    std::string temp1;
    std::cin >> temp1;
    const char *filename_be = temp1.c_str(); //Konvertálás függvényekhez
    std::cout << "Kérem adja meg ahova kerüljön a fájl: " << std::endl;
    std::string temp2;
    std::cin >> temp2;
    //
    //**************************************
    //--------------------------------------
    //**************************************
    const char *filename_ki = temp2.c_str(); //Konvertálás függvényekhez
    LinkedList *list = new LinkedList();
    int DO = NOTHINGTODO;
    try
    {
        DO = list->WhatToDo(filename_be);
        switch (DO)
        {
        case BUILD:
            std::cout << "Tömörítési folyamatot hajtok végre." << std::endl;
            break;
        case COMPRESS:
            std::cout << "Visszatömörítési folyamatot hajtok végre." << std::endl;
            break;
        case NOTHINGTODO:
            throw "Nem megfelelő bemeneti file!";
            break;

        default:
            throw "Nem megfelelő bemeneti file!";
            break; /* :O */
        }
    }
    catch (char const *msg )
    {
        std::cerr << "Sajnos érvénytelen bementi file-t adott meg, amennyiben szeretné újra megpróbálni kérem írja be a zárójelben lévő szöveget (again), amennyiben  kiszeretné szeretne lépni, gépelje be a zárójelben lévő szöveget ( exit )." << std::endl;
        std::string re_start;
        std::cin >> re_start;
        if (re_start == "again")
            goto label;
        else if (re_start == "exit")
        {
            std::cout << "Ön a program futásának befejezését választotta." << std::endl;
            return 0;
        }
    }
    try
    {
        LinkedList *list_test = new LinkedList();
        LinkedList *list_test_copy = new LinkedList();

        if (DO == BUILD)
        {
            list_test->BuildList(filename_be);
            Word *codes_here = new Word[CODES_PRINT_LIMIT];

            list_test_copy->BuildList(filename_be);
            list_test_copy->ListAlgorithm_Count();
            list_test->ListAlgorithm_Count();
            list_test->insertionSort(&list_test->head);
            list_test->ListAlgorithm_delete();
            list_test->insertionSort(&list_test->head);
            list_test->reverse();
            list_test->SetCodes();
            list_test->GoToArray5(codes_here);
            list_test->deleteList();
            list_test_copy->WriteList_FUNCTION1(filename_ki, codes_here);
            list_test_copy->deleteList();
            delete[] codes_here;
            std::cout << "Művelet sikeres, kérem keresse ezen mappában a " << filename_ki << " állományt" << std::endl;
        }
        else if (DO == COMPRESS)
        {
            list_test->WriteList_FUNCTION2(filename_be, filename_ki);
            std::cout << "Művelet sikeres, kérem keresse ezen mappában a " << filename_ki << " állományt" << std::endl;
        }
        list->deleteList();
        delete list;
        delete list_test;
        delete list_test_copy;
    }
    catch (const std::length_error &err)
    {   
        std::cerr<<err.what()<<std::endl;
        std::cerr << "Nincs elegendő hely több szónak,amennyiben szeretné újra megpróbálni kérem írja be a zárójelben lévő szöveget (again), amennyiben  kiszeretné szeretne lépni, gépelje be a zárójelben lévő szöveget ( exit )" << std::endl;
        std::string re_start;
        std::cin >> re_start;
        if (re_start == "again")
            goto label;
        else if (re_start == "exit")
        {
            std::cout << "Ön a program futásának befejezését választotta." << std::endl;
            return 0;
        }
    }
    catch (const std::invalid_argument &err)
    {   
        std::cerr<<err.what()<<std::endl;
        std::cerr << "Sajnos érvénytelen bementi file-t adott meg, amennyiben szeretné újra megpróbálni kérem írja be a zárójelben lévő szöveget (again), amennyiben  kiszeretné szeretne lépni, gépelje be a zárójelben lévő szöveget ( exit )." << std::endl;
        std::string re_start;
        std::cin >> re_start;
        if (re_start == "again")
            goto label;
        else if (re_start == "exit")
        {
            std::cout << "Ön a program futásának befejezését választotta." << std::endl;
            return 0;
        }
    }
    //**************************************  
    return 0;
}