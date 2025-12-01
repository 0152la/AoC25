#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

constexpr uint8_t max_val = 100;
constexpr uint8_t init_val = 50;

int16_t
rotate(std::vector<std::string> rot_seq)
{
    int16_t val = init_val;
    uint16_t zero_count = 0;
    for (const std::string& rot : rot_seq)
    {
        uint32_t to_rot = std::stoul(rot.substr(1)) % max_val;
        switch (rot[0])
        {
            case 'L':
                val -= to_rot - max_val;
                break;
            case 'R':
                val += to_rot;
                break;
            default:
                throw std::runtime_error("Unhandled direction");
        }
        val %= max_val;
        assert(val >= 0);
        if (val == 0)
        {
            zero_count += 1;
        }
    }
    return zero_count;
}

int32_t
rotate_clicks(std::vector<std::string> rot_seq)
{
    int32_t val = init_val;
    uint32_t zero_count = 0;
    uint32_t click_count = 0;
    bool was_zero;
    for (const std::string& rot : rot_seq)
    {
        int32_t to_rot = std::stoul(rot.substr(1));
        if (rot[0] == 'L')
        {
            to_rot = -to_rot;
        }

        was_zero = val == 0;

        val += to_rot;

        if (val == 0)
        {
            zero_count += 1;
        }
        else if (val % max_val == 0)
        {
            click_count += std::abs(val) / max_val - 1;
            zero_count += 1;
            val = 0;
        }
        else
        {
            click_count += std::abs(val) / max_val;
            val = val % max_val;
            if (val < 0)
            {
                if (!was_zero)
                {
                    click_count += 1;
                }
                val += max_val;
            }
        }
    }
    std::cout << "CLICK " << click_count << " -- ZERO " << zero_count
              << " -- TOTAL " << click_count + zero_count << std::endl;
    return zero_count + click_count;
}

int32_t
brute_force_clicks(std::vector<std::string> rot_seq)
{
    int32_t val = init_val;
    uint32_t zero_count = 0;
    uint32_t click_count = 0;
    int32_t diff;
    for (const std::string& rot : rot_seq)
    {
        int32_t to_rot = std::stoul(rot.substr(1));
        diff = rot[0] == 'L' ? -1 : 1;
        for (size_t i = 0; i < std::abs(to_rot); ++i)
        {
            val += diff;
            if (val % max_val == 0)
            {
                val = 0;
            }
            if (val == 0)
            {
                click_count += 1;
            }
        }
        if (val == 0)
        {
            click_count -= 1;
            zero_count += 1;
        }
    }
    std::cout << "CLICK " << click_count << " -- ZERO " << zero_count
              << " -- TOTAL " << click_count + zero_count << std::endl;
    return zero_count + click_count;
}

void
do_one_test(std::vector<std::string>&& rot_test)
{
    std::string test_str = std::accumulate(std::next(rot_test.begin()),
        rot_test.end(), rot_test.front(),
        [](std::string s, std::string n) { return s + ',' + n; });
    rotate_clicks(rot_test);
}

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
    if (rotate_clicks(t) != brute_force_clicks(t))
    {
        std::cout << "Not matching result for " << _path << std::endl;
    }
}

int
main()
{
    std::vector<std::string> tests
        = { "input", "input2", "input3", "input4", "input5" };
    for (const auto& t : tests)
    {
        do_one_file(t);
    }
    return 0;
}
