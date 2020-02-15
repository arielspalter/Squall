// The MIT License (MIT)
// Copyright © 2020 Ariel Spalter

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
// and associated documentation files (the “Software”), to deal in the Software without 
// restriction, including without limitation the rights to use, copy, modify, merge, publish, 
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or 
// substantial portions of the Software.

// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#include "series.h"
#include <memory>
#include <map>
#include <iomanip>
#include <numeric>
#ifdef _WIN32
#define CLEAR "cls"
#else //In any other OS
#define CLEAR "clear"
#endif
typedef std::vector<std::string> __Column;
typedef std::tuple<std::vector<double>, std::vector<double>, std::vector<double>, std::vector<int>> __GroupBy;
typedef std::vector<double> __Aggregates;
#ifndef DATAFRAME_H
#define DATAFRAME_H
extern int all_rows;
//prototype global functions
__Indices AND(__Indices condition_1, __Indices condition_2)
{
  __Indices temp(condition_1.size() + condition_2.size());
  __Indices returned_vector;
  std::sort(condition_1.begin(), condition_1.end());
  std::sort(condition_2.begin(), condition_2.end());
  __Indices::iterator it, st;
  it = std::set_intersection(condition_1.begin(), condition_1.end(),
                             condition_2.begin(), condition_2.end(),
                             temp.begin());

  for (st = temp.begin(); st != it; st++)
  {
    returned_vector.push_back(*st);
  }
  return returned_vector;
}
__Indices OR(__Indices condition_1, __Indices condition_2)
{
  __Indices temp(condition_1.size() + condition_2.size());
  __Indices returned_vector;
  std::sort(condition_1.begin(), condition_1.end());
  std::sort(condition_2.begin(), condition_2.end());
  __Indices::iterator it, st;
  it = std::set_union(condition_1.begin(), condition_1.end(),
                      condition_2.begin(), condition_2.end(),
                      temp.begin());

  for (st = temp.begin(); st != it; st++)
  {
    returned_vector.push_back(*st);
  }

  return returned_vector;
}
// struct to hold range of indexes
struct get_index_range
{
  int range_begin;
  int range_end;
};
// struct to obtain and hold column headers
struct column_names
{
  __Column all_names;
  column_names(std::initializer_list<std::string> names)
  {
    all_names = names;
  }
  column_names(__Table table)
  {
    for (size_t i = 0; i < table[0].size(); ++i)
    {
      all_names.push_back(table[0][i]);
    }
  }
  __Column SelectedColumns()
  {
    __Column returned_vector;
    for (size_t i = 0; i < all_names.size(); ++i)
    {
      returned_vector.push_back(all_names[i]);
    }
    return returned_vector;
  }
};
// struct to hold any row's data
struct rows_data
{
  __Row row;
  int number_of_cells;
  rows_data(std::initializer_list<std::string> cells)
  {
    for (auto &cell : cells)
    {
      row.push_back(cell);
    }
    number_of_cells = row.size();
  }
};

class DataFrame
{
public:
  __Table raw_data;
  bool has_header;
  DataFrame() {}

  DataFrame(std::initializer_list<rows_data> cells)
  {
    std::vector<int> row_sizes;

    for (auto &cell : cells)
    {
      row_sizes.push_back(cell.row.size());
      if (std::adjacent_find(row_sizes.begin(), row_sizes.end(), std::not_equal_to<>()) != row_sizes.end())
      {
        std::cout << "All rows must be of the same size" << '\n';
        raw_data = empty_table();
        return;
      }
      else
      {
        __Row r;
        for (auto &c : cell.row)
        {
          r.push_back(c);
        }
        raw_data.push_back(r);
      }
    }
  }
// overload [] to subset row by index
  Series<std::string> operator[](const int &index)
  {
    int max_width = FindMaxWidth();
    Series<std::string> returned_series("row " + std::to_string(index));

    for (size_t i = 0; i < raw_data[index].size(); ++i)
    {
      if (index > raw_data.size())
      {
        std::cout << "row number is out of range" << '\n';
        return returned_series;
      }
      returned_series._data.push_back(raw_data[index][i]);
    }
    return returned_series;
  }

  // overload [] to subset row by range
  DataFrame operator[](get_index_range gir)
  {
    DataFrame returned_dataframe;
    __Row headers;
    if (has_header)
    {
      for (size_t i = 0; i < raw_data[0].size(); ++i)
      {
        headers.push_back(raw_data[0][i]);
      }
    }
    else
    {
      for (size_t i = 0; i < raw_data[0].size(); ++i)
      {
        headers.push_back("");
      }
    }
    std::cout.sync_with_stdio(false);
    returned_dataframe.raw_data.push_back(headers);
    if (gir.range_end >= raw_data.size())
    {
      __Row error;
      error.push_back("DataFrame was not created since selected rows were out of range");
      // std::cout << "End column is out of range. Only the begining of the range is included" << '\n';
      returned_dataframe.raw_data.clear();
      returned_dataframe.raw_data.push_back(error);
      return returned_dataframe;
    }

    for (size_t i = gir.range_begin; i <= gir.range_end; ++i)
    {
      __Row line;

      for (size_t j = 0; j < raw_data[gir.range_begin].size(); ++j)
      {
        line.push_back(raw_data[i][j]);
      }

      returned_dataframe.raw_data.push_back(line);
    }
    return returned_dataframe;
  }

  // overload [] to subset column by header

  Series<std::string> operator[](const char *inp)
  {

    std::string s = inp;
    Series<std::string> returned_series(s);
    returned_series.number_of_rows = raw_data.size() - 1;
    returned_series.series_has_headers = has_header;
    int column_number;
    bool column_exists = false;
    for (size_t i = 0; i < raw_data[0].size(); ++i)
    {
      if (ToLower(raw_data[0][i]) == ToLower(s))
      {
        column_number = i;
        column_exists = true;
      }
    }
    if (column_exists)
    {
      for (size_t i = 0; i < raw_data.size(); ++i)
      {
        returned_series._data.push_back(raw_data[i][column_number]);
      }
    }
    else
    {
      std::cout << "column not found" << '\n';
      return returned_series;
    }
    return returned_series;
  }
  
  void Shape()
  {
    _shape();
    std::cout << "# " << number_of_rows << " Rows" << '\n';
    std::cout << "# " << number_of_columns << " Columns" << '\n';
    if (has_header)
    {
      std::cout << std::setw(15) << "** Data Frame includes headers row" << '\n';
    }
    else
    {
      std::cout << std::setw(15) << "** Data Frame does not include headers row" << '\n';
    }
  }

  DataFrame &FillAllNa()
  {
    for (size_t i = 0; i < raw_data.size(); ++i)
    {
      for (size_t j = 0; j < raw_data[0].size(); ++j)
      {
        if (raw_data[i][j] == "" || raw_data[i][j] == " " || raw_data[i][j].empty())
        {
          raw_data[i][j] = "N/A";
        }
      }
    }
    return *this;
  }
  DataFrame &NaToMean(const char *column_name)
  {
    int column_number;
    bool column_exists = false;
    std::string name = column_name;
    for (size_t i = 0; i < raw_data[0].size(); ++i)
    {
      if (ToLower(raw_data[0][i]) == ToLower(column_name))
      {
        column_number = i;
        column_exists = true;
      }
    }
    double mean = (*this)[column_name].Mean();
    if (column_exists && mean != 0)
    {
      for (size_t i = 0; i < raw_data.size(); ++i)
      {
        if (raw_data[i][column_number] == "" || raw_data[i][column_number] == " " || raw_data[i][column_number].empty())
        {
          raw_data[i][column_number] = std::to_string(mean);
        }
      }
    }
    return *this;
  }
  DataFrame &NaToSum(const char *column_name)
  {
    int column_number;
    bool column_exists = false;
    std::string name = column_name;
    double sum = (*this)[column_name].Sum();
    for (size_t i = 0; i < raw_data[0].size(); ++i)
    {
      if (ToLower(raw_data[0][i]) == ToLower(column_name))
      {
        column_number = i;
        column_exists = true;
      }
    }
    if (column_exists && sum != 0)
    {
      for (size_t i = 0; i < raw_data.size(); ++i)
      {
        if (raw_data[i][column_number] == "" || raw_data[i][column_number] == " " || raw_data[i][column_number].empty())
        {
          raw_data[i][column_number] = std::to_string(sum);
        }
      }
    }
    return *this;
  }
  DataFrame &NaToSTDEV(const char *column_name)
  {
    int column_number;
    bool column_exists = false;
    std::string name = column_name;
    double stdv = (*this)[column_name].STDEV();
    for (size_t i = 0; i < raw_data[0].size(); ++i)
    {
      if (ToLower(raw_data[0][i]) == ToLower(column_name))
      {
        column_number = i;
        column_exists = true;
      }
    }
    if (column_exists && stdv != 0)
    {
      for (size_t i = 0; i < raw_data.size(); ++i)
      {
        if (raw_data[i][column_number] == "" || raw_data[i][column_number] == " " || raw_data[i][column_number].empty())
        {
          raw_data[i][column_number] = std::to_string(stdv);
        }
      }
    }
    return *this;
  }
  DataFrame &NaToVariance(const char *column_name)
  {
    int column_number;
    bool column_exists = false;
    std::string name = column_name;
    double variance = (*this)[column_name].Variance();
    for (size_t i = 0; i < raw_data[0].size(); ++i)
    {
      if (ToLower(raw_data[0][i]) == ToLower(column_name))
      {
        column_number = i;
        column_exists = true;
      }
    }
    if (column_exists && variance != 0)
    {
      for (size_t i = 0; i < raw_data.size(); ++i)
      {
        if (raw_data[i][column_number] == "" || raw_data[i][column_number] == " " || raw_data[i][column_number].empty())
        {
          raw_data[i][column_number] = std::to_string(variance);
        }
      }
    }
    return *this;
  }
  DataFrame &NaToZero(const char *column_name)
  {
    int column_number;
    bool column_exists = false;
    std::string name = column_name;
    for (size_t i = 0; i < raw_data[0].size(); ++i)
    {
      if (ToLower(raw_data[0][i]) == ToLower(column_name))
      {
        column_number = i;
        column_exists = true;
      }
    }
    if (column_exists)
    {
      for (size_t i = 0; i < raw_data.size(); ++i)
      {
        if (raw_data[i][column_number] == "" || raw_data[i][column_number] == " " || raw_data[i][column_number].empty())
        {
          raw_data[i][column_number] = "0";
        }
      }
    }
    return *this;
  }
  DataFrame &FormatHeaders()
  {
    for (size_t j = 0; j < raw_data[0].size(); ++j)
    {
      for (int i = 0; i < raw_data[0][j].size(); ++i)
      {
        if (raw_data[0][j][i] == ' ')
        {
          raw_data[0][j][i] = '_';
        }
      }
    }
    return *this;
  }

  void PrintDataFrame()
  {
    if (raw_data.size() == 0 || raw_data.empty())
    {
      std::cout << "dataframe is empty";
      return;
    }
    int max_width = FindMaxWidth() + 2;
    // auto variable_width = ColumnWidth();
    auto variable_width = ColumnWidth();
    std::cout.sync_with_stdio(false);
    for (size_t i = 0; i < raw_data.size(); ++i)
    {
      std::cout << '\n';
      std::string row_number = std::to_string(i + 1);
      row_number.resize(5);
      std::cout << "## " << row_number << " |";
      for (size_t j = 0; j < raw_data[0].size(); ++j)
      {
        std::cout << std::left << std::setw(variable_width[j] + 3) << raw_data[i][j] << "|";
      }
    }
  }
  // print entire dataframe
  void PrintDataFrame(bool fixed_width)
  {
    int max_width = FindMaxWidth() + 2;
    auto variable_width = ColumnWidth();
    auto acc = std::accumulate(variable_width.begin(), variable_width.end(), 0);
    std::cout.sync_with_stdio(false);
    for (size_t i = 0; i < raw_data.size(); ++i)
    {
      // std::cout <<std::setw(5)<< std::setfill('_')<<'\n'<<'\n';
      std::cout << '\n';

      std::string row_number = std::to_string(i + 1);
      row_number.resize(5);

      std::cout << "## " << row_number << " |";
      for (size_t j = 0; j < raw_data[0].size(); ++j)
      {
        if (!fixed_width)
        {
          std::cout << std::left << std::setw(variable_width[j] + 3) << raw_data[i][j] << "|";
        }
        else
        {
          std::cout << std::right << std::setw(max_width) << raw_data[i][j] << "|";
        }
      }
    }
  }
  void PrintRow(const int &row_index)
  {
    int max_width = FindMaxWidth();
    Series<std::string> returned_series("row " + std::to_string(row_index));

    Series<std::string> headers;
    for (size_t i = 0; i < raw_data[0].size(); ++i)
    {
      headers._data.push_back(raw_data[0][i]);
    }

    for (size_t i = 0; i < raw_data[row_index].size(); ++i)
    {
      if (row_index > raw_data.size())
      {
        std::cout << '\n'
                  << '\n'
                  << "row number is out of range" << '\n';
        return;
      }
      returned_series._data.push_back(raw_data[row_index][i]);
    }
    std::cout.sync_with_stdio(false);
    if (has_header)
    {
      std::string filler = "         ";
      std::cout << '\n'
                << filler << " #";
      for (auto &x : headers._data)
      {
        std::cout << std::setw(max_width + 2) << x;
      }
    }
    std::cout << '\n';
    std::string row_number = std::to_string(row_index);
    row_number.resize(5);
    std::cout << '\n'
              << "row " << row_number << " #";
    for (auto &y : returned_series._data)
    {
      std::cout << std::setw(max_width + 2) << y;
    }
  }
  // print subset of rows
  void PrintRows(const int &begin, const int &end)
  {
    int max_width = FindMaxWidth();
    Series<std::string> headers;

    for (size_t i = 0; i < raw_data[0].size(); ++i)
    {
      headers._data.push_back(raw_data[0][i]);
    }
    std::cout.sync_with_stdio(false);
    if (has_header)
    {
      std::string filler = "         ";
      std::cout << '\n'
                << filler << " #";
      for (auto &x : headers._data)
      {
        std::cout << std::setw(max_width + 2) << x;
      }
    }
    if (end >= raw_data.size())
    {
      std::cout << '\n'
                << '\n'
                << "row number is out of range" << '\n';
      return;
    }
    for (size_t i = begin; i <= end; ++i)
    {
      std::string row_number = std::to_string(i);
      row_number.resize(5);
      std::cout << '\n'
                << "row " << row_number << " #";
      for (size_t j = 0; j < raw_data[begin].size(); ++j)
      {
        std::cout << std::setw(max_width + 2) << raw_data[i][j];
      }
    }
  }
  void PrintColumn(const std::string &column_name)
  {
    int column_number;
    bool column_exists = false;
    for (size_t i = 0; i < raw_data[0].size(); ++i)
    {
      if (ToLower(raw_data[0][i]) == ToLower(column_name))
      {
        column_number = i;
        column_exists = true;
      }
    }
    if (column_exists)
    {
      for (size_t i = 0; i < raw_data.size(); ++i)
      {
        std::string row_number = std::to_string(i + 1);
        row_number.resize(5);
        std::cout << "## " << row_number << " # " << raw_data[i][column_number] << '\n';
      }
    }
    else
    {
      std::cout << "column not found" << '\n';
    }
  }
  // print columns by range
  void PrintColumns(const int &begin, const int &end)
  {
    int max_width = FindMaxWidth() + 2;
    auto variable_width = ColumnWidth();
    if (end >= raw_data[0].size())
    {
      bool column_exists = false;
      std::cout << "columns are out of range" << '\n';
      return;
    }
    std::cout.sync_with_stdio(false);
    for (size_t i = 0; i < raw_data.size(); ++i)
    {
      std::cout << '\n';
      std::string row_number = std::to_string(i + 1);
      row_number.resize(5);
      std::cout << "## " << row_number << " #";
      for (size_t j = begin; j <= end; ++j)
      {
        std::cout << std::right << std::setw(variable_width[j] + 3) << raw_data[i][j];
      }
    }
  }
  DataFrame DropNa(const int &axis, const int &number_of_na)
  {
    DataFrame returned_dataframe;
    std::map<int, int> n;
    // auto it = n.begin();
    raw_data = raw_data;
    __Indices indexes;
    if (axis == 0)
    {
      for (size_t i = 0; i < raw_data.size(); ++i)
      {
        int count_na = 0;
        for (size_t j = 0; j < raw_data[0].size(); ++j)
        {
          if (raw_data[i][j] == "" || raw_data[i][j] == " " || raw_data[i][j].empty() || raw_data[i][j] == "N/A")
          {
            count_na++;
            if (count_na > number_of_na)
            {
              indexes.push_back(i);
            }
          }
        }
      }
      for (size_t i = 0; i < indexes.size(); ++i)
      {
        remove_rows(raw_data, indexes[i] - i);
      }
    }
    else if (axis == 1)
    {
      for (size_t i = 0; i < raw_data.size(); ++i)
      {

        for (size_t j = 0; j < raw_data[0].size(); ++j)
        {
          if (raw_data[i][j] == "" || raw_data[i][j].empty() || raw_data[i][j] == "N/A")
          {
            n[j]++;
          }
        }
      }
      for (std::map<int, int>::iterator it = n.begin(); it != n.end(); it++)
      {
        if (it->second > number_of_na)
        {
          indexes.push_back(it->first);
        }
      }
      std::sort(indexes.begin(), indexes.end());

      for (size_t i = 0; i < raw_data.size(); ++i)
      {
        for (size_t j = 0; j < indexes.size(); ++j)
        {
          remove_columns(raw_data[i], indexes[j] - j);
        }
      }
    }
    else
    {
      std::cout << "Axis has to be either 0 for rows or 1 for columns";
      *this = empty_datafarme();
    }

    // std::cout << "column number: " << j << " has: " << count_na << '\n';

    return *this;
  }
  inline DataFrame &DropNa()
  {
    *this = drop_na();
    return *this;
  }
  DataFrame &DropNa(const int &axis)
  {
    if (axis == 0)
    {
      *this = drop_na();
    }
    else if (axis == 1)
    {
      *this = drop_na(true);
    }
    else
    {
      std::cout << "Axis has to be either 0 for rows or 1 for columns";
      *this = empty_datafarme();
      return *this;
    }

    return *this;
  }

  void DescribeNa()
  {
    float number_of_rows = raw_data.size();
    // if (has_header)
    // {
    //   number_of_rows = number_of_rows - 1;
    // }
    std::map<std::string, int> na_count;
    std::cout << "--- Count Missing Values by Column ---" << '\n';
    std::cout << "DataFrame has " << number_of_rows << " rows in total" << '\n';
    std::cout << "-------------------------------------" << '\n';
    for (size_t i = 0; i < raw_data.size(); ++i)
    {

      for (size_t j = 0; j < raw_data[0].size(); ++j)
      {
        int number_of_na = 0;
        if (raw_data[i][j] == "" || raw_data[i][j] == " " || raw_data[i][j].empty() || raw_data[i][j] == "N/A")
        {
          na_count[raw_data[0][j]]++;
        }
      }
    }

    if (na_count.size() == 0)
    {
      std::cout << "** No missing values were found!" << '\n';
    }
    for (std::map<std::string, int>::iterator it = na_count.begin(); it != na_count.end(); it++)
    {
      float d = (*it).second / number_of_rows * 100;
      std::cout << (*it).first << " has: " << (*it).second << " missing values (" << std::setprecision(3) << d << "% of total # of rows)" << '\n';
    }
  }

  DataFrame &RenameColumn(const std::string &original_name, const std::string &modified_name)
  {
    for (size_t i = 0; i < raw_data.size(); ++i)
    {
      for (size_t j = 0; j < raw_data[0].size(); ++j)
      {
        if (ToLower(raw_data[0][j]) == ToLower(original_name))
        {
          raw_data[0][j] = modified_name;
        }
      }
    }
    return *this;
  }
  DataFrame &RenameColumns(std::initializer_list<std::string> columns_names)
  {
    __Column input_names;
    for (auto &column_name : columns_names)
    {
      input_names.push_back(column_name);
    }
    if (input_names.size() > raw_data[0].size())
    {
      std::cout << "Too many columns!" << '\n';
      return *this;
    }
    for (size_t i = 0; i < input_names.size(); ++i)
    {
      for (size_t j = 0; j < raw_data[0].size(); ++j)
      {
        raw_data[0][i] = input_names[i];
      }
    }
    return *this;
  }
  DataFrame &Replace(const std::string &original_value, const std::string &new_value)
  {
    for (size_t i = 0; i < raw_data.size(); ++i)
    {

      for (size_t j = 0; j < raw_data[0].size(); ++j)
      {
        if (ToLower(raw_data[i][j]) == ToLower(original_value))
        {
          raw_data[i][j] = new_value;
        }
      }
    }
    return *this;
  }

  DataFrame &IsNull()
  {
    raw_data = raw_data;

    for (size_t i = 1; i < raw_data.size(); ++i)
    {
      for (size_t j = 0; j < raw_data[0].size(); ++j)
      {
        if (raw_data[i][j] == "" || raw_data[i][j] == " " || raw_data[i][j].empty())
        {
          raw_data[i][j] = "true";
        }
        else
        {
          raw_data[i][j] = "false";
        }
      }
    }
    return *this;
  }
  DataFrame &IsNull(std::string if_null, std::string if_not_null)
  {
    raw_data = raw_data;

    for (size_t i = 1; i < raw_data.size(); ++i)
    {
      for (size_t j = 0; j < raw_data[0].size(); ++j)
      {
        if (raw_data[i][j] == "" || raw_data[i][j] == " " || raw_data[i][j].empty())
        {
          raw_data[i][j] = if_null;
        }
        else
        {
          raw_data[i][j] = if_not_null;
        }
      }
    }
    return *this;
  }
  DataFrame Subset(const __Indices &input)
  {
    DataFrame returned_dataframe;
    if (has_header)
    {
      for (size_t i = 0; i < 1; ++i)
      {
        returned_dataframe.raw_data.push_back(raw_data[i]);
      }
    }
    for (size_t i = 0; i < raw_data.size(); ++i)
    {

      for (size_t k = 0; k < input.size(); ++k)
      {
        if (input[k] == i)
        {
          returned_dataframe.raw_data.push_back(raw_data[i]);
        }
      }
    }

    return returned_dataframe;
  }
  DataFrame Subset(std::initializer_list<std::string> column_names)
  {
    DataFrame returned_dataframe;
    returned_dataframe.raw_data = raw_data;
    __Column temp_names;
    __Indices column_numbers;
    __Indices column_numbers_reversed;
    __Indices columns_indexes;
    for (auto &name : column_names)
    {
      temp_names.push_back(name);
    }
    for (size_t i = 0; i < temp_names.size(); ++i)
    {
      for (size_t j = 0; j < raw_data[0].size(); ++j)
      {
        if (ToLower(raw_data[0][j]) == ToLower(temp_names[i]))
        {
          column_numbers.push_back(j);
        }
      }
    }
    if (column_numbers.size() == 0 || column_numbers.empty())
    {
      returned_dataframe = empty_datafarme();
      return returned_dataframe;
    }
    for (size_t i = 0; i < raw_data[0].size(); ++i)
    {
      columns_indexes.push_back(i);
    }
    __Indices temp(columns_indexes.size() + column_numbers.size());
    __Indices returned_vector;
    std::sort(column_numbers.begin(), column_numbers.end());
    std::sort(columns_indexes.begin(), columns_indexes.end());
    __Indices::iterator it, st;
    it = std::set_difference(columns_indexes.begin(), columns_indexes.end(),
                             column_numbers.begin(), column_numbers.end(),
                             temp.begin());

    for (st = temp.begin(); st != it; st++)
    {
      column_numbers_reversed.push_back(*st);
    }
    std::sort(column_numbers.begin(), column_numbers.end());
    for (size_t i = 0; i < returned_dataframe.raw_data.size(); ++i)
    {
      for (size_t j = 0; j < column_numbers_reversed.size(); ++j)
      {
        remove_columns(returned_dataframe.raw_data[i], column_numbers_reversed[j] - j);
      }
    }
    return returned_dataframe;
  }
  DataFrame Subset(const int &range_begin, const int &range_end)
  {
    DataFrame returned_dataframe;
    __Row headers;
    if (has_header)
    {
      for (size_t i = 0; i < raw_data[0].size(); ++i)
      {
        headers.push_back(raw_data[0][i]);
      }
    }
    else
    {
      for (size_t i = 0; i < raw_data[0].size(); ++i)
      {
        headers.push_back("");
      }
    }
    std::cout.sync_with_stdio(false);
    returned_dataframe.raw_data.push_back(headers);
    if (range_end >= raw_data.size())
    {
      __Row error;
      error.push_back("DataFrame was not created since selected rows were out of range");

      returned_dataframe.raw_data.clear();
      returned_dataframe.raw_data.push_back(error);
      return returned_dataframe;
    }

    for (size_t i = range_begin; i <= range_end; ++i)
    {
      __Row line;

      for (size_t j = 0; j < raw_data[range_begin].size(); ++j)
      {
        line.push_back(raw_data[i][j]);
      }

      returned_dataframe.raw_data.push_back(line);
    }
    return returned_dataframe;
  }
  __Indices AND(__Indices &condition_1, __Indices &condition_2)
  {
    __Indices temp(condition_1.size() + condition_2.size());
    __Indices returned_vector;
    std::sort(condition_1.begin(), condition_1.end());
    std::sort(condition_2.begin(), condition_2.end());
    __Indices::iterator it, st;
    it = std::set_intersection(condition_1.begin(), condition_1.end(),
                               condition_2.begin(), condition_2.end(),
                               temp.begin());

    for (st = temp.begin(); st != it; st++)
    {
      returned_vector.push_back(*st);
    }

    return returned_vector;
  }
  __Indices OR(__Indices &condition_1, __Indices &condition_2)
  {
    __Indices temp(condition_1.size() + condition_2.size());
    __Indices returned_vector;
    std::sort(condition_1.begin(), condition_1.end());
    std::sort(condition_2.begin(), condition_2.end());
    __Indices::iterator it, st;
    it = std::set_union(condition_1.begin(), condition_1.end(),
                        condition_2.begin(), condition_2.end(),
                        temp.begin());

    for (st = temp.begin(); st != it; st++)
    {
      returned_vector.push_back(*st);
    }

    return returned_vector;
  }
  void DescribeDataFrame()
  {
    for (size_t i = 0; i < raw_data[0].size(); ++i)
    {
      Series<std::string> *x = new Series<std::string>(raw_data[0][i]);
      for (size_t j = 0; j < raw_data.size(); ++j)
      {
        x->_data.push_back(raw_data[j][i]);
      }
      x->series_has_headers = has_header;
      x->Describe();
      delete x;
    }
  }
  DataFrame GroupBy(column_names groupby_columns, column_names grouped_columns)
  {
    DataFrame returned_dataframe = create_groupby_dataframe(groupby_columns, grouped_columns);
    is_groupby_done = false;
    __Column grouped_columns_names = grouped_columns.SelectedColumns();
    __Indices grouped_columns_indexes;
    for (size_t i = 0; i < raw_data[0].size(); ++i)
    {
      for (size_t j = 0; j < grouped_columns_names.size(); ++j)
      {
        if (ToLower(raw_data[0][i]) == ToLower(grouped_columns_names[j]))
        {
          grouped_columns_indexes.push_back(i);
        }
      }
    }
    for (size_t i = 0; i < grouped_columns_indexes.size(); ++i)
    {
      print_groupby_values(groupby_columns, grouped_columns_indexes[i]);
    }
    returned_dataframe = create_groupby_dataframe(groupby_columns, grouped_columns);
    return returned_dataframe;
  }

  DataFrame GroupBy(column_names groupby_columns, column_names grouped_columns, bool remove_na)
  {
    DataFrame returned_dataframe = create_groupby_dataframe(groupby_columns, grouped_columns, remove_na);
    is_groupby_done = false;
    __Column grouped_columns_names = grouped_columns.SelectedColumns();
    __Indices grouped_columns_indexes;

    for (size_t i = 0; i < raw_data[0].size(); ++i)
    {
      for (size_t j = 0; j < grouped_columns_names.size(); ++j)
      {
        if (ToLower(raw_data[0][i]) == ToLower(grouped_columns_names[j]))
        {
          grouped_columns_indexes.push_back(i);
        }
      }
    }
    std::cout << '\n'
              << "GroupBy is Calculated" << '\n';
    for (size_t i = 0; i < grouped_columns_indexes.size(); ++i)
    {
      print_groupby_values(groupby_columns, grouped_columns_indexes[i]);
    }
    returned_dataframe = create_groupby_dataframe(groupby_columns, grouped_columns);
    return returned_dataframe;
  }

  DataFrame &CreateColumn(const std::string &column_name, Series<std::string> new_column_data)
  {
    // DataFrame returned_dataframe;
    if (raw_data.size() == 0)
    {
      raw_data.resize(new_column_data._data.size() + 1);
    }
    new_column_header(column_name);
    int series_length_offset = 0;
    int start = 0;
    if (new_column_data.series_has_headers)
    {
      start = 1;
    }
    if (new_column_data._data.size() > raw_data.size())
    {
      std::cout << "Length of the series is greater than the # of rows in the data farme" << '\n';
      return *this;
    }
    if (raw_data.size() - 1 == new_column_data._data.size())
    {
      series_length_offset = 1;
    }
    else if (raw_data.size() == new_column_data._data.size())
    {
      series_length_offset = 0;
    }

    int last_column = raw_data[0].size() - 1;
    for (size_t j = start; j < new_column_data._data.size(); ++j)
    {
      raw_data[j + series_length_offset][last_column] = new_column_data._data[j];
    }
    return *this;
  }
  DataFrame &CreateColumn(const std::string &column_name, const double &new_column_data)
  {
    DataFrame returned_dataframe;
    new_column_header(column_name);
    int last_column = raw_data[0].size() - 1;
    for (size_t j = 1; j < raw_data.size(); ++j)
    {
      raw_data[j][last_column] = std::to_string(new_column_data);
    }
    return *this;
  }
  DataFrame &CreateColumn(const std::string &column_name, const std::string &new_column_data)
  {
    DataFrame returned_dataframe;
    new_column_header(column_name);
    int last_column = raw_data[0].size() - 1;
    for (size_t j = 1; j < raw_data.size(); ++j)
    {
      raw_data[j][last_column] = new_column_data;
    }
    return *this;
  }
  inline DataFrame Melt()
  {
    DataFrame returned_dataframe = melt_variables();

    return returned_dataframe;
  }
  inline DataFrame Melt(column_names id_variables)
  {
    int number_of_id_variables = id_variables.all_names.size();

    DataFrame non_melted = non_melted_variables(id_variables);
    DataFrame returned_dataframe;
    DataFrame melted = melt_variables(id_variables);
    int number_of__rows = non_melted.raw_data[0].size();
    for (size_t i = 0; i < number_of_id_variables; ++i)
    {
      returned_dataframe.raw_data.insert(returned_dataframe.raw_data.begin() + i, non_melted.raw_data.begin(), non_melted.raw_data.end());
    }
    for (size_t i = 1; i < number_of_id_variables; ++i)
    {
      returned_dataframe.raw_data.erase(returned_dataframe.raw_data.begin());
    }
    for (size_t i = 0; i < returned_dataframe.raw_data.size(); ++i)
    {
      returned_dataframe.raw_data[i].insert(returned_dataframe.raw_data[i].end(), melted.raw_data[i].begin(), melted.raw_data[i].end());
    }

    return returned_dataframe;
  }
  DataFrame Transpose()
  {
    DataFrame returned_dataframe;
    __Table headers_column;
    __Table data_column;
    __Table all_data;

    // column 1 - contains the headers
    for (size_t i = 0; i < raw_data[0].size(); ++i)
    {
      for (size_t j = 0; j < 1; ++j)
      {
        __Column column;
        column.push_back(raw_data[j][i]);
        headers_column.push_back(column);
      }
    }
    // column 2 - data
    int original_count = headers_column.size();
    headers_column.resize(original_count * raw_data.size());
    for (size_t i = 0; i < raw_data.size(); ++i)
    {
      std::copy_n(headers_column.begin(), original_count, headers_column.begin() + original_count * i);
    }

    for (size_t i = 1; i < raw_data.size(); ++i)
    {
      for (size_t j = 0; j < raw_data[0].size(); ++j)
      {
        __Column column;
        column.push_back(raw_data[i][j]);
        data_column.push_back(column);
      }
    }
    // stitching the two columns together
    for (size_t i = 0; i < data_column.size(); ++i)
    {
      __Row row;
      for (size_t j = 0; j < data_column[0].size(); ++j)
      {
        row.push_back(headers_column[i][j]);
        row.push_back(data_column[i][j]);
      }
      all_data.push_back(row);
    }
    returned_dataframe.raw_data = all_data;
    return returned_dataframe;
  }
  DataFrame Transpose(bool columns_to_rows)
  {
    DataFrame returned_dataframe;
    for (size_t j = 0; j < raw_data[0].size(); ++j)
    {
      __Row row;
      for (size_t k = 0; k < raw_data.size(); ++k)
      {
        row.push_back(raw_data[k][j]);
      }
      returned_dataframe.raw_data.push_back(row);
    }
    return returned_dataframe;
  }
  DataFrame Cast(std::initializer_list<DataFrame> dataframes)
  {
    DataFrame returned_dataframe;
    int n;
    std::vector<DataFrame> input_dataframes;
    for (auto &x : dataframes)
    {
      input_dataframes.push_back(x);
    }
    for (size_t i = 0; i < input_dataframes.size(); ++i)
    {
      __Row row;
      for (size_t j = 0; j < input_dataframes[i].raw_data.size(); ++j)
      {
        for (size_t k = 0; k < input_dataframes[i].raw_data[0].size(); ++k)
        {
          row.push_back(input_dataframes[i].raw_data[j][k]);
        }
      }
      returned_dataframe.raw_data.push_back(row);
    }

    return returned_dataframe;
  }
  DataFrame DeTranspose()
  {
    DataFrame transposed_dataframe;
    for (size_t i = 0; i < raw_data[0].size(); ++i)
    {
      __Row row;
      for (size_t j = 0; j < raw_data.size(); ++j)
      {
        row.push_back(raw_data[j][i]);
      }
      transposed_dataframe.raw_data.push_back(row);
    }
    return transposed_dataframe;
  }
  DataFrame Concat(std::initializer_list<DataFrame> dataframes)
  {
    DataFrame returned_dataframe;
    std::vector<DataFrame> input_dataframes;
    std::vector<int> number_of_columns;
    std::vector<int> number_of_rows;
    for (auto &x : dataframes)
    {
      number_of_columns.push_back(x.raw_data[0].size());
      number_of_rows.push_back(x.raw_data.size());
      input_dataframes.push_back(x);
    }
    // Checks dimensions of each of the dataframes.
    // if dimensions are inconsistant, returns empty dataframe and warning
    if (std::adjacent_find(number_of_rows.begin(), number_of_rows.end(),
                           std::not_equal_to<>()) != number_of_rows.end() ||
        std::adjacent_find(number_of_columns.begin(), number_of_columns.end(),
                           std::not_equal_to<>()) != number_of_columns.end())
    {
      std::cout << "Data Frames must have same number of rows and columns" << '\n';
      returned_dataframe = empty_datafarme();
      return returned_dataframe;
    }
    // concatenates the dataframes
    for (size_t x = 0; x < input_dataframes.size(); x++)
    {
      for (size_t j = 0; j < input_dataframes[0].raw_data[0].size(); ++j)
      {
        __Row row;
        for (size_t k = 0; k < input_dataframes[0].raw_data.size(); ++k)
        {
          row.push_back(input_dataframes[x].raw_data[k][j]);
        }
        returned_dataframe.raw_data.push_back(row);
      }
    }
    // transpose back to original shape
    DataFrame transposed_dataframe = transpose_dataframe(returned_dataframe);

    return transposed_dataframe;
  }
  DataFrame Concat(std::initializer_list<DataFrame> dataframes, bool transposed)
  {
    DataFrame returned_dataframe;
    std::vector<DataFrame> input_dataframes;
    std::vector<int> number_of_columns;
    std::vector<int> number_of_rows;
    for (auto &x : dataframes)
    {
      number_of_columns.push_back(x.raw_data[0].size());
      number_of_rows.push_back(x.raw_data.size());
      input_dataframes.push_back(x);
    }
    // Checks dimensions of each of the dataframes.
    // if dimensions are inconsistant, returns empty dataframe and warning
    if (std::adjacent_find(number_of_rows.begin(), number_of_rows.end(),
                           std::not_equal_to<>()) != number_of_rows.end() ||
        std::adjacent_find(number_of_columns.begin(), number_of_columns.end(),
                           std::not_equal_to<>()) != number_of_columns.end())
    {
      std::cout << "Data Frames must have same number of rows and columns" << '\n';
      returned_dataframe = empty_datafarme();
      return returned_dataframe;
    }
    // concatenates the dataframes
    for (size_t x = 0; x < input_dataframes.size(); x++)
    {
      for (size_t j = 0; j < input_dataframes[0].raw_data[0].size(); ++j)
      {
        __Row row;
        for (size_t k = 0; k < input_dataframes[0].raw_data.size(); ++k)
        {
          row.push_back(input_dataframes[x].raw_data[k][j]);
        }
        returned_dataframe.raw_data.push_back(row);
      }
    }
    return returned_dataframe;
  }
  void Info()
  {
    std::cout << "** Dimensions **" << '\n';
    Shape();
    std::cout << '\n'
              << "** Columns names **" << '\n';
    for (size_t i = 0; i < raw_data[0].size(); ++i)
    {
      std::cout << raw_data[0][i] << '\n';
    }
    std::cout << '\n';
    DescribeNa();
  }
  inline void Head()
  {
    PrintRows(1, 5);
  }
  inline void Head(const int &number_of_rows)
  {
    PrintRows(1, number_of_rows);
  }
  inline void Tail()
  {
    PrintRows(raw_data.size() - 5, raw_data.size() - 1);
  }
  inline void Tail(const int &number_of_rows)
  {
    PrintRows(raw_data.size() - number_of_rows, raw_data.size() - 1);
  }

private:
  int number_of_rows;
  int number_of_columns;
  bool is_groupby_done;

  __Indices ColumnWidth()
  {
    __Indices all_widths;
    std::map<int, double> widths_map;
    for (size_t i = 0; i < raw_data[0].size(); ++i)
    {
      __Indices column_width;
      for (size_t j = 0; j < raw_data.size(); ++j)
      {
        column_width.push_back(raw_data[j][i].size());
      }
      __Indices::iterator max_width = std::max_element(column_width.begin(), column_width.end());
      all_widths.push_back(*max_width);
    }
    return all_widths;
  }

  DataFrame transpose_dataframe(const DataFrame &input_dataframe)
  {
    DataFrame transposed_dataframe;
    for (size_t i = 0; i < input_dataframe.raw_data[0].size(); ++i)
    {
      __Row row;
      for (size_t j = 0; j < input_dataframe.raw_data.size(); ++j)
      {
        row.push_back(input_dataframe.raw_data[j][i]);
      }
      transposed_dataframe.raw_data.push_back(row);
    }
    return transposed_dataframe;
  }
  DataFrame melt_variables(column_names id_variables)
  {
    DataFrame returned_dataframe;
    column_names names(id_variables);
    __Column all_names = names.SelectedColumns();
    __Row headers;
    headers.push_back("Id_Variables");
    headers.push_back("Value");
    returned_dataframe.raw_data.push_back(headers);
    int number_of_rows = raw_data.size() - 1;
    int number_of_id_variales = all_names.size();
    for (size_t i = 0; i < raw_data[0].size(); ++i)
    {
      for (size_t j = 0; j < raw_data.size(); ++j)
      {
        for (size_t k = 0; k < all_names.size(); ++k)
        {
          if (ToLower(all_names[k]) == ToLower(raw_data[0][i]))
          {
            __Column column;
            column.push_back(all_names[k]);
            column.push_back(raw_data[j][i]);
            returned_dataframe.raw_data.push_back(column);
          }
        }
      }
    }
    // remove original table's headers
    for (size_t i = 0; i < number_of_id_variales; ++i)
    {
      remove_rows(returned_dataframe.raw_data, i * (number_of_rows) + 1);
    }
    return returned_dataframe;
  }
  DataFrame melt_variables()
  {
    DataFrame returned_dataframe;
    column_names names(raw_data);
    __Column all_names = names.SelectedColumns();
    __Row headers;
    headers.push_back("Id_Variables");
    headers.push_back("Value");
    returned_dataframe.raw_data.push_back(headers);
    int number_of_rows = raw_data.size() - 1;
    int number_of_id_variales = all_names.size();
    for (size_t i = 0; i < raw_data[0].size(); ++i)
    {
      for (size_t j = 0; j < raw_data.size(); ++j)
      {
        if (all_names[i] == raw_data[0][i])
        {
          __Column column;
          column.push_back(all_names[i]);
          column.push_back(raw_data[j][i]);
          returned_dataframe.raw_data.push_back(column);
        }
      }
    }
    // remove original table's headers
    for (size_t i = 0; i < number_of_id_variales; ++i)
    {
      remove_rows(returned_dataframe.raw_data, i * (number_of_rows) + 1);
    }
    return returned_dataframe;
  }
  DataFrame non_melted_variables(column_names id_variables)
  {
    DataFrame returned_dataframe;
    column_names names(id_variables);
    __Column all_names = names.SelectedColumns();
    __Indices column_indexes;
    for (size_t i = 0; i < raw_data[0].size(); ++i)
    {
      for (size_t j = 0; j < all_names.size(); ++j)
      {
        if (ToLower(raw_data[0][i]) == ToLower(all_names[j]))
        {
          column_indexes.push_back(i);
        }
      }
    }
    returned_dataframe.raw_data = raw_data;
    for (size_t i = 0; i < column_indexes.size(); ++i)
    {
      for (size_t j = 0; j < returned_dataframe.raw_data.size(); ++j)
      {
        remove_columns(returned_dataframe.raw_data[j], column_indexes[i] - i);
      }
    }

    return returned_dataframe;
  }

  void new_column_header(const std::string &column_name)
  {
    __Table temp;
    for (size_t i = 0; i < raw_data.size(); ++i)
    {
      __Row n;
      for (size_t j = 0; j < raw_data[0].size(); ++j)
      {
        n.push_back(raw_data[i][j]);
      }
      n.push_back("");
      temp.push_back(n);
    }
    raw_data = temp;
    int num_of_col = raw_data[0].size();
    raw_data[0][num_of_col - 1] = column_name;
  }
  void print_grouped_column_name(const int &aggregated_column_index)
  {
    std::cout << '\n';
    std::cout << "*** Statistics of " << raw_data[0][aggregated_column_index] << "***" << '\n';
    std::cout << '\n';
  }
  int FindMaxWidth()
  {
    __Indices all_widths;
    int temp = 0;
    if (raw_data.empty() || raw_data.capacity() == 0 || raw_data.size() == 0)
    {
      std::cout << "empty data frame";
      return temp;
    }
    if (raw_data[0].size() == 0)
    {
      std::cout << "empty column";
      return temp;
    }
    for (size_t i = 0; i < raw_data.size(); ++i)
    {
      for (size_t j = 0; j < raw_data[0].size(); ++j)
      {
        int width = raw_data[i][j].size();
        all_widths.push_back(width);
      }
    }
    __Indices::iterator max_width = std::max_element(all_widths.begin(), all_widths.end());
    return *max_width;
  }
  int FindMaxWidth(__Column &input_vector)
  {
    __Indices all_widths;
    int temp = 0;
    for (size_t i = 0; i < input_vector.size(); ++i)
    {
      int width = input_vector[i].size();
      all_widths.push_back(width);
    }
    __Indices::iterator max_width = std::max_element(all_widths.begin(), all_widths.end());
    return *max_width;
  }
  int FindMaxWidth(const int &column_index)
  {
    __Indices all_widths;
    int temp = 0;
    for (size_t i = 0; i < raw_data.size(); ++i)
    {
      int width = raw_data[i][column_index].size();
      all_widths.push_back(width);
    }
    __Indices::iterator max_width = std::max_element(all_widths.begin(), all_widths.end());
    return *max_width;
  }

  int FindMaxWidth(std::vector<double> &input_vector)
  {
    __Indices all_widths;
    int temp = 0;
    for (size_t i = 0; i < input_vector.size(); ++i)
    {
      auto string = std::to_string(input_vector[i]);
      int width = string.size();
      all_widths.push_back(width);
    }
    __Indices::iterator max_width = std::max_element(all_widths.begin(), all_widths.end());
    return *max_width;
  }
  int FindMaxWidth(__Indices &input_vector)
  {
    __Indices all_widths;
    int temp = 0;
    for (size_t i = 0; i < input_vector.size(); ++i)
    {
      auto string = std::to_string(input_vector[i]);
      int width = string.size();
      all_widths.push_back(width);
    }
    __Indices::iterator max_width = std::max_element(all_widths.begin(), all_widths.end());
    return *max_width;
  }
  __Indices unique_indexes(__Indices &input_vector)
  {
    __Indices returned_vector;
    std::sort(input_vector.begin(), input_vector.end());
    input_vector.erase(std::unique(input_vector.begin(), input_vector.end()), input_vector.end());
    returned_vector = input_vector;
    return returned_vector;
  }

  void _shape()
  {
    number_of_columns = raw_data[0].size();
    number_of_rows = raw_data.size();
  }
  void remove_rows(__Table &row_to_be_removed, size_t pos)
  {
    __Table::iterator it = row_to_be_removed.begin();
    std::advance(it, pos);
    row_to_be_removed.erase(it);
  }
  void remove_columns(__Column &column_to_be_removed, size_t pos)
  {
    __Column::iterator it = column_to_be_removed.begin();
    std::advance(it, pos);
    column_to_be_removed.erase(it);
  }
  DataFrame drop_na()
  {
    DataFrame returned_dataframe;
    returned_dataframe.raw_data = raw_data;
    __Indices indexes;
    for (size_t i = 0; i < returned_dataframe.raw_data.size(); ++i)
    {
      for (size_t j = 0; j < returned_dataframe.raw_data[0].size(); ++j)
      {
        if (returned_dataframe.raw_data[i][j] == "" || returned_dataframe.raw_data[i][j] == " " || returned_dataframe.raw_data[i][j].empty() || returned_dataframe.raw_data[i][j] == "N/A")
        {
          indexes.push_back(i);
        }
      }
    }
    __Indices _unique_indexes = unique_indexes(indexes);
    for (size_t i = 0; i < _unique_indexes.size(); ++i)
    {
      remove_rows(returned_dataframe.raw_data, _unique_indexes[i] - i);
    }
    return returned_dataframe;
  }
  void void_drop_na()
  {
    __Indices indexes;
    for (size_t i = 0; i < raw_data.size(); ++i)
    {
      for (size_t j = 0; j < raw_data[0].size(); ++j)
      {
        if (raw_data[i][j] == "" || raw_data[i][j] == " " || raw_data[i][j].empty() || raw_data[i][j] == "N/A")
        {
          indexes.push_back(i);
        }
      }
    }
    __Indices _unique_indexes = unique_indexes(indexes);
    for (size_t i = 0; i < _unique_indexes.size(); ++i)
    {
      remove_rows(raw_data, _unique_indexes[i] - i);
    }
  }
  DataFrame drop_na(bool drop_columns)
  {
    DataFrame returned_dataframe;
    returned_dataframe.raw_data = raw_data;
    __Indices indexes;
    for (size_t i = 0; i < returned_dataframe.raw_data.size(); ++i)
    {
      for (size_t j = 0; j < returned_dataframe.raw_data[0].size(); ++j)
      {
        if (returned_dataframe.raw_data[i][j] == "" || returned_dataframe.raw_data[i][j] == " " || returned_dataframe.raw_data[i][j].empty() || returned_dataframe.raw_data[i][j] == "N/A")
        {
          if (drop_columns)
          {
            indexes.push_back(j);
          }
        }
      }
    }
    std::sort(indexes.begin(), indexes.end());
    __Indices _unique_indexes = unique_indexes(indexes);
    for (size_t i = 0; i < returned_dataframe.raw_data.size(); ++i)
    {
      for (size_t j = 0; j < _unique_indexes.size(); ++j)
      {
        remove_columns(returned_dataframe.raw_data[i], _unique_indexes[j] - j);
      }
    }

    return returned_dataframe;
  }
  DataFrame empty_datafarme()
  {
    DataFrame returned_dataframe;
    for (size_t i = 0; i < 1; ++i)
    {
      __Row line;
      for (size_t j = 0; j < 1; ++j)
      {

        line.push_back("empty data frame");
      }
      returned_dataframe.raw_data.push_back(line);
    }
    return returned_dataframe;
  }
  __Table empty_table()
  {
    __Table returned_table;
    __Row empty_row;
    empty_row.push_back("Empty table");
    returned_table.push_back(empty_row);
    return returned_table;
  }
  bool is_number(std::string &input_string)
  {
    char *p;
    strtod(input_string.c_str(), &p);
    return *p == 0;
  }
  // returns tuple with aggregated columns with two groupby columns
  __GroupBy group_by_2_col(column_names groupby, int grouped_column_index)
  {
    __GroupBy returned_groupby;
    __Aggregates sums;
    __Aggregates means;
    __Aggregates props;
    std::vector<int> counts;
    __Column groupby_names = groupby.SelectedColumns();
    __Indices groupby_columns_indexes = groupby_col_indexes(groupby);
    __Table groupby_values;
    double sum_of_column_value;
    for (size_t i = 0; i < groupby_columns_indexes.size(); ++i)
    {
      std::set<std::string> unique_values_set;
      double all = 0;
      for (size_t j = 1; j < raw_data.size(); ++j)
      {
        if (is_number(raw_data[j][grouped_column_index]))
        {
          all += stod(raw_data[j][grouped_column_index]);
        }
        unique_values_set.insert(raw_data[j][groupby_columns_indexes[i]]);
      }
      sum_of_column_value = all;
      __Column unique_values_vector(unique_values_set.begin(), unique_values_set.end());
      groupby_values.push_back(unique_values_vector);
    }
    int number_of_groupby_columns = groupby_names.size();
    for (size_t i = 0; i < groupby_values[0].size(); ++i)
    {
      for (size_t j = 0; j < groupby_values[1].size(); ++j)
      {
        double sum = 0;
        double number_of_cases = 0;
        for (size_t row = 0; row < raw_data.size(); row++)
        {
          if (groupby_values[0][i] == raw_data[row][groupby_columns_indexes[0]] && groupby_values[1][j] == raw_data[row][groupby_columns_indexes[1]])
          {
            if (is_number(raw_data[row][grouped_column_index]))
            {
              sum += stod(raw_data[row][grouped_column_index]);
            }
            // count the cases in each combo
            number_of_cases++;
          }
        }
        sums.push_back(sum);
        means.push_back(sum / number_of_cases);
        props.push_back((number_of_cases / raw_data.size()) * 100);
        counts.push_back(number_of_cases);
      }
    }
    returned_groupby = std::make_tuple(sums, means, props, counts);
    return returned_groupby;
  }
  // returns tuple with aggregated columns with one groupby column
  __GroupBy group_by_1_col(column_names groupby, int grouped_column_index)
  {
    __GroupBy returned_groupby;
    __Aggregates sums;
    __Aggregates means;
    __Aggregates props;
    std::vector<int> counts;
    __Column groupby_names = groupby.SelectedColumns();
    __Indices groupby_columns_indexes = groupby_col_indexes(groupby);
    __Table groupby_values;
    double sum_of_column_value;
    for (size_t i = 0; i < groupby_columns_indexes.size(); ++i)
    {
      std::set<std::string> unique_values_set;
      double all = 0;
      for (size_t j = 1; j < raw_data.size(); ++j)
      {
        if (is_number(raw_data[j][grouped_column_index]))
        {
          all += stod(raw_data[j][grouped_column_index]);
        }
        unique_values_set.insert(raw_data[j][groupby_columns_indexes[i]]);
      }
      sum_of_column_value = all;
      __Column unique_values_vector(unique_values_set.begin(), unique_values_set.end());
      groupby_values.push_back(unique_values_vector);
    }
    int number_of_groupby_columns = groupby_names.size();
    for (size_t i = 0; i < groupby_values[0].size(); ++i)
    {
      double sum = 0;
      double number_of_cases = 0;
      for (size_t row = 0; row < raw_data.size(); row++)
      {
        if (groupby_values[0][i] == raw_data[row][groupby_columns_indexes[0]])
        {
          if (is_number(raw_data[row][grouped_column_index]))
          {
            sum += stod(raw_data[row][grouped_column_index]);
          }
          number_of_cases++;
        }
      }
      sums.push_back(sum);
      means.push_back(sum / number_of_cases);
      props.push_back((number_of_cases / raw_data.size()) * 100);
      counts.push_back(number_of_cases);
    }
    returned_groupby = std::make_tuple(sums, means, props, counts);
    return returned_groupby;
  }
  // returns column indexes of aggregated columns
  __Indices grouped_col_indexes(column_names &groupby, column_names &aggregated_columns)
  {
    // to be grouped
    __Column aggregated_columns_names = aggregated_columns.SelectedColumns();
    __Indices aggregated_columns_indexes;
    for (size_t i = 0; i < raw_data[0].size(); ++i)
    {
      for (size_t j = 0; j < aggregated_columns_names.size(); ++j)
      {
        if (ToLower(raw_data[0][i]) == ToLower(aggregated_columns_names[j]))
        {
          aggregated_columns_indexes.push_back(i);
        }
      }
    }
    return aggregated_columns_indexes;
  }
  // returns column indexes of groupby columns
  __Indices groupby_col_indexes(column_names &groupby)
  {
    // to be grouped
    __Column groupby_names = groupby.SelectedColumns();
    __Indices groupby_columns_indexes;
    // columns indexes of "groupby" columns
    for (size_t i = 0; i < raw_data[0].size(); ++i)
    {
      for (size_t j = 0; j < groupby_names.size(); ++j)
      {
        if (ToLower(raw_data[0][i]) == ToLower(groupby_names[j]))
        {
          groupby_columns_indexes.push_back(i);
        }
      }
    }
    return groupby_columns_indexes;
  }

  __Table create_groupby_object(column_names &groupby, const int &to_be_grouped)
  {
    __Column groupby_names = groupby.SelectedColumns();
    __Indices groupby_columns_indexes = groupby_col_indexes(groupby);
    __Table groupby_values;
    double sum_of_column_value;
    for (size_t i = 0; i < groupby_columns_indexes.size(); ++i)
    {
      std::set<std::string> unique_values_set;
      for (size_t j = 1; j < raw_data.size(); ++j)
      {
        unique_values_set.insert(raw_data[j][groupby_columns_indexes[i]]);
      }
      __Column unique_values_vector(unique_values_set.begin(), unique_values_set.end());
      groupby_values.push_back(unique_values_vector);
    }
    return groupby_values;
  }
  __Row groupby_headers()
  {
    
  }
  // saves groupby calculations to data frame
  DataFrame create_groupby_dataframe(column_names &groupby, column_names &to_be_grouped)
  {
    DataFrame reurned_dataframe;
    this->FillAllNa();
    int number_of_groupby_columns = groupby.SelectedColumns().size();
    __Column groupby_names = groupby.SelectedColumns();
    __Table temp;
    __Column groupby_values_col_1;
    __Column groupby_values_col_2;
    __Column groupby_values_both;
    __Indices grouped_columns_indexes = grouped_col_indexes(groupby, to_be_grouped);
    if (number_of_groupby_columns == 1)
    {
      for (size_t i = 0; i < grouped_columns_indexes.size(); ++i)
      {
        temp.clear();
        __Table groupby_values = create_groupby_object(groupby, grouped_columns_indexes[i]);
        __GroupBy groupby_columns = group_by_1_col(groupby, grouped_columns_indexes[i]);
        groupby_values_col_1 = groupby_values[0];
        __Column sum_column = convert_grouped_column_to_string(std::get<0>(groupby_columns));
        __Column mean_column = convert_grouped_column_to_string(std::get<1>(groupby_columns));
        __Column prop_column = convert_grouped_column_to_string(std::get<2>(groupby_columns));
        __Column count_column = convert_grouped_column_to_string(std::get<3>(groupby_columns));
        temp.push_back(sum_column);
        temp.push_back(mean_column);
        temp.push_back(prop_column);
        temp.push_back(count_column);
        __Row headers{"--" + groupby_names[0] + "--", "--Sum " + raw_data[0][grouped_columns_indexes[i]] + "--", "--Mean " + raw_data[0][grouped_columns_indexes[i]] + "--", "--Props(%) " + raw_data[0][grouped_columns_indexes[i]] + "--", "--Counts " + raw_data[0][grouped_columns_indexes[i]] + "--"};
        reurned_dataframe.raw_data.push_back(headers);
        for (size_t val = 0; val < temp[0].size(); val++)
        {
          __Row row;
          row.push_back(groupby_values_col_1[val]);
          for (size_t j = 0; j < temp.size(); ++j)
          {
            row.push_back(temp[j][val]);
          }
          reurned_dataframe.raw_data.push_back(row);
        }
      }
    }
    else if (number_of_groupby_columns == 2)
    {
      for (size_t i = 0; i < grouped_columns_indexes.size(); ++i)
      {
        temp.clear();
        __Table groupby_values = create_groupby_object(groupby, grouped_columns_indexes[i]);
        __GroupBy groupby_columns = group_by_2_col(groupby, grouped_columns_indexes[i]);
        groupby_values_col_1 = groupby_values[0];
        groupby_values_col_2 = groupby_values[1];
        __Column sum_column = convert_grouped_column_to_string(std::get<0>(groupby_columns));
        __Column mean_column = convert_grouped_column_to_string(std::get<1>(groupby_columns));
        __Column prop_column = convert_grouped_column_to_string(std::get<2>(groupby_columns));
        __Column count_column = convert_grouped_column_to_string(std::get<3>(groupby_columns));
        temp.push_back(sum_column);
        temp.push_back(mean_column);
        temp.push_back(prop_column);
        temp.push_back(count_column);
        for (size_t val = 0; val < groupby_values_col_1.size(); val++)
        {
          for (size_t j = 0; j < groupby_values_col_2.size(); ++j)
          {
            groupby_values_both.push_back(groupby_values_col_1[val] + " & " + groupby_values_col_2[j]);
          }
        }
        __Row headers{"--" + groupby_names[0] + " & " + groupby_names[1] + "--", "--Sum " + raw_data[0][grouped_columns_indexes[i]] + "--", "--Mean " + raw_data[0][grouped_columns_indexes[i]] + "--", "--Props(%) " + raw_data[0][grouped_columns_indexes[i]] + "--", "--Counts " + raw_data[0][grouped_columns_indexes[i]] + "--"};
        reurned_dataframe.raw_data.push_back(headers);
        for (size_t val = 0; val < temp[0].size(); val++)
        {
          __Row row;
          row.push_back(groupby_values_both[val]);
          for (size_t j = 0; j < temp.size(); ++j)
          {
            row.push_back(temp[j][val]);
          }
          reurned_dataframe.raw_data.push_back(row);
        }
      }
    }
    return reurned_dataframe;
  }

  DataFrame create_groupby_dataframe(column_names &groupby, column_names &to_be_grouped, bool remove_na)
  {
    DataFrame reurned_dataframe;
    this->void_drop_na();
    int number_of_groupby_columns = groupby.SelectedColumns().size();
    __Column groupby_names = groupby.SelectedColumns();
    __Table temp;
    __Column groupby_values_col_1;
    __Column groupby_values_col_2;
    __Column groupby_values_both;
    __Indices grouped_columns_indexes = grouped_col_indexes(groupby, to_be_grouped);
    if (number_of_groupby_columns == 1)
    {
      for (size_t i = 0; i < grouped_columns_indexes.size(); ++i)
      {
        temp.clear();
        __Table groupby_values = create_groupby_object(groupby, grouped_columns_indexes[i]);
        __GroupBy groupby_columns = group_by_1_col(groupby, grouped_columns_indexes[i]);
        groupby_values_col_1 = groupby_values[0];
        __Column sum_column = convert_grouped_column_to_string(std::get<0>(groupby_columns));
        __Column mean_column = convert_grouped_column_to_string(std::get<1>(groupby_columns));
        __Column prop_column = convert_grouped_column_to_string(std::get<2>(groupby_columns));
        __Column count_column = convert_grouped_column_to_string(std::get<3>(groupby_columns));
        temp.push_back(sum_column);
        temp.push_back(mean_column);
        temp.push_back(prop_column);
        temp.push_back(count_column);
        __Row headers{"--" + groupby_names[0] + "--", "--Sum " + raw_data[0][grouped_columns_indexes[i]] + "--", "--Mean " + raw_data[0][grouped_columns_indexes[i]] + "--", "--Props(%) " + raw_data[0][grouped_columns_indexes[i]] + "--", "--Counts " + raw_data[0][grouped_columns_indexes[i]] + "--"};
        reurned_dataframe.raw_data.push_back(headers);
        for (size_t val = 0; val < temp[0].size(); val++)
        {
          __Row row;
          row.push_back(groupby_values_col_1[val]);
          for (size_t j = 0; j < temp.size(); ++j)
          {
            row.push_back(temp[j][val]);
          }
          reurned_dataframe.raw_data.push_back(row);
        }
      }
    }
    else if (number_of_groupby_columns == 2)
    {
      for (size_t i = 0; i < grouped_columns_indexes.size(); ++i)
      {
        temp.clear();
        __Table groupby_values = create_groupby_object(groupby, grouped_columns_indexes[i]);
        __GroupBy groupby_columns = group_by_2_col(groupby, grouped_columns_indexes[i]);
        groupby_values_col_1 = groupby_values[0];
        groupby_values_col_2 = groupby_values[1];
        __Column sum_column = convert_grouped_column_to_string(std::get<0>(groupby_columns));
        __Column mean_column = convert_grouped_column_to_string(std::get<1>(groupby_columns));
        __Column prop_column = convert_grouped_column_to_string(std::get<2>(groupby_columns));
        __Column count_column = convert_grouped_column_to_string(std::get<3>(groupby_columns));
        temp.push_back(sum_column);
        temp.push_back(mean_column);
        temp.push_back(prop_column);
        temp.push_back(count_column);
        for (size_t val = 0; val < groupby_values_col_1.size(); val++)
        {
          for (size_t j = 0; j < groupby_values_col_2.size(); ++j)
          {
            groupby_values_both.push_back(groupby_values_col_1[val] + " & " + groupby_values_col_2[j]);
          }
        }
        __Row headers{"--" + groupby_names[0] + " & " + groupby_names[1] + "--", "--Sum " + raw_data[0][grouped_columns_indexes[i]] + "--", "--Mean " + raw_data[0][grouped_columns_indexes[i]] + "--", "--Props(%) " + raw_data[0][grouped_columns_indexes[i]] + "--", "--Counts " + raw_data[0][grouped_columns_indexes[i]] + "--"};
        reurned_dataframe.raw_data.push_back(headers);
        for (size_t val = 0; val < temp[0].size(); val++)
        {

          __Row row;
          row.push_back(groupby_values_both[val]);
          for (size_t j = 0; j < temp.size(); ++j)
          {
            row.push_back(temp[j][val]);
          }
          reurned_dataframe.raw_data.push_back(row);
        }
      }
    }
    return reurned_dataframe;
  }
  __Column convert_grouped_column_to_string(std::vector<double> &input_column)
  {
    __Column returned_vector;
    for (size_t i = 0; i < input_column.size(); ++i)
    {
      returned_vector.push_back(std::to_string(input_column[i]));
    }
    return returned_vector;
  }
  __Column convert_grouped_column_to_string(std::vector<int> &input_column)
  {
    __Column returned_vector;
    for (size_t i = 0; i < input_column.size(); ++i)
    {
      returned_vector.push_back(std::to_string(input_column[i]));
    }
    return returned_vector;
  }
  void print_groupby_values(column_names &groupby, const int &aggregated_column_index)
  {
    __Table groupby_values = create_groupby_object(groupby, aggregated_column_index);
    __Column groupby_names = groupby.SelectedColumns();
    __Indices groupby_columns_indexes = groupby_col_indexes(groupby);
    int number_of_groupby_columns = groupby_columns_indexes.size();
    if (number_of_groupby_columns == 2)
    {
      auto max = 15;
      int max_width_column_1 = FindMaxWidth(groupby_columns_indexes[0]);
      int max_width_column_2 = FindMaxWidth(groupby_columns_indexes[1]);
      auto num_width = 5;
      __GroupBy grouped_columns = group_by_2_col(groupby, aggregated_column_index);
      print_grouped_column_name(aggregated_column_index);
      // auto grouped_columns = print_grouped_column_name(groupby, aggregated_column_index);
      __Aggregates x1 = std::get<0>(grouped_columns);
      __Aggregates x2 = std::get<1>(grouped_columns);
      __Aggregates x3 = std::get<2>(grouped_columns);
      std::vector<int> x4 = std::get<3>(grouped_columns);
      int ind_1 = 0;
      int ind_2 = 0;
      int ind_3 = 0;
      int ind_4 = 0;
      std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
      std::cout.precision(2);                                // for fixed format, two decimal places
      std::cout << "--Group By: " << groupby_names[0] << ", " << groupby_names[1] << '\n'
                << '\n';
      std::cout << std::left << std::setw(max_width_column_1) << groupby_names[0] << "\t" << std::setw(max_width_column_2) << groupby_names[1] << std::setw(num_width) << "\t"
                << "|"
                << "Sum" << std::setw(num_width) << "\t"
                << "|"
                << "Mean" << std::setw(num_width) << "\t"
                << "|"
                << "Prop (%)" << std::setw(num_width) << "\t"
                << "|"
                << "Counts" << '\n';
      std::cout << '\n';
      for (size_t i = 0; i < groupby_values[0].size(); ++i)
      {
        for (size_t j = 0; j < groupby_values[1].size(); ++j)
        {

          // std::cout << '\n';
          std::cout << std::left << std::setw(max_width_column_1) << groupby_values[0][i] << "\t" << std::setw(max_width_column_2) << groupby_values[1][j] << std::setw(num_width) << "\t"
                    << "|" << x1[ind_1++] << std::setw(num_width) << "\t"
                    << "|" << x2[ind_2++] << std::setw(num_width) << "\t"
                    << "|" << x3[ind_3++] << std::setw(num_width) << "\t"
                    << "|" << x4[ind_4++] << '\n';
        }
      }
    }
    else if (number_of_groupby_columns == 1)
    {
      auto max = FindMaxWidth(groupby_columns_indexes[0]);
      auto num_width = 15;
      auto grouped_columns = group_by_1_col(groupby, aggregated_column_index);
      print_grouped_column_name(aggregated_column_index);
      __Aggregates x1 = std::get<0>(grouped_columns);
      __Aggregates x2 = std::get<1>(grouped_columns);
      __Aggregates x3 = std::get<2>(grouped_columns);
      std::vector<int> x4 = std::get<3>(grouped_columns);
      int ind_1 = 0;
      int ind_2 = 0;
      int ind_3 = 0;
      int ind_4 = 0;
      std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
      std::cout.precision(2);                                // for fixed format, two decimal places
      std::cout << "--Group By: " << groupby_names[0] << '\n'
                << '\n';
      std::cout << std::left << std::setw(max) << "\t"
                << "|" << std::setw(num_width) << "Sum"
                << "\t"
                << "|" << std::setw(num_width) << "Mean"
                << "\t"
                << "|" << std::setw(num_width) << "Prop (%)"
                << "\t"
                << "|" << std::setw(num_width) << "Counts" << '\n';
      std::cout << '\n';
      for (size_t i = 0; i < groupby_values[0].size(); ++i)
      {

        std::cout << std::left << std::setw(max) << groupby_values[0][i] << "\t"
                  << "|" << std::setw(num_width) << x1[ind_1++] << "\t"
                  << "|" << std::setw(num_width) << x2[ind_2++] << "\t"
                  << "|" << std::setw(num_width) << x3[ind_3++] << "\t"
                  << "|" << std::setw(num_width) << x4[ind_4++] << '\n';
      }
    }
    std::cout << "----------------------------------------------------------------------------------------------";
    std::cout << '\n'
              << '\n'
              << '\n';
  }
  std::string ToLower(const std::string &input_string)
  {
    std::string lower_string;
    for (auto &s : input_string)
    {
      lower_string.push_back(tolower(s));
    }
    return lower_string;
  }

};

#endif
