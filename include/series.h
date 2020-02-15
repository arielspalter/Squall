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
#include <string>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <type_traits>
#include <algorithm>
#include <iomanip>
#include <map>
// #include <bits/stdc++.h>
#include <math.h>
#include <ctype.h>
#include <set>
typedef std::vector<int> __Indices;
typedef std::vector<std::vector<std::string>> __Table;
typedef std::vector<std::string> __Row;
#ifndef SERIES_H
#define SERIES_H
int all_rows;
// original indexes of numeric values
struct NumericValuesIndexes
{
  double numeric_values;
  int index;
  NumericValuesIndexes(double val, int ind) : numeric_values(val), index(ind) {}
};
template <typename T>
class Series
{
public:
  bool series_has_headers = false;
  int number_of_rows;
  std::vector<NumericValuesIndexes> numeric_indexes;
  Series() {}
  Series(std::initializer_list<T> input_elements)
  {
    for (auto &inp : input_elements)
    {
      _data.push_back(inp);
    }
  }
  Series(std::string series_name) : series_name(series_name) {}
  std::vector<T> _data;

  __Indices operator>(const double &greater_than)
  {
    to_numeric_values(true);
    __Indices returned_indexes;
    for (size_t i = 0; i < numeric_indexes.size(); ++i)
    {
      if (numeric_indexes[i].numeric_values > greater_than)
      {
        returned_indexes.push_back(numeric_indexes[i].index);
      }
    }
    return returned_indexes;
  }

  __Indices operator>(const std::string &greater_than)
  {
    __Indices returned_indexes;
    for (size_t i = 1; i < _data.size(); ++i)
    {
      if (_series_tolower(_data[i]) > _series_tolower(greater_than))
      {
        returned_indexes.push_back(i);
      }
    }
    return returned_indexes;
  }

  __Indices operator<(const double &less_than)
  {
    to_numeric_values(true);
    __Indices returned_indexes;
    for (size_t i = 0; i < numeric_indexes.size(); ++i)
    {
      if (numeric_indexes[i].numeric_values < less_than)
      {
        returned_indexes.push_back(numeric_indexes[i].index);
      }
    }
    return returned_indexes;
  }

  __Indices operator<(const std::string &less_than)
  {
    __Indices returned_indexes;
    for (size_t i = 1; i < _data.size(); ++i)
    {
      if (_series_tolower(_data[i]) < _series_tolower(less_than) && !_data[i].empty() && _data[i] != "" && _data[i] != " ")
      {
        returned_indexes.push_back(i);
      }
    }
    return returned_indexes;
  }

  __Indices operator==(const double &equals_to)
  {
    to_numeric_values(true);
    __Indices returned_indexes;
    for (size_t i = 0; i < numeric_indexes.size(); ++i)
    {
      if (numeric_indexes[i].numeric_values == equals_to)
      {
        returned_indexes.push_back(numeric_indexes[i].index);
      }
    }
    return returned_indexes;
  }

  __Indices operator==(const std::string &equals_to)
  {
    __Indices returned_indexes;
    returned_indexes.reserve(number_of_rows);
    number_of_rows = _data.size();
    for (size_t i = 0; i < _data.size(); ++i)
    {
      if (_series_tolower(_data[i]) == _series_tolower(equals_to))
      {
        returned_indexes.push_back(i);
      }
    }
    return returned_indexes;
  }

  __Indices operator!=(const double &not_equals_to)
  {
    to_numeric_values(true);
    __Indices returned_indexes;
    for (size_t i = 0; i < numeric_indexes.size(); ++i)
    {
      if (numeric_indexes[i].numeric_values != not_equals_to)
      {
        returned_indexes.push_back(numeric_indexes[i].index);
      }
    }
    return returned_indexes;
  }

  __Indices operator!=(const std::string &not_equals_to)
  {
    __Indices returned_indexes;
    for (size_t i = 1; i < _data.size(); ++i)
    {
      if (_series_tolower(_data[i]) != _series_tolower(not_equals_to) && !_data[i].empty() && _data[i] != "" && _data[i] != " ")
      {
        returned_indexes.push_back(i);
      }
    }
    return returned_indexes;
  }

  __Indices operator<=(const double &equals_or_less)
  {
    to_numeric_values(true);
    __Indices returned_indexes;
    for (size_t i = 0; i < numeric_indexes.size(); ++i)
    {
      if (numeric_indexes[i].numeric_values <= equals_or_less)
      {
        returned_indexes.push_back(numeric_indexes[i].index);
      }
    }
    return returned_indexes;
  }

  __Indices operator<=(const std::string &equals_or_less)
  {
    __Indices returned_indexes;
    for (size_t i = 1; i < _data.size(); ++i)
    {
      if (_series_tolower(_data[i]) <= _series_tolower(equals_or_less) && !_data[i].empty() && _data[i] != "" && _data[i] != " ")
      {
        returned_indexes.push_back(i);
      }
    }
    return returned_indexes;
  }

  __Indices operator>=(const double &equals_or_more)
  {
    to_numeric_values(true);
    __Indices returned_indexes;
    for (size_t i = 0; i < numeric_indexes.size(); ++i)
    {
      if (numeric_indexes[i].numeric_values >= equals_or_more)
      {
        returned_indexes.push_back(numeric_indexes[i].index);
      }
    }
    return returned_indexes;
  }

  __Indices operator>=(const std::string &equals_or_more)
  {
    __Indices returned_indexes;
    for (size_t i = 1; i < _data.size(); ++i)
    {
      if (_series_tolower(_data[i]) >= _series_tolower(equals_or_more))
      {
        returned_indexes.push_back(i);
      }
    }
    return returned_indexes;
  }

  Series<std::string> operator+(Series<std::string> input_series)
  {
    Series<std::string> returned_series;
    std::vector<double> numbers_1 = to_numeric_values(true);
    std::vector<double> numbers_2 = input_series.to_numeric_values(true);
    if (numbers_1.size() == 0 || numbers_2.size() == 0)
    {
      std::cout << "addition operator (+) works on numerical series only." << '\n'
                << "your columns may not be numerical, or have a wrong name" << '\n';
      return returned_series;
    }
    __Row temp;
    int size = numbers_1.size();

    for (size_t i = 0; i < size; ++i)
    {
      std::stringstream ss;
      std::string t;
      ss << numbers_1[i] + numbers_2[i];
      while (ss >> t)
      {
        returned_series._data.push_back(t);
      }
    }
    return returned_series;
  }

  Series<std::string> operator-(Series<std::string> input_series)
  {
    Series<std::string> returned_series;
    std::vector<double> numbers_1 = to_numeric_values(true);
    std::vector<double> numbers_2 = input_series.to_numeric_values(true);
    if (numbers_1.size() == 0 || numbers_2.size() == 0)
    {
      std::cout << "subtraction operator (-) works on numerical series only." << '\n'
                << "your columns may not be numerical, or have a wrong name" << '\n';
      return returned_series;
    }
    __Row temp;
    int size = numbers_1.size();
    for (size_t i = 0; i < size; ++i)
    {
      std::stringstream ss;
      std::string t;
      ss << numbers_1[i] - numbers_2[i];
      while (ss >> t)
      {
        returned_series._data.push_back(t);
      }
    }
    return returned_series;
  }

  Series<std::string> operator*(Series<std::string> input_series)
  {
    Series<std::string> returned_series;
    std::vector<double> numbers_1 = to_numeric_values(true);
    std::vector<double> numbers_2 = input_series.to_numeric_values(true);
    if (numbers_1.size() == 0 || numbers_2.size() == 0)
    {
      std::cout << "multiplication operator (*) works on numerical series only." << '\n'
                << "your columns may not be numerical, or have a wrong name" << '\n';
      return returned_series;
    }
    __Row temp;
    int size = numbers_1.size();
    for (size_t i = 0; i < size; ++i)
    {
      std::stringstream ss;
      std::string t;
      ss << numbers_1[i] * numbers_2[i];
      while (ss >> t)
      {
        returned_series._data.push_back(t);
      }
    }
    return returned_series;
  }

  Series<std::string> operator/(Series<std::string> input_series)
  {
    Series<std::string> returned_series;
    std::vector<double> numbers_1 = to_numeric_values(true);
    std::vector<double> numbers_2 = input_series.to_numeric_values(true);
    if (numbers_1.size() == 0 || numbers_2.size() == 0)
    {
      std::cout << "division operator (/) works on numerical series only." << '\n'
                << "your columns may not be numerical, or have a wrong name" << '\n';
      return returned_series;
    }
    __Row temp;
    int size = numbers_1.size();
    for (size_t i = 0; i < size; ++i)
    {
      std::stringstream ss;
      std::string t;
      ss << numbers_1[i] / numbers_2[i];
      while (ss >> t)
      {
        returned_series._data.push_back(t);
      }
    }
    return returned_series;
  }

  Series<std::string> operator+(const double &input_number)
  {
    Series<std::string> returned_series;
    std::vector<double> numbers_1 = to_numeric_values(true);
    if (numbers_1.size() == 0)
    {
      std::cout << "addition operator (+) works on numerical series only." << '\n'
                << "your columns may not be numerical, or have a wrong name" << '\n';
      return returned_series;
    }
    __Row temp;
    int size = numbers_1.size();
    for (size_t i = 0; i < size; ++i)
    {
      std::stringstream ss;
      std::string t;
      ss << numbers_1[i] + input_number;
      while (ss >> t)
      {
        returned_series._data.push_back(t);
      }
    }
    return returned_series;
  }
  Series<std::string> operator-(const double &input_number)
  {
    Series<std::string> returned_series;
    std::vector<double> numbers_1 = to_numeric_values(true);
    if (numbers_1.size() == 0)
    {
      std::cout << "subtraction operator (-) works on numerical series only." << '\n'
                << "your columns may not be numerical, or have a wrong name" << '\n';
      return returned_series;
    }
    __Row temp;
    int size = numbers_1.size();

    for (size_t i = 0; i < size; ++i)
    {
      std::stringstream ss;
      std::string t;
      ss << numbers_1[i] - input_number;
      while (ss >> t)
      {
        returned_series._data.push_back(t);
      }
    }
    return returned_series;
  }
  Series<std::string> operator*(const double &input_number)
  {
    Series<std::string> returned_series;
    std::vector<double> numbers_1 = to_numeric_values(true);
    if (numbers_1.size() == 0)
    {
      std::cout << "multiplication operator (*) works on numerical series only." << '\n'
                << "your columns may not be numerical, or have a wrong name" << '\n';
      return returned_series;
    }
    __Row temp;
    int size = numbers_1.size();

    for (size_t i = 0; i < size; ++i)
    {
      std::stringstream ss;
      std::string t;
      ss << numbers_1[i] * input_number;
      while (ss >> t)
      {
        returned_series._data.push_back(t);
      }
    }
    return returned_series;
  }

  Series<std::string> operator/(const double &input_number)
  {
    Series<std::string> returned_series;
    std::vector<double> numbers_1 = to_numeric_values(true);
    if (numbers_1.size() == 0)
    {
      std::cout << "division operator (/) works on numerical series only." << '\n'
                << "your columns may not be numerical, or have a wrong name" << '\n';
      return returned_series;
    }
    __Row temp;
    int size = numbers_1.size();

    for (size_t i = 0; i < size; ++i)
    {
      std::stringstream ss;
      std::string t;
      ss << numbers_1[i] / input_number;
      while (ss >> t)
      {
        returned_series._data.push_back(t);
      }
    }
    return returned_series;
  }

  inline void Describe()
  {
    if (series_name.empty())
    {
      series_name = "Series";
    }
    if (!is_series_numeric())
    {
      _describe_string();
    }
    else
    {
      _describe_double();
    }
  }

  double Sum()
  {
    double sum = _sum();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Sum of " << series_name << ": " << sum;
    return sum;
  }
  double Mean()
  {
    if (series_name.empty())
    {
      series_name = "Series";
    }
    double mean = _mean();
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Mean of " << series_name << ": " << mean;
    return mean;
  }

  double Median()
  {
    double median = _median();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Median of " << series_name << ": " << median;
    return median;
  }

  double Max()
  {
    double max = _max();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Maximum value of " << series_name << ": " << max;
    return max;
  }
  double Min()
  {
    double min = _min();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Minimum value of " << series_name << ": " << min;
    return min;
  }
  std::vector<double> Quantiles()
  {
    std::vector<double> returned_quartiles;
    std::vector<double> iqr = _iqr();
    std::cout << '\n'
              << "* Quantiles: ";
    for (size_t i = 0; i < iqr.size(); ++i)
    {
      returned_quartiles.push_back(iqr[i]);
    }
    std::cout << "Q1: " << iqr[0] << "| Q2 (median): " << iqr[1] << "| Q3: " << iqr[2] << '\n';
    return returned_quartiles;
  }

  double Quantiles(const float &input_quantile)
  {

    double iqr = _iqr(input_quantile);
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << series_name << '\n'
              << "** Quantile ";

    std::cout << input_quantile * 100 << "%"
              << " : " << iqr << '\n';
    return iqr;
  }
  double STDEV()
  {
    double stadv = _stdv();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout
        << "* Standard Deviation of " << series_name << ": " << stadv;
    return stadv;
  }
  double Variance()
  {
    double variance = _variance();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Variance of " << series_name << ": " << variance;
    return variance;
  }
  __Indices Contains(const std::string &input_string)
  {
    __Indices returned_indexes;
    for (size_t i = 1; i < _data.size(); ++i)
    {
      if (_series_tolower(_data[i]).find(_series_tolower(input_string)) != std::string::npos)
      {
        returned_indexes.push_back(i);
      }
    }
    return returned_indexes;
  }

  void PrintSeries()
  {
    std::cout.sync_with_stdio(false);
    std::cout << '\n';
    for (auto &d : _data)
    {
      std::cout << d << '\n';
    }
  }

  std::map<std::string, int> _string_occurences()
  {
    std::map<std::string, int> returned_map;
    int begins;
    if (series_has_headers)
    {
      begins = 1;
    }
    else
    {
      begins = 0;
    }
    for (size_t i = begins; i < _data.size(); ++i)
    {
      if (_data[i] == "" || _data[i].empty())
      {
        _data[i] = "N/A";
      }
    }
    for (size_t i = begins; i < _data.size(); ++i)
    {
      returned_map[_data[i]]++;
    }

    return returned_map;
  }
  void SetValue(const __Indices &input_values_index, std::string set_value)
  {
    __Row temp(all_rows);
    for (size_t i = 0; i < all_rows; i++)
    {
      for (size_t j = 0; j < input_values_index.size(); j++)
      {
        if (input_values_index[j] == i)
        {
          temp.at(i - 1) = set_value;
        }
      }
    }
    _data = temp;
  }
  void SetValue(__Indices &&input_values_index, const std::string &set_value)
  {
    auto _input(std::move(input_values_index));
    __Row temp(all_rows);
    __Row::iterator it;
    for (size_t i = 0; i < all_rows; i++)
    {
      for (size_t j = 0; j < _input.size(); j++)
      {
        if (_input[j] == i)
        {
          temp.at(i - 1) = set_value;
        }
      }
    }
    _data = temp;
  }
  void SetValue(std::initializer_list<__Indices> input_values_index, std::initializer_list<std::string> set_value)
  {
    int size = 0;
    int number_of_conditions = input_values_index.size();
    int number_of_values = set_value.size();
    __Row values;
    __Indices indexes;
    int index = -1;
    for (auto &condition : set_value)
    {
      values.push_back(condition);
    }
    for (auto &condition : input_values_index)
    {
      size += condition.size();
      for (size_t i = 0; i < condition.size(); ++i)
      {
        indexes.push_back(condition[i]);
      }
    }
    int sum_of_rows = all_rows * (all_rows + 1) / 2;
    int sum_of_indexes = 0;
    for (size_t i = 0; i < indexes.size(); ++i)
    {
      sum_of_indexes += indexes[i];
    }
    __Row temp(size);
    if (number_of_conditions != number_of_values)
    {
      std::cout << "Could not set the values. The number of conditions and new values must be identical" << '\n';
      return;
    }
    if (indexes.size() > all_rows)
    {
      std::cout << "** The conditions you have provided overlap. every condition must cover a unique set of rows" << '\n';
      series_has_headers = true;
      _data = temp;
      return;
    }
    if (indexes.size() < all_rows)
    {
      std::cout << "The conditions you've provided cover " << indexes.size() << " out of the " << all_rows << " # of rows in the data frame" << '\n';
      return;
    }
    for (auto &condition : input_values_index)
    {
      index++;
      for (size_t i = 0; i < condition.size(); ++i)
      {
        temp.at(condition[i] - 1) = values[index];
      }
    }
    _data = temp;
  }
  Series<std::string> Distinct()
  {
    Series<std::string> returned_series;
    std::set<std::string> distinct_values;
    for (size_t i = 1; i < _data.size(); ++i)
    {
      distinct_values.insert(_data[i]);
    }
    _data.assign(distinct_values.begin(), distinct_values.end());
    returned_series._data = _data;

    return returned_series;
  }

private:
  std::string series_name;

  bool IsNumber(std::string &input_string)
  {
    char *p;
    strtod(input_string.c_str(), &p);
    return *p == 0;
  }
  bool IsFloat(std::string &input_string)
  {
    char *end = 0;
    double val = strtod(input_string.c_str(), &end);
    return end != input_string.c_str() && *end == '\0' && val != HUGE_VAL;
  }
  std::vector<double> to_numeric_values()
  {
    std::vector<double> returned_vector;
    for (size_t i = 0; i < _data.size(); ++i)
    {
      std::string temp;
      std::stringstream temp_stream;
      temp_stream << _data[i];
      while (temp_stream >> temp)
      {
        if (IsNumber(temp))
        {
          numeric_indexes.push_back(NumericValuesIndexes(stod(temp), i));
          returned_vector.push_back(stod(temp));
        }
      }
    }
    return returned_vector;
  }
  std::vector<double> to_numeric_values(bool fill_empty_with_zero)
  {
    std::vector<double> returned_vector;
    for (size_t i = 0; i < _data.size(); ++i)
    {
      std::string temp;
      std::stringstream temp_stream;
      if (_data[i] == "" || _data[i] == " " || _data[i].empty() || _data[i] == "N/A")
      {
        _data[i] = "0";
      }
      temp_stream << _data[i];

      while (temp_stream >> temp)
      {
        if (IsNumber(temp))
        {
          numeric_indexes.push_back(NumericValuesIndexes(stod(temp), i));
          returned_vector.push_back(stod(temp));
        }
      }
    }
    return returned_vector;
  }

  double _sum()
  {
    double sum = 0;
    std::vector<double> numbers = to_numeric_values();
    if (numbers.size() == 0 || numbers.empty() || !is_series_numeric())
    {
      return sum;
    }
    for (size_t i = 0; i < numbers.size(); ++i)
    {
      sum += numbers[i];
    }
    return sum;
  }
  double _mean()
  {
    std::vector<double> numbers = to_numeric_values();
    double sums = 0.0;
    for (auto &x : numbers)
    {
      sums += x;
    }
    double mean = 0.0;
    if (numbers.size() == 0 || numbers.empty() || !is_series_numeric())
    {
      return 0;
    }
    mean = sums / numbers.size();
    return mean;
  }
  double _median()
  {
    std::vector<double> numbers = to_numeric_values();
    std::sort(numbers.begin(), numbers.end());
    int n = numbers.size();
    double median = 0;
    if (numbers.size() == 0 || numbers.empty() || !is_series_numeric())
    {
      return 0;
    }
    if (numbers.size() % 2 == 0)
    {
      median = (numbers[numbers.size() / 2 - 1] + numbers[numbers.size() / 2]) / 2;
    }
    else
    {
      median = numbers[numbers.size() / 2];
    }
    return median;
  }

  double _max()
  {
    std::vector<double> numbers = to_numeric_values();
    std::sort(numbers.begin(), numbers.end());
    int n = numbers.size();
    if (numbers.size() == 0 || numbers.empty() || !is_series_numeric())
    {
      return 0;
    }
    auto max = std::max_element(numbers.begin(), numbers.end());
    return *max;
  }

  double _min()
  {
    std::vector<double> numbers = to_numeric_values();
    std::sort(numbers.begin(), numbers.end());
    int n = numbers.size();
    if (numbers.size() == 0 || numbers.empty() || !is_series_numeric())
    {
      return 0;
    }
    auto min = std::min_element(numbers.begin(), numbers.end());
    return *min;
  }
  double _stdv()
  {
    std::vector<double> numbers = to_numeric_values();
    int n = numbers.size();
    double sum = 0.0, mean = 0.0, variance = 0.0, stdDeviation = 0.0;

    if (numbers.size() == 0 || numbers.empty() || !is_series_numeric())
    {
      return 0;
    }
    for (size_t i = 0; i < n; ++i)
    {
      sum += numbers[i];
    }
    mean = sum / n;
    for (size_t i = 0; i < n; ++i)
    {
      variance += pow(numbers[i] - mean, 2);
    }
    variance /= n;
    stdDeviation = sqrt(variance);
    return stdDeviation;
  }
  double _variance()
  {
    std::vector<double> numbers = to_numeric_values();
    int n = numbers.size();
    double sum = 0.0, mean = 0.0, variance = 0.0;

    if (numbers.size() == 0 || numbers.empty() || !is_series_numeric())
    {
      return 0;
    }
    for (size_t i = 0; i < n; ++i)
    {
      sum += numbers[i];
    }
    mean = sum / n;
    for (size_t i = 0; i < n; ++i)
    {
      variance += pow(numbers[i] - mean, 2);
    }
    variance /= n;
    return variance;
  }

  std::vector<double> _iqr()
  {
    std::vector<double> quartiles;
    std::vector<double> numbers = to_numeric_values();
    double Q1;
    double Q2;
    double Q3;
    std::sort(numbers.begin(), numbers.end());
    int N = numbers.size();
    double n1 = (N - 1) * 0.25 + 1;
    double n3 = (N - 1) * 0.75 + 1;
    int k1 = (int)n1;
    int k3 = (int)n3;
    double d1 = n1 - k1;
    double d3 = n3 - k3;
    Q1 = numbers[k1 - 1] + d1 * (numbers[k1] - numbers[k1 - 1]);
    Q3 = numbers[k3 - 1] + d3 * (numbers[k3] - numbers[k3 - 1]);
    if (numbers.size() == 0 || numbers.empty() || !is_series_numeric())
    {
      quartiles.push_back(0);
      return quartiles;
    }
    if (numbers.size() % 2 == 0)
    {
      Q2 = (numbers[numbers.size() / 2 - 1] + numbers[numbers.size() / 2]) / 2;
    }
    else
      Q2 = numbers[numbers.size() / 2];
    quartiles.push_back(Q1);
    quartiles.push_back(Q2);
    quartiles.push_back(Q3);
    return quartiles;
  }

  double _iqr(const float &input_quantile)
  {
    std::vector<double> numbers = to_numeric_values();
    double returned_quantile;
    std::sort(numbers.begin(), numbers.end());
    int N = numbers.size();
    double n1 = (N - 1) * input_quantile + 1;
    int k1 = (int)n1;
    double d1 = n1 - k1;
    returned_quantile = numbers[k1 - 1] + d1 * (numbers[k1] - numbers[k1 - 1]);
    return returned_quantile;
  }
  void _describe_double()
  {
    if (_sum() == 0 && _mean() == 0 && _median() == 0 && _min() == 0 && _max() == 0)
    {
      std::cout << '\n'
                << "Series not found. check you column/row name or range";
    }
    else
    {
      std::cout << '\n'
                << '\n'
                << "### Summary Statistics of " << series_name << " ###" << '\n';
      Sum();
      Mean();
      Median();
      Min();
      Max();
      Quantiles();
      STDEV();
      Variance();
    }
  }

  void _describe_string()
  {
    std::map<std::string, int> string_occurences = _string_occurences();
    std::map<std::string, int>::iterator it;
    int n;
    std::string headers = series_has_headers ? "True" : "False";
    if (series_has_headers)
    {
      n = _data.size() - 1;
    }
    else
    {
      n = _data.size();
    }
    std::cout << '\n'
              << '\n'
              << "### Summary of " << series_name << " ###" << '\n'
              << "Total Number of Rows in " << series_name << ": " << n << '\n'
              << series_name << " Has Headers: " << headers << '\n';

    for (it = string_occurences.begin(); it != string_occurences.end(); it++)
    {
      std::cout << std::setprecision(2) << '\n'
                << "Value: "
                << "'" << it->first << "'" << '\n'
                << std::setw(32) << std::setfill('_') << '\n'
                << "  Count: " << it->second << " | "
                << "  Percent: " << (double)it->second / n * 100 << "%" << '\n'
                << '\n';
    }
  }

  bool is_series_numeric()
  {
    int number_of_chars = 0;
    int number_of_ints = 0;
    int begin;
    bool is_num;
    if (series_has_headers)
    {
      begin = 1;
    }
    else
    {
      begin = 0;
    }
    for (size_t i = begin; i < _data.size(); ++i)
    {
      std::string temp;
      std::stringstream temp_stream;
      temp_stream << _data[i];
      while (temp_stream >> temp)
      {
        for (size_t i = 0; i < temp.size(); ++i)
        {
          if (isalpha(temp[i]) || !isdigit(temp[i]) && temp[i] != '.')
          {
            number_of_chars++;
          }
          else if (isdigit(temp[i]))
          {
            number_of_ints++;
          }
        }
      }
    }
    if (number_of_chars == 0)
    {
      is_num = true;
    }
    else
    {
      is_num = false;
    }
    return is_num;
  }
  bool has_spaces()
  {
    int number_of_spaces = 0;
    bool has_space = false;
    for (size_t i = 0; i < _data.size(); ++i)
    {
      std::string temp;
      std::stringstream temp_stream;
      temp_stream << _data[i];
      while (temp_stream >> temp)
      {
        for (size_t i = 0; i < temp.size(); ++i)
        {
          if (isspace(temp[i]))
          {
            number_of_spaces++;
          }
        }
      }
    }
    if (number_of_spaces > 0)
    {
      has_space = true;
    }
    return has_space;
  }
  std::vector<double> fill_with_mean()
  {
    double mean = _mean();
    std::vector<double> returned_vector;
    std::vector<double> numbers = to_numeric_values(true);
    for (size_t i = 0; i < numbers.size(); ++i)
    {
      if (numbers[i] == 0)
      {
        numbers[i] = mean;
      }
      returned_vector.push_back(numbers[i]);
    }
    return returned_vector;
  }

  int FindMaxWwidth()
  {
    for (size_t i = 0; i < _data.size(); ++i)
    {
      int x = strlen(_data[i]);
    }
  }

  std::string _series_tolower(std::string input_string)
  {
    std::string lower_string;
    for (auto &s : input_string)
    {
      lower_string.push_back(tolower(s));
    }
    return lower_string;
  }
};

// SPECIALIZED CLASS <int>

template <>
class Series<int>
{
public:
  bool series_has_headers;
  Series(){};
  Series(std::initializer_list<int> input_elements)
  {
    for (auto &inp : input_elements)
    {
      _data.push_back(inp);
    }
  }
  Series(std::string){};

  std::vector<int> _data;

  void Describe()
  {
    if (series_name.empty())
    {
      series_name = "Series";
    }
    _describe_double();
  }
  double Sum()
  {
    double sum = _sum();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Sum of " << series_name << ": " << sum;
    return sum;
  }
  double Mean()
  {
    if (series_name.empty())
    {
      series_name = "Series";
    }
    double mean = _mean();
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Mean of " << series_name << ": " << mean;
    return mean;
  }
  double Median()
  {
    double median = _median();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Median of " << series_name << ": " << median;
    return median;
  }
  double Max()
  {
    double max = _max();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Maximum value of " << series_name << ": " << max;
    return max;
  }
  double Min()
  {
    double min = _min();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Minimum value of " << series_name << ": " << min;
    return min;
  }
  std::vector<double> Quantiles()
  {
    std::vector<double> returned_quartiles;
    std::vector<double> iqr = _iqr();
    std::cout << '\n'
              << "* Quantiles: ";
    for (size_t i = 0; i < iqr.size(); ++i)
    {
      returned_quartiles.push_back(iqr[i]);
    }
    std::cout << "Q1: " << iqr[0] << "| Q2 (median): " << iqr[1] << "| Q3: " << iqr[2] << '\n';
    return returned_quartiles;
  }
  double Quantiles(const float &input_quantile)
  {

    double iqr = _iqr(input_quantile);
    std::cout << '\n'
              << "* Quantile ";

    std::cout << input_quantile * 100 << "%"
              << " : " << iqr << '\n';
    return iqr;
  }
  double STDEV()
  {
    double stadv = _stdv();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout
        << "* Standard Deviation of " << series_name << ": " << stadv;
    return stadv;
  }
  double Variance()
  {
    double variance = _variance();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Variance of " << series_name << ": " << variance;
    return variance;
  }
  void PrintSeries()
  {
    std::cout.sync_with_stdio(false);
    std::cout << '\n';
    for (auto &d : _data)
    {
      std::cout << d << '\n';
    }
  }
  Series<int> Distinct()
  {
    Series<int> returned_series;
    std::set<int> distinct_values;
    for (size_t i = 0; i < _data.size(); ++i)
    {
      distinct_values.insert(_data[i]);
    }
    _data.assign(distinct_values.begin(), distinct_values.end());
    returned_series._data = _data;

    return returned_series;
  }

private:
  std::string series_name;
  double _sum()
  {
    double sum = 0;
    if (_data.size() == 0 || _data.empty())
    {
      return sum;
    }
    for (size_t i = 0; i < _data.size(); ++i)
    {
      sum += _data[i];
    }
    return sum;
  }
  double _mean()
  {
    double mean = 0;
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }

    mean = _sum() / _data.size();
    return mean;
  }
  double _median()
  {
    std::sort(_data.begin(), _data.end());
    int n = _data.size();
    double median = 0;
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    if (_data.size() % 2 == 0)
    {
      median = (_data[_data.size() / 2 - 1] + _data[_data.size() / 2]) / 2;
    }
    else
    {
      median = _data[_data.size() / 2];
    }
    return median;
  }

  double _max()
  {
    std::sort(_data.begin(), _data.end());
    int n = _data.size();
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    auto max = std::max_element(_data.begin(), _data.end());
    return *max;
  }
  double _min()
  {
    std::sort(_data.begin(), _data.end());
    int n = _data.size();
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    auto min = std::min_element(_data.begin(), _data.end());
    return *min;
  }
  double _stdv()
  {

    int n = _data.size();
    double sum = 0.0, mean = 0.0, variance = 0.0, stdDeviation = 0.0;

    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    for (size_t i = 0; i < n; ++i)
    {
      sum += _data[i];
    }
    mean = sum / n;
    for (size_t i = 0; i < n; ++i)
    {
      variance += pow(_data[i] - mean, 2);
    }
    variance /= n;
    stdDeviation = sqrt(variance);
    return stdDeviation;
  }
  double _variance()
  {
    int n = _data.size();
    double sum = 0.0, mean = 0.0, variance = 0.0;

    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    for (size_t i = 0; i < n; ++i)
    {
      sum += _data[i];
    }
    mean = sum / n;
    for (size_t i = 0; i < n; ++i)
    {
      variance += pow(_data[i] - mean, 2);
    }
    variance /= n;
    return variance;
  }

  void _describe_double()
  {
    if (_sum() == 0 && _mean() == 0 && _median() == 0 && _min() == 0 && _max() == 0)
    {
      std::cout << "Series not found. check you column/row name or range";
    }
    else
    {
      if (series_name.empty())
      {
        series_name = "Series";
      }
      std::cout << '\n'
                << '\n'
                << "### Summary Statistics ###" << '\n'
                << series_name << " Has " << _data.size() << " Rows";
      Sum();
      Mean();
      Median();
      Min();
      Max();
      Quantiles();
      STDEV();
      Variance();
    }
  }
  std::vector<double> _iqr()
  {
    std::vector<double> quartiles;
    double Q1;
    double Q2;
    double Q3;
    std::sort(_data.begin(), _data.end());
    int N = _data.size();
    double n1 = (N - 1) * 0.25 + 1;
    double n3 = (N - 1) * 0.75 + 1;
    int k1 = (int)n1;
    int k3 = (int)n3;
    double d1 = n1 - k1;
    double d3 = n3 - k3;
    Q1 = _data[k1 - 1] + d1 * (_data[k1] - _data[k1 - 1]);
    Q3 = _data[k3 - 1] + d3 * (_data[k3] - _data[k3 - 1]);
    if (_data.size() == 0 || _data.empty())
    {
      quartiles.push_back(0);
      return quartiles;
    }
    if (_data.size() % 2 == 0)
    {
      Q2 = (_data[_data.size() / 2 - 1] + _data[_data.size() / 2]) / 2;
    }
    else

      Q2 = _data[_data.size() / 2];
    quartiles.push_back(Q1);
    quartiles.push_back(Q2);
    quartiles.push_back(Q3);
    return quartiles;
  }
  double _iqr(float input_quantile)
  {
    double returned_quantile;
    std::sort(_data.begin(), _data.end());
    int N = _data.size();
    double n1 = (N - 1) * input_quantile + 1;
    int k1 = (int)n1;
    double d1 = n1 - k1;
    returned_quantile = _data[k1 - 1] + d1 * (_data[k1] - _data[k1 - 1]);

    return returned_quantile;
  }
};

// SPECIALIZED CLASS <unsigned int>

template <>
class Series<unsigned int>
{
public:
  bool series_has_headers;
  Series(){};
  Series(std::initializer_list<unsigned int> input_elements)
  {
    for (auto &inp : input_elements)
    {
      _data.push_back(inp);
    }
  }
  Series(std::string){};

  std::vector<unsigned int> _data;

  void Describe()
  {
    if (series_name.empty())
    {
      series_name = "Series";
    }
    _describe_double();
  }
  double Sum()
  {
    double sum = _sum();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Sum of " << series_name << ": " << sum;
    return sum;
  }
  double Mean()
  {
    if (series_name.empty())
    {
      series_name = "Series";
    }
    double mean = _mean();
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Mean of " << series_name << ": " << mean;
    return mean;
  }
  double Median()
  {
    double median = _median();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Median of " << series_name << ": " << median;
    return median;
  }
  double Max()
  {
    double max = _max();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Maximum value of " << series_name << ": " << max;
    return max;
  }
  double Min()
  {
    double min = _min();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Minimum value of " << series_name << ": " << min;
    return min;
  }
  std::vector<double> Quantiles()
  {
    std::vector<double> returned_quartiles;
    std::vector<double> iqr = _iqr();
    std::cout << '\n'
              << "* Quantiles: ";
    for (size_t i = 0; i < iqr.size(); ++i)
    {
      returned_quartiles.push_back(iqr[i]);
    }
    std::cout << "Q1: " << iqr[0] << "| Q2 (median): " << iqr[1] << "| Q3: " << iqr[2] << '\n';
    return returned_quartiles;
  }
  double Quantiles(const float &input_quantile)
  {

    double iqr = _iqr(input_quantile);
    std::cout << '\n'
              << "* Quantile ";

    std::cout << input_quantile * 100 << "%"
              << " : " << iqr << '\n';
    return iqr;
  }
  double STDEV()
  {
    double stadv = _stdv();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout
        << "* Standard Deviation of " << series_name << ": " << stadv;
    return stadv;
  }
  double Variance()
  {
    double variance = _variance();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Variance of " << series_name << ": " << variance;
    return variance;
  }
  void PrintSeries()
  {
    std::cout.sync_with_stdio(false);
    std::cout << '\n';
    for (auto &d : _data)
    {
      std::cout << d << '\n';
    }
  }
  Series<unsigned int> Distinct()
  {
    Series<unsigned int> returned_series;
    std::set<unsigned int> distinct_values;
    for (size_t i = 0; i < _data.size(); ++i)
    {
      distinct_values.insert(_data[i]);
    }
    _data.assign(distinct_values.begin(), distinct_values.end());
    returned_series._data = _data;

    return returned_series;
  }

private:
  std::string series_name;
  double _sum()
  {
    double sum = 0;
    if (_data.size() == 0 || _data.empty())
    {
      return sum;
    }
    for (size_t i = 0; i < _data.size(); ++i)
    {
      sum += _data[i];
    }
    return sum;
  }
  double _mean()
  {
    double mean = 0;
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    mean = _sum() / _data.size();
    return mean;
  }
  double _median()
  {
    std::sort(_data.begin(), _data.end());
    int n = _data.size();
    double median = 0;
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    if (_data.size() % 2 == 0)
    {
      median = (_data[_data.size() / 2 - 1] + _data[_data.size() / 2]) / 2;
    }
    else
    {
      median = _data[_data.size() / 2];
    }
    return median;
  }

  double _max()
  {
    std::sort(_data.begin(), _data.end());
    int n = _data.size();
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    auto max = std::max_element(_data.begin(), _data.end());
    return *max;
  }
  double _min()
  {
    std::sort(_data.begin(), _data.end());
    int n = _data.size();
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    auto min = std::min_element(_data.begin(), _data.end());
    return *min;
  }
  double _stdv()
  {

    int n = _data.size();
    double sum = 0.0, mean = 0.0, variance = 0.0, stdDeviation = 0.0;

    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    for (size_t i = 0; i < n; ++i)
    {
      sum += _data[i];
    }
    mean = sum / n;
    for (size_t i = 0; i < n; ++i)
    {
      variance += pow(_data[i] - mean, 2);
    }
    variance /= n;
    stdDeviation = sqrt(variance);
    return stdDeviation;
  }
  double _variance()
  {
    int n = _data.size();
    double sum = 0.0, mean = 0.0, variance = 0.0;

    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    for (size_t i = 0; i < n; ++i)
    {
      sum += _data[i];
    }
    mean = sum / n;
    for (size_t i = 0; i < n; ++i)
    {
      variance += pow(_data[i] - mean, 2);
    }
    variance /= n;
    return variance;
  }

  void _describe_double()
  {
    if (_sum() == 0 && _mean() == 0 && _median() == 0 && _min() == 0 && _max() == 0)
    {
      std::cout << "Series not found. check you column/row name or range";
    }
    else
    {
      if (series_name.empty())
      {
        series_name = "Series";
      }
      std::cout << '\n'
                << '\n'
                << "### Summary Statistics ###" << '\n'
                << series_name << " Has " << _data.size() << " Rows";
      Sum();
      Mean();
      Median();
      Min();
      Max();
      Quantiles();
      STDEV();
      Variance();
    }
  }
  std::vector<double> _iqr()
  {
    std::vector<double> quartiles;
    double Q1;
    double Q2;
    double Q3;
    std::sort(_data.begin(), _data.end());
    int N = _data.size();
    double n1 = (N - 1) * 0.25 + 1;
    double n3 = (N - 1) * 0.75 + 1;
    int k1 = (int)n1;
    int k3 = (int)n3;
    double d1 = n1 - k1;
    double d3 = n3 - k3;
    Q1 = _data[k1 - 1] + d1 * (_data[k1] - _data[k1 - 1]);
    Q3 = _data[k3 - 1] + d3 * (_data[k3] - _data[k3 - 1]);
    if (_data.size() == 0 || _data.empty())
    {
      quartiles.push_back(0);
      return quartiles;
    }
    if (_data.size() % 2 == 0)
    {
      Q2 = (_data[_data.size() / 2 - 1] + _data[_data.size() / 2]) / 2;
    }
    else

      Q2 = _data[_data.size() / 2];
    quartiles.push_back(Q1);
    quartiles.push_back(Q2);
    quartiles.push_back(Q3);
    return quartiles;
  }
  double _iqr(float input_quantile)
  {
    double returned_quantile;
    std::sort(_data.begin(), _data.end());
    int N = _data.size();
    double n1 = (N - 1) * input_quantile + 1;
    int k1 = (int)n1;
    double d1 = n1 - k1;
    returned_quantile = _data[k1 - 1] + d1 * (_data[k1] - _data[k1 - 1]);

    return returned_quantile;
  }
};

// SPECIALIZED CLASS <double>

template <>
class Series<double>
{
public:
  bool series_has_headers;
  Series(){};
  Series(std::initializer_list<double> input_elements)
  {
    for (auto &inp : input_elements)
    {
      _data.push_back(inp);
    }
  }
  Series(std::string){};

  std::vector<double> _data;

  void Describe()
  {
    if (series_name.empty())
    {
      series_name = "Series";
    }
    _describe_double();
  }
  double Sum()
  {
    double sum = _sum();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Sum of " << series_name << ": " << sum;
    return sum;
  }
  double Mean()
  {
    if (series_name.empty())
    {
      series_name = "Series";
    }
    double mean = _mean();
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Mean of " << series_name << ": " << mean;
    return mean;
  }
  double Median()
  {
    double median = _median();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Median of " << series_name << ": " << median;
    return median;
  }
  double Max()
  {
    double max = _max();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Maximum value of " << series_name << ": " << max;
    return max;
  }
  double Min()
  {
    double min = _min();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Minimum value of " << series_name << ": " << min;
    return min;
  }
  std::vector<double> Quantiles()
  {
    std::vector<double> returned_quartiles;
    std::vector<double> iqr = _iqr();
    std::cout << '\n'
              << "* Quantiles: ";
    for (size_t i = 0; i < iqr.size(); ++i)
    {
      returned_quartiles.push_back(iqr[i]);
    }
    std::cout << "Q1: " << iqr[0] << "| Q2 (median): " << iqr[1] << "| Q3: " << iqr[2] << '\n';
    return returned_quartiles;
  }
  double Quantiles(const float &input_quantile)
  {

    double iqr = _iqr(input_quantile);
    std::cout << '\n'
              << "* Quantile ";

    std::cout << input_quantile * 100 << "%"
              << " : " << iqr << '\n';
    return iqr;
  }
  double STDEV()
  {
    double stadv = _stdv();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout
        << "* Standard Deviation of " << series_name << ": " << stadv;
    return stadv;
  }
  double Variance()
  {
    double variance = _variance();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Variance of " << series_name << ": " << variance;
    return variance;
  }
  void PrintSeries()
  {
    std::cout.sync_with_stdio(false);
    std::cout << '\n';
    std::cout.setf(std::ios::fixed, std::ios::floatfield);
    std::cout.precision(6);
    for (auto &d : _data)
    {
      std::cout << d << '\n';
    }
  }
  Series<double> Distinct()
  {
    Series<double> returned_series;
    std::set<double> distinct_values;
    for (size_t i = 0; i < _data.size(); ++i)
    {
      distinct_values.insert(_data[i]);
    }
    _data.assign(distinct_values.begin(), distinct_values.end());
    returned_series._data = _data;

    return returned_series;
  }

private:
  std::string series_name;
  double _sum()
  {
    double sum = 0;
    if (_data.size() == 0 || _data.empty())
    {
      return sum;
    }
    for (size_t i = 0; i < _data.size(); ++i)
    {
      sum += _data[i];
    }
    return sum;
  }
  double _mean()
  {
    double mean = 0;
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    mean = _sum() / _data.size();
    return mean;
  }
  double _median()
  {
    std::sort(_data.begin(), _data.end());
    int n = _data.size();
    double median = 0;
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    if (_data.size() % 2 == 0)
    {
      median = (_data[_data.size() / 2 - 1] + _data[_data.size() / 2]) / 2;
    }
    else
    {
      median = _data[_data.size() / 2];
    }
    return median;
  }
  double _max()
  {
    std::sort(_data.begin(), _data.end());
    int n = _data.size();
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    auto max = std::max_element(_data.begin(), _data.end());
    return *max;
  }
  double _min()
  {
    std::sort(_data.begin(), _data.end());
    int n = _data.size();
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    auto min = std::min_element(_data.begin(), _data.end());
    return *min;
  }
  double _stdv()
  {
    int n = _data.size();
    double sum = 0.0, mean = 0.0, variance = 0.0, stdDeviation = 0.0;

    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    for (size_t i = 0; i < n; ++i)
    {
      sum += _data[i];
    }
    mean = sum / n;
    for (size_t i = 0; i < n; ++i)
    {
      variance += pow(_data[i] - mean, 2);
    }
    variance /= n;
    stdDeviation = sqrt(variance);
    return stdDeviation;
  }
  double _variance()
  {
    int n = _data.size();
    double sum = 0.0, mean = 0.0, variance = 0.0;

    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    for (size_t i = 0; i < n; ++i)
    {
      sum += _data[i];
    }
    mean = sum / n;
    for (size_t i = 0; i < n; ++i)
    {
      variance += pow(_data[i] - mean, 2);
    }
    variance /= n;
    return variance;
  }

  void _describe_double()
  {
    if (_sum() == 0 && _mean() == 0 && _median() == 0 && _min() == 0 && _max() == 0)
    {
      std::cout << "Series not found. check you column/row name or range";
    }
    else
    {
      if (series_name.empty())
      {
        series_name = "Series";
      }
      std::cout << '\n'
                << '\n'
                << "### Summary Statistics ###" << '\n'
                << series_name << " Has " << _data.size() << " Rows";
      Sum();
      Mean();
      Median();
      Min();
      Max();
      Quantiles();
      STDEV();
      Variance();
    }
  }
  std::vector<double> _iqr()
  {
    std::vector<double> quartiles;
    double Q1;
    double Q2;
    double Q3;
    std::sort(_data.begin(), _data.end());
    int N = _data.size();
    double n1 = (N - 1) * 0.25 + 1;
    double n3 = (N - 1) * 0.75 + 1;
    int k1 = (int)n1;
    int k3 = (int)n3;
    double d1 = n1 - k1;
    double d3 = n3 - k3;
    Q1 = _data[k1 - 1] + d1 * (_data[k1] - _data[k1 - 1]);
    Q3 = _data[k3 - 1] + d3 * (_data[k3] - _data[k3 - 1]);
    if (_data.size() == 0 || _data.empty())
    {
      quartiles.push_back(0);
      return quartiles;
    }
    if (_data.size() % 2 == 0)
    {
      Q2 = (_data[_data.size() / 2 - 1] + _data[_data.size() / 2]) / 2;
    }
    else
      Q2 = _data[_data.size() / 2];
    quartiles.push_back(Q1);
    quartiles.push_back(Q2);
    quartiles.push_back(Q3);
    return quartiles;
  }
  double _iqr(float input_quantile)
  {
    double returned_quantile;
    std::sort(_data.begin(), _data.end());
    int N = _data.size();
    double n1 = (N - 1) * input_quantile + 1;
    int k1 = (int)n1;
    double d1 = n1 - k1;
    returned_quantile = _data[k1 - 1] + d1 * (_data[k1] - _data[k1 - 1]);
    return returned_quantile;
  }
};

// SPECIALIZED CLASS <float>

template <>
class Series<float>
{
public:
  bool series_has_headers;
  Series(){};
  Series(std::initializer_list<float> input_elements)
  {
    for (auto &inp : input_elements)
    {
      _data.push_back(inp);
    }
  }
  Series(std::string){};

  std::vector<float> _data;

  void Describe()
  {
    if (series_name.empty())
    {
      series_name = "Series";
    }
    _describe_double();
  }
  double Sum()
  {
    double sum = _sum();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Sum of " << series_name << ": " << sum;
    return sum;
  }
  double Mean()
  {
    if (series_name.empty())
    {
      series_name = "Series";
    }
    double mean = _mean();
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Mean of " << series_name << ": " << mean;
    return mean;
  }
  double Median()
  {
    double median = _median();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Median of " << series_name << ": " << median;
    return median;
  }
  double Max()
  {
    double max = _max();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Maximum value of " << series_name << ": " << max;
    return max;
  }
  double Min()
  {
    double min = _min();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Minimum value of " << series_name << ": " << min;
    return min;
  }
  std::vector<double> Quantiles()
  {
    std::vector<double> returned_quartiles;
    std::vector<double> iqr = _iqr();
    std::cout << '\n'
              << "* Quantiles: ";
    for (size_t i = 0; i < iqr.size(); ++i)
    {
      returned_quartiles.push_back(iqr[i]);
    }
    std::cout << "Q1: " << iqr[0] << "| Q2 (median): " << iqr[1] << "| Q3: " << iqr[2] << '\n';
    return returned_quartiles;
  }
  double Quantiles(const float &input_quantile)
  {
    double iqr = _iqr(input_quantile);
    std::cout << '\n'
              << "* Quantile ";
    std::cout << input_quantile * 100 << "%"
              << " : " << iqr << '\n';
    return iqr;
  }
  double STDEV()
  {
    double stadv = _stdv();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout
        << "* Standard Deviation of " << series_name << ": " << stadv;
    return stadv;
  }
  double Variance()
  {
    double variance = _variance();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Variance of " << series_name << ": " << variance;
    return variance;
  }
  void Info();
  void PrintSeries()
  {
    std::cout.sync_with_stdio(false);
    std::cout.setf(std::ios::fixed, std::ios::floatfield);
    std::cout.precision(6);
    std::cout << '\n';
    for (auto &d : _data)
    {
      std::cout << d << '\n';
    }
  }
  Series<float> Distinct()
  {
    Series<float> returned_series;
    std::set<float> distinct_values;
    for (size_t i = 0; i < _data.size(); ++i)
    {
      distinct_values.insert(_data[i]);
    }
    _data.assign(distinct_values.begin(), distinct_values.end());
    returned_series._data = _data;
    return returned_series;
  }

private:
  std::string series_name;
  double _sum()
  {
    double sum = 0;
    if (_data.size() == 0 || _data.empty())
    {
      return sum;
    }
    for (size_t i = 0; i < _data.size(); ++i)
    {
      sum += _data[i];
    }
    return sum;
  }
  double _mean()
  {
    double mean = 0;
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    mean = _sum() / _data.size();
    return mean;
  }
  double _median()
  {
    std::sort(_data.begin(), _data.end());
    int n = _data.size();
    double median = 0;
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    if (_data.size() % 2 == 0)
    {
      median = (_data[_data.size() / 2 - 1] + _data[_data.size() / 2]) / 2;
    }
    else
    {
      median = _data[_data.size() / 2];
    }

    return median;
  }
  double _max()
  {
    std::sort(_data.begin(), _data.end());
    int n = _data.size();
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    auto max = std::max_element(_data.begin(), _data.end());
    return *max;
  }
  double _min()
  {
    std::sort(_data.begin(), _data.end());
    int n = _data.size();
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    auto min = std::min_element(_data.begin(), _data.end());
    return *min;
  }
  double _stdv()
  {

    int n = _data.size();
    double sum = 0.0, mean = 0.0, variance = 0.0, stdDeviation = 0.0;

    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    for (size_t i = 0; i < n; ++i)
    {
      sum += _data[i];
    }
    mean = sum / n;
    for (size_t i = 0; i < n; ++i)
    {
      variance += pow(_data[i] - mean, 2);
    }
    variance /= n;
    stdDeviation = sqrt(variance);
    return stdDeviation;
  }
  double _variance()
  {
    int n = _data.size();
    double sum = 0.0, mean = 0.0, variance = 0.0;

    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    for (size_t i = 0; i < n; ++i)
    {
      sum += _data[i];
    }
    mean = sum / n;
    for (size_t i = 0; i < n; ++i)
    {
      variance += pow(_data[i] - mean, 2);
    }
    variance /= n;
    return variance;
  }

  void _describe_double()
  {
    if (_sum() == 0 && _mean() == 0 && _median() == 0 && _min() == 0 && _max() == 0)
    {
      std::cout << "Series not found. check you column/row name or range";
    }
    else
    {
      if (series_name.empty())
      {
        series_name = "Series";
      }
      std::cout << '\n'
                << '\n'
                << "### Summary Statistics ###" << '\n'
                << series_name << " Has " << _data.size() << " Rows";
      Sum();
      Mean();
      Median();
      Min();
      Max();
      Quantiles();
      STDEV();
      Variance();
    }
  }
  std::vector<double> _iqr()
  {
    std::vector<double> quartiles;
    double Q1;
    double Q2;
    double Q3;
    std::sort(_data.begin(), _data.end());
    int N = _data.size();
    double n1 = (N - 1) * 0.25 + 1;
    double n3 = (N - 1) * 0.75 + 1;
    int k1 = (int)n1;
    int k3 = (int)n3;
    double d1 = n1 - k1;
    double d3 = n3 - k3;
    Q1 = _data[k1 - 1] + d1 * (_data[k1] - _data[k1 - 1]);
    Q3 = _data[k3 - 1] + d3 * (_data[k3] - _data[k3 - 1]);
    if (_data.size() == 0 || _data.empty())
    {
      quartiles.push_back(0);
      return quartiles;
    }
    if (_data.size() % 2 == 0)
    {
      Q2 = (_data[_data.size() / 2 - 1] + _data[_data.size() / 2]) / 2;
    }
    else

      Q2 = _data[_data.size() / 2];
    quartiles.push_back(Q1);
    quartiles.push_back(Q2);
    quartiles.push_back(Q3);
    return quartiles;
  }
  double _iqr(float input_quantile)
  {
    double returned_quantile;
    std::sort(_data.begin(), _data.end());
    int N = _data.size();
    double n1 = (N - 1) * input_quantile + 1;
    int k1 = (int)n1;
    double d1 = n1 - k1;
    returned_quantile = _data[k1 - 1] + d1 * (_data[k1] - _data[k1 - 1]);

    return returned_quantile;
  }
};

// SPECIALIZED CLASS <long>

template <>
class Series<long>
{
public:
  bool series_has_headers;
  Series(){};
  Series(std::initializer_list<long> input_elements)
  {
    for (auto &inp : input_elements)
    {
      _data.push_back(inp);
    }
  }
  Series(std::string){};

  std::vector<long> _data;

  void Describe()
  {
    if (series_name.empty())
    {
      series_name = "Series";
    }
    _describe_double();
  }
  double Sum()
  {
    double sum = _sum();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Sum of " << series_name << ": " << sum;
    return sum;
  }
  double Mean()
  {
    if (series_name.empty())
    {
      series_name = "Series";
    }
    double mean = _mean();
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Mean of " << series_name << ": " << mean;
    return mean;
  }
  double Median()
  {
    double median = _median();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Median of " << series_name << ": " << median;
    return median;
  }
  double Max()
  {
    double max = _max();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Maximum value of " << series_name << ": " << max;
    return max;
  }
  double Min()
  {
    double min = _min();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Minimum value of " << series_name << ": " << min;
    return min;
  }
  std::vector<double> Quantiles()
  {
    std::vector<double> returned_quartiles;
    std::vector<double> iqr = _iqr();
    std::cout << '\n'
              << "* Quantiles: ";
    for (size_t i = 0; i < iqr.size(); ++i)
    {
      returned_quartiles.push_back(iqr[i]);
    }
    std::cout << "Q1: " << iqr[0] << "| Q2 (median): " << iqr[1] << "| Q3: " << iqr[2] << '\n';
    return returned_quartiles;
  }
  double Quantiles(const float &input_quantile)
  {

    double iqr = _iqr(input_quantile);
    std::cout << '\n'
              << "* Quantile ";

    std::cout << input_quantile * 100 << "%"
              << " : " << iqr << '\n';
    return iqr;
  }
  double STDEV()
  {
    double stadv = _stdv();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout
        << "* Standard Deviation of " << series_name << ": " << stadv;
    return stadv;
  }
  double Variance()
  {
    double variance = _variance();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Variance of " << series_name << ": " << variance;
    return variance;
  }
  void PrintSeries()
  {
    std::cout.sync_with_stdio(false);
    std::cout << '\n';
    for (auto &d : _data)
    {
      std::cout << d << '\n';
    }
  }
  Series<long> Distinct()
  {
    Series<long> returned_series;
    std::set<long> distinct_values;
    for (size_t i = 0; i < _data.size(); ++i)
    {
      distinct_values.insert(_data[i]);
    }
    _data.assign(distinct_values.begin(), distinct_values.end());
    returned_series._data = _data;

    return returned_series;
  }

private:
  std::string series_name;
  double _sum()
  {
    double sum = 0;
    if (_data.size() == 0 || _data.empty())
    {
      return sum;
    }
    for (size_t i = 0; i < _data.size(); ++i)
    {
      sum += _data[i];
    }
    return sum;
  }
  double _mean()
  {
    double mean = 0;
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }

    mean = _sum() / _data.size();
    return mean;
  }
  double _median()
  {
    std::sort(_data.begin(), _data.end());
    int n = _data.size();
    double median = 0;
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    if (_data.size() % 2 == 0)
    {
      median = (_data[_data.size() / 2 - 1] + _data[_data.size() / 2]) / 2;
    }
    else
    {
      median = _data[_data.size() / 2];
    }

    return median;
  }
  double _max()
  {
    std::sort(_data.begin(), _data.end());
    int n = _data.size();
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    auto max = std::max_element(_data.begin(), _data.end());
    return *max;
  }
  double _min()
  {
    std::sort(_data.begin(), _data.end());
    int n = _data.size();
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    auto min = std::min_element(_data.begin(), _data.end());
    return *min;
  }
  double _stdv()
  {

    int n = _data.size();
    double sum = 0.0, mean = 0.0, variance = 0.0, stdDeviation = 0.0;

    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    for (size_t i = 0; i < n; ++i)
    {
      sum += _data[i];
    }
    mean = sum / n;
    for (size_t i = 0; i < n; ++i)
    {
      variance += pow(_data[i] - mean, 2);
    }
    variance /= n;
    stdDeviation = sqrt(variance);
    return stdDeviation;
  }
  double _variance()
  {
    int n = _data.size();
    double sum = 0.0, mean = 0.0, variance = 0.0;

    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    for (size_t i = 0; i < n; ++i)
    {
      sum += _data[i];
    }
    mean = sum / n;
    for (size_t i = 0; i < n; ++i)
    {
      variance += pow(_data[i] - mean, 2);
    }
    variance /= n;
    return variance;
  }

  void _describe_double()
  {
    if (_sum() == 0 && _mean() == 0 && _median() == 0 && _min() == 0 && _max() == 0)
    {
      std::cout << "Series not found. check you column/row name or range";
    }
    else
    {
      if (series_name.empty())
      {
        series_name = "Series";
      }
      std::cout << '\n'
                << '\n'
                << "### Summary Statistics ###" << '\n'
                << series_name << " Has " << _data.size() << " Rows";
      Sum();
      Mean();
      Median();
      Min();
      Max();
      Quantiles();
      STDEV();
      Variance();
    }
  }
  std::vector<double> _iqr()
  {
    std::vector<double> quartiles;
    double Q1;
    double Q2;
    double Q3;
    std::sort(_data.begin(), _data.end());
    int N = _data.size();
    double n1 = (N - 1) * 0.25 + 1;
    double n3 = (N - 1) * 0.75 + 1;
    int k1 = (int)n1;
    int k3 = (int)n3;
    double d1 = n1 - k1;
    double d3 = n3 - k3;
    Q1 = _data[k1 - 1] + d1 * (_data[k1] - _data[k1 - 1]);
    Q3 = _data[k3 - 1] + d3 * (_data[k3] - _data[k3 - 1]);
    if (_data.size() == 0 || _data.empty())
    {
      quartiles.push_back(0);
      return quartiles;
    }
    if (_data.size() % 2 == 0)
    {
      Q2 = (_data[_data.size() / 2 - 1] + _data[_data.size() / 2]) / 2;
    }
    else

      Q2 = _data[_data.size() / 2];
    quartiles.push_back(Q1);
    quartiles.push_back(Q2);
    quartiles.push_back(Q3);
    return quartiles;
  }
  double _iqr(float input_quantile)
  {
    double returned_quantile;
    std::sort(_data.begin(), _data.end());
    int N = _data.size();
    double n1 = (N - 1) * input_quantile + 1;
    int k1 = (int)n1;
    double d1 = n1 - k1;
    returned_quantile = _data[k1 - 1] + d1 * (_data[k1] - _data[k1 - 1]);

    return returned_quantile;
  }
};

// SPECIALIZED CLASS <short>

template <>
class Series<short>
{
public:
  bool series_has_headers;
  Series(){};
  Series(std::initializer_list<short> input_elements)
  {
    for (auto &inp : input_elements)
    {
      _data.push_back(inp);
    }
  }
  Series(std::string){};

  std::vector<short> _data;

  void Describe()
  {
    if (series_name.empty())
    {
      series_name = "Series";
    }
    _describe_double();
  }
  double Sum()
  {
    double sum = _sum();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Sum of " << series_name << ": " << sum;
    return sum;
  }
  double Mean()
  {
    if (series_name.empty())
    {
      series_name = "Series";
    }
    double mean = _mean();
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Mean of " << series_name << ": " << mean;
    return mean;
  }
  double Median()
  {
    double median = _median();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Median of " << series_name << ": " << median;
    return median;
  }
  double Max()
  {
    double max = _max();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Maximum value of " << series_name << ": " << max;
    return max;
  }
  double Min()
  {
    double min = _min();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Minimum value of " << series_name << ": " << min;
    return min;
  }
  std::vector<double> Quantiles()
  {
    std::vector<double> returned_quartiles;
    std::vector<double> iqr = _iqr();
    std::cout << '\n'
              << "* Quantiles: ";
    for (size_t i = 0; i < iqr.size(); ++i)
    {
      returned_quartiles.push_back(iqr[i]);
    }
    std::cout << "Q1: " << iqr[0] << "| Q2 (median): " << iqr[1] << "| Q3: " << iqr[2] << '\n';
    return returned_quartiles;
  }
  double Quantiles(const float &input_quantile)
  {

    double iqr = _iqr(input_quantile);
    std::cout << '\n'
              << "* Quantile ";

    std::cout << input_quantile * 100 << "%"
              << " : " << iqr << '\n';
    return iqr;
  }
  double STDEV()
  {
    double stadv = _stdv();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout
        << "* Standard Deviation of " << series_name << ": " << stadv;
    return stadv;
  }
  double Variance()
  {
    double variance = _variance();
    if (series_name.empty())
    {
      series_name = "Series";
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // set fixed floating format
    std::cout.precision(2);                                // for fixed format, two decimal places
    std::cout << '\n'
              << "* Variance of " << series_name << ": " << variance;
    return variance;
  }
  void PrintSeries()
  {
    std::cout.sync_with_stdio(false);
    std::cout << '\n';
    for (auto &d : _data)
    {
      std::cout << d << '\n';
    }
  }
  Series<short> Distinct()
  {
    Series<short> returned_series;
    std::set<short> distinct_values;
    for (size_t i = 0; i < _data.size(); ++i)
    {
      distinct_values.insert(_data[i]);
    }
    _data.assign(distinct_values.begin(), distinct_values.end());
    returned_series._data = _data;

    return returned_series;
  }

private:
  std::string series_name;
  double _sum()
  {
    double sum = 0;
    if (_data.size() == 0 || _data.empty())
    {
      return sum;
    }
    for (size_t i = 0; i < _data.size(); ++i)
    {
      sum += _data[i];
    }
    return sum;
  }
  double _mean()
  {
    double mean = 0;
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }

    mean = _sum() / _data.size();
    return mean;
  }
  double _median()
  {
    std::sort(_data.begin(), _data.end());
    int n = _data.size();
    double median = 0;
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    if (_data.size() % 2 == 0)
    {
      median = (_data[_data.size() / 2 - 1] + _data[_data.size() / 2]) / 2;
    }
    else
    {
      median = _data[_data.size() / 2];
    }

    return median;
  }
  double _max()
  {
    std::sort(_data.begin(), _data.end());
    int n = _data.size();
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    auto max = std::max_element(_data.begin(), _data.end());
    return *max;
  }
  double _min()
  {
    std::sort(_data.begin(), _data.end());
    int n = _data.size();
    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    auto min = std::min_element(_data.begin(), _data.end());
    return *min;
  }
  double _stdv()
  {

    int n = _data.size();
    double sum = 0.0, mean = 0.0, variance = 0.0, stdDeviation = 0.0;

    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    for (size_t i = 0; i < n; ++i)
    {
      sum += _data[i];
    }
    mean = sum / n;
    for (size_t i = 0; i < n; ++i)
    {
      variance += pow(_data[i] - mean, 2);
    }
    variance /= n;
    stdDeviation = sqrt(variance);
    return stdDeviation;
  }
  double _variance()
  {
    int n = _data.size();
    double sum = 0.0, mean = 0.0, variance = 0.0;

    if (_data.size() == 0 || _data.empty())
    {
      return 0;
    }
    for (size_t i = 0; i < n; ++i)
    {
      sum += _data[i];
    }
    mean = sum / n;
    for (size_t i = 0; i < n; ++i)
    {
      variance += pow(_data[i] - mean, 2);
    }
    variance /= n;
    return variance;
  }

  void _describe_double()
  {
    if (_sum() == 0 && _mean() == 0 && _median() == 0 && _min() == 0 && _max() == 0)
    {
      std::cout << "Series not found. check you column/row name or range";
    }
    else
    {
      if (series_name.empty())
      {
        series_name = "Series";
      }
      std::cout << '\n'
                << '\n'
                << "### Summary Statistics ###" << '\n'
                << series_name << " Has " << _data.size() << " Rows";
      Sum();
      Mean();
      Median();
      Min();
      Max();
      Quantiles();
      STDEV();
      Variance();
    }
  }
  std::vector<double> _iqr()
  {
    std::vector<double> quartiles;
    double Q1;
    double Q2;
    double Q3;
    std::sort(_data.begin(), _data.end());
    int N = _data.size();
    double n1 = (N - 1) * 0.25 + 1;
    double n3 = (N - 1) * 0.75 + 1;
    int k1 = (int)n1;
    int k3 = (int)n3;
    double d1 = n1 - k1;
    double d3 = n3 - k3;
    Q1 = _data[k1 - 1] + d1 * (_data[k1] - _data[k1 - 1]);
    Q3 = _data[k3 - 1] + d3 * (_data[k3] - _data[k3 - 1]);
    if (_data.size() == 0 || _data.empty())
    {
      quartiles.push_back(0);
      return quartiles;
    }
    if (_data.size() % 2 == 0)
    {
      Q2 = (_data[_data.size() / 2 - 1] + _data[_data.size() / 2]) / 2;
    }
    else

      Q2 = _data[_data.size() / 2];
    quartiles.push_back(Q1);
    quartiles.push_back(Q2);
    quartiles.push_back(Q3);
    return quartiles;
  }
  double _iqr(float input_quantile)
  {
    double returned_quantile;
    std::sort(_data.begin(), _data.end());
    int N = _data.size();
    double n1 = (N - 1) * input_quantile + 1;
    int k1 = (int)n1;
    double d1 = n1 - k1;
    returned_quantile = _data[k1 - 1] + d1 * (_data[k1] - _data[k1 - 1]);

    return returned_quantile;
  }
};

// SPECIALIZED CLASS <char>

template <>
class Series<char>
{
public:
  bool series_has_headers;
  Series(){};
  Series(std::initializer_list<char> input_elements)
  {
    series_has_headers = false;
    for (auto &inp : input_elements)
    {
      std::string i;
      i = inp;
      _data.push_back(i);
    }
  }
  Series(std::string){};

  std::vector<std::string> _data;

  void Describe()
  {
    if (series_name.empty())
    {
      series_name = "Series";
    }
    _describe_string();
  }

  void PrintSeries()
  {
    std::cout.sync_with_stdio(false);
    std::cout << '\n';
    for (auto &d : _data)
    {
      std::cout << d << '\n';
    }
  }
  Series<std::string> Distinct()
  {
    Series<std::string> returned_series;
    std::set<std::string> distinct_values;
    for (size_t i = 0; i < _data.size(); ++i)
    {
      distinct_values.insert(_data[i]);
    }
    _data.assign(distinct_values.begin(), distinct_values.end());
    returned_series._data = _data;

    return returned_series;
  }

private:
  std::string series_name;

  std::map<std::string, int> _string_occurences()
  {
    std::map<std::string, int> returned_map;
    int begins;
    if (series_has_headers)
    {
      begins = 1;
    }
    else
    {
      begins = 0;
    }
    for (size_t i = begins; i < _data.size(); ++i)
    {
      if (_data[i] == "" || _data[i].empty())
      {
        _data[i] = "N/A";
      }
    }
    for (size_t i = begins; i < _data.size(); ++i)
    {
      returned_map[_data[i]]++;
    }

    return returned_map;
  }
  void _describe_string()
  {
    std::map<std::string, int> string_occurences = _string_occurences();
    std::map<std::string, int>::iterator it;
    int n;
    std::string headers = series_has_headers ? "True" : "False";
    if (series_has_headers)
    {
      n = _data.size() - 1;
    }
    else
    {
      n = _data.size();
    }

    std::cout << '\n'
              << '\n'
              << "### Summary of " << series_name << " ###" << '\n'
              << "Total Number of Rows in " << series_name << ": " << n << '\n'
              << series_name << " Has Headers: " << headers << '\n';

    for (it = string_occurences.begin(); it != string_occurences.end(); it++)
    {
      std::cout << std::setprecision(2) << '\n'
                << "Value: "
                << "'" << it->first << "'" << '\n'
                << std::setw(32) << std::setfill('_') << '\n'
                << "  Count: " << it->second << " | "
                << "  Percent: " << (double)it->second / n * 100 << "%" << '\n'
                << '\n';
    }
  }
};

#endif