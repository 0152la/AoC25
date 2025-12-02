#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

const char in_delim = ',';
const char range_delim = '-';

struct in_range
{
    size_t st;
    size_t ed;

    in_range(const std::string& _in)
    {
        this->st = std::stoul(_in.substr(0, _in.find(range_delim)));
        this->ed = std::stoul(_in.substr(_in.find(range_delim) + 1, _in.npos));
    }
};

/* Part 1 *********************************************************************/

size_t
get_invalid_id(size_t _id)
{
    size_t l = std::log10(_id) + 1;
    l = std::pow(10, l);
    l = _id * l + _id;
    return l;
    // return _id * std::pow(10, std::log10(_id) + 1) + _id;
}

size_t
sum_invalid_ids(const in_range& range)
{
    size_t sum = 0;

    std::cout << " -- RANGE " << range.st << "-" << range.ed << '\n';

    size_t curr_id = std::log10(range.st);
    curr_id = range.st / std::pow(10, curr_id / 2 + 1);
    size_t curr_id_check = get_invalid_id(curr_id);

    while (curr_id_check <= range.ed)
    {
        if (range.st <= curr_id_check)
        {
            std::cout << curr_id_check << std::endl;
            sum += curr_id_check;
        }
        curr_id += 1;
        curr_id_check = get_invalid_id(curr_id);
    }

    return sum;
}

/* Part 2 *********************************************************************/

size_t
gen_id(size_t id, size_t digit_count, size_t max_count)
{
    size_t seed = id % (size_t) std::pow(10, digit_count);
    size_t gen = seed;
    for (size_t i = 1; i < max_count / digit_count; ++i)
    {
        gen = gen * std::pow(10, digit_count) + seed;
    }

    return gen;
}

bool
check_duplicated(size_t id)
{
    size_t curr_count = 1;
    size_t max_count = log10(id) + 1;

    while (curr_count <= max_count / 2 + 1)
    {
        if (curr_count == max_count)
        {
            break;
        }
        if (max_count % curr_count == 0
            && id == gen_id(id, curr_count, max_count))
        {
            return true;
        }
        curr_count += 1;
    }
    return false;
}

size_t
sum_duplicated_ids(const in_range& range)
{
    size_t sum = 0;
    std::cout << " -- RANGE " << range.st << "-" << range.ed << '\n';

    for (size_t curr_id = range.st; curr_id <= range.ed; ++curr_id)
    {
        if (check_duplicated(curr_id))
        {
            std::cout << curr_id << " - ";
            sum += curr_id;
        }
    }
    std::cout << std::endl;
    return sum;
}

/* Main ***********************************************************************/

void
do_one_file(std::string _path)
{
    std::vector<in_range> t;
    std::ifstream fd(_path);
    std::string buf;

    std::getline(fd, buf);
    size_t pos = 0;
    while (true)
    {
        size_t sz = buf.find(in_delim, pos) - pos;
        t.push_back(in_range(buf.substr(pos, sz)));
        pos = buf.find(in_delim, pos + 1);
        if (pos == buf.npos)
        {
            break;
        }
        pos += 1;
    }
    fd.close();

    // size_t sum = 0;
    // for (const auto& i : t)
    //{
    // sum += sum_invalid_ids(i);
    //}
    // std::cout << "SUM " << sum << '\n';

    size_t dupes = 0;
    for (const auto& i : t)
    {
        dupes += sum_duplicated_ids(i);
    }
    std::cout << "DUPES " << dupes << '\n';
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
