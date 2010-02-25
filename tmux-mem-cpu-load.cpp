
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include <unistd.h> // sleep

float cpu_percentage()
{
  string stat_line;
  size_t line_start_pos;
  size_t line_end_pos;
  unsigned long long current_user;
  unsigned long long current_system;
  unsigned long long current_nice;
  unsigned long long current_idle;
  unsigned long long next_user;
  unsigned long long next_system;
  unsigned long long next_nice;
  unsigned long long next_idle;
  unsigned long long diff_user;
  unsigned long long diff_system;
  unsigned long long diff_nice;
  unsigned long long diff_idle;

  ifstream stat_file("/proc/stat");
  getline(stat_file, stat_line);
  stat_file.close();

  // skip "cpu"
  line_start_pos = stat_line.find_first_not_of(" ", 3);
  line_end_pos = stat_line.find_first_of(' ', line_start_pos);
  line_end_pos = stat_line.find_first_of(' ', line_end_pos + 1);
  line_end_pos = stat_line.find_first_of(' ', line_end_pos + 1);
  line_end_pos = stat_line.find_first_of(' ', line_end_pos + 1);

  istringstream iss;
  iss.str( stat_line.substr( line_start_pos, line_end_pos - line_start_pos ) );
  iss >> current_user >> current_nice >> current_system >> current_idle;
  iss.clear();

  sleep(1);

  stat_file.open("/proc/stat");
  getline(stat_file, stat_line);
  stat_file.close();

  // skip "cpu"
  line_start_pos = stat_line.find_first_not_of(" ", 3);
  line_end_pos = stat_line.find_first_of(' ', line_start_pos);
  line_end_pos = stat_line.find_first_of(' ', line_end_pos + 1);
  line_end_pos = stat_line.find_first_of(' ', line_end_pos + 1);
  line_end_pos = stat_line.find_first_of(' ', line_end_pos + 1);

  iss.str( stat_line.substr( line_start_pos, line_end_pos - line_start_pos ) );
  iss >> next_user >> next_nice >> next_system >> next_idle;
  iss.clear();

  diff_user   = next_user - current_user;
  diff_system = next_system - current_system;
  diff_nice   = next_nice - current_nice;
  diff_idle   = next_idle - current_idle;

  return static_cast<float>(diff_user + diff_system + diff_nice)/static_cast<float>(diff_user + diff_system + diff_nice + diff_idle)*100.0;
}

string cpu_string()
{
  string meter = "[          ]";
  int meter_count = 0;
  float percentage;
  ostringstream oss;
  oss.precision( 1 );
  oss.setf( ios::fixed | ios::right );

  percentage = cpu_percentage();
  meter_count = 1;
  while(meter_count*9.99 < percentage)
    {
    meter[meter_count] = '|';
    meter_count++;
    }

  oss << "CPU:";
  oss << meter;
  oss.width( 5 );
  oss << percentage;
  oss << "%";

  return oss.str();
}

int main(int argc, char** argv)
{
  try
  {
  std::cout << cpu_string();
  }
  catch(const exception &e)
  {
    cerr << "Error: " << e.what() << endl;
    return 1;
  }
  
  return 0;
}

