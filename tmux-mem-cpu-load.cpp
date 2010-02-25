/** 
 * @file cpuusage.cpp
 * @brief print out the percent cpu usage at a given interval
 * @author Matthew McCormick (thewtex)
 * @version 
 * @date 2009-08-12
 */
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include <unistd.h> // sleep

// in seconds
const static unsigned int INTERVAL=1;

class CPUPercentage
{
public:
  CPUPercentage();
  ~CPUPercentage();

  const float& get_percentage();

private:
  ifstream m_stat_file;
  unsigned long long m_current_user;
  unsigned long long m_current_system;
  unsigned long long m_current_nice;
  unsigned long long m_current_idle;
  unsigned long long m_next_user;
  unsigned long long m_next_system;
  unsigned long long m_next_nice;
  unsigned long long m_next_idle;
  unsigned long long m_diff_user;
  unsigned long long m_diff_system;
  unsigned long long m_diff_nice;
  unsigned long long m_diff_idle;

  string m_stat_line;
  size_t m_line_start_pos;
  size_t m_line_end_pos;
  istringstream m_iss;

  float m_percentage;
};

CPUPercentage::CPUPercentage():
  m_current_user(0),
  m_current_system(0),
  m_current_nice(0),
  m_current_idle(0),
  m_next_user(0),
  m_next_system(0),
  m_next_nice(0),
  m_next_idle(0),
  m_diff_user(0),
  m_diff_system(0),
  m_diff_nice(0),
  m_diff_idle(0)
{
  m_stat_file.exceptions(ifstream::eofbit|ifstream::failbit|ifstream::badbit);
}

CPUPercentage::~CPUPercentage()
{
  if(m_stat_file.is_open())
    m_stat_file.close();
}

const float& CPUPercentage::get_percentage()
{
  m_stat_file.open("/proc/stat");
  getline(m_stat_file, m_stat_line);
  m_stat_file.close();

  // skip "cpu"
  m_line_start_pos = m_stat_line.find_first_not_of(" ", 3);
  m_line_end_pos = m_stat_line.find_first_of(' ', m_line_start_pos);
  m_line_end_pos = m_stat_line.find_first_of(' ', m_line_end_pos + 1);
  m_line_end_pos = m_stat_line.find_first_of(' ', m_line_end_pos + 1);
  m_line_end_pos = m_stat_line.find_first_of(' ', m_line_end_pos + 1);
  m_iss.str(m_stat_line.substr(m_line_start_pos, m_line_end_pos - m_line_start_pos));
  m_iss >> m_next_user >> m_next_nice >> m_next_system >> m_next_idle;
  m_iss.clear();

  m_diff_user   = m_next_user - m_current_user;
  m_diff_system = m_next_system - m_current_system;
  m_diff_nice   = m_next_nice - m_current_nice;
  m_diff_idle   = m_next_idle - m_current_idle;
  m_percentage = static_cast<float>(m_diff_user + m_diff_system + m_diff_nice)/static_cast<float>(m_diff_user + m_diff_system + m_diff_nice + m_diff_idle)*100.0;

  m_current_user = m_next_user;
  m_current_system = m_next_system;
  m_current_nice = m_next_nice;
  m_current_idle = m_next_idle;

  return m_percentage;
}


int main(int argc, char** argv)
{
  string meter = "[          ]";
  int meter_count = 0;
  float percentage = 0.0;
  CPUPercentage cpu_per;
  cout.precision(1);
  cout.setf(ios::fixed | ios::right);
  cout.width(5);
  try
  {
  while(true)
    {
    percentage = cpu_per.get_percentage();
    
    meter_count = 1;
    while(meter_count*9.99 < percentage)
      {
      meter[meter_count] = '|';
      meter_count++;
      }
    while(meter_count <= 10)
      {
      meter[meter_count] = ' ';
      meter_count++;
      }
    cout << meter << ' ';
    cout.width(5);
    cout << percentage << endl; 
    sleep(1);
    }
  }
  catch(const exception &e)
  {
    cerr << "Error: " << e.what() << endl;
    return 1;
  }
  
  return 0;
}

