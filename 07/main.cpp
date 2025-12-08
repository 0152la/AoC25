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

const char source = 'S';
const char mirror = '^';
const char air = '.';

/* Part 1 *********************************************************************/

size_t
count_splits(std::vector<std::string> layout)
{
    std::vector<bool> beam_front(layout[0].size(), false);
    size_t split_count = 0;

    for (size_t i = 0; i < layout.size(); ++i)
    {
        std::vector<bool> next_front(layout[0].size(), false);
        for (size_t j = 0; j < layout[0].size(); ++j)
        {
            // Starting
            if (layout[i][j] == source)
            {
                next_front[j] = true;
            }

            if (beam_front[j])
            {
                // Continuing
                if (layout[i][j] == air)
                {
                    next_front[j] = true;
                }
                // Splitting
                if (layout[i][j] == mirror)
                {
                    next_front[j - 1] = true;
                    next_front[j + 1] = true;
                    next_front[j] = false;
                    split_count += 1;
                }
            }
        }
        beam_front = next_front;
    }

    std::cout << "SPLITS " << split_count << std::endl;
    return split_count;
}

/* Part 2 *********************************************************************/

size_t
count_timelines_old(std::vector<std::string> layout)
{
    std::vector<size_t> particles;

    particles.push_back(std::distance(layout[0].begin(),
        std::find(layout[0].begin(), layout[0].end(), source)));

    for (size_t i = 1; i < layout.size(); ++i)
    {
        std::cout << "ROW " << i << std::endl;
        std::vector<size_t> new_particles;
        for (size_t p : particles)
        {
            {
                // Continuing
                if (layout[i][p] == air)
                {
                    new_particles.push_back(p);
                }
                // Splitting
                if (layout[i][p] == mirror)
                {
                    new_particles.push_back(p - 1);
                    new_particles.push_back(p + 1);
                }
            }
        }
        particles = new_particles;
    }

    std::cout << "TIMELINES " << particles.size() << std::endl;
    return particles.size();
}

size_t
count_timelines(std::vector<std::string> layout)
{
    std::vector<size_t> particle_front(layout[0].size(), false);
    size_t source_id = std::distance(layout[0].begin(),
        std::find(layout[0].begin(), layout[0].end(), source));
    particle_front[source_id] = 1;

    for (size_t i = 1; i < layout.size(); ++i)
    {
        for (size_t j = 0; j < layout[0].size(); ++j)
        {
            // Splitting
            if (layout[i][j] == mirror)
            {
                particle_front[j - 1] += particle_front[j];
                particle_front[j + 1] += particle_front[j];
                particle_front[j] = 0;
            }
        }
    }

    for (size_t i = 0; i < particle_front.size(); ++i)
    {
        if (particle_front[i] != 0)
        {
            std::cout << particle_front[i] << ",";
        }
    }
    std::cout << std::endl;

    size_t t = 0;
    for (size_t p : particle_front)
    {
        t += p;
    }

    //size_t timelines
        //= std::accumulate(particle_front.begin(), particle_front.end(), decltype(particle_front)::value_type(0),
            //[](size_t acc, size_t p_count) { return acc + p_count; });
    size_t timelines { std::accumulate(particle_front.begin(), particle_front.end(), 0,
            [](size_t acc, size_t p_count) { return acc + p_count; }) };
    //auto timelines = size_t {};

    std::cout << "TIMELINES " << timelines << std::endl;
    std::cout << "OTHER " << t << std::endl;
    return timelines;
}

/* Main ***********************************************************************/

void
do_one_file(std::string _path)
{
    std::vector<std::string> ins;
    std::ifstream fd(_path);
    std::string buf;

    while (std::getline(fd, buf))
    {
        ins.push_back(buf);
    }

    // count_splits(ins);
    count_timelines(ins);
    //count_timelines_old(ins);
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
