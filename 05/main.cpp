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

using idd_t = uint64_t;

const char range_delim = '-';

struct range
{
    idd_t lo;
    idd_t hi;

    range(idd_t _lo, idd_t _hi) :
        lo(_lo),
        hi(_hi) { };

    range(std::string s)
    {
        size_t delim_idx = s.find(range_delim);
        this->lo = std::stoull(s.substr(0, delim_idx));
        this->hi = std::stoull(s.substr(delim_idx + 1, s.npos));
    }

    void pr(void) const
    {
        std::cout << "RANGE == LO " << this->lo << " -- HI " << this->hi
                  << std::endl;
    }
};

/* Part 1 *********************************************************************/

uint32_t
check_ids(const std::vector<range> rgs, const std::vector<idd_t> ids)
{
    uint32_t fresh = 0;
    for (idd_t id : ids)
    {
        for (range r : rgs)
        {
            if (id >= r.lo && id <= r.hi)
            {
                std::cout << "FRESH " << id << std::endl;
                fresh += 1;
                break;
            }
        }
    }
    std::cout << "TOTAL " << fresh << std::endl;
    return fresh;
}

/* Part 2 *********************************************************************/

bool
check_overlap(const range r1, const range r2)
{
    return (r2.lo >= r1.lo && r2.lo <= r1.hi)
        || (r1.lo >= r2.lo && r1.lo <= r2.hi);
}

std::vector<range>
consolidate_ranges(const std::vector<range> init)
{
    std::vector<range> new_rgs(init);
    size_t cons_count = 0;

    for (size_t i = 0; i < init.size(); ++i)
    {
        range curr_rg = init[i];
        for (size_t j = i + 1; j < init.size(); ++j)
        {
            range other_rg = init[j];
            if (check_overlap(curr_rg, other_rg))
            {
                curr_rg = range(std::min(curr_rg.lo, other_rg.lo),
                    std::max(curr_rg.hi, other_rg.hi));
                new_rgs.erase(std::next(new_rgs.begin(), j - cons_count));
                cons_count += 1;
            }
        }

        if (cons_count != 0)
        {
            new_rgs.erase(std::next(new_rgs.begin(), i));
            new_rgs.push_back(curr_rg);
            break;
        }
    }

    return new_rgs;
}

uint64_t
consolidate_ranges_all(const std::vector<range> rgs)
{
    uint64_t sz = rgs.size();
    uint64_t old_sz = 0;
    std::vector<range> rgs_buf(rgs);

    while (sz != old_sz)
    {
        old_sz = sz;
        rgs_buf = consolidate_ranges(rgs_buf);
        sz = rgs_buf.size();
    }

    uint64_t sum = 0;
    for (const auto& r : rgs_buf)
    {
        sum += r.hi - r.lo + 1;
    }

    std::cout << "TOTAL " << sum << '\n';

    return sum;
}

/* Main ***********************************************************************/

void
do_one_file(std::string _path)
{
    std::vector<range> rgs;
    std::vector<idd_t> ids;
    std::ifstream fd(_path);
    std::string buf;

    // Parse ranges
    while (std::getline(fd, buf))
    {
        if (buf.empty())
        {
            break;
        }
        rgs.emplace_back(buf);
    }

    // Parse IDs to check
    while (std::getline(fd, buf))
    {
        ids.push_back(std::stoull(buf));
    }
    fd.close();

    // check_ids(rgs, ids);
    consolidate_ranges_all(rgs);
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
