#include <iostream>
#include <queue>

using namespace std;

typedef pair<int, int> PII;

const int N = 15;
const int M = 1e5 + 10;

int n;                    // 进程数
int arrival[N], burst[N]; // 到达时间，服务时间
// 定义响应比
double response_ratio[N];
int state[N] = {0}; // 记录进程是否进入过队列（也就是是否完成）
// 记录周转时间，带权周转时间
double turn_around_time[N], weighted_turn_around_time[N];
// 指定优先级比较函数，用于优先队列
struct cmp
{
    bool operator()(PII a, PII b)
    {
        return a.first < b.first;
    }
};

/*
测试数据
4
0 7
2 4
4 1
5 4
*/

int main()
{
    int n;
    cin >> n;
    // 输入各个进程的到达时间，服务时间
    for (int i = 0; i < n; i++)
        cin >> arrival[i] >> burst[i];
    // 按照到达时间排序
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (arrival[i] > arrival[j])
            {
                swap(arrival[i], arrival[j]);
                swap(burst[i], burst[j]);
            }
    cout << "========= 进程基本信息 =========" << endl
         << endl;
    // 打印各个进程排序后的编号、到达时间、服务时间、优先级
    cout << "进程\t到达时间\t服务时间" << endl;
    for (int i = 0; i < n; i++)
        cout << i + 1 << "\t" << arrival[i] << "\t\t" << burst[i] << endl;
    cout << endl
         << endl
         << "============== 执行细节 ==============" << endl
         << endl;
    // 先处理最先到达的进程，由此计算响应比
    int totTime = burst[0] + arrival[0]; // 总时间
    bool flag = false;                   // 进程是否全部执行完毕
    // 打印这些时刻的进程执行情况
    for (int i = 0; i < arrival[0]; i++)
        cout << "时刻：" << i + 1 << "，无进程执行" << endl;
    for (int i = arrival[0]; i < totTime; i++)
    {
        if (burst[0] > 0)
            cout << "时刻：" << i + 1 << "，执行进程：" << 1 << "，剩余服务时间：" << burst[0] - i - 1 << endl;
        if (burst[0] - i - 1 == 0)
            cout << "进程" << 1 << "执行完毕" << endl;
    }
    state[0] = 1;
    // 计算响应比
    for (int i = 0; i < n; i++)
        response_ratio[i] = (double)(burst[i] + arrival[i]) / burst[i];
    // 计算周转时间，带权周转时间
    turn_around_time[0] = burst[0] + arrival[0];
    weighted_turn_around_time[0] = (double)turn_around_time[0] / burst[0];
    // 优先队列，按照响应比为关键字排序
    priority_queue<PII, vector<PII>, cmp> processing_queue;
    // 创建burst的副本
    int burst_copy[N];
    for (int i = 0; i < n; i++)
        burst_copy[i] = burst[i];
    while (!flag)
    {
        // 将所有到达的进程加入队列
        for (int i = 0; i < n; i++)
            if (arrival[i] <= totTime && state[i] == 0)
            {
                processing_queue.push(make_pair(response_ratio[i], i));
                state[i] = 1;
            }
        // 若队列为空，则时间加一
        if (processing_queue.empty())
        {
            cout << "时刻：" << ++totTime << "，无进程执行" << endl;
            continue;
        }
        // 从队列中取出响应比最高的进程执行
        PII p = processing_queue.top();
        processing_queue.pop(); // 该算法是非剥夺的，取出即会执行完毕
        int index = p.second;
        // 打印这些时刻的进程执行情况
        for (int i = 0; i < burst[index]; i++)
        {
            if (burst[index] > 0)
                cout << "时刻：" << totTime + i + 1 << "，执行进程：" << index + 1 << "，剩余服务时间：" << burst[index] - i - 1 << endl;
            if (burst[index] - i - 1 == 0)
            {
                cout << "进程" << index + 1 << "执行完毕" << endl;
                // 计算响应比
                for (int i = 0; i < n; i++)
                    response_ratio[i] = (double)(burst_copy[i] + arrival[i]) / burst_copy[i];
                // 计算周转时间和带权周转时间
                turn_around_time[index] = totTime + burst_copy[index] - arrival[index];
                weighted_turn_around_time[index] = (double)turn_around_time[index] / burst_copy[index];
            }
        }
        totTime += burst[index];   // 更新总时间
        burst[index] = 0;   // 更新服务时间，执行完毕剩余0
        // 判断是否所有进程都执行完毕，执行完毕则退出循环
        flag = true;
        for (int i = 1; i < n; i++)
            if (burst[i] > 0)
                flag = false;
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
