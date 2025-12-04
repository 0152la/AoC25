#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

/* Part 1 *********************************************************************/

struct char_freq
{
    uint8_t c;
    uint32_t c_count;
    uint16_t idx_l;
    uint16_t idx_r;

    char_freq(char _c) :
        c(_c - '0'),
        c_count(0),
        idx_l(-1),
        idx_r(0) { };

    std::string to_str(void) const
    {
        std::ostringstream ss;
        ss << "CHAR " << (int) c << " -- COUNT " << c_count << " -- IDX_L "
           << idx_l << " -- IDX_R " << idx_r;
        return ss.str();
    }
};

uint64_t
get_joltage(std::vector<std::string> bats)
{
    uint64_t sum = 0;

    std::map<char, char_freq> fqs;
    uint16_t idx;
    uint16_t b_max;
    for (const auto& b : bats)
    {
        fqs.clear();
        idx = 0;
        for (const char c : b)
        {
            if (fqs.find(c) == fqs.end())
            {
                char_freq new_cf(c);
                new_cf.idx_l = idx;
                fqs.insert(std::make_pair(c, new_cf));
            }
            fqs.at(c).c_count += 1;
            fqs.at(c).idx_r = idx;
            idx += 1;
        }

        if ((*fqs.rbegin()).second.c_count >= 2)
        {
            b_max = (*fqs.rbegin()).second.c * 11;
        }
        else
        {
            char_freq c1 = (*fqs.rbegin()).second;

            auto c_it = std::next(fqs.rbegin());
            while (c_it != fqs.rend())
            {
                if ((*c_it).second.idx_r > c1.idx_l)
                {
                    b_max = c1.c * 10 + c_it->second.c;
                    break;
                }
                c_it = std::next(c_it);
            }
            if (c_it == fqs.rend())
            {
                char_freq c2 = std::next(fqs.rbegin())->second;
                b_max = c2.c * 10 + c1.c;
            }
        }

        sum += b_max;
        std::string b2(b);
        std::sort(b2.begin(), b2.end());
        std::cout << "PACK " << b << " -- MAX " << b_max << '\n';
    }

    std::cout << "JOLTAGE " << sum << '\n';
    return sum;
}

/* Part 2 *********************************************************************/

constexpr uint8_t digit_count = 12;

std::string
find_in_window(const std::string b, size_t out_count)
{
    std::string res;
    std::string work(b);
    size_t left = out_count;
    size_t window = b.size() - out_count + 1;

    while (window <= work.size() && work.size() > left
        && work.size() - window >= left - 1)
    {
        std::string buf = work.substr(0, window);
        std::sort(buf.begin(), buf.end());
        res.push_back(buf.back());
        work.erase(
            work.begin(), std::find(work.begin(), work.end(), buf.back()) + 1);
        left -= 1;
    }

    if (left != 0)
    {
        if (work.size() == left)
        {
            res += work;
        }
        else
        {
            res += find_in_window(work, left);
        }
    }

    assert(res.size() == out_count);
    return res;
}

uint64_t
get_big_joltage2(std::vector<std::string> bats)
{
    uint8_t left_to_find;
    std::string j;
    uint64_t s = 0;

    for (const auto& b : bats)
    {
        std::string v = find_in_window(b, digit_count);
        s += std::stoul(v);
    }

    std::cout << "SUM " << s << std::endl;
    return s;
}

/* Main ***********************************************************************/

void
do_one_file(std::string _path)
{
    std::vector<std::string> t;
    std::ifstream fd(_path);
    std::string buf;

    while (std::getline(fd, buf))
    {
        t.push_back(buf);
    }
    fd.close();

    // get_joltage(t);
    get_big_joltage2(t);
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
