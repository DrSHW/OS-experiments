#include <iostream>
#include <queue>

using namespace std;

typedef pair<int, int> PII;

const int N = 15;
const int M = 1e5 + 10;

int n;                    // ������
int arrival[N], burst[N]; // ����ʱ�䣬����ʱ��
int state[N] = {0};       // ��¼�����Ƿ��������У�Ҳ�����Ƿ���ɣ�
int cur_time;             // ��¼��ǰʱ��
// ��¼��תʱ�䣬��Ȩ��תʱ��
double turn_around_time[N], weighted_turn_around_time[N];
double avg_turn_around_time = 0, avg_weighted_turn_around_time = 0;
// ��¼��Ӧ�ȣ�����HRRF�㷨
double response_ratio[N];
// ��¼���ȼ�������FPF_Preemptive�㷨
int priority[N];

/*
��������1��
4
0 7
2 4
4 1
5 4

��������2��
4
0 5 1
3 2 2
5 6 3
10 10 4
*/

void FCFS()
{
    // ���ȶ��У����յ���ʱ������
    priority_queue<PII, vector<PII>, greater<PII>> processing_queue;
    // �����н��̲������
    for (int i = 0; i < n; i++)
        processing_queue.push({arrival[i], i});
    // ��¼��ǰʱ��
    cur_time = 0;
    // ��¼��ɽ�����
    int cnt = 0;
    // �����в�Ϊ��ʱ
    while (!processing_queue.empty())
    {
        // ȡ�����׽���
        auto t = processing_queue.top();
        processing_queue.pop();
        // �����ǰ���̻�δ���
        if (!state[t.second])
        {
            // �����̻�δ����ȴ����̵���
            if (cur_time < t.first)
            {
                for (int i = 0; i < t.first - cur_time; i++)
                    cout << "ʱ�̣�" << cur_time + i + 1 << "���޽���ִ��" << endl;
                cur_time = t.first;
            }
            // ���µ�ǰʱ��
            cur_time = max(cur_time, t.first) + burst[t.second];
            // ������תʱ�䣬��Ȩ��תʱ��
            turn_around_time[t.second] = cur_time - arrival[t.second];
            weighted_turn_around_time[t.second] = (double)turn_around_time[t.second] / burst[t.second];
            // ����ƽ����תʱ�䣬ƽ����Ȩ��תʱ��
            avg_turn_around_time += turn_around_time[t.second];
            avg_weighted_turn_around_time += weighted_turn_around_time[t.second];
            // ��ǵ�ǰ�������
            state[t.second] = 1;
            // ��ɽ�������һ
            cnt++;
        }
        // ��ӡ��ǰ���̵�ִ�����
        for (int i = 0; i < burst[t.second]; i++)
            cout << "ʱ�̣�" << cur_time - burst[t.second] + i + 1 << "������" << t.second + 1 << "ִ��" << endl;
        cout << "����" << t.second + 1 << "ִ�����" << endl;
        // �����ɽ��������ڽ�������������ѭ��
        if (cnt == n)
            break;
    }
    // ����ƽ����תʱ�䣬ƽ����Ȩ��תʱ��
    avg_turn_around_time /= n;
    avg_weighted_turn_around_time /= n;
}

void SJF()
{
    // ���ȶ��У����շ���ʱ������
    priority_queue<PII, vector<PII>, greater<PII>> processing_queue;
    // ��¼��ǰʱ��
    cur_time = 0;
    // ��¼��ɽ�����
    int cnt = 0;
    // ����ɽ�����С�ڽ�������ʱ
    while (cnt < n)
    {
        // ��ն���
        while (!processing_queue.empty())
            processing_queue.pop();
        // �����е���ʱ��С�ڵ��ڵ�ǰʱ��Ľ��̲������
        for (int i = 0; i < n; i++)
            if (arrival[i] <= cur_time && !state[i])
                processing_queue.push({burst[i], i});
        // �������Ϊ�գ���ǰʱ���һ
        if (processing_queue.empty())
        {
            cur_time++;
            cout << "ʱ�̣�" << cur_time << "���޽���ִ��" << endl;
            continue;
        }
        // ȡ�����׽���
        auto t = processing_queue.top();
        processing_queue.pop();
        // ���µ�ǰʱ��
        cur_time += burst[t.second];
        // ������תʱ�䣬��Ȩ��תʱ��
        turn_around_time[t.second] = cur_time - arrival[t.second];
        weighted_turn_around_time[t.second] = (double)turn_around_time[t.second] / burst[t.second];
        // ����ƽ����תʱ�䣬ƽ����Ȩ��תʱ��
        avg_turn_around_time += turn_around_time[t.second];
        avg_weighted_turn_around_time += weighted_turn_around_time[t.second];
        // ��ǵ�ǰ�������
        state[t.second] = 1;
        // ��ɽ�������һ
        cnt++;
        // ��ӡ��ǰ���̵�ִ�����
        for (int i = 0; i < burst[t.second]; i++)
            cout << "ʱ�̣�" << cur_time - burst[t.second] + i + 1 << "������" << t.second + 1 << "ִ��" << endl;
        cout << "����" << t.second + 1 << "ִ�����" << endl;
    }
    // ����ƽ����תʱ�䣬ƽ����Ȩ��תʱ��
    avg_turn_around_time /= n;
    avg_weighted_turn_around_time /= n;
}

// �����Ӧ�������㷨
void HRRF()
{
    // �ȴ������ȵ���Ľ��̣��ɴ˼�����Ӧ��
    cur_time = burst[0] + arrival[0]; // ��ʱ��
    bool flag = false;                // �����Ƿ�ȫ��ִ�����
    // ��ӡ��Щʱ�̵Ľ���ִ�����
    for (int i = 0; i < arrival[0]; i++)
        cout << "ʱ�̣�" << i + 1 << "���޽���ִ��" << endl;
    for (int i = arrival[0]; i < cur_time; i++)
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
    priority_queue<PII, vector<PII>> processing_queue;
    // ����burst�ĸ���
    int burst_copy[N];
    for (int i = 0; i < n; i++)
        burst_copy[i] = burst[i];
    while (!flag)
    {
        // �����е���Ľ��̼������
        for (int i = 0; i < n; i++)
            if (arrival[i] <= cur_time && state[i] == 0)
            {
                processing_queue.push(make_pair(response_ratio[i], i));
                state[i] = 1;
            }
        // ������Ϊ�գ���ʱ���һ
        if (processing_queue.empty())
        {
            cout << "ʱ�̣�" << ++cur_time << "���޽���ִ��" << endl;
            continue;
        }
        // �Ӷ�����ȡ����Ӧ����ߵĽ���ִ��
        PII p = processing_queue.top();
        processing_queue.pop(); // ���㷨�Ƿǰ���ģ�ȡ������ִ�����
        int index = p.second;
        // ��ӡ��Щʱ�̵Ľ���ִ�����
        for (int i = 0; i < burst_copy[index]; i++)
        {
            if (burst_copy[index] > 0)
                cout << "ʱ�̣�" << cur_time + i + 1 << "��ִ�н��̣�" << index + 1 << "��ʣ�����ʱ�䣺" << burst[index] - i - 1 << endl;
            if (burst_copy[index] - i - 1 == 0)
            {
                cout << "����" << index + 1 << "ִ�����" << endl;
                // ������Ӧ��
                for (int i = 0; i < n; i++)
                    response_ratio[i] = (double)(burst[i] + arrival[i]) / burst[i];
                // ������תʱ��ʹ�Ȩ��תʱ��
                turn_around_time[index] = cur_time + burst[index] - arrival[index];
                weighted_turn_around_time[index] = (double)turn_around_time[index] / burst[index];
            }
        }
        cur_time += burst_copy[index]; // ������ʱ��
        burst_copy[index] = 0;         // ���·���ʱ�䣬ִ�����ʣ��0
        // �ж��Ƿ����н��̶�ִ����ϣ�ִ��������˳�ѭ��
        flag = true;
        for (int i = 1; i < n; i++)
            if (burst_copy[i] > 0)
                flag = false;
    }
    cout << "���н��̾�ִ����ϣ���ʱ" << cur_time << "��" << endl;
    cout << endl
         << endl
         << "============================= ִ���ܽ� =============================" << endl
         << endl;
    // ���㲢��ӡƽ����תʱ�䣬ƽ����Ȩ��תʱ��
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
    cur_time = 0;      // ��ʱ��
    bool flag = false; // �����Ƿ�ȫ��ִ�����
    // ����brust[N]�ĸ���
    int burst_copy[N];
    for (int i = 0; i < n; i++)
        burst_copy[i] = burst[i];
    // ����һ�������ȼ�Ϊ�ؼ��ֵ����ȶ���
    priority_queue<PII, vector<PII>> processing_queue;
    while (!flag)
    {
        flag = true;
        for (int i = 0; i < n; i++)
            if (burst_copy[i] > 0)
                flag = false;
        // ����Ƿ����½��̵���
        for (int i = 0; i < n; i++)
            if (arrival[i] <= cur_time && burst_copy[i] > 0 && state[i] == 0)
            {
                processing_queue.push({priority[i], i});
                state[i] = 1;
            }

        // ִ�����ȼ���ߵĽ���
        if (processing_queue.size() > 0)
        {
            PII temp = processing_queue.top(); // ȡ�����ȼ���ߵĽ���
            int idx = temp.second;             // ȡ�����̱�ţ��õ���ż�������ȫ����Ϣ
            burst_copy[idx]--;                      // ����ʱ���1
            cur_time++;                        // ��ʱ���1
            // ��ӡ��ʱ��ִ�еĽ���
            cout << "ʱ�̣�" << cur_time << "��ִ�н��̣�" << idx + 1 << "��ʣ�����ʱ�䣺" << burst_copy[idx] << endl;
            if (burst_copy[idx] == 0)
            {
                // ������תʱ�䣬��Ȩ��תʱ��
                turn_around_time[idx] = cur_time - arrival[idx];
                weighted_turn_around_time[idx] = turn_around_time[idx] / burst[idx];
                cout << "����" << idx + 1 << "ִ�����" << endl;
                // ִ����Ϻ󼴿ɴӶ�����ɾ��
                processing_queue.pop();
            }
        }
        else
        {
            cout << "ʱ�̣�" << cur_time << "���޽���ִ��" << endl;
            cur_time++; // ��ʱ���1
        }
    }
    cout << "���н��̾�ִ����ϣ���ʱ" << cur_time << "��" << endl;

    cout << endl
         << endl
         << "============================= ִ���ܽ� =============================" << endl
         << endl;

    // ���㲢��ӡƽ����תʱ�䣬ƽ����Ȩ��תʱ��
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
    // ���ȶ��У����շ���ʱ������
    priority_queue<PII, vector<PII>> processing_queue;
    // ��¼��ǰʱ��
    cur_time = 0;
    // ��¼��ɽ�����
    int cnt = 0;
    // ����ɽ�����С�ڽ�������ʱ
    while (cnt < n)
    {
        // ��ն���
        while (!processing_queue.empty())
            processing_queue.pop();
        // �����е���ʱ��С�ڵ��ڵ�ǰʱ��Ľ��̲������
        for (int i = 0; i < n; i++)
            if (arrival[i] <= cur_time && !state[i])
                processing_queue.push({priority[i], i});
        // �������Ϊ�գ���ǰʱ���һ
        if (processing_queue.empty())
        {
            cur_time++;
            cout << "ʱ�̣�" << cur_time << "���޽���ִ��" << endl;
            continue;
        }
        // ȡ�����׽���
        auto t = processing_queue.top();
        processing_queue.pop();
        // ���µ�ǰʱ��
        cur_time += burst[t.second];
        // ������תʱ�䣬��Ȩ��תʱ��
        turn_around_time[t.second] = cur_time - arrival[t.second];
        weighted_turn_around_time[t.second] = (double)turn_around_time[t.second] / burst[t.second];
        // ����ƽ����תʱ�䣬ƽ����Ȩ��תʱ��
        avg_turn_around_time += turn_around_time[t.second];
        avg_weighted_turn_around_time += weighted_turn_around_time[t.second];
        // ��ǵ�ǰ�������
        state[t.second] = 1;
        // ��ɽ�������һ
        cnt++;
        // ��ӡ��ǰ���̵�ִ�����
        for (int i = 0; i < burst[t.second]; i++)
            cout << "ʱ�̣�" << cur_time - burst[t.second] + i + 1 << "������" << t.second + 1 << "ִ��" << endl;
        cout << "����" << t.second + 1 << "ִ�����" << endl;
    }
    // ����ƽ����תʱ�䣬ƽ����Ȩ��תʱ��
    avg_turn_around_time /= n;
    avg_weighted_turn_around_time /= n;
}

void show_result()
{
    // ��ӡ�������̵���תʱ�䣬��Ȩ��תʱ��
    // ��ӡ����ִ�����
    cout << "����\t����ʱ��\t����ʱ��\t��תʱ��\t��Ȩ��תʱ��" << endl;
    for (int i = 0; i < n; i++)
        cout << i + 1 << "\t" << arrival[i] << "\t\t" << burst[i] << "\t\t" << turn_around_time[i] << "\t\t" << weighted_turn_around_time[i] << endl;
    // ��ӡƽ����תʱ�䣬ƽ����Ȩ��תʱ��
    cout << "ƽ����תʱ�䣺" << avg_turn_around_time << endl;
    cout << "ƽ����Ȩ��תʱ�䣺" << avg_weighted_turn_around_time << endl;
}

int main()
{
    // �û�����ģ��
    int _option;
    cout << "��ѡ������㷨��" << endl;
    cout << "1.�����ȷ������" << endl;
    cout << "2.�����ҵ����" << endl;
    cout << "3.�����Ӧ�ȵ���" << endl;
    cout << "4.�ɰ���ʽ������Ȩ����" << endl;
    cout << "5.���ɰ���ʽ������Ȩ����" << endl;
    while (cin >> _option)
        if (_option < 1 || _option > 5)
        {
            cout << "����������������룡" << endl;
            continue;
        }
        else
            break;
    cout << "�������������" << endl;
    while (cin >> n)
        if (n < 1)
        {
            cout << "����������С��1�����������룡" << endl;
            continue;
        }
        else
            break;
    // ����FCFS��SJF��HRRN�㷨������Ҫ������̵���ʱ��ͷ���ʱ�䣬������Ҫ����������ȼ�
    if (_option < 4)
    {
        cout << "��������̵���ʱ��ͷ���ʱ�䣺" << endl;
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
        // ִ���û�ѡ��ĵ����㷨
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
        cout << "��������̵���ʱ�䡢����ʱ������ȼ���" << endl;
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
        // ִ���û�ѡ��ĵ����㷨
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
    show_result();  // չʾ���
    return 0;
}
