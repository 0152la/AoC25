#include <cassert>
#include <algorithm>
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

        // for (const auto& [c, cf] : fqs)
        //{
        // std::cout << cf.to_str() << '\n';
        //}

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
        std::cout << b2 << std::endl;
        std::cout << "PACK " << b << " -- MAX " << b_max << '\n';
    }

    std::cout << "JOLTAGE " << sum << '\n';
    return sum;
}

/* Part 2 *********************************************************************/

//constexpr uint8_t digit_count = 2;
constexpr uint8_t digit_count = 12;

uint64_t
get_big_joltage(std::vector<std::string> bats)
{
    uint64_t sum = 0;

    for (const auto& b : bats)
    {
        std::string b2(b);
        std::sort(b2.begin(), b2.end());
        char biggest_digit = b2[b2.length() - digit_count];

        std::string new_b;
        for (const char c : b)
        {
            if (c >= biggest_digit)
            {
                new_b.push_back(c);
            }
        }
        uint8_t to_remove = new_b.size() - digit_count;
        std::vector<uint8_t> to_remove_idxs;
        // uint8_t idx = new_b.length() - 1;
        uint8_t idx = 0;

        // std::string::reverse_iterator new_b_rit = new_b.begin();
        std::string::iterator new_b_rit = new_b.begin();
        while (new_b_rit != new_b.end())
        {
            if (*new_b_rit == biggest_digit)
            {
                to_remove_idxs.push_back(idx);
                if (to_remove_idxs.size() == to_remove)
                {
                    break;
                }
            }
            // idx -= 1;
            idx += 1;
            new_b_rit = std::next(new_b_rit);
        }

        idx = 0;
        std::string new_new_b;
        for (const auto& c : new_b)
        {
            if (std::find(to_remove_idxs.begin(), to_remove_idxs.end(), idx)
                == to_remove_idxs.end())
            {
                new_new_b.push_back(c);
            }
            idx += 1;
        }

        std::cout << b << std::endl;
        std::cout << b2 << std::endl;
        std::cout << new_b << std::endl;
        std::cout << new_new_b << std::endl;
    }

    std::cout << "BIG JOLTAGE " << sum << '\n';
    return sum;
}

std::string
find_in_window(const std::string b, size_t out_count)
{
    std::string res;
    std::string work(b);
    size_t left = out_count;
    size_t window = b.size() - out_count + 1;

    //while (work.size() - window > out_count)
    while (window <= work.size() && work.size() > left && work.size() - window >= left - 1)
    {
        std::string buf = work.substr(0, window);
        std::sort(buf.begin(), buf.end());
        res.push_back(buf.back());
        work.erase(
            work.begin(), std::find(work.begin(), work.end(), buf.back()) + 1);
        left -= 1;

        //std::cout << "LEFT " << left << " == WORK " << work << " == RES " << res
                  //<< '\n';
    }

    if (left != 0)
    {
        if (work.size() == left)
        {
            res += work;
        }
        else
        {
            //res += find_in_window(work.substr(0, window), work.size() - left);
            //res += work.substr(window);
            res += find_in_window(work, left);
        }
    }


    std::cout << "IN " << b << '\n';
    std::cout << "RES " << res << '\n';
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

        // j.clear();
        // left_to_find = digit_count;
        // uint8_t window = b.size() - left_to_find;
        // std::string b_work(b);
        // std::cout << b << std::endl;
        // while (b_work.size() - window >= left_to_find)
        //{
        // std::string b2 = b_work.substr(0, window);
        // std::sort(b2.begin(), b2.end());
        // j.push_back(b2.back());

        // b_work.erase(b_work.begin(),
        // std::find(b_work.begin(), b_work.end(), b2.back()) + 1);
        //// b_work.erase(std::find(b_work.begin(), b_work.end(), b2.back()));

        // std::cout << "-----\n";
        // std::cout << b2 << std::endl;
        // std::cout << j << std::endl;
        // std::cout << b_work << std::endl;

        // left_to_find -= 1;
        //}

        //j += b_work;
        //std::cout << j << std::endl;
        //s += std::stoul(j);
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
