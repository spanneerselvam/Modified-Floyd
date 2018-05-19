#include <iostream>
#include <fstream>
#include "digraph.h"

using namespace std;

int main(int argc, char *argv[])
{

    vector<string> names {"apsp1.txt", "apsp2.txt", "apsp3.txt", "apsp4.txt"};

    for (auto s: names)
    {
	    ifstream is(s);
	    if (is.fail())
	    {
		    cout << "Can't open file!" << endl;
		    is.close();
		    return 1;
	    }

	    digraph G;
	    is >> G;
	    cout << "Input graph:" << endl;
	    cout << G << endl;

	    cout << "Output: " << endl;
	    unordered_set<string> S = G.V();
	    std::vector<string> V;
	    for (auto e: S)
		    V.push_back(e);

	    sort(V.begin(), V.end());
	    cout << "Vertices in sorted order: " << endl;
	    for (auto v: V)
		    cout << v << " ";
	    cout << endl;
	    cout << endl;

	    vector<vector<size_t>> M = G.apsp();
	    for (size_t r = 0; r < G.n(); ++r)
	    {
		    for (size_t c = 0; c < G.n(); ++c)
			    cout << M[r][c] << " ";
		    cout << endl;
	    }
	    is.close();
    }

    return 0;
}

