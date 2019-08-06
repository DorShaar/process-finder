// ProccessFinder.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include "ProcessFinder.h"

int main(int argumentsCount, char* argv[])
{
     std::cout << "Welcome to Process Finder!\n";

     size_t size = strlen(argv[1]) + 1;
     size_t charactersConvertedCount;
     wchar_t* text = new wchar_t[size];
     mbstowcs_s(&charactersConvertedCount, text, size, argv[1], size - 1); //Plus null.

     ProcessFinder processFinder;
     processFinder.print_using_processes(text);

     delete(text);
}