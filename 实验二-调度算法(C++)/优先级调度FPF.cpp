/*
    status: Accepted
*/

#include <iostream>
#include <queue>

using namespace std;

typedef pair<int, int> PII;

const int N = 15;
const int M = 1e5 + 10;

int n;                                 // 进程数
int arrival[N], burst[N], priority[N]; // 到达时间，服务时间，优先级
int state[N] = {0};                    // 记录进程是否在队列中
// 记录周转时间，带权周转时间
double turn_around_time[N], weighted_turn_around_time[N];

// 指定优先级比较函数
struct cmp
{
  bool operator()(PII a, PII b)
  {
    return a.first < b.first;
  }
};
/*
测试数据
5
0 6 3
1 4 3
2 1 4
4 1 5
8 3 4
*/

int main()
{
  int n;
  cin >> n;
  // 输入各个进程的到达时间，服务时间，优先级
  for (int i = 0; i < n; i++)
    cin >> arrival[i] >> burst[i] >> priority[i];
  // 按照到达时间排序
  for (int i = 0; i < n; i++)
    for (int j = i + 1; j < n; j++)
      if (arrival[i] > arrival[j])
      {
        swap(arrival[i], arrival[j]);
        swap(burst[i], burst[j]);
        swap(priority[i], priority[j]);
      }

  cout << "========= 进程基本信息 =========" << endl
       << endl;
  // 打印各个进程排序后的编号、到达时间、服务时间、优先级
  cout << "进程\t到达时间\t服务时间\t优先级" << endl;
  for (int i = 0; i < n; i++)
    cout << i + 1 << "\t" << arrival[i] << "\t\t" << burst[i] << "\t\t" << priority[i] << endl;

  cout << endl
       << endl
       << "============== 执行细节 ==============" << endl
       << endl;
  int totTime = 0;   // 总时间
  bool flag = false; // 进程是否全部执行完毕
  // 创建brust[N]的副本
  int burst_copy[N];
  for (int i = 0; i < n; i++)
    burst_copy[i] = burst[i];
  // 定义一个以优先级为关键字的优先队列
  priority_queue<PII, vector<PII>, cmp> processing_queue;
  while (!flag)
  {
    flag = true;
    for (int i = 0; i < n; i++)
      if (burst[i] > 0)
        flag = false;
    // 检测是否有新进程到达
    for (int i = 0; i < n; i++)
      if (arrival[i] <= totTime && burst[i] > 0 && state[i] == 0)
      {
        processing_queue.push({priority[i], i});
        state[i] = 1;
      }

    // 执行优先级最高的进程
    if (processing_queue.size() > 0)
    {
      PII temp = processing_queue.top(); // 取出优先级最高的进程
      int idx = temp.second;             // 取出进程编号，拿到编号即可索引全部信息
      burst[idx]--;                      // 服务时间减1
      totTime++;                         // 总时间加1
      // 打印本时刻执行的进程
      cout << "时刻：" << totTime << "，执行进程：" << idx + 1 << "，剩余服务时间：" << burst[idx] << endl;
      if (burst[idx] == 0)
      {
        // 计算周转时间，带权周转时间
        turn_around_time[idx] = totTime - arrival[idx];
        weighted_turn_around_time[idx] = turn_around_time[idx] / burst_copy[idx];
        cout << "进程" << idx + 1 << "执行完毕" << endl;
        // 执行完毕后即可从队列中删除
        processing_queue.pop();
      }
    }
    else
    {
      cout << "时刻：" << totTime << "，无进程执行" << endl;
      totTime++; // 总时间加1
    }
  }
  cout << "所有进程均执行完毕，耗时" << totTime << "。" << endl;

  cout << endl
       << endl
       << "============================= 执行总结 =============================" << endl
       << endl;
  // 打印进程执行情况
  cout << "进程\t到达时间\t服务时间\t周转时间\t带权周转时间" << endl;
  for (int i = 0; i < n; i++)
    cout << i + 1 << "\t" << arrival[i] << "\t\t" << burst_copy[i] << "\t\t" << turn_around_time[i] << "\t\t" << weighted_turn_around_time[i] << endl;
  // 计算并打印平均周转时间，平均带权周转时间
  double avg_turn_around_time = 0, avg_weighted_turn_around_time = 0;
  for (int i = 0; i < n; i++)
  {
    avg_turn_around_time += turn_around_time[i];
    avg_weighted_turn_around_time += weighted_turn_around_time[i];
  }
  avg_turn_around_time /= n;
  avg_weighted_turn_around_time /= n;
  cout << "平均周转时间：" << avg_turn_around_time << endl;
  cout << "平均带权周转时间：" << avg_weighted_turn_around_time << endl;
  return 0;
}
