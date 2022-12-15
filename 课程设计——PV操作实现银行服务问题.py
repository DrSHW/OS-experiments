"""
    status: Pending
"""

import threading
import time
from tkinter import Tk, Label, Canvas, Button
from queue import Queue
from random import random
from time import sleep
import logging

# 日志设置，显示时间、消息
logging.basicConfig(level=logging.DEBUG,
                    format="%(asctime)s %(message)s",
                    datefmt='%Y-%m-%d  %H:%M:%S %a'
                    )


class Window:
    """ 封装了窗口布局，窗口组件操作，窗口事件处理等行为 """
    # 颜色列表，用于为各组件着色
    colors = ['Lavender', 'Gold', 'Yellow', 'PaleGreen', 'PaleTurquoise', 'LightCoral', 'Violet', 'Tan']
    root = Tk()  # 实例化窗口对象
    root.geometry('300x250')  # 设置窗口大小
    root.resizable(0, 0)  # 禁止窗口缩放
    root.title('银行服务模型')  # 设置窗口标题
    counter_area = Canvas(root, height=60, width=100, bg='MistyRose')  # 创建柜台
    seats_area = Canvas(root, relief='ridge', width=280, height=77, bg='LightCyan')  # 创建座位区，可容纳十个按钮
    # 创建统计区，显示当前等候人数与已服务完毕的人数
    statistics_area = Label(root, text='当前等待人数：0 人；当前已服务完毕人数：0 人。', height=3, width=40, bg='White')
    instance_id = 1  # 按钮编号
    waiting_instance_num = 0  # 等待区按钮数
    waiting_instance_list = Queue()  # 等待队列
    sitting_instance_num = 0  # 座位区按钮数
    sitting_instance_list = []  # 座位区按钮列表
    finished_instance_num = 0  # 服务完毕的次数

    # 重新封装按钮类，包含按钮编号和按钮颜色两个属性，便于之后的置换
    class Instance:
        instance = None  # 按钮实例，Button对象

        def __init__(self, color, index):
            self.color = color  # 按钮颜色
            self.index = index  # 按钮编号

    def __init__(self):
        self.counter_area.place(x=100, y=10)  # 设置柜台位置
        self.seats_area.place(x=10, y=80)  # 设置座位区位置
        self.statistics_area.place(x=10, y=170)  # 设置统计区位置
        self.counter_area.create_text(50, 12, text='柜台')  # 设置柜台文字
        # 设置柜台服务区位置，初始无顾客，为空闲状态
        self.on_service = Label(self.root, text='空闲', relief="ridge", height=1, width=6, bg='white')
        self.counter_area.create_window(50, 40, window=self.on_service)  # 设置柜台服务区位置
        self.seats_area.create_text(140, 40, text='座位区')  # 设置座位区文字

        for i in range(10):  # 创建十个空白标签，代表空闲的座位
            x = (32 + i * 55) % 275  # 计算座位区中每个座位的x坐标
            y = 40 * (i // 5 + 1) - 20  # 计算座位区中每个座位的y坐标
            # 实例化空白标签
            empty_seats_area = Label(self.root, text='空位', height=1, width=6, relief="ridge", bg='white')
            self.seats_area.create_window(x, y, window=empty_seats_area)  # 设置座位区位置

    def add_instance_to_seats_area(self, cus_id):
        """ 向座位区添加一个按钮 """
        new_instance = self.waiting_instance_list.get()  # 从等待队列中取出一个按钮
        x = (32 + wd.sitting_instance_num * 55) % 275  # 计算座位区中每个座位的x坐标
        y = 40 * (wd.sitting_instance_num // 5 + 1) - 20  # 计算座位区中每个座位的y坐标
        # 将按钮添加到座位区
        self.seats_area.create_window(x, y, window=new_instance.instance)
        # 将按钮添加到座位区客户列表
        self.sitting_instance_list.append(new_instance)  # 将按钮添加到座位区客户列表
        self.sitting_instance_num += 1  # 座位区按钮数加一
        self.waiting_instance_num -= 1  # 等待区按钮数减一

    def set_on_service_available(self):
        """ 设置柜台为 等待 被叫号的客户 到来的状态 """
        self.on_service = Label(self.root, text='已叫号', relief="ridge", height=1, width=6, bg='white')
        self.counter_area.create_window(50, 40, window=self.on_service)
        sleep(random())

    def remove_instance_from_seats_area(self, cus_id):
        """ 从座位区删除一个按钮 """
        instance = self.sitting_instance_list[0]  # 取得座位区第一个按钮
        self.on_service.destroy()  # 销毁柜台服务区的标签
        # 将新按钮添加到柜台服务区，按钮信息为需要进入柜台的按钮编号（即第一个按钮编号）
        self.on_service = Button(self.root, text='客户' + str(instance.index), height=1, width=6, bg=instance.color)
        self.counter_area.create_window(50, 40, window=self.on_service)  # 将按钮设置在柜台服务区上
        for index in range(len(self.sitting_instance_list)):  # 遍历座位区按钮列表，通过置换颜色、文字信息的方式实现按钮的移动
            if index == len(self.sitting_instance_list) - 1:  # 如果是最后一个按钮，将其删除
                last_guest = self.sitting_instance_list.pop()  # 取得最后一个按钮
                last_guest.instance.destroy()  # 销毁之
                break
            # 置换颜色、文字、实体信息
            self.sitting_instance_list[index].instance.config(bg=self.sitting_instance_list[index + 1].color,
                                                              text='客户' + str(
                                                                  self.sitting_instance_list[index + 1].index))
            self.sitting_instance_list[index].color = self.sitting_instance_list[index + 1].color
            self.sitting_instance_list[index].index = self.sitting_instance_list[index + 1].index
        self.sitting_instance_num -= 1  # 座位区按钮数减一

    def run(self):
        self.root.mainloop()  # 启动GUI


class Customer:
    """ 封装所有客户的行为 """
    @staticmethod
    def arrive(cus_id):
        """ 客户到达 """
        logging.info('客户' + str(cus_id) + '等待取号')  # 输出日志
        wd.instance_id += 1  # 客户编号加一
        # 实例化一个客户对象
        color = wd.colors[int(random() * 8)]  # 随机生成一个颜色
        guest = wd.Instance(color, cus_id)
        guest.instance = Button(wd.root, text='客户' + str(guest.index), height=1, width=6, bg=guest.color)

        wd.waiting_instance_list.put(guest)  # 将客户添加到等待区客户列表
        wd.waiting_instance_num += 1  # 等待区客户数加一
        wd.statistics_area.config(text='当前等待人数：' + str(wd.waiting_instance_num) + ' 人；当前已服务完毕人数：' \
                                       + str(wd.finished_instance_num) + ' 人。')  # 更新统计区信息

    @staticmethod
    def take_number(cus_id):
        """ 客户取号 """
        sleep(random())  # 模拟客户取号时间
        wd.add_instance_to_seats_area(cus_id)  # 将客户添加到座位区，即座位区添加一个按钮
        wd.statistics_area.config(text='当前等待人数：' + str(wd.waiting_instance_num) + ' 人；当前已服务完毕人数：' \
                                       + str(wd.finished_instance_num) + ' 人。')  # 更新统计区信息
        logging.info('客户' + str(cus_id) + '已取号并入座')  # 输出日志


class Receptionist:
    """ 封装所有营业员的行为 """
    productivity = random() + 0.8  # 处理一个客户的时间

    @staticmethod
    def service(cus_id):
        """ 营业员服务 """
        if wd.waiting_instance_num > 5:  # 如果等待区客户数大于5，则速度提升
            productivity = random() / 2
        else:
            productivity = random() + 0.8
        sleep(productivity)  # 模拟办理业务的时间
        logging.info('客户' + str(cus_id) + '办理业务完毕')  # 输出日志
        wd.on_service.destroy()  # 服务完毕，从座位区删除客户
        wd.finished_instance_num += 1  # 已服务完毕客户数加1
        wd.statistics_area.config(text='当前等待人数：' + str(wd.waiting_instance_num) + ' 人；当前已服务完毕人数：' \
                                       + str(wd.finished_instance_num) + ' 人。')  # 更新统计区信息

    @staticmethod
    def call_number(cus_id):
        """ 营业员叫号 """
        wd.set_on_service_available()  # 设置柜台为可服务状态，即设置柜台为 等待 被叫号的客户 到来的状态
        logging.info('柜员叫' + str(cus_id) + '号客户办理业务')  # 输出日志
        sleep(random())  # 模拟客户前往柜台的时间
        wd.remove_instance_from_seats_area(cus_id)  # 从座位区删除客户，即座位区删除一个按钮
        logging.info('客户' + str(cus_id) + '正在办理业务...')  # 输出日志


class Monitor:
    """ 监视器类，用于定义信号量、进程控制 """
    # 定义信号量
    mutex = threading.Semaphore(1)  # 互斥使用取号机
    empty = threading.Semaphore(10)  # 空座位的数量，设置为10
    full = threading.Semaphore(0)  # 已占座位的数量，初值为0
    service = threading.Semaphore(1)  # 等待叫号，初始状态下银行一定是做好准备的，故设为1
    c = Customer()  # 实例化一个客户对象
    r = Receptionist()  # 实例化一个营业员对象

    def run_customer(self):
        """ 一个客户任务，该任务会执行一次到达、取号、办理业务的过程 """
        cus_id = wd.instance_id
        self.c.arrive(cus_id)  # 客户到达
        self.empty.acquire()  # 等待空位
        self.mutex.acquire()  # 申请使用取号机
        self.c.take_number(cus_id)  # 客户取号
        self.mutex.release()  # 取号完毕
        self.service.acquire()  # 等待营业员叫号
        self.full.release()  # 通知营业员有客户到来

    def run_receptionist(self):
        """ 实例化一个营业员任务，该任务会不断地重复等待、叫号、服务的过程 """
        while True:
            cus_id = wd.instance_id - wd.waiting_instance_num - wd.sitting_instance_num
            self.full.acquire()  # 等待客户到来，没有则休息（阻塞）
            self.r.call_number(cus_id)  # 营业员叫号
            self.empty.release()  # 客户离开座位，释放一个空位
            self.r.service(cus_id)  # 营业员服务
            self.service.release()  # 服务完毕，释放服务信号量

    def generate_customer(self):
        """ 源源不断地生成客户 """
        while True:
            time.sleep(random() + 0.8)  # 模拟客户到达的时间间隔
            threading.Thread(target=self.run_customer).start()  # 启动一个客户任务线程


if __name__ == '__main__':
    wd = Window()  # 实例化一个窗口对象
    m = Monitor()  # 实例化一个监视器对象
    threading.Thread(target=m.generate_customer).start()  # 启动一个源源不断地生成客户的线程
    threading.Thread(target=m.run_receptionist).start()  # 启动一个营业员任务线程
    wd.run()  # 启动GUI
