#include <iostream>
#include <queue>

using namespace std;

typedef pair<int, int> PII;

const int N = 15;
const int M = 1e5 + 10;

int n;                                 // ������
int arrival[N], burst[N], priority[N]; // ����ʱ�䣬����ʱ�䣬���ȼ�
int state[N] = {0};                    // ��¼�����Ƿ��ڶ�����
// ��¼��תʱ�䣬��Ȩ��תʱ��
double turn_around_time[N], weighted_turn_around_time[N];

// ָ�����ȼ��ȽϺ���
struct cmp
{
    bool operator()(PII a, PII b)
    {
        return a.first < b.first;
    }
};
/*
��������
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
    // ����������̵ĵ���ʱ�䣬����ʱ�䣬���ȼ�
    for (int i = 0; i < n; i++)
        cin >> arrival[i] >> burst[i] >> priority[i];
    // ���յ���ʱ������
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (arrival[i] > arrival[j])
            {
                swap(arrival[i], arrival[j]);
                swap(burst[i], burst[j]);
                swap(priority[i], priority[j]);
            }

    cout << "========= ���̻�����Ϣ =========" << endl
         << endl;
    // ��ӡ�������������ı�š�����ʱ�䡢����ʱ�䡢���ȼ�
    cout << "����\t����ʱ��\t����ʱ��\t���ȼ�" << endl;
    for (int i = 0; i < n; i++)
        cout << i + 1 << "\t" << arrival[i] << "\t\t" << burst[i] << "\t\t" << priority[i] << endl;

    cout << endl
         << endl
         << "============== ִ��ϸ�� ==============" << endl
         << endl;
    int totTime = 0;   // ��ʱ��
    bool flag = false; // �����Ƿ�ȫ��ִ�����
    // ����brust[N]�ĸ���
    int burst_copy[N];
    for (int i = 0; i < n; i++)
        burst_copy[i] = burst[i];
    // ����һ�������ȼ�Ϊ�ؼ��ֵ����ȶ���
    priority_queue<PII, vector<PII>, cmp> processing_queue;
    while (!flag)
    {
        flag = true;
        for (int i = 0; i < n; i++)
            if (burst[i] > 0)
                flag = false;
        // ����Ƿ����½��̵���
        for (int i = 0; i < n; i++)
            if (arrival[i] <= totTime && burst[i] > 0 && state[i] == 0)
            {
                processing_queue.push({priority[i], i});
                state[i] = 1;
            }

        // ִ�����ȼ���ߵĽ���
        if (processing_queue.size() > 0)
        {
            PII temp = processing_queue.top(); // ȡ�����ȼ���ߵĽ���
            int idx = temp.second;             // ȡ�����̱�ţ��õ���ż�������ȫ����Ϣ
            burst[idx]--;                      // ����ʱ���1
            totTime++;                         // ��ʱ���1
            // ��ӡ��ʱ��ִ�еĽ���
            cout << "ʱ�̣�" << totTime << "��ִ�н��̣�" << idx + 1 << "��ʣ�����ʱ�䣺" << burst[idx] << endl;
            if (burst[idx] == 0)
            {
                // ������תʱ�䣬��Ȩ��תʱ��
                turn_around_time[idx] = totTime - arrival[idx];
                weighted_turn_around_time[idx] = turn_around_time[idx] / burst_copy[idx];
                cout << "����" << idx + 1 << "ִ�����" << endl;
                // ִ����Ϻ󼴿ɴӶ�����ɾ��
                processing_queue.pop();
            }
        }
        else
        {
            cout << "ʱ�̣�" << totTime << "���޽���ִ��" << endl;
            totTime++; // ��ʱ���1
        }
    }
    cout << "���н��̾�ִ����ϣ���ʱ" << totTime << "��" << endl;

    cout << endl
         << endl
         << "============================= ִ���ܽ� =============================" << endl
         << endl;
    // ��ӡ����ִ�����
    cout << "����\t����ʱ��\t����ʱ��\t��תʱ��\t��Ȩ��תʱ��" << endl;
    for (int i = 0; i < n; i++)
        cout << i + 1 << "\t" << arrival[i] << "\t\t" << burst_copy[i] << "\t\t" << turn_around_time[i] << "\t\t" << weighted_turn_around_time[i] << endl;
    // ���㲢��ӡƽ����תʱ�䣬ƽ����Ȩ��תʱ��
    double avg_turn_around_time = 0, avg_weighted_turn_around_time = 0;
    for (int i = 0; i < n; i++)
    {
        avg_turn_around_time += turn_around_time[i];
        avg_weighted_turn_around_time += weighted_turn_around_time[i];
    }
    avg_turn_around_time /= n;
    avg_weighted_turn_around_time /= n;
    cout << "ƽ����תʱ�䣺" << avg_turn_around_time << endl;
    cout << "ƽ����Ȩ��תʱ�䣺" << avg_weighted_turn_around_time << endl;
    return 0;
}
