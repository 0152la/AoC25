#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

const char paper = '@';
const uint8_t lim = 4;

using paper_map = std::vector<std::vector<bool>>;

bool
check_cell(const paper_map& pm, uint16_t row, uint16_t col)
{
    return pm.at(row).at(col);
}

bool
check_accessible(const paper_map& pm, uint16_t row, uint16_t col)
{
    std::vector<std::pair<uint16_t, uint16_t>> check_idxs {
        { row - 1, col - 1 }, { row - 1, col }, { row - 1, col + 1 },
        { row, col - 1 }, { row, col + 1 }, { row + 1, col - 1 },
        { row + 1, col }, { row + 1, col + 1 }
    };
    uint8_t near = 0;

    for (const auto& [c_row, c_col] : check_idxs)
    {
        if (c_row < 0 || c_col < 0 || c_row >= pm.size()
            || c_col >= pm[0].size())
        {
            continue;
        }
        if (check_cell(pm, c_row, c_col))
        {
            near += 1;
        }

        if (near == lim)
        {
            return false;
        }
    }

    return true;
}
/* Part 1 *********************************************************************/


uint16_t
find_accessible(const paper_map& pm)
{
    uint16_t acc = 0;
    for (uint16_t row = 0; row < pm.size(); ++row)
    {
        for (uint16_t col = 0; col < pm[row].size(); ++col)
        {
            if (check_cell(pm, row, col) && check_accessible(pm, row, col))
            {
                std::cout << "ACCESS " << row << " -- " << col << std::endl;
                acc += 1;
            }
        }
    }
    std::cout << "TOTAL " << acc << std::endl;
    return acc;
}

/* Part 2 *********************************************************************/

uint32_t remove_accessible(paper_map& pm)
{
    uint16_t acc = 0;

    for (uint16_t row = 0; row < pm.size(); ++row)
    {
        for (uint16_t col = 0; col < pm[row].size(); ++col)
        {
            if (check_cell(pm, row, col) && check_accessible(pm, row, col))
            {
                acc += 1;
                pm[row][col] = false;
            }
        }
    }
    std::cout << "REMOVED " << acc << std::endl;
    return acc;
}

uint32_t remove_accessible_all(paper_map& pm)
{
    uint16_t acc = 0;
    uint16_t old_acc = -1;

    while (acc != old_acc)
    {
        old_acc = acc;
        acc += remove_accessible(pm);
    }

    std::cout << "TOTAL " << acc << '\n';
    return acc;
}


/* Main ***********************************************************************/

void
add_paper_line(paper_map& m, const std::string line)
{
    std::vector<bool> new_line;
    for (const char c : line)
    {
        new_line.push_back(c == paper ? true : false);
    }
    m.push_back(new_line);
}

void
do_one_file(std::string _path)
{
    paper_map pm;
    std::ifstream fd(_path);
    std::string buf;

    while (std::getline(fd, buf))
    {
        add_paper_line(pm, buf);
    }
    fd.close();

    find_accessible(pm);
    remove_accessible_all(pm);
}

int
main()
{
    std::vector<std::string> tests = { "input1" };
    for (const auto& t : tests)
    {
        std::cout << "== TEST " << t << std::endl;
        do_one_file(t);
    }
    return 0;
}
