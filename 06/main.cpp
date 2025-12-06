#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using op_t = uint64_t;

struct operation
{
    std::vector<op_t> operands;
    char symbol;

    void add_operand(op_t op) { this->operands.push_back(op); };

    void set_symbol(char s) { this->symbol = s; };

    std::string to_str(void) const
    {
        std::ostringstream ss;
        ss << "SYM " << this->symbol << " -- OPS "
           << std::accumulate(std::next(this->operands.begin()),
                  this->operands.end(), std::to_string(this->operands.front()),
                  [](std::string s, op_t o)
                  { return s + ", " + std::to_string(o); });
        return ss.str();
    }
};

size_t
compute_ops(std::vector<operation> ops)
{
    size_t tot = 0;
    for (const auto& o : ops)
    {
        size_t sum = (o.symbol == '+' ? 0 : 1);
        for (const auto op : o.operands)
        {
            sum = (o.symbol == '+' ? sum + op : sum * op);
        }
        tot += sum;

        std::cout << o.to_str() << " == RESULT " << sum << '\n';
    }
    std::cout << "TOTAL " << tot << std::endl;
    return tot;
}

/* Part 1 *********************************************************************/

std::vector<operation>
parse_ops(const std::vector<std::string> ins)
{
    std::vector<operation> ops;

    std::string buf;
    size_t idx;

    for (size_t i = 0; i < ins.size(); ++i)
    {
        idx = 0;
        std::stringstream line(ins[i]);
        while (std::getline(line, buf, ' '))
        {
            if (buf.empty())
            {
                continue;
            }
            if (i == 0)
            {
                ops.emplace_back();
            }

            if (i == ins.size() - 1)
            {
                assert(buf.size() == 1);
                ops[idx].set_symbol(buf[0]);
            }
            else
            {
                ops[idx].add_operand(std::stoul(buf));
            }
            idx += 1;
        }
    }

    return ops;
}

/* Part 2 *********************************************************************/

std::vector<std::vector<char>>
get_char_matrix(const std::vector<std::string> ins)
{
    std::vector<std::vector<char>> in_matrix;

    for (const auto& row : ins)
    {
        std::vector<char> curr_row;
        for (const char c : row)
        {
            curr_row.push_back(c);
        }
        in_matrix.push_back(curr_row);
    }

    for (const auto& r : in_matrix)
    {
        for (const auto& c : r)
        {
            std::cout << (c == ' ' ? '_' : c);
        }
        std::cout << '\n';
    }

    return in_matrix;
}

std::vector<size_t>
get_space_idxs(const std::vector<char> row)
{
    std::vector<size_t> idxs;
    for (size_t i = 0; i < row.size(); ++i)
    {
        if (row[i] == ' ')
        {
            idxs.push_back(i);
        }
    }
    return idxs;
}

std::vector<size_t>
get_delim_idxs(const std::vector<std::vector<char>> matrix)
{
    std::vector<size_t> space_idxs = get_space_idxs(matrix.front());
    std::vector<size_t> buf;

    for (size_t i = 1; i < matrix.size(); ++i)
    {
        buf.clear();
        std::vector<size_t> new_space_idxs = get_space_idxs(matrix[i]);
        std::set_intersection(space_idxs.begin(), space_idxs.end(),
            new_space_idxs.begin(), new_space_idxs.end(),
            std::back_inserter(buf));
        space_idxs = buf;
    }

    return space_idxs;
}

std::vector<operation>
parse_matrix(const std::vector<std::vector<char>> matrix,
    const std::vector<size_t> space_idxs)
{
    std::vector<operation> ops(space_idxs.size() + 1);
    size_t curr_operation = 0;
    std::string curr_operand = "";

    for (size_t col = matrix[0].size() - 1; ; --col)
    {
        if (std::find(space_idxs.begin(), space_idxs.end(), col)
            != space_idxs.end())
        {
            curr_operation += 1;
            continue;
        }
        for (size_t row = 0; row < matrix.size(); ++row)
        {
            if (matrix[row][col] == ' ')
            {
                continue;
            }

            if (row == matrix.size() - 1)
            {
                ops[curr_operation].set_symbol(matrix[row][col]);
            }
            else
            {
                curr_operand.push_back(matrix[row][col]);
            }
        }
        ops[curr_operation].add_operand(std::stoul(curr_operand));
        curr_operand.clear();

        if (col == 0)
        {
            break;
        }

    }

    for (const auto& o : ops)
    {
        std::cout << "OP " << o.to_str() << std::endl;
    }

    return ops;
}

std::vector<operation>
parse_columns(const std::vector<std::string> ins)
{

    std::vector<std::vector<char>> in_matrix = get_char_matrix(ins);
    std::vector<size_t> in_matrix_delim_cols = get_delim_idxs(in_matrix);

    std::cout << "SPACES "
              << std::accumulate(std::next(in_matrix_delim_cols.begin()),
                     in_matrix_delim_cols.end(),
                     std::to_string(in_matrix_delim_cols.front()),
                     [](std::string s, size_t i)
                     { return s + ", " + std::to_string(i); })
              << std::endl;

    return parse_matrix(in_matrix, in_matrix_delim_cols);
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
        ins.emplace_back(buf);
    }

    // std::vector<operation> ops = parse_ops(ins);
    std::vector<operation> ops = parse_columns(ins);

    std::cout << "TOTAL " << compute_ops(ops) << '\n';
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
