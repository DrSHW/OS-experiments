"""
    status: Pending
"""

from tkinter import *
import random
from tkinter import messagebox
import math


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
        状态矩阵类，包含物理块数、状态矩阵、页表
    """
    def __init__(self, block_num=128, block_size=8):
        self.block_num = block_num  # 物理块数
        # 单个物理块的大小
        self.block_size = block_size
        self.matrix = [0] * self.block_num  # 初始化状态矩阵
        self.page_list = []  # 初始页表为空

    def __str__(self):
        return str(self.matrix)

    def __repr__(self):
        return self.__str__()

    def get_free_blocks(self):
        free_blocks = []  # 存放空闲物理块号
        for i in range(len(self.matrix)):  # 遍历状态矩阵
            if self.matrix[i] == 0:  # 如果物理块空闲
                free_blocks.append(i)  # 记录空闲块号
        return free_blocks

    def allocate(self, blocks, size, is_full=True):  # 传入物理块号列表，块大小，是否为完全分配
        if size > self.block_size and is_full:  # 如果作业大小大于单个物理块大小，且为完全分配
            size = self.block_size  # 作业大小等于单个物理块大小
        elif size > self.block_size and not is_full:  # 如果作业大小大于单个物理块大小，且为不完全分配
            size = size % self.block_size  # 作业大小等于作业大小对单个物理块大小取余，即偏移量
        for block in blocks:
            have_free_block = False  # 是否有空闲块
            for page in self.page_list:  # 遍历页表
                if page == -1:  # 如果页表中有空闲页
                    have_free_block = True  # 有空闲块
                    page_list_index = self.page_list.index(page)  # 页表中空闲块的索引
                    self.page_list[page_list_index] = block  # 将页表中的空闲块号替换为新的物理块号
                    break
            if not have_free_block:  # 如果没有空闲块
                self.page_list.append(block)  # 将物理块号加入页表
            self.matrix[block] = size  # 表示该内存块被占用，占用的大小为size

    def recycle(self, blocks):
        for block in blocks:
            self.matrix[block] = 0  # 释放物理块
        # 删去页表中的物理块号
        for page in self.page_list:
            if page in blocks:
                # 将值置为-1，表示该页已被释放
                self.page_list[self.page_list.index(page)] = -1


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

    def get_job_by_id(self, job_id):
        for job in self.jobs:  # 遍历作业列表
            if job.job_id == job_id:  # 找到要查找的作业
                return job
        return None  # 找不到则返回None

    def get_job_by_block(self, block):
        for job in self.jobs:  # 遍历作业列表
            if block in job.blocks:  # 找到要查找的作业
                return job  # 返回作业编号
        return None

    def delete_job(self, job_id):
        for job in self.jobs:  # 遍历作业列表
            if job.job_id == job_id:  # 找到要删除的作业
                self.jobs.remove(job)  # 删除作业
                return job
        return None  # 找不到则返回None


# 初始化
def init():
    global status_matrix, job_list
    status_matrix = StatusMatrix(128, 8)  # 初始化状态矩阵
    job_list = JobList()  # 初始化状态矩阵和作业列表
    print('初始化成功！')
    print('初始状态矩阵：')
    for i in range(0, 128, 8):
        print(status_matrix.matrix[i:i + 8])
    print('初始作业列表、页表为空。')
    print('----------------------------------------')


# 查看当前的内存状态
def check():
    global status_matrix, job_list
    print('状态矩阵：')
    for i in range(0, 128, 8):
        print(status_matrix.matrix[i:i + 8])
    print('作业列表：')
    for job in job_list.jobs:
        print(job)
    print('页表：')
    print('页号\t\t\t物理块号\t\t\t占用空间')
    for i in range(len(status_matrix.page_list)):
        if status_matrix.page_list[i] == -1:
            print(i, '\t\t\t', '空闲', '\t\t\t', '空闲')
        else:
            print(i + 1, '\t\t\t', status_matrix.page_list[i], '\t\t\t',
                  status_matrix.matrix[status_matrix.page_list[i]])
    print('----------------------------------------')


# 分配内存给作业
def allocate():
    global status_matrix, job_list
    # 判断参数是否完整
    if not (wd.entry1.get() and wd.entry2.get()):
        messagebox.showinfo('提示', '请输入作业号和作业大小！')
        return
    try:
        job_id = int(wd.entry1.get())  # 获取输入的作业号
        size = int(wd.entry2.get())  # 获取输入的作业号和大小
    # 判断参数是否合法
    except ValueError:
        messagebox.showinfo('提示', '输入的作业号或作业大小不合法！')
        # 清空输入框
        wd.entry1.delete(0, END)
        wd.entry2.delete(0, END)
        return
    if job_id < 0 or size < 0:
        messagebox.showinfo('提示', '输入的作业号或作业大小应大于0！')
        # 清空输入框
        wd.entry1.delete(0, END)
        wd.entry2.delete(0, END)
        return
    # 判断作业号是否已存在
    if job_list.get_job_by_id(job_id) is not None:
        print('作业号已存在！')
        messagebox.showinfo('提示', '作业号已存在，分配失败！')
        # 清空输入框
        wd.entry1.delete(0, END)
        wd.entry2.delete(0, END)
        return
    block_num = math.ceil(size / status_matrix.block_size)  # 假设每个物理块大小为8KB，计算需要的物理块数
    free_blocks = status_matrix.get_free_blocks()  # 获取空闲物理块
    if len(free_blocks) >= block_num:  # 如果空闲物理块数大于等于作业大小，则分配成功，否则分配失败
        job = Job(job_id, size)  # 创建作业
        blocks = random.sample(free_blocks, block_num)  # 随机分配
        job.blocks = blocks  # 记录分配的物理块号
        job.status = 1  # 记录作业状态
        job_list.add_job(job)  # 添加到作业列表
        status_matrix.allocate(blocks[0: block_num - 1], size)  # 先将满页表的物理块随机分配
        status_matrix.allocate([blocks[block_num - 1]], size, is_full=False)  # 再将余下的作业大小填入一个随机物理块
        print(str(job_id) + '号作业分配成功！')
        print('----------------------------------------')
        messagebox.showinfo('提示', '分配成功！')
        # 清空输入框
        wd.entry1.delete(0, END)
        wd.entry2.delete(0, END)
    else:
        print('空闲物理块空间不足，' + str(job_id) + '号作业分配失败！')
        print('----------------------------------------')
        messagebox.showinfo('提示', '空闲物理块空间不足，' + str(job_id) + '号作业分配失败！')
        # 清空输入框
        wd.entry1.delete(0, END)
        wd.entry2.delete(0, END)


# 回收作业，释放内存
def recycle():
    global status_matrix, job_list
    # 判断参数是否完整
    if not wd.entry3.get():
        messagebox.showinfo('提示', '请输入作业号！')
        return
    try:
        job_id = int(wd.entry3.get())  # 获取输入的作业号
    except ValueError:
        messagebox.showinfo('提示', '输入的作业号不合法！')
        # 清空输入框
        wd.entry3.delete(0, END)
        return
    if job_id < 0:
        messagebox.showinfo('提示', '输入的作业号应大于0！')
        # 清空输入框
        wd.entry3.delete(0, END)
        return
    job = job_list.delete_job(job_id)  # 从作业列表中删除作业
    if job is not None:
        status_matrix.recycle(job.blocks)  # 回收物理块
        print(str(job_id) + '号作业回收成功！')
        print('----------------------------------------')
        wd.entry3.delete(0, END)  # 清空输入框
        messagebox.showinfo('提示', '回收成功！')
    else:
        print('该作业号不存在，回收失败！')
        print('----------------------------------------')
        wd.entry3.delete(0, END)  # 清空输入框
        messagebox.showinfo('提示', '该作业号不存在，回收失败！')


# 查找作业
def search_job():
    global status_matrix, job_list
    # 判断参数是否完整
    if not wd.entry4.get():
        messagebox.showinfo('提示', '请输入作业号！')
        return
    try:
        job_id = int(wd.entry4.get())  # 获取输入的作业号
    except ValueError:
        messagebox.showinfo('提示', '输入的作业号不合法！')
        wd.entry4.delete(0, END)  # 清空输入框
        return
    if job_id < 0:
        messagebox.showinfo('提示', '输入的作业号应大于0！')
        wd.entry4.delete(0, END)  # 清空输入框
        return
    job = job_list.get_job_by_id(job_id)  # 从作业列表中查找作业
    if job is not None:
        print('查找成功！')
        # 在job_list中查找到作业，显示作业信息
        print('该作业的作业号为：', job.job_id)
        print('该作业的作业大小为：', job.size)
        print('该作业的作业状态为：', job.status)
        print('该作业的占用的物理块号为：', job.blocks)
        print('----------------------------------------')
        messagebox.showinfo('提示', '查找成功！')
        wd.entry4.delete(0, END)  # 清空输入框
    else:
        print('该作业号不存在，查找失败！')
        print('----------------------------------------')
        messagebox.showinfo('提示', '该作业号不存在，查找失败！')
        wd.entry4.delete(0, END)  # 清空输入框


# 查找逻辑地址
def search_page():
    global status_matrix, job_list
    # 判断参数是否完整
    if not wd.entry5.get() or not wd.entry6.get():
        messagebox.showinfo('提示', '请输入完整的逻辑地址！')
        return
    try:
        page_id = int(wd.entry5.get())  # 获取输入的作业号
        offset = int(wd.entry6.get())  # 获取输入的偏移量
    except ValueError:
        messagebox.showinfo('提示', '输入的逻辑地址不合法！')
        # 清空输入框
        wd.entry5.delete(0, END)
        wd.entry6.delete(0, END)
        return
    if page_id < 0:
        messagebox.showinfo('提示', '输入的页号应大于0！')
        wd.entry5.delete(0, END)  # 清空输入框
        return
    if page_id > len(status_matrix.page_list):
        messagebox.showinfo('提示', '输入的页号超出范围！')
        wd.entry5.delete(0, END)  # 清空输入框
        return
    if offset < 0:
        messagebox.showinfo('提示', '输入的偏移量应大于0！')
        wd.entry6.delete(0, END)  # 清空输入框
        return
    if offset > status_matrix.block_size:
        messagebox.showinfo('提示', '输入的偏移量超出单页面大小！')
        wd.entry6.delete(0, END)  # 清空输入框
        return
    page = status_matrix.page_list[page_id]  # 从页表中查找页
    if page != -1:
        print('查找成功！')
        # 在page_list中查找到页，显示页信息
        print('该页的页号为：', page_id)
        print('该页对应物理块号为：', status_matrix.page_list[page_id - 1])
        print('该页中实际占用空间', status_matrix.matrix[status_matrix.page_list[page_id - 1]])
        job = job_list.get_job_by_block(status_matrix.page_list[page_id - 1])  # 查找该页对应的作业
        print('该页对应的作业号为：', job.job_id)
        print('该逻辑地址对应的物理地址为：' + str(status_matrix.block_size * (status_matrix.page_list[page_id - 1] - 1) + offset))
        print('----------------------------------------')
        messagebox.showinfo('提示', '查找成功！')
        wd.entry5.delete(0, END)
    else:
        print('该页被释放，为空闲状态。')
        print('----------------------------------------')
        messagebox.showinfo('提示', '该页被释放，为空闲状态。')
        wd.entry5.delete(0, END)


# 退出
def _quit():
    print('退出成功！')
    messagebox.showinfo('提示', '退出成功！')
    wd.root.destroy()


class Window:
    """
        界面类，用于创建界面中的各个组件
    """
    root = Tk()
    # 全局设置区域
    init_button = Button(root, text='初始化', command=init)
    check_button = Button(root, text='查看当前状态', command=check)
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
    search_job_button = Button(root, text='查找', command=search_job)
    label5 = Label(root, text='页号')
    entry5 = Entry(root)
    notice_label = Label(root, text='提示：页号从0开始，页内偏移量应小于块大小')
    label6 = Label(root, text='页内偏移量')
    entry6 = Entry(root)
    search_page_button = Button(root, text='计算物理地址', command=search_page)
    # 退出区域
    exit_button = Button(root, text='退出', command=_quit)

    def __init__(self):
        self.root.title('内存分配模拟')
        self.root.geometry('600x350')
        self.root.resizable(0, 0)
        # 全局设置区域
        self.init_button.place(x=50, y=50, width=100, height=30)
        self.check_button.place(x=200, y=50, width=100, height=30)
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
        self.search_job_button.place(x=150, y=200, width=100, height=30)
        self.notice_label.place(x=280, y=150, width=250, height=30)
        self.label5.place(x=250, y=200, width=50, height=30)
        self.entry5.place(x=300, y=200, width=50, height=30)
        self.label6.place(x=350, y=200, width=80, height=30)
        self.entry6.place(x=430, y=200, width=50, height=30)
        self.search_page_button.place(x=480, y=200, width=80, height=30)
        # 退出区域
        self.exit_button.place(x=50, y=250, width=100, height=30)

    def run(self):
        self.root.mainloop()  # 启动GUI


if __name__ == '__main__':
    init()  # 初始化关键参数
    wd = Window()  # 实例化窗口对象
    wd.run()  # 启动GUI
