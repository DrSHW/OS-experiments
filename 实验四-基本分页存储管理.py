"""
    status: Pending
"""

from tkinter import *
import random
from tkinter import messagebox


class Job:
    """
        作业类，包含作业号、作业大小、占用物理块号、状态
    """
    def __init__(self, job_id, size):
        self.job_id = job_id  # 作业号
        self.size = size  # 作业大小
        self.blocks = []  # 占用物理块号
        self.status = 0  # 作业状态，0表示未分配，1表示已分配

    def __str__(self):
        return '作业号：' + str(self.job_id) + '，大小：' + str(self.size) + '，占用物理块号：' + str(self.blocks) + '，状态：' + str(
            self.status)

    def __repr__(self):
        return self.__str__()


class StatusMatrix:
    """
        状态矩阵类，包含物理块数、状态矩阵
    """
    def __init__(self, size):
        self.size = size  # 物理块数
        self.matrix = [0] * self.size  # 初始化状态矩阵

    def __str__(self):
        return str(self.matrix)

    def __repr__(self):
        return self.__str__()

    def get_free_blocks(self):
        free_blocks = []  # 存放空闲块号
        for i in range(len(self.matrix)):  # 遍历状态矩阵
            if self.matrix[i] == 0:  # 如果物理块空闲
                free_blocks.append(i)  # 记录空闲块号
        return free_blocks

    def allocate(self, blocks):
        for block in blocks:
            self.matrix[block] = 1  # 1表示已分配

    def recycle(self, blocks):
        for block in blocks:
            self.matrix[block] = 0  # 释放物理块


class JobList:
    """
        作业列表类，包含作业列表
    """
    def __init__(self):
        self.jobs = []  # 作业列表

    def __str__(self):
        return str(self.jobs)

    def __repr__(self):
        return self.__str__()

    def add_job(self, job):
        self.jobs.append(job)  # 添加作业

    def get_job(self, job_id):
        for job in self.jobs:  # 遍历作业列表
            if job.job_id == job_id:  # 找到要查找的作业
                return job
        return None

    def delete_job(self, job_id):
        for job in self.jobs:  # 遍历作业列表
            if job.job_id == job_id:  # 找到要删除的作业
                self.jobs.remove(job)  # 删除作业
                return job
        return None


# 初始化
def init():
    global status_matrix, job_list
    status_matrix = StatusMatrix(100)  # 初始化状态矩阵
    job_list = JobList()  # 初始化状态矩阵和作业列表
    print('初始化成功！')
    print('状态矩阵：', status_matrix)
    print('作业列表：', job_list)
    print('----------------------------------------')


# 分配
def allocate():
    global status_matrix, job_list
    job_id = int(wd.entry1.get())  # 获取输入的作业号
    size = int(wd.entry2.get())  # 获取输入的作业号和大小
    job = Job(job_id, size)  # 创建作业
    free_blocks = status_matrix.get_free_blocks()  # 获取空闲物理块
    if len(free_blocks) >= size:  # 如果空闲物理块数大于等于作业大小，则分配成功，否则分配失败
        blocks = random.sample(free_blocks, size)  # 随机分配
        job.blocks = blocks  # 记录分配的物理块号
        job.status = 1  # 记录作业状态
        job_list.add_job(job)  # 添加到作业列表
        status_matrix.allocate(blocks)  # 更新状态矩阵
        print('分配成功！')
        print('状态矩阵：', status_matrix)
        print('作业列表：', job_list)
        print('----------------------------------------')
        messagebox.showinfo('提示', '分配成功！')
        # 清空输入框
        wd.entry1.delete(0, END)
        wd.entry2.delete(0, END)
    else:
        print('分配失败！')
        print('状态矩阵：', status_matrix)
        print('作业列表：', job_list)
        print('----------------------------------------')
        messagebox.showinfo('提示', '分配失败！')


# 回收
def recycle():
    global status_matrix, job_list
    job_id = int(wd.entry3.get())  # 获取输入的作业号
    job = job_list.delete_job(job_id)  # 从作业列表中删除作业
    if job is not None:
        status_matrix.recycle(job.blocks)  # 回收物理块
        print('回收成功！')
        print('状态矩阵：', status_matrix)
        print('作业列表：', job_list)
        print('----------------------------------------')
        wd.entry3.delete(0, END)  # 清空输入框
        messagebox.showinfo('提示', '回收成功！')
    else:
        print('回收失败！')
        print('状态矩阵：', status_matrix)
        print('作业列表：', job_list)
        print('----------------------------------------')
        messagebox.showinfo('提示', '回收失败！')


# 查找
def search():
    global status_matrix, job_list
    job_id = int(wd.entry4.get())  # 获取输入的作业号
    job = job_list.get_job(job_id)  # 从作业列表中查找作业
    if job is not None:
        print('查找成功！')
        print('状态矩阵：', status_matrix)
        print('作业列表：', job_list)
        print('----------------------------------------')
        messagebox.showinfo('提示', '查找成功！')
        wd.entry4.delete(0, END)  # 清空输入框
    else:
        print('查找失败！')
        print('状态矩阵：', status_matrix)
        print('作业列表：', job_list)
        print('----------------------------------------')
        messagebox.showinfo('提示', '查找失败！')


# 退出
def _quit():
    print('退出成功！')
    messagebox.showinfo('提示', '退出成功！')
    wd.root.destroy()


init()


class Window:
    """ 
        界面类，用于创建界面中的各个组件
    """
    root = Tk()
    # 初始化区域
    init_button = Button(root, text='初始化', command=init)
    # 分配区域
    label1 = Label(root, text='作业号')
    entry1 = Entry(root)
    label2 = Label(root, text='作业大小')
    entry2 = Entry(root)
    allocate_button = Button(root, text='分配', command=allocate)
    # 回收区域
    label3 = Label(root, text='作业号')
    entry3 = Entry(root)
    recycle_button = Button(root, text='回收', command=recycle)
    # 查找区域
    label4 = Label(root, text='作业号')
    entry4 = Entry(root)
    search_button = Button(root, text='查找', command=search)
    # 退出区域
    exit_button = Button(root, text='退出', command=_quit)

    def __init__(self):
        self.root.title('内存分配模拟')
        self.root.geometry('500x300')
        self.root.resizable(0, 0)
        # 初始化区域
        self.init_button.place(x=50, y=50, width=100, height=30)
        # 分配区域
        self.label1.place(x=50, y=100, width=50, height=30)
        self.entry1.place(x=100, y=100, width=50, height=30)
        self.label2.place(x=150, y=100, width=50, height=30)
        self.entry2.place(x=200, y=100, width=50, height=30)
        self.allocate_button.place(x=250, y=100, width=100, height=30)
        # 回收区域
        self.label3.place(x=50, y=150, width=50, height=30)
        self.entry3.place(x=100, y=150, width=50, height=30)
        self.recycle_button.place(x=150, y=150, width=100, height=30)
        # 查找区域
        self.label4.place(x=50, y=200, width=50, height=30)
        self.entry4.place(x=100, y=200, width=50, height=30)
        self.search_button.place(x=150, y=200, width=100, height=30)
        # 退出区域
        self.exit_button.place(x=50, y=250, width=100, height=30)

    def run(self):
        self.root.mainloop()    # 启动GUI


if __name__ == '__main__':
    wd = Window()   # 实例化窗口对象
    wd.run()    # 启动GUI
