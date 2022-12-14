#include <iostream>
#include <queue>

using namespace std;

typedef pair<int, int> PII;

const int N = 15;
const int M = 1e5 + 10;

int n;                    // 进程数
int arrival[N], burst[N]; // 到达时间，服务时间
int state[N] = {0};       // 记录进程是否进入过队列（也就是是否完成）
int cur_time;             // 记录当前时间
// 记录周转时间，带权周转时间
double turn_around_time[N], weighted_turn_around_time[N];
double avg_turn_around_time = 0, avg_weighted_turn_around_time = 0;
// 记录响应比，用于HRRF算法
double response_ratio[N];
// 记录优先级，用于FPF_Preemptive算法
int priority[N];

/*
测试数据1：
4
0 7
2 4
4 1
5 4

测试数据2：
4
0 5 1
3 2 2
5 6 3
10 10 4
*/

void FCFS()
{
    // 优先队列，按照到达时间排序
    priority_queue<PII, vector<PII>, greater<PII>> processing_queue;
    // 将所有进程插入队列
    for (int i = 0; i < n; i++)
        processing_queue.push({arrival[i], i});
    // 记录当前时间
    cur_time = 0;
    // 记录完成进程数
    int cnt = 0;
    // 当队列不为空时
    while (!processing_queue.empty())
    {
        // 取出队首进程
        auto t = processing_queue.top();
        processing_queue.pop();
        // 如果当前进程还未完成
        if (!state[t.second])
        {
            // 若进程还未到达，等待进程到达
            if (cur_time < t.first)
            {
                for (int i = 0; i < t.first - cur_time; i++)
                    cout << "时刻：" << cur_time + i + 1 << "，无进程执行" << endl;
                cur_time = t.first;
            }
            // 更新当前时间
            cur_time = max(cur_time, t.first) + burst[t.second];
            // 更新周转时间，带权周转时间
            turn_around_time[t.second] = cur_time - arrival[t.second];
            weighted_turn_around_time[t.second] = (double)turn_around_time[t.second] / burst[t.second];
            // 更新平均周转时间，平均带权周转时间
            avg_turn_around_time += turn_around_time[t.second];
            avg_weighted_turn_around_time += weighted_turn_around_time[t.second];
            // 标记当前进程完成
            state[t.second] = 1;
            // 完成进程数加一
            cnt++;
        }
        // 打印当前进程的执行情况
        for (int i = 0; i < burst[t.second]; i++)
            cout << "时刻：" << cur_time - burst[t.second] + i + 1 << "，进程" << t.second + 1 << "执行" << endl;
        cout << "进程" << t.second + 1 << "执行完毕" << endl;
        // 如果完成进程数等于进程总数，跳出循环
        if (cnt == n)
            break;
    }
    // 计算平均周转时间，平均带权周转时间
    avg_turn_around_time /= n;
    avg_weighted_turn_around_time /= n;
}

void SJF()
{
    // 优先队列，按照服务时间排序
    priority_queue<PII, vector<PII>, greater<PII>> processing_queue;
    // 记录当前时间
    cur_time = 0;
    // 记录完成进程数
    int cnt = 0;
    // 当完成进程数小于进程总数时
    while (cnt < n)
    {
        // 清空队列
        while (!processing_queue.empty())
            processing_queue.pop();
        // 将所有到达时间小于等于当前时间的进程插入队列
        for (int i = 0; i < n; i++)
            if (arrival[i] <= cur_time && !state[i])
                processing_queue.push({burst[i], i});
        // 如果队列为空，当前时间加一
        if (processing_queue.empty())
        {
            cur_time++;
            cout << "时刻：" << cur_time << "，无进程执行" << endl;
            continue;
        }
        // 取出队首进程
        auto t = processing_queue.top();
        processing_queue.pop();
        // 更新当前时间
        cur_time += burst[t.second];
        // 更新周转时间，带权周转时间
        turn_around_time[t.second] = cur_time - arrival[t.second];
        weighted_turn_around_time[t.second] = (double)turn_around_time[t.second] / burst[t.second];
        // 更新平均周转时间，平均带权周转时间
        avg_turn_around_time += turn_around_time[t.second];
        avg_weighted_turn_around_time += weighted_turn_around_time[t.second];
        // 标记当前进程完成
        state[t.second] = 1;
        // 完成进程数加一
        cnt++;
        // 打印当前进程的执行情况
        for (int i = 0; i < burst[t.second]; i++)
            cout << "时刻：" << cur_time - burst[t.second] + i + 1 << "，进程" << t.second + 1 << "执行" << endl;
        cout << "进程" << t.second + 1 << "执行完毕" << endl;
    }
    // 计算平均周转时间，平均带权周转时间
    avg_turn_around_time /= n;
    avg_weighted_turn_around_time /= n;
}

// 最高响应比优先算法
void HRRF()
{
    // 先处理最先到达的进程，由此计算响应比
    cur_time = burst[0] + arrival[0]; // 总时间
    bool flag = false;                // 进程是否全部执行完毕
    // 打印这些时刻的进程执行情况
    for (int i = 0; i < arrival[0]; i++)
        cout << "时刻：" << i + 1 << "，无进程执行" << endl;
    for (int i = arrival[0]; i < cur_time; i++)
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
    priority_queue<PII, vector<PII>> processing_queue;
    // 创建burst的副本
    int burst_copy[N];
    for (int i = 0; i < n; i++)
        burst_copy[i] = burst[i];
    while (!flag)
    {
        // 将所有到达的进程加入队列
        for (int i = 0; i < n; i++)
            if (arrival[i] <= cur_time && state[i] == 0)
            {
                processing_queue.push(make_pair(response_ratio[i], i));
                state[i] = 1;
            }
        // 若队列为空，则时间加一
        if (processing_queue.empty())
        {
            cout << "时刻：" << ++cur_time << "，无进程执行" << endl;
            continue;
        }
        // 从队列中取出响应比最高的进程执行
        PII p = processing_queue.top();
        processing_queue.pop(); // 该算法是非剥夺的，取出即会执行完毕
        int index = p.second;
        // 打印这些时刻的进程执行情况
        for (int i = 0; i < burst_copy[index]; i++)
        {
            if (burst_copy[index] > 0)
                cout << "时刻：" << cur_time + i + 1 << "，执行进程：" << index + 1 << "，剩余服务时间：" << burst[index] - i - 1 << endl;
            if (burst_copy[index] - i - 1 == 0)
            {
                cout << "进程" << index + 1 << "执行完毕" << endl;
                // 计算响应比
                for (int i = 0; i < n; i++)
                    response_ratio[i] = (double)(burst[i] + arrival[i]) / burst[i];
                // 计算周转时间和带权周转时间
                turn_around_time[index] = cur_time + burst[index] - arrival[index];
                weighted_turn_around_time[index] = (double)turn_around_time[index] / burst[index];
            }
        }
        cur_time += burst_copy[index]; // 更新总时间
        burst_copy[index] = 0;         // 更新服务时间，执行完毕剩余0
        // 判断是否所有进程都执行完毕，执行完毕则退出循环
        flag = true;
        for (int i = 1; i < n; i++)
            if (burst_copy[i] > 0)
                flag = false;
    }
    cout << "所有进程均执行完毕，耗时" << cur_time << "。" << endl;
    cout << endl
         << endl
         << "============================= 执行总结 =============================" << endl
         << endl;
    // 计算并打印平均周转时间，平均带权周转时间
    avg_turn_around_time = 0, avg_weighted_turn_around_time = 0;
    for (int i = 0; i < n; i++)
    {
        avg_turn_around_time += turn_around_time[i];
        avg_weighted_turn_around_time += weighted_turn_around_time[i];
    }
    avg_turn_around_time /= n;
    avg_weighted_turn_around_time /= n;
}

void FPF_Preemptive()
{
    cur_time = 0;      // 总时间
    bool flag = false; // 进程是否全部执行完毕
    // 创建brust[N]的副本
    int burst_copy[N];
    for (int i = 0; i < n; i++)
        burst_copy[i] = burst[i];
    // 定义一个以优先级为关键字的优先队列
    priority_queue<PII, vector<PII>> processing_queue;
    while (!flag)
    {
        flag = true;
        for (int i = 0; i < n; i++)
            if (burst_copy[i] > 0)
                flag = false;
        // 检测是否有新进程到达
        for (int i = 0; i < n; i++)
            if (arrival[i] <= cur_time && burst_copy[i] > 0 && state[i] == 0)
            {
                processing_queue.push({priority[i], i});
                state[i] = 1;
            }

        // 执行优先级最高的进程
        if (processing_queue.size() > 0)
        {
            PII temp = processing_queue.top(); // 取出优先级最高的进程
            int idx = temp.second;             // 取出进程编号，拿到编号即可索引全部信息
            burst_copy[idx]--;                      // 服务时间减1
            cur_time++;                        // 总时间加1
            // 打印本时刻执行的进程
            cout << "时刻：" << cur_time << "，执行进程：" << idx + 1 << "，剩余服务时间：" << burst_copy[idx] << endl;
            if (burst_copy[idx] == 0)
            {
                // 计算周转时间，带权周转时间
                turn_around_time[idx] = cur_time - arrival[idx];
                weighted_turn_around_time[idx] = turn_around_time[idx] / burst[idx];
                cout << "进程" << idx + 1 << "执行完毕" << endl;
                // 执行完毕后即可从队列中删除
                processing_queue.pop();
            }
        }
        else
        {
            cout << "时刻：" << cur_time << "，无进程执行" << endl;
            cur_time++; // 总时间加1
        }
    }
    cout << "所有进程均执行完毕，耗时" << cur_time << "。" << endl;

    cout << endl
         << endl
         << "============================= 执行总结 =============================" << endl
         << endl;

    // 计算并打印平均周转时间，平均带权周转时间
    avg_turn_around_time = 0, avg_weighted_turn_around_time = 0;
    for (int i = 0; i < n; i++)
    {
        avg_turn_around_time += turn_around_time[i];
        avg_weighted_turn_around_time += weighted_turn_around_time[i];
    }
    avg_turn_around_time /= n;
    avg_weighted_turn_around_time /= n;
}

void FPF_NonPreemptive()
{
    // 优先队列，按照服务时间排序
    priority_queue<PII, vector<PII>> processing_queue;
    // 记录当前时间
    cur_time = 0;
    // 记录完成进程数
    int cnt = 0;
    // 当完成进程数小于进程总数时
    while (cnt < n)
    {
        // 清空队列
        while (!processing_queue.empty())
            processing_queue.pop();
        // 将所有到达时间小于等于当前时间的进程插入队列
        for (int i = 0; i < n; i++)
            if (arrival[i] <= cur_time && !state[i])
                processing_queue.push({priority[i], i});
        // 如果队列为空，当前时间加一
        if (processing_queue.empty())
        {
            cur_time++;
            cout << "时刻：" << cur_time << "，无进程执行" << endl;
            continue;
        }
        // 取出队首进程
        auto t = processing_queue.top();
        processing_queue.pop();
        // 更新当前时间
        cur_time += burst[t.second];
        // 更新周转时间，带权周转时间
        turn_around_time[t.second] = cur_time - arrival[t.second];
        weighted_turn_around_time[t.second] = (double)turn_around_time[t.second] / burst[t.second];
        // 更新平均周转时间，平均带权周转时间
        avg_turn_around_time += turn_around_time[t.second];
        avg_weighted_turn_around_time += weighted_turn_around_time[t.second];
        // 标记当前进程完成
        state[t.second] = 1;
        // 完成进程数加一
        cnt++;
        // 打印当前进程的执行情况
        for (int i = 0; i < burst[t.second]; i++)
            cout << "时刻：" << cur_time - burst[t.second] + i + 1 << "，进程" << t.second + 1 << "执行" << endl;
        cout << "进程" << t.second + 1 << "执行完毕" << endl;
    }
    // 计算平均周转时间，平均带权周转时间
    avg_turn_around_time /= n;
    avg_weighted_turn_around_time /= n;
}

void show_result()
{
    // 打印各个进程的周转时间，带权周转时间
    // 打印进程执行情况
    cout << "进程\t到达时间\t服务时间\t周转时间\t带权周转时间" << endl;
    for (int i = 0; i < n; i++)
        cout << i + 1 << "\t" << arrival[i] << "\t\t" << burst[i] << "\t\t" << turn_around_time[i] << "\t\t" << weighted_turn_around_time[i] << endl;
    // 打印平均周转时间，平均带权周转时间
    cout << "平均周转时间：" << avg_turn_around_time << endl;
    cout << "平均带权周转时间：" << avg_weighted_turn_around_time << endl;
}

int main()
{
    // 用户交互模块
    int _option;
    cout << "请选择调度算法：" << endl;
    cout << "1.先来先服务调度" << endl;
    cout << "2.最短作业调度" << endl;
    cout << "3.最高响应比调度" << endl;
    cout << "4.可剥夺式高优先权调度" << endl;
    cout << "5.不可剥夺式高优先权调度" << endl;
    while (cin >> _option)
        if (_option < 1 || _option > 5)
        {
            cout << "输入错误，请重新输入！" << endl;
            continue;
        }
        else
            break;
    cout << "请输入进程数：" << endl;
    while (cin >> n)
        if (n < 1)
        {
            cout << "进程数不能小于1，请重新输入！" << endl;
            continue;
        }
        else
            break;
    // 若是FCFS、SJF、HRRN算法，则需要输入进程到达时间和服务时间，否则还需要输入进程优先级
    if (_option < 4)
    {
        cout << "请输入进程到达时间和服务时间：" << endl;
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
        // 执行用户选择的调度算法
        switch (_option)
        {
        case 1:
            FCFS();
            break;
        case 2:
            SJF();
            break;
        case 3:
            HRRF();
            break;
        }
    }
    else
    {
        cout << "请输入进程到达时间、服务时间和优先级：" << endl;
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
        // 执行用户选择的调度算法
        switch (_option)
        {
        case 4:
            FPF_Preemptive();
            break;
        case 5:
            FPF_NonPreemptive();
            break;
        }
    }
    show_result();  // 展示结果
    return 0;
}
