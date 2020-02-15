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
#include "dataframe.h"
#include <chrono>
#include <fstream>
#ifdef _WIN32
#define CLEAR "cls"
#else //In any other OS
#define CLEAR "clear"
#endif
#ifndef IOPERATION_H
#define IOPERATION_H
extern int all_rows;
// struct to obtain and hold dataframe dimenstions 
struct Dimensions
{
  int num_of_rows;
  int num_of_columns;
  bool file_exists = true;
  void GetDimensions(std::string filename)
  {
    __Row oneline;
    __Table alllines;
    std::string line, word;
    std::ifstream myreader(filename);
    if (!myreader.is_open())
    {
      std::cout << " ** File not found **" << '\n';
      file_exists = false;
      return;
    }
    while (getline(myreader, line, '\n'))
    {
      oneline.push_back(std::move(line));
    }
    for (size_t i = 0; i < oneline.size(); ++i)
    {
      std::stringstream s(oneline[i]);
      __Row arr;
      while (getline(s, word, ','))
      {
        arr.push_back(word);
      }
      alllines.push_back(arr);
    }
    num_of_rows = alllines.size();
    num_of_columns = alllines[0].size();
  }
};
// instantiate global dimensions object
Dimensions _dimensions;
// CSV parser
struct CsvParser
{
private:
  std::string file_base_name(std::string &filename)
  {
    std::string file_basename;
    std::string temp;
    int size = filename.size();
    for (int i = size; i > 0; i--)
    {
      temp.push_back(filename[i]);
      if (filename[i] == '\\')
      {
        break;
      }
    }
    temp.pop_back();
    for (size_t i = temp.size(); i > 0; --i)
    {
      file_basename.push_back(temp[i]);
    }
    return file_basename;
  }

  std::string file_base_name_wo_extention(std::string &filename)
  {
    std::string file_basename;
    std::string temp;
    std::string temp_wo_extention;
    int size = filename.size();
    for (int i = size; i > 0; i--)
    {
      temp.push_back(filename[i]);
      if (filename[i] == '\\')
      {
        break;
      }
    }
    temp.pop_back();
    for (size_t i = temp.size(); i > 0; i--)
    {
      temp_wo_extention.push_back(temp[i]);
      if (temp[i] == '.')
      {
        break;
      }
    }
    for (size_t i = 0; i < temp_wo_extention.size(); ++i)
    {
      file_basename.push_back(temp_wo_extention[i]);
    }
    file_basename.pop_back();
    return file_basename;
  }

public:
  bool is_done;
  DataFrame read_csv(std::string filename)
  {
    DataFrame df;

    _dimensions.GetDimensions(filename);
    df.has_header = true;
    __Row oneline;
    __Table alllines;
    std::string line, word;
    std::ifstream myreader(filename);
    if (!_dimensions.file_exists)
    {
      return df;
    }
    while (getline(myreader, line, '\n'))
    {
      oneline.push_back(std::move(line));
    }
    for (size_t i = 0; i < oneline.size(); ++i)
    {
      std::stringstream s(oneline[i]);
      __Row arr;
      while (getline(s, word, ','))
      {
        arr.push_back(word);
      }
      // if last cell in a line has no line break
      if (arr.size() < _dimensions.num_of_columns)
      {
        arr.push_back(" ");
      }
      alllines.push_back(arr);
    }
    df.raw_data = alllines;
    is_done = true;
    all_rows = df.raw_data.size() - 1;
    return df;
  }
  DataFrame read_csv(std::string filename, bool has_headers)
  {
    DataFrame df;
    df.has_header = has_headers;
    _dimensions.GetDimensions(filename);
    __Row oneline;
    __Table alllines;
    std::string line, word;
    std::ifstream myreader(filename);
    if (!_dimensions.file_exists)
    {
      return df;
    }
    while (getline(myreader, line, '\n'))
    {
      oneline.push_back(std::move(line));
    }
    for (size_t i = 0; i < oneline.size(); ++i)
    {

      std::stringstream s(oneline[i]);
      __Row arr;
      while (getline(s, word, ','))
      {
        arr.push_back(word);
      }
      // if last cell in a line has no line break
      if (arr.size() < _dimensions.num_of_columns)
      {
        arr.push_back("");
      }

      alllines.push_back(arr);
    }
    df.raw_data = alllines;
    is_done = true;
    return df;
  }
  DataFrame read_csv(std::string filename, bool has_headers, bool fill_na)
  {
    DataFrame df;
    df.has_header = has_headers;
    _dimensions.GetDimensions(filename);
    __Row oneline;
    __Table alllines;
    std::string line, word;
    std::ifstream myreader(filename);
    if (!_dimensions.file_exists)
    {
      return df;
    }
    while (getline(myreader, line, '\n'))
    {
      oneline.push_back(std::move(line));
    }
    for (size_t i = 0; i < oneline.size(); ++i)
    {
      std::stringstream s(oneline[i]);
      __Row arr;
      while (getline(s, word, ','))
      {
        arr.push_back(word);
      }
      // if last cell in a line has no line break
      if (arr.size() < _dimensions.num_of_columns)
      {
        if (fill_na)
        {
          arr.push_back("N/A");
        }
        else
        {
          arr.push_back("");
        }
      }
      if (fill_na)
      {
        for (size_t k = 0; k < arr.size(); k++)
        {
          if (arr[k] == "" || arr[k].empty())
          {
            arr[k] = "N/A";
          }
        }
      }

      alllines.push_back(arr);
    }

    df.raw_data = alllines;
    is_done = true;
    return df;
  }

  // Not working since MinGw currently doesn't support native c++ thread class
  DataFrame WaitingAnimation(std::string filename)
  {
    DataFrame df;
    _dimensions.GetDimensions(filename);
    if (!_dimensions.file_exists)
    {
      return df;
    }
    std::cout << "Loading Data...";
    std::chrono::seconds s(1);

    is_done = false;
    while (!is_done)
    {
      std::cout << "." << std::flush;
      std::cout << "." << std::flush;
      std::cout << "." << std::flush;
      std::cout << "\b\b\b   \b\b\b" << std::flush;

      df = read_csv(filename);
    }
    system(CLEAR);
    return df;
  }
  DataFrame WaitingAnimation(std::string filename, bool has_headers)
  {
    DataFrame df;
    _dimensions.GetDimensions(filename);
    if (!_dimensions.file_exists)
    {
      return df;
    }
    std::cout << "Loading Data...";
    std::chrono::seconds s(1);

    is_done = false;
    while (!is_done)
    {
      std::cout << "." << std::flush;
      std::cout << "." << std::flush;
      std::cout << "." << std::flush;
      std::cout << "\b\b\b   \b\b\b" << std::flush;
      df = read_csv(filename, has_headers);
    }
    system(CLEAR);
    return df;
  }
  DataFrame WaitingAnimation(std::string filename, bool has_headers, bool fill_na)
  {
    DataFrame df;
    _dimensions.GetDimensions(filename);
    if (!_dimensions.file_exists)
    {
      return df;
    }
    std::cout << "Loading Data...";
    std::chrono::seconds s(1);
    is_done = false;
    while (!is_done)
    {
      std::cout << "." << std::flush;
      std::cout << "." << std::flush;
      std::cout << "." << std::flush;
      std::cout << "\b\b\b   \b\b\b" << std::flush;
      df = read_csv(filename, has_headers, fill_na);
    }
    system(CLEAR);
    return df;
  }

  void WriteCsv(std::string filename, DataFrame output_dataframe)
  {
    if (output_dataframe.raw_data.size() == 0 || output_dataframe.raw_data.empty())
    {
      std::cout << "dataframe is empty";
      return;
    }
    std::ofstream output_file(filename);

    // write headers
    for (size_t i = 0; i < 1; ++i)
    {
      for (size_t j = 0; j < output_dataframe.raw_data[0].size(); ++j)
      {
        output_file << output_dataframe.raw_data[i][j] << ',';
      }
    }
    // write data
    for (size_t i = 1; i < output_dataframe.raw_data.size(); ++i)
    {
      output_file << '\n';
      for (size_t j = 0; j < output_dataframe.raw_data[0].size(); ++j)
      {
        output_file << output_dataframe.raw_data[i][j] << ',';
      }
    }
    std::cout << '\n'
              << '\n'
              << "** The file" << file_base_name(filename) << " has been saved";
  }
};
// JSON Parser
struct JsonParser
{
private:
  std::string file_base_name(std::string &filename)
  {
    std::string file_basename;
    std::string temp;
    int size = filename.size();

    for (int i = size; i > 0; i--)
    {
      temp.push_back(filename[i]);
      if (filename[i] == '\\')
      {
        break;
      }
    }
    temp.pop_back();
    for (size_t i = temp.size(); i > 0; --i)
    {
      file_basename.push_back(temp[i]);
    }
    return file_basename;
  }

  std::string file_base_name_wo_extention(std::string &filename)
  {
    std::string file_basename;
    std::string temp;
    std::string temp_wo_extention;
    int size = filename.size();
    for (int i = size; i > 0; i--)
    {
      temp.push_back(filename[i]);
      if (filename[i] == '\\')
      {
        break;
      }
    }
    temp.pop_back();
    for (size_t i = temp.size(); i > 0; i--)
    {
      temp_wo_extention.push_back(temp[i]);
      if (temp[i] == '.')
      {
        break;
      }
    }
    for (size_t i = 0; i < temp_wo_extention.size(); ++i)
    {
      file_basename.push_back(temp_wo_extention[i]);
    }
    file_basename.pop_back();
    return file_basename;
  }

public:
  __Table make_json(std::string filename, DataFrame output_dataframe)
  {
    __Table headers_column;
    __Table data_column;
    __Table all_data;
    if (output_dataframe.raw_data.size() == 0 || output_dataframe.raw_data.empty())
    {
      std::cout << "dataframe is empty";
      return all_data;
    }
    // column 1 - contains the headers
    int size = output_dataframe.raw_data[0].size();
    for (size_t i = 0; i < output_dataframe.raw_data[0].size(); ++i)
    {
      for (size_t j = 0; j < 1; ++j)
      {
        __Column column;
        column.push_back(output_dataframe.raw_data[j][i]);
        headers_column.push_back(column);
      }
    }
    // column 2 - data
    int number_of_attributes = headers_column.size();
    headers_column.resize(number_of_attributes * output_dataframe.raw_data.size());
    for (size_t i = 0; i < output_dataframe.raw_data.size(); ++i)
    {
      std::copy_n(headers_column.begin(), number_of_attributes, headers_column.begin() + number_of_attributes * i);
    }
    for (size_t i = 1; i < output_dataframe.raw_data.size(); ++i)
    {
      for (size_t j = 0; j < output_dataframe.raw_data[0].size(); ++j)
      {
        __Column column;
        column.push_back(output_dataframe.raw_data[i][j]);
        data_column.push_back(column);
      }
    }
    // stitching the two columns together
    for (size_t i = 0; i < data_column.size(); ++i)
    {
      __Row row;
      for (size_t j = 0; j < data_column[0].size(); ++j)
      {
        row.push_back("\"");
        row.push_back(headers_column[i][j]);
        row.push_back("\"");
        row.push_back(":");
        row.push_back("\"");
        row.push_back(data_column[i][j]);
        row.push_back("\"");
        // adding commas after every attribute but the the last in every object
        if (i % number_of_attributes - (number_of_attributes - 1) == 0 && i != 0)
        {
          row.push_back("");
        }
        else
        {
          row.push_back(",");
        }
      }
      all_data.push_back(row);
    }
    return all_data;
  }

  void WriteJson(std::string filename, DataFrame output_dataframe)
  {
    __Table returned_table = make_json(filename, output_dataframe);
    int original_count = output_dataframe.raw_data[0].size();
    std::ofstream output_file(filename);
    output_file << "[";
    output_file << "{";
    for (size_t i = 0; i < returned_table.size(); ++i)
    {
      output_file << '\n';
      if (i % original_count - (original_count - 1) + (original_count - 1) == 0 && i != 0)
      {
        output_file << "}," << '\n';
        output_file << "{" << '\n';
      }
      for (size_t j = 0; j < returned_table[0].size(); ++j)
      {
        output_file << returned_table[i][j];
      }
    }
    output_file << '\n'
                << "}" << '\n';
    output_file << '\n'
                << "]";
    std::cout << '\n'
              << "** The file" << file_base_name(filename) << " has been saved";
  }
};

// instantiate global csv and json parsers
CsvParser _csv_parser;
JsonParser _json_parser;

namespace ioperations
{
inline DataFrame ReadCsv(std::string filename)
{
  return  _csv_parser.WaitingAnimation(filename);
}
// read csv overload 2 - file name + whether or not there's header row
inline DataFrame ReadCsv(std::string filename, bool has_headers)
{
  return  _csv_parser.read_csv(filename, has_headers);
}
// read csv overload 3 - file name + whether or not there's header row + if to fill empty cells with "N/A"
inline DataFrame ReadCsv(std::string filename, bool has_headers, bool fill_na)
{
  return  _csv_parser.read_csv(filename, has_headers, fill_na);
}

inline void WriteCsv(std::string filename, DataFrame output_dataframe)
{
  _csv_parser.WriteCsv(filename,output_dataframe);
}

inline void WriteJson(std::string filename, DataFrame output_dataframe)
{
  _json_parser.WriteJson(filename, output_dataframe);
}

} // namespace ioperations

#endif