#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

/* Part 1 *********************************************************************/

/* Part 2 *********************************************************************/

/* Main ***********************************************************************/

void
do_one_file(std::string _path)
{
    std::vector<std::string> ins;
    std::ifstream fd(_path);
    std::string buf;

    while (std::getline(fd, buf))
    {
        ins.emplace_back(buf);
    }
}

int
main()
{
    std::vector<std::string> tests = { "input" };
    for (const auto& t : tests)
    {
        std::cout << "== TEST " << t << std::endl;
        do_one_file(t);
    }
    return 0;
}
