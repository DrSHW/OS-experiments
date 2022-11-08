#include <iostream>
#include <queue>

using namespace std;

typedef pair<int, int> PII;

const int N = 15;
const int M = 1e5 + 10;

int n;                    // ������
int arrival[N], burst[N]; // ����ʱ�䣬����ʱ��
// ������Ӧ��
double response_ratio[N];
int state[N] = {0}; // ��¼�����Ƿ��������У�Ҳ�����Ƿ���ɣ�
// ��¼��תʱ�䣬��Ȩ��תʱ��
double turn_around_time[N], weighted_turn_around_time[N];
// ָ�����ȼ��ȽϺ������������ȶ���
struct cmp
{
    bool operator()(PII a, PII b)
    {
        return a.first < b.first;
    }
};

/*
��������
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
    // ����������̵ĵ���ʱ�䣬����ʱ��
    for (int i = 0; i < n; i++)
        cin >> arrival[i] >> burst[i];
    // ���յ���ʱ������
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (arrival[i] > arrival[j])
            {
                swap(arrival[i], arrival[j]);
                swap(burst[i], burst[j]);
            }
    cout << "========= ���̻�����Ϣ =========" << endl
         << endl;
    // ��ӡ�������������ı�š�����ʱ�䡢����ʱ�䡢���ȼ�
    cout << "����\t����ʱ��\t����ʱ��" << endl;
    for (int i = 0; i < n; i++)
        cout << i + 1 << "\t" << arrival[i] << "\t\t" << burst[i] << endl;
    cout << endl
         << endl
         << "============== ִ��ϸ�� ==============" << endl
         << endl;
    // �ȴ������ȵ���Ľ��̣��ɴ˼�����Ӧ��
    int totTime = burst[0] + arrival[0]; // ��ʱ��
    bool flag = false;                   // �����Ƿ�ȫ��ִ�����
    // ��ӡ��Щʱ�̵Ľ���ִ�����
    for (int i = 0; i < arrival[0]; i++)
        cout << "ʱ�̣�" << i + 1 << "���޽���ִ��" << endl;
    for (int i = arrival[0]; i < totTime; i++)
    {
        if (burst[0] > 0)
            cout << "ʱ�̣�" << i + 1 << "��ִ�н��̣�" << 1 << "��ʣ�����ʱ�䣺" << burst[0] - i - 1 << endl;
        if (burst[0] - i - 1 == 0)
            cout << "����" << 1 << "ִ�����" << endl;
    }
    state[0] = 1;
    // ������Ӧ��
    for (int i = 0; i < n; i++)
        response_ratio[i] = (double)(burst[i] + arrival[i]) / burst[i];
    // ������תʱ�䣬��Ȩ��תʱ��
    turn_around_time[0] = burst[0] + arrival[0];
    weighted_turn_around_time[0] = (double)turn_around_time[0] / burst[0];
    // ���ȶ��У�������Ӧ��Ϊ�ؼ�������
    priority_queue<PII, vector<PII>, cmp> processing_queue;
    // ����burst�ĸ���
    int burst_copy[N];
    for (int i = 0; i < n; i++)
        burst_copy[i] = burst[i];
    while (!flag)
    {
        // �����е���Ľ��̼������
        for (int i = 0; i < n; i++)
            if (arrival[i] <= totTime && state[i] == 0)
            {
                processing_queue.push(make_pair(response_ratio[i], i));
                state[i] = 1;
            }
        // ������Ϊ�գ���ʱ���һ
        if (processing_queue.empty())
        {
            cout << "ʱ�̣�" << ++totTime << "���޽���ִ��" << endl;
            continue;
        }
        // �Ӷ�����ȡ����Ӧ����ߵĽ���ִ��
        PII p = processing_queue.top();
        processing_queue.pop(); // ���㷨�Ƿǰ���ģ�ȡ������ִ�����
        int index = p.second;
        // ��ӡ��Щʱ�̵Ľ���ִ�����
        for (int i = 0; i < burst[index]; i++)
        {
            if (burst[index] > 0)
                cout << "ʱ�̣�" << totTime + i + 1 << "��ִ�н��̣�" << index + 1 << "��ʣ�����ʱ�䣺" << burst[index] - i - 1 << endl;
            if (burst[index] - i - 1 == 0)
            {
                cout << "����" << index + 1 << "ִ�����" << endl;
                // ������Ӧ��
                for (int i = 0; i < n; i++)
                    response_ratio[i] = (double)(burst_copy[i] + arrival[i]) / burst_copy[i];
                // ������תʱ��ʹ�Ȩ��תʱ��
                turn_around_time[index] = totTime + burst_copy[index] - arrival[index];
                weighted_turn_around_time[index] = (double)turn_around_time[index] / burst_copy[index];
            }
        }
        totTime += burst[index];   // ������ʱ��
        burst[index] = 0;   // ���·���ʱ�䣬ִ�����ʣ��0
        // �ж��Ƿ����н��̶�ִ����ϣ�ִ��������˳�ѭ��
        flag = true;
        for (int i = 1; i < n; i++)
            if (burst[i] > 0)
                flag = false;
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
