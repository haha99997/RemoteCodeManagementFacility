///////////////////////////////////////////////////////////////////
// Repository.cpp - holds a stack                                //
// ver 1.0                                                       //
// Application: CSE687 Pr#2, Spring 2015                         //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013   //
// Author:      Chao Zuo                                         //
///////////////////////////////////////////////////////////////////

#include "Repository.h"
#include <iostream>

#ifdef TEST_REPOSITORY

int main()
{
	Repository repo;
	repo.stack().push(XmlProcessing::makeTaggedElement(std::string("ok")));
	repo.stack().push(XmlProcessing::makeTaggedElement(std::string("ok")));
	std::cout << repo[0].top() << repo[1].top() << std::endl;
}

#endif