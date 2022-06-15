#include "wordclass.h"

Word::Word(const std::string &param_string, const std::string &param_character, int pcount)
{
    if (param_string.length() <= MAX_STRING_LENGHT)
    {
        szo = param_string;
    }
    else
    {
        std::cerr << "Nem hozható létre ekkora mérretű szó!" << std::endl;
        throw std::length_error("Nem hozható létre ekkora mérretű szó!");
    }
    setwordCounter(pcount);
    setCode(param_character);
}
Word::Word(const Word &theother)
{
    setCode("00");
    *this = theother;
}
Word &Word::operator=(const Word &theother)
{
    if (this->GetwordLenght() == 0)
    {
        this->SetwordDefault();
    }

    if (this == &theother) // önértékadás
        return *this;

    this->setCode(theother.code);
    szo = theother.szo;
    wordcounter = theother.wordcounter;

    return *this;
}
void Word::setwordCounter(const int& param)
{
    if (param >= WORD_COUNT_DEFAULT && param < MAX_WORD_COUNT)
    {
        wordcounter = param;
        return;
    }
    else
    {
        std::cerr<< "Érvénytelen számláló beállítás" << std::endl;
        throw std::invalid_argument("Érvénytelen szószámláló beállítás");
    }
}
inline void Word::SetwordDefault()
{
    szo = "default";
    setCode("00");
    wordcounter = WORD_COUNT_DEFAULT;
    return;
}
inline int Word::GetwordLenght() const
{
    return szo.length();
}
inline bool Word::IswordsEqual(const Word &theOther) const
{
    return this->szo == theOther.szo;
}
inline int Word::GetwordCount() const
{
    return wordcounter;
}
inline const std::string& Word::getWord() const
{
    return szo;
}
inline const std::string& Word::getCode() const
{
    return code;
}
bool Word::setCode(const std::string &character_param)
{
    if (code == character_param)
        return true;
    if (character_param.length() > 2)
    {
       // std::cerr <<  "Érvénytelen méretű kód a file-ban"   << std::endl;     
        throw std::length_error("Érvénytelen méretű kód a file-ban");
    }

    if (character_param == "00" ||
        character_param == "!1" ||
        character_param == "!2" ||
        character_param == "!3" ||
        character_param == "!4" ||
        character_param == "!5")
    {
        code = character_param;
        return true;
    }
    else
    { 
        //std::cerr << "Érvénytelen formátumú kód!" << std::endl;
        throw  std::invalid_argument("Érvénytelen formátumú kód");
    }
}
void Word::setWord(const std::string &word_param)
{
    if (this->szo == word_param)
        return;
    if (word_param.length() > MAX_STRING_LENGHT)
    {
        //std::cerr << "Érvénytelen méretű szó a file-ban" << std::endl;
        throw std::invalid_argument("Érvénytelen méretű szó a file-ban");
    }
    else
        szo = word_param;
}
bool Word::isCode(const std::string &character_param) const
{
    if (
        character_param == "!1" ||
        character_param == "!2" ||
        character_param == "!3" ||
        character_param == "!4" ||
        character_param == "!5"  )
        return true;
    return false;
}
bool Word::IsArrayElement(const Word &param, Word *codes, const int limit) const
{
    for (int i = 0; i < limit; i++)
        if (codes[i].IswordsEqual(param))
            return true;

    return false;
}
std::string Word::GetCodeFromArray(const Word &param, Word *codes, const int limit) 
{
    for (int i = 0; i < limit; i++)
        if (codes[i].getWord() == param.getWord())
            return codes[i].getCode();

    //Ha nem eleme, baj van ->
    return "00";
}
std::string Word::GetWordFromArray(const std::string &param_code, Word *words, const int limit) 
{
    for (int i = 0; i < limit; i++)
        if (words[i].getCode() == param_code)
            return words[i].getWord();
    //Ha valami hiba volt ->
    return "default";
}
std::istream& operator>>(std::istream& input,  Word& word)
{
    std::string temp;
    input>>temp;
    word = Word(temp);
    return input;
}
std::ostream& operator<<(std::ostream& output, const Word& word)
{
    output<<"Word: "<<word.getWord()<<"\tWord's Code "<<word.getCode()<<"\tWord Counted "<<word.GetwordCount()<<" times \t"<<"Word's length"<<word.GetwordLenght();
    return output;
}
