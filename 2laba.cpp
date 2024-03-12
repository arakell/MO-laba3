#include <iostream>
#include <vector>
#include "libxl.h"
#include "GradientMethod.cpp"
#include "ConjugateGradMethod.cpp"
#include "Functions.h"
#include "Broiden.cpp"
#include "SecondPirson.cpp"
using namespace libxl;
using namespace std;



int main()
{

    Book* book = xlCreateXMLBook(); // xlCreateXMLBook() for xlsx
    Format* format = book->addFormat();
    format->setNumFormat(book->addCustomNumFormat(L"0.00000"));
    Sheet* sheet;
    vector<double> X1 = { 0, 0 };
    vector<double> X2 = { -1.2, 1 };
    
    
    sheet = book->addSheet(L"Broiden1");
    Broiden* b1 = new Broiden(book, sheet, format, X1, f1, gradf1);
    b1->method();

    sheet = book->addSheet(L"Broiden2");
    Broiden* b2 = new Broiden(book, sheet, format, X2, f2, gradf2);
    b2->method();
    

    
    sheet = book->addSheet(L"SecondPirson1");
    SecondPirson* Sp1 = new SecondPirson(book, sheet, format, X1, f1, gradf1);
    Sp1->method();
    
    sheet = book->addSheet(L"SecondPirson2");
    SecondPirson* Sp2 = new SecondPirson(book, sheet, format, X2, f2, gradf2);
    Sp2->method();
    
    book->save(L"laba2.xlsx");
    book->release();
    return 0;
}
