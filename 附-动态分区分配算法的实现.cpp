#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <algorithm>
using namespace std;

const int MAX_SIZE = 2560;

// 可变分区存储管理系统模拟
// 每一个作业的基本信息
typedef struct job
{
  string ID;      // 作业名
  int cpuRunTime; // 已经运行的CPU时间
  int runtime;    // 需要的运行时间
  int size;       // 作业大小
  string state;   // 作业状态
} Job;

// 作业队列结构
typedef struct runTable
{
  Job job;             // 作业
  int start;           // 起始地址
  int end;             // 结束地址
  int allocatedMethod; // 分配方式
} RunTable;

// 空闲分区表
typedef struct freeTable
{
  int size;     // 空闲区大小
  int start;    // 起始地址
  int end;      // 结束地址
  string state; // 空闲区状态
} FreeTable;

// 作业队列，用来存放处于运行状态的作业
vector<RunTable> run_table;
// 阻塞队列,用于存放无法加入到内存的作业
vector<RunTable> block_table;
// 空闲区队列，用于存放内存中的空闲区
vector<FreeTable> free_table;

int currentTime = 0; // 当前的时间片
int jobPtr = 0;      // 用于FF和BF算法的指针

/*
测试数据：
J1 15 600
J2 8 1000
J3 40 300
J4 30 700
J5 20 500
*/

// 初始化，设置内存最大为MAX_SIZE
void init()
{
  FreeTable ft;
  ft.start = 0;
  ft.end = MAX_SIZE;
  ft.size = MAX_SIZE;
  free_table.push_back(ft);
}

void showTime()
{
  cout << "当前时间片：" << currentTime << endl
       << endl;
}

// FF
bool FF(RunTable &j)
{
  for (int i = 0; i < free_table.size(); i++)
    // 如果选到的空闲区大小大于作业大小
    if (free_table[i].size > j.job.size)
    {
      j.start = free_table[i].start;                        // 更新起始地址
      j.end = j.start + j.job.size;                         // 更新结束地址
      free_table[i].start = j.end;                          // 更新空闲区起始地址
      free_table[i].size = free_table[i].size - j.job.size; // 更新空闲区大小
      run_table.push_back(j);                               // 将作业加入到运行队列
      return true;                                          // 加入成功
    }
    // 如果选到的空闲区大小等于作业大小，就把这个空闲区删掉
    else if (free_table[i].size == j.job.size)
    {
      j.start = free_table[i].start;            // 更新起始地址
      j.end = j.start + j.job.size;             // 更新结束地址
      free_table.erase(free_table.begin() + i); // 删除空闲区
      run_table.push_back(j);                   // 将作业加入到运行队列
      return true;                              // 加入成功
    }
  return false; // 加入失败
}

// NF
bool NF(RunTable &j)
{
  int i = jobPtr;
  for (int q = 0; q < free_table.size(); q++, i++)
  {
    if (i == free_table.size()) // 如果指针超出空闲区队列的范围，就将指针指向队列的第一个空闲区
      i = 0;
    // 如果选到的空闲区大小大于作业大小
    if (free_table[i].size > j.job.size)
    {
      j.start = free_table[i].start;
      j.end = j.start + j.job.size;
      free_table[i].start = j.end;
      free_table[i].size = free_table[i].size - j.job.size;
      run_table.push_back(j);
      return true; // 加入成功
    }
    // 如果选到的空闲区大小等于作业大小，就把这个空闲区删掉
    else if (free_table[i].size == j.job.size)
    {
      j.start = free_table[i].start;
      j.end = j.start + j.job.size;
      free_table.erase(free_table.begin() + i);
      run_table.push_back(j);
      return true; // 加入成功
    }
  }
  jobPtr = i;
  return false;
}

// BF
bool BF(RunTable &j)
{
  int minSize = free_table[0].size;
  int minIndex = 0;
  for (int i = 0; i < free_table.size(); i++)
    // 找满足条件的最小空闲区
    if (free_table[i].size >= j.job.size)
      if (free_table[i].size < minSize)
      {
        minSize = free_table[i].size;
        minIndex = i;
      }
  // 如果找到了满足条件的最小空闲区，和上面两种算法一样分配即可
  if (minSize > j.job.size)
  {
    j.start = free_table[minIndex].start;
    j.end = j.start + j.job.size;
    free_table[minIndex].start = j.end;
    free_table[minIndex].size = free_table[minIndex].size - j.job.size;
    run_table.push_back(j);
    return true; // 加入成功
  }
  else if (minSize == j.job.size)
  {
    j.start = free_table[minIndex].start;
    j.end = j.start + j.job.size;
    free_table.erase(free_table.begin() + minIndex);
    run_table.push_back(j);
    return true; // 加入成功
  }
  return false;
}

// WF
bool WF(RunTable &j)
{
  int maxSize = free_table[0].size;
  int maxIndex = 0;
  for (int i = 0; i < free_table.size(); i++)
    // 找满足条件的最大空闲区
    if (free_table[i].size >= j.job.size)
      if (free_table[i].size > maxSize)
      {
        maxSize = free_table[i].size;
        maxIndex = i;
      }
  // 如果找到了满足条件的最大空闲区，和上面两种算法一样分配即可
  if (maxSize > j.job.size)
  {
    j.start = free_table[maxIndex].start;
    j.end = j.start + j.job.size;
    free_table[maxIndex].start = j.end;
    free_table[maxIndex].size = free_table[maxIndex].size - j.job.size;
    run_table.push_back(j);
    return true; // 加入成功
  }
  else if (maxSize == j.job.size)
  {
    j.start = free_table[maxIndex].start;
    j.end = j.start + j.job.size;
    free_table.erase(free_table.begin() + maxIndex);
    run_table.push_back(j);
    return true; // 加入成功
  }
  return false;
}

// 根据内存起始和终结位置回收内存
void free(int start, int end)
{
  // 空闲表根据起始地址排序
  sort(free_table.begin(), free_table.end(), [](FreeTable a, FreeTable b)
       { return a.start < b.start; });
  // 找到空闲表中第一个起始地址大于回收内存的起始地址的空闲区
  int i = 0;
  for (; i < free_table.size(); i++)
    if (free_table[i].start > start)
      break;
  // 如果回收内存的起始地址和终结地址都在空闲表中的空闲区之间
  if (i != 0 && i != free_table.size())
    // 如果回收内存的起始地址和终结地址都在空闲表中的空闲区之间
    if (free_table[i - 1].start + free_table[i - 1].size == start && free_table[i].start == end)
    {
      free_table[i - 1].size = free_table[i - 1].size + free_table[i].size + end - start;
      free_table.erase(free_table.begin() + i);
    }
    // 如果回收内存的起始地址在空闲表中的空闲区之间，终结地址在空闲表中的空闲区之外
    else if (free_table[i - 1].start + free_table[i - 1].size == start && free_table[i].start > end)
    {
      free_table[i - 1].size = free_table[i - 1].size + end - start;
    }
    // 如果回收内存的起始地址在空闲表中的空闲区之外，终结地址在空闲表中的空闲区之间
    else if (free_table[i - 1].start + free_table[i - 1].size < start && free_table[i].start == end)
    {
      free_table[i].start = start;
      free_table[i].size = free_table[i].size + end - start;
    }
    // 如果回收内存的起始地址和终结地址都在空闲表中的空闲区之外
    else
    {
      FreeTable f;
      f.start = start;
      f.size = end - start;
      free_table.insert(free_table.begin() + i, f);
    }
  // 如果回收内存的起始地址和终结地址都在空闲表中的空闲区之外
  else if (i == 0 && i == free_table.size())
  {
    FreeTable f;
    f.start = start;
    f.size = end - start;
    free_table.insert(free_table.begin() + i, f);
  }
  // 如果回收内存的起始地址在空闲表中的空闲区之外，终结地址在空闲表中的空闲区之间
  else if (i == 0 && i != free_table.size())
    if (free_table[i].start == end)
    {
      free_table[i].start = start;
      free_table[i].size = free_table[i].size + end - start;
    }
    else
    {
      FreeTable f;
      f.start = start;
      f.size = end - start;
      free_table.insert(free_table.begin() + i, f);
    }
  // 如果回收内存的起始地址在空闲表中的空闲区之间，终结地址在空闲表中的空闲区之外
  else if (i != 0 && i == free_table.size())
    if (free_table[i - 1].start + free_table[i - 1].size == start)
      free_table[i - 1].size = free_table[i - 1].size + end - start;
    else
    {
      FreeTable f;
      f.start = start;
      f.size = end - start;
      free_table.insert(free_table.begin() + i, f);
    }
}

// 创建作业
void createJob()
{
  RunTable j;
  cout << "请依次输入作业的名称、运行时间、作业大小：" << endl;
  cin >> j.job.ID >> j.job.runtime >> j.job.size;
  j.job.cpuRunTime = 0;
  j.job.state = "运行中";
  cout << "请选择将采用的作业分配算法:\t1.FF算法\t2.NF算法\t3.BF算法\t4.WF算法" << endl;
  int choice;
  bool flag = false;
  cin >> choice;
  j.allocatedMethod = choice;
  switch (choice)
  {
  case 1:
    flag = FF(j);
    break;
  case 2:
    flag = NF(j);
    break;
  case 3:
    flag = BF(j);
    break;
  case 4:
    flag = WF(j);
    break;
  default:
    cout << "分配算法选择错误" << endl;
    system("pause");
  }
  if (!flag)
  {
    j.job.state = "阻塞";
    block_table.push_back(j);
    cout << "作业" << j.job.ID << "因为内存不足而被阻塞" << endl;
  }
}

// 运行一个时间片
void run()
{
  currentTime++;
  for (int i = 0; i < run_table.size(); i++)
  {
    run_table[i].job.cpuRunTime++;
    if (run_table[i].job.cpuRunTime == run_table[i].job.runtime)
    {
      cout << "作业" << run_table[i].job.ID << "运行结束" << endl;
      run_table[i].job.state = "结束";
      free(run_table[i].start, run_table[i].end); // 释放内存
      run_table.erase(run_table.begin() + i);
      i--;
    }
  }
}

// 检查阻塞队列中的作业是否可以运行
void check()
{
  for (int i = 0; i < block_table.size(); i++)
  {
    // 根据作业分配算法重新分配内存
    bool flag = false;
    switch (block_table[i].allocatedMethod)
    {
    case 1:
      flag = FF(block_table[i]);
      break;
    case 2:
      flag = NF(block_table[i]);
      break;
    case 3:
      flag = BF(block_table[i]);
      break;
    case 4:
      flag = WF(block_table[i]);
      break;
    default:
      break;
    }
    if (flag)
    {
      showTime();
      cout << "作业" << block_table[i].job.ID << "因为内存空闲而被调度" << endl;
      block_table[i].job.state = "运行中";
      block_table.erase(block_table.begin() + i);
      i--;
    }
  }
}

// 打印分区情况
void showTable()
{
  system("cls");
  showTime();
  cout << "\n=========  空闲分区表  =========\n"
       << endl;
  cout << "分区号\t"
       << "分区大小\t"
       << "分区始址\t" << endl;
  for (int i = 0; i < free_table.size(); i++)
    cout << i << "\t" << free_table[i].size << "\t\t" << free_table[i].start << endl;
  cout << "\n============================================   运行作业表   ===================================================\n"
       << endl;
  cout << "作业名\t"
       << "作业大小\t"
       << "作业状态\t"
       << "作业执行时间\t"
       << "作业运行时间\t"
       << "起始地址\t"
       << "末尾地址\t"
       << "分配方式" << endl;
  for (int i = 0; i < run_table.size(); i++)
  {
    cout << run_table[i].job.ID << "\t"
         << run_table[i].job.size << "\t\t"
         << run_table[i].job.state << "\t\t"
         << run_table[i].job.runtime << "\t\t"
         << run_table[i].job.cpuRunTime << "\t\t"
         << run_table[i].start << "\t\t"
         << run_table[i].end << "\t\t";
    switch (run_table[i].allocatedMethod)
    {
    case 1:
      cout << "FF算法" << endl;
      break;
    case 2:
      cout << "NF算法" << endl;
      break;
    case 3:
      cout << "BF算法" << endl;
      break;
    case 4:
      cout << "WF算法" << endl;
      break;
    default:
      break;
    }
  }
  cout << "\n=========  阻塞作业表  =========\n"
       << endl;
  cout << "作业名\t"
       << "作业大小\t"
       << "作业状态\t" << endl;
  for (int i = 0; i < block_table.size(); i++)
    cout << block_table[i].job.ID << "\t"
         << block_table[i].job.size << "\t\t"
         << block_table[i].job.state << "\t\t"
         << endl;
  cout << "\n=================================================================================\n"
       << endl;
}

int main()
{
  init();
  int choice;
  cout << "\n=======  初始空闲分区表  ======="
       << endl;
  cout << "分区号\t"
       << "分区大小\t"
       << "分区始址\t" << endl;
  for (int i = 0; i < free_table.size(); i++)
    cout << i << "\t" << free_table[i].size << "\t\t" << free_table[i].start << endl;
  while (true)
  {
    check();
    cout << "请选择操作:\t1.创建作业\t2.运行一个时间片\t3.退出" << endl;
    cin >> choice;
    switch (choice)
    {
    case 1:
      createJob(); // 再添加一个作业
      run();       // 先运行一个时间片
      break;
    case 2:
      run();
      break;
    case 3:
      exit(0);
    default:
      break;
    }
    showTable();
  }
  return 0;
}
