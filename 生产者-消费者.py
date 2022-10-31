import threading
from tkinter import Tk, Label, Canvas, Button
from queue import Queue
from random import random
from time import sleep


class Widget:
    # 初始化生产者的颜色
    color_list = ['grey', 'pink', 'yellow', 'green', 'purple', 'red']

    # 实例化TK对象，用于创建窗口
    root = Tk()
    # 设置窗口大小
    root.geometry('450x180')
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

    product_list = []  # 缓冲区中的产品列表
    num = 0  # 此时缓冲区中的产品数量

    def __init__(self):
        # 排列生产者矩形
        for index in range(1, 6):
            getattr(self, 'p' + str(index)).place(x=10, y=30 * index - 20)

        # 排列消费者矩形
        for index in range(1, 4):
            getattr(self, 'c' + str(index)).place(x=356, y=50 * index - 40)

        self.buffer.create_text(120, 40, text='缓冲区')  # 创建缓冲区的文字
        self.buffer.place(x=100, y=40)  # 将缓冲区放入窗口中

    def add_product(self, pd_index, obj_index):
        # 实例化按钮
        product = Button(self.root, text='产品' + str(obj_index), bg=self.color_list[pd_index])
        # 将按钮放入缓冲区的矩形中，按行列从左至右，从上至下排列
        x = (40 * self.num) % 200 + 40
        y = 40 * (self.num // 5 + 1)
        self.buffer.create_window(x, y - 20, window=product)  # 将按钮放入缓冲区
        self.num += 1  # 缓冲区中产品数量加一
        self.product_list.append(product)  # 将按钮放入缓冲区中

    def reduce_product(self):
        if self.product_list:  # 如果缓冲区中有产品
            b = self.product_list.pop()  # 弹出最后一个产品
            self.num -= 1  # 缓冲区中按钮数量减一
            b.destroy()  # 销毁按钮
        return self.num  # 返回缓冲区中按钮数量

    def run(self):
        self.root.mainloop()  # 进入消息循环


class Manager:
    mutex = threading.Semaphore(1)  # 互斥信号量，实现对缓冲区的互斥访问
    empty = threading.Semaphore(10)  # 同步信号量，表示空闲缓冲区的数量
    full = threading.Semaphore(0)  # 同步信号量，表示产品的数量，即非空缓冲区的数量

    q = Queue(10)  # 创建上限为10的缓冲区

    # 生产者进程
    def producer(self, pd_index):
        while True:
            # 生产产品
            self.__produce(pd_index)
            self.empty.acquire()  # 实现同步，消耗一个空闲缓冲区
            self.mutex.acquire()  # 实现互斥
            # 将产品放入缓冲区
            self.__add_item(pd_index, self.q)
            self.mutex.release()  # 实现互斥
            self.full.release()  # 实现同步，增加一个产品

    # 消费者进程
    def consumer(self, c_index):
        while True:
            self.full.acquire()  # 实现同步，消耗一个产品
            self.mutex.acquire()  # 实现互斥
            # 从缓冲区取出产品
            self.__get_item(c_index, self.q)
            self.mutex.release()  # 实现互斥
            self.empty.release()  # 实现同步，增加一个空闲缓冲区
            # 消费产品
            self.__consume(c_index)

    @staticmethod
    def __produce(pd_index):
        sleep(random())  # 模拟生产产品的时间
        getattr(wg, 'p' + str(pd_index)).config(bg=wg.color_list[pd_index])  # 持有产品后，生产者变为彩色

    @staticmethod
    def __add_item(pd_index, b_q):
        sleep(random())  # 模拟将产品放入缓冲区的时间
        wg.add_product(pd_index, wg.num + 1)  # 将产品放入缓冲区
        b_q.put(
            {'pd_index': pd_index, 'obj_index': wg.num}  # 将生产的产品放入缓冲区
        )
        getattr(wg, 'p' + str(pd_index)).config(bg=wg.color_list[0])  # 将产品放入缓冲区后，生产者变为灰色

    @staticmethod
    def __get_item(c_index, b_q):
        sleep(random())  # 模拟从缓冲区取出产品的时间
        obj_index = b_q.get()  # 从缓冲区取出产品
        getattr(wg, 'c' + str(c_index)).config(bg='blue')  # 持有产品后，消费者变为蓝色
        wg.reduce_product()  # 从缓冲区中删除产品

    @staticmethod
    def __consume(c_index):
        sleep(random() / 1.5)  # 模拟消费产品的时间
        getattr(wg, 'c' + str(c_index)).config(bg=wg.color_list[0])  # 消费产品后，消费者变为灰色


if __name__ == '__main__':
    wg = Widget()  # 创建窗口对象
    m = Manager()  # 实例化管程

    # 创建5个生产者线程
    for i in range(5):
        threading.Thread(target=m.producer, args=(i + 1, )).start()

    # 创建3个消费者线程
    for i in range(3):
        threading.Thread(target=m.consumer, args=(i + 1, )).start()

    # 启动GUI
    wg.run()
