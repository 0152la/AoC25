#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using coord_t = uint32_t;
using dim_t = uint8_t;

constexpr dim_t dims = 3;

struct jbox
{
    std::array<coord_t, dims> coords;

    jbox(std::string _line)
    {
        std::string buf;
        std::istringstream line_ss(_line);
        size_t idx = 0;
        constexpr char delim = ',';
        while (std::getline(line_ss, buf, delim))
        {
            coords[idx] = std::stoul(buf);
            idx += 1;
        }
    }

    std::string to_str(void) const
    {
        std::ostringstream ss;
        ss << "JBOX == X " << coords[0] << " -- Y " << coords[1] << " -- Z "
           << coords[2];
        return ss.str();
    }

    std::string to_str_short(void) const
    {
        std::ostringstream ss;
        ss << "[" << coords[0] << "," << coords[1] << "," << coords[2] << "]";
        return ss.str();
    }
};

size_t
calc_distance(const jbox j1, const jbox j2)
{
    size_t jb_distance = 0;
    for (dim_t i = 0; i < dims; ++i)
    {
        jb_distance
            += std::pow((int32_t) j1.coords[i] - (int32_t) j2.coords[i], 2);
    }
    jb_distance = std::sqrt(jb_distance);

    return jb_distance;
}

struct jbox_dist
{
    size_t dist;
    size_t jb1_idx;
    size_t jb2_idx;

    jbox_dist(size_t _jb1_idx, size_t _jb2_idx, const std::vector<jbox> jbs) :
        jb1_idx(_jb1_idx),
        jb2_idx(_jb2_idx)
    {
        this->dist = calc_distance(jbs.at(_jb1_idx), jbs.at(_jb2_idx));
    }

    std::string to_str(const std::vector<jbox> jbs) const
    {
        std::ostringstream ss;
        ss << "J1 " << jbs.at(this->jb1_idx).to_str_short() << " - J2 "
           << jbs.at(this->jb2_idx).to_str_short() << " DIST " << this->dist;
        return ss.str();
    }

    bool has_idx(size_t idx) { return jb1_idx == idx || jb2_idx == idx; };
};

std::vector<jbox_dist>
calc_all_distances(const std::vector<jbox> boxes)
{
    std::vector<jbox_dist> jd;

    for (size_t i = 0; i < boxes.size() - 1; ++i)
    {
        for (size_t j = i + 1; j < boxes.size(); ++j)
        {
            jd.emplace_back(i, j, boxes);
        }
    }

    std::sort(jd.begin(), jd.end(),
        [](const jbox_dist& jd1, const jbox_dist& jd2)
        { return jd1.dist <= jd2.dist; });

    return jd;
}

/* Part 1 *********************************************************************/

constexpr uint32_t conn_count = 1000;

std::vector<std::set<size_t>>
make_circuits(const std::vector<jbox> boxes, const std::vector<jbox_dist> dists)
{
    std::vector<std::set<size_t>> circs;
    for (const auto& jbox_dist : dists)
    {
        std::set<size_t> np { jbox_dist.jb1_idx, jbox_dist.jb2_idx };
        circs.emplace_back(np);
    }

    bool changed = true;
    while (changed)
    {
        changed = false;
        for (decltype(circs)::iterator p_it = circs.begin();
            p_it != std::prev(circs.end()); p_it = std::next(p_it))
        {
            for (decltype(p_it) o_it = std::next(p_it); o_it != circs.end();
                o_it = std::next(o_it))
            {
                for (size_t idx : *p_it)
                {
                    if (std::find((*o_it).begin(), (*o_it).end(), idx)
                        != (*o_it).end())
                    {
                        (*p_it).insert((*o_it).begin(), (*o_it).end());
                        (*o_it).clear();
                        changed = true;
                    }
                }
            }
        }
    }

    std::erase_if(circs, [](std::set<size_t> c) { return c.empty(); });

    return circs;
}

size_t
get_cable_count(const std::vector<jbox> boxes)
{
    std::vector<jbox_dist> jd = calc_all_distances(boxes);
    decltype(jd)::iterator to_keep = jd.begin();
    std::advance(to_keep, conn_count);
    jd = std::vector<jbox_dist>(jd.begin(), to_keep);

    std::vector<std::set<size_t>> jcs = make_circuits(boxes, jd);

    std::set<size_t> circ_sizes;
    for (const auto& c : jcs)
    {
        circ_sizes.insert(c.size());
    }

    size_t cables = 1;
    size_t idx = 0;
    for (auto it = circ_sizes.rbegin(); idx < 3; it = std::next(it), idx += 1)
    {
        cables *= *it;
    }

    std::cout << "CABLES " << cables << std::endl;
    return cables;
}

/* Part 2 *********************************************************************/

std::pair<size_t, size_t>
get_last_conn(const std::vector<jbox_dist> dists, size_t jb_count)
{
    std::vector<bool> seen(jb_count, false);
    size_t procd = 0;

    for (const auto& d : dists)
    {
        // Ignore processes boxes
        if (seen[d.jb1_idx] && seen[d.jb2_idx])
        {
            continue;
        }

        if (!seen[d.jb1_idx])
        {
            seen[d.jb1_idx] = true;
            procd +=1;
        }

        if (!seen[d.jb2_idx])
        {
            seen[d.jb2_idx] = true;
            procd +=1;
        }

        if (procd == jb_count)
        {
            return std::make_pair(d.jb1_idx, d.jb2_idx);
        }
    }

    assert(false);
}

size_t
get_last_conn_mult(const std::vector<jbox> boxes)
{
    const std::vector<jbox_dist> jd = calc_all_distances(boxes);

    const std::pair<size_t, size_t> end_conn = get_last_conn(jd, boxes.size());

    size_t res = boxes[end_conn.first].coords[0] * boxes[end_conn.second].coords[0];
    std::cout << "RES " << res;
    std::cout << "\n-- IDX1 " << end_conn.first << " - " << boxes[end_conn.first].to_str_short();
    std::cout << "\n-- IDX2 " << end_conn.second << " - " << boxes[end_conn.second].to_str_short();
    std::cout << '\n';
    return res;
}

/* Main ***********************************************************************/

void
do_one_file(std::string _path)

{
    std::vector<jbox> boxes;
    std::ifstream fd(_path);
    std::string buf;

    while (std::getline(fd, buf))
    {
        boxes.emplace_back(buf);
    }

    //get_cable_count(boxes);
    get_last_conn_mult(boxes);
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
