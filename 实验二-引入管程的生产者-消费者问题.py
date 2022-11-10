"""
    status: Accepted
"""

import threading
from tkinter import Tk, Label, Canvas, Button
from queue import Queue
from random import random
from time import sleep


# 实现synchronized装饰器，保证互斥访问
def synchronized(func):
    func.__lock__ = threading.Lock()  # 执行前加锁

    def synced_func(*args, **kws):
        with func.__lock__:  # with上下文管理，执行结束自动解锁
            return func(*args, **kws)

    return synced_func


class Widget:
    """ 窗口 """
    # 初始化生产者的颜色
    color_list = ['grey', 'pink', 'yellow', 'green', 'purple', 'orange']

    # 实例化TK对象，用于创建窗口
    root = Tk()
    # 设置窗口大小
    root.geometry('450x165')
    # 禁止窗口缩放
    root.resizable(0, 0)
    # 设置窗口标题
    root.title('生产者消费者模型')

    # 实例化5个生产者矩形
    p1 = Label(root, height=1, width=10, text='生产者1', bg=color_list[0])
    p2 = Label(root, height=1, width=10, text='生产者2', bg=color_list[0])
    p3 = Label(root, height=1, width=10, text='生产者3', bg=color_list[0])
    p4 = Label(root, height=1, width=10, text='生产者4', bg=color_list[0])
    p5 = Label(root, height=1, width=10, text='生产者5', bg=color_list[0])

    # 实例化3个消费者矩形
    c1 = Label(root, height=2, width=10, text='消费者1', bg=color_list[0])
    c2 = Label(root, height=2, width=10, text='消费者2', bg=color_list[0])
    c3 = Label(root, height=2, width=10, text='消费者3', bg=color_list[0])

    # 实例化缓冲区矩形
    buffer = Canvas(root, relief='ridge', width=240, height=80, bg='white')

    button_list = []  # 缓冲区中显示的按钮列表
    num = 0  # 此时缓冲区中的按钮数量

    def __init__(self):
        # 排列生产者矩形
        for index in range(1, 6):
            getattr(self, 'p' + str(index)).place(x=10, y=30 * index - 20)

        # 排列消费者矩形
        for index in range(1, 4):
            getattr(self, 'c' + str(index)).place(x=356, y=50 * index - 40)

        self.buffer.create_text(120, 40, text='缓冲区')  # 创建缓冲区的文字
        self.buffer.place(x=100, y=40)  # 将缓冲区放入窗口中

    def add_button_in_buffer(self, pd_index, obj_index):
        """ 向缓冲区矩形中添加按钮 """
        # 实例化按钮
        btn = Button(self.root, text='产品' + str(obj_index), bg=self.color_list[pd_index])
        # 将按钮放入缓冲区的矩形中，按行列从左至右，从上至下排列
        x = (40 * self.num) % 200 + 40
        y = 40 * (self.num // 5 + 1)
        self.buffer.create_window(x, y - 20, window=btn)  # 将按钮放入缓冲区
        self.num += 1  # 缓冲区中按钮数量加一
        self.button_list.append({
            'product': btn,
            'pd_index': pd_index
        })  # 将按钮放入缓冲区中

    def reduce_button_in_buffer(self):
        """ 删除缓冲区矩形中的按钮，并置换颜色以模拟队头被消耗 """
        if self.button_list:  # 如果缓冲区中有按钮
            # 遍历队列
            for index in range(len(self.button_list)):
                if index == len(self.button_list) - 1:
                    # 如果是最后一个按钮，删除
                    b = self.button_list.pop()  # 弹出最后一个按钮
                    self.num -= 1
                    b['product'].destroy()  # 销毁按钮
                    break
                # 置换颜色
                self.button_list[index]['product'].config(bg=self.color_list[self.button_list[index + 1]['pd_index']])
                self.button_list[index]['pd_index'] = self.button_list[index + 1]['pd_index']
        return self.num  # 返回缓冲区中按钮数量

    def run(self):
        self.root.mainloop()  # 进入消息循环


class Monitor:
    """ 管程类，负责对缓冲区的访问控制 """
    empty = threading.Semaphore(10)  # 同步信号量，表示空闲缓冲区的数量
    full = threading.Semaphore(0)  # 同步信号量，表示产品的数量，即非空缓冲区的数量

    q = Queue(10)  # 创建上限为10的缓冲区

    @synchronized
    def insert_item(self, pd_index):
        """ 生产者对缓冲区的访问控制，添加synchronized，最多只能有一个线程进入管程 """
        self.empty.acquire()  # 实现同步，消耗一个空闲缓冲区
        # 将产品放入缓冲区
        self.__add_item(pd_index, self.q)
        self.full.release()  # 实现同步，增加一个产品

    @synchronized
    def remove_item(self, c_index):
        """ 消费者对缓冲区的访问控制，添加synchronized，最多只能有一个线程进入管程 """
        self.full.acquire()  # 实现同步，消耗一个产品
        # 从缓冲区取出产品
        self.__get_item(c_index, self.q)
        self.empty.release()  # 实现同步，增加一个空闲缓冲区

    @staticmethod
    def __add_item(pd_index, b_q):
        """ 将产品放入缓冲区，设为私有方法 """
        sleep(random())  # 模拟将产品放入缓冲区的时间
        wg.add_button_in_buffer(pd_index, wg.num + 1)  # 模拟将产品放入缓冲区
        b_q.put(
            {'pd_index': pd_index, 'obj_index': wg.num}  # 将生产的产品放入缓冲区
        )
        getattr(wg, 'p' + str(pd_index)).config(bg=wg.color_list[0])  # 将产品放入缓冲区后，生产者变为灰色

    @staticmethod
    def __get_item(c_index, b_q):
        """ 从缓冲区中取出产品，设为私有方法 """
        sleep(random())  # 模拟从缓冲区取出产品的时间
        obj_index = b_q.get()  # 从缓冲区取出产品
        getattr(wg, 'c' + str(c_index)).config(bg='blue')  # 持有产品后，消费者变为蓝色
        wg.reduce_button_in_buffer()  # 模拟从缓冲区中删除产品


class Producer:
    """ 生产者类 """
    def __init__(self, monitor):    # 初始化，传入管程对象
        self.monitor = monitor

    def run(self, pd_index):
        """ 生产者主线程 """
        while True:
            # 生产产品
            self.__produce(pd_index)
            # 将产品放入缓冲区
            self.monitor.insert_item(pd_index)

    @staticmethod
    def __produce(pd_index):
        """ 生产产品，生产者类的私有方法 """
        sleep(random())  # 模拟生产产品的时间
        getattr(wg, 'p' + str(pd_index)).config(bg=wg.color_list[pd_index])  # 持有产品后，生产者变为彩色


class Consumer:
    """ 消费者类 """
    def __init__(self, monitor):    # 初始化，传入管程对象
        self.monitor = monitor

    def run(self, c_index):
        """ 消费者主线程 """
        while True:
            # 从缓冲区取出产品
            self.monitor.remove_item(c_index)
            # 消费产品
            self.__consume(c_index)

    @staticmethod
    def __consume(c_index):
        """ 消费产品，，消费者类的私有方法 """
        sleep(random() / 1.25)  # 模拟消费产品的时间
        getattr(wg, 'c' + str(c_index)).config(bg=wg.color_list[0])  # 消费产品后，消费者变为灰色


if __name__ == '__main__':
    wg = Widget()  # 创建窗口对象
    m = Monitor()  # 实例化管程
    pd = Producer(m)  # 实例化生产者
    cs = Consumer(m)  # 实例化消费者

    # 创建5个生产者线程
    for i in range(5):
        threading.Thread(target=pd.run, args=(i + 1,)).start()

    # 创建3个消费者线程
    for i in range(3):
        threading.Thread(target=cs.run, args=(i + 1,)).start()

    # 启动GUI
    wg.run()
