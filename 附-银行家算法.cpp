#include <iostream>

using namespace std;

int n, m; // n个进程，m种资源

const int MAX = 100; // 最大进程数

int available[MAX];       // 系统可用资源数
int _max[MAX][MAX];       // 最大需求矩阵
int allocation[MAX][MAX]; // 已分配矩阵
int need[MAX][MAX];       // 还需资源矩阵

int request[MAX]; // 请求向量
bool finish[MAX]; // 是否完成
int safe[MAX]; // 存储安全序列

/*
测试数据：
Test 1
In:
5 3
10 5 7
7 5 3
3 2 2
9 0 2
2 2 2
4 3 3
0 1 0
2 0 0
3 0 2
2 1 1
0 0 2
Out:
1 3 4 0 2

Test 2
In:
5 3
10 5 7
8 5 3 
3 2 2
9 5 2
2 2 2
4 3 6
0 1 0
2 0 0
3 0 2
2 1 1
0 0 2
Out:
No safe sequence
*/

bool find_senquence()
{
    int work[MAX]; // 可用资源向量
    int count = 0; // 安全序列长度
    // 初始化
    for (int i = 0; i < n; i++)
    {
        work[i] = available[i];
        finish[i] = false;
    }
    // 总资源减去已用资源
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            work[j] -= allocation[i][j];
    // 寻找安全序列
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (!finish[j])     // 判断j是否完成，若完成则直接跳过
            {
                int k;
                for (k = 0; k < m; k++) // 比对各维度的资源，任一维度不满足则跳出
                    if (need[j][k] > work[k])
                        break;
                if (k == m) 
                {
                    for (k = 0; k < m; k++)
                        work[k] += allocation[j][k];
                    finish[j] = true;
                    safe[count++] = j;
                }
            }
    // 判断是否有安全序列；若所有进程都能成功结束，则存在安全序列
    if (count == n)
        return true;
    else
        return false;
}

int main()
{
    // 参数输入
    cin >> n >> m;
    for (int i = 0; i < m; i++)
        cin >> available[i];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> _max[i][j];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> allocation[i][j];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = _max[i][j] - allocation[i][j];
    cout << "Process\t\t" << "Max\t" << "Allocation\t" << "Need\t" << endl;
    for (int i = 0; i < n; i++)
    {
        cout << "P" << i << "\t\t";
        for (int j = 0; j < m; j++)
            cout << _max[i][j] << " ";
        cout << "\t";
        for (int j = 0; j < m; j++)
            cout << allocation[i][j] << " ";
        cout << "\t\t";
        for (int j = 0; j < m; j++)
            cout << need[i][j] << " ";
        cout << endl;
    }
    cout << "Available\t";
    for (int i = 0; i < m; i++)
        cout << available[i] << ' ';
    cout << endl;

    if(find_senquence())  // 判断是否存在安全序列
    {
        cout << "存在安全序列，为：";
        for (int i = 0; i < n; i++)
            cout << safe[i] << " ";
        cout << endl;
    } else
        cout << "无安全序列" << endl;
    return 0;
}
