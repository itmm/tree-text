#include "tt.h"

#include <iostream>

int main() {
	auto root { TT::List::create("root") };
	root->push_back(TT::Word::create("a"));
	root->push_back(TT::Word::create("b"));
	{
		TT::Writer wr;
		root->write(wr);
	}
	std::cout << "OK\n";
}
