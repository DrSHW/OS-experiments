from tkinter import *
import queue
import threading
import random
from time import sleep


class Widget:
    color_list = ['grey', 'pink', 'yellow', 'green', 'purple', 'red']
    root = Tk()
    # 设置窗口大小
    root.geometry('450x180')
    # 设置窗口标题
    root.title('生产者消费者模型')

    p1 = Label(root, height=1, width=10, text='生产者1', bg='grey')
    p2 = Label(root, height=1, width=10, text='生产者2', bg='grey')
    p3 = Label(root, height=1, width=10, text='生产者3', bg='grey')
    p4 = Label(root, height=1, width=10, text='生产者4', bg='grey')
    p5 = Label(root, height=1, width=10, text='生产者5', bg='grey')

    c1 = Label(root, height=2, width=10, text='消费者1', bg='grey')
    c2 = Label(root, height=2, width=10, text='消费者2', bg='grey')
    c3 = Label(root, height=2, width=10, text='消费者3', bg='grey')

    buffer = Canvas(root, relief='ridge', width=240, height=80, bg='white')

    product_list = []
    num = 0  # 此时缓冲区中的产品数量

    def __init__(self):
        for index in range(1, 6):
            getattr(self, 'p' + str(index)).place(x=10, y=30 * index - 20)

        for index in range(1, 4):
            getattr(self, 'c' + str(index)).place(x=356, y=50 * index - 40)

        self.buffer.create_text(120, 40, text='缓冲区')
        self.buffer.place(x=100, y=40)

    def add_product(self, pd_index, obj_index):
        product = Button(self.root, text='产品' + str(obj_index), bg=self.color_list[pd_index])
        # 将按钮放入缓冲区的矩形中
        x = (40 * self.num) % 200 + 40
        y = 40 * (self.num // 5 + 1)
        self.buffer.create_window(x, y - 20, window=product)
        self.num += 1
        self.product_list.append(product)

    def reduce_product(self):
        if self.product_list:
            b = self.product_list.pop()
            self.num -= 1
            b.destroy()
        return self.num

    def run(self):
        self.root.mainloop()


class Manager:
    mutex = threading.Semaphore(1)  # 互斥信号量，实现对缓冲区的互斥访问
    empty = threading.Semaphore(10)  # 同步信号量，表示空闲缓冲区的数量
    full = threading.Semaphore(0)  # 同步信号量，表示产品的数量，即非空缓冲区的数量
    wg = Widget()

    # 生产者进程
    def producer(self, pd_index, b_q):
        while True:
            sleep(random.random())
            getattr(self.wg, 'p' + str(pd_index)).config(bg=self.wg.color_list[pd_index])
            self.empty.acquire()  # 实现同步，消耗一个空闲缓冲区
            self.mutex.acquire()  # 实现互斥
            sleep(random.random())
            self.wg.add_product(pd_index, self.wg.num + 1)
            b_q.put(
                {'pd_index': pd_index, 'obj_index': self.wg.num}  # 将生产的产品放入缓冲区
            )
            getattr(self.wg, 'p' + str(pd_index)).config(bg='grey')
            self.mutex.release()  # 实现互斥
            self.full.release()  # 实现同步，增加一个产品

    # 消费者进程
    def consumer(self, c_index, b_q):
        while True:
            self.full.acquire()  # 实现同步，消耗一个产品
            self.mutex.acquire()  # 实现互斥
            sleep(random.random())
            obj_index = b_q.get()
            getattr(self.wg, 'c' + str(c_index)).config(bg='blue')
            self.wg.reduce_product()
            self.mutex.release()  # 实现互斥
            self.empty.release()  # 实现同步，增加一个空闲缓冲区
            sleep(random.random() / 1.5)
            getattr(self.wg, 'c' + str(c_index)).config(bg='grey')


if __name__ == '__main__':
    m = Manager()
    q = queue.Queue(10)  # 创建上限为10的缓冲区

    # 创建5个生产者线程
    for i in range(5):
        threading.Thread(target=m.producer, args=(i + 1, q)).start()

    # 创建3个消费者线程
    for i in range(3):
        threading.Thread(target=m.consumer, args=(i + 1, q)).start()

    m.wg.run()
