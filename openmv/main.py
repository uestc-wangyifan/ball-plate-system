import sensor, image, time,math,pyb
from pyb import UART,LED
import json
import ustruct
import display

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 2000)
#sensor.set_windowing((240, 240))
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
lcd = display.SPIDisplay()
#sensor.set_auto_exposure(False,exposure_us=2000)
red_threshold_01=(96, 100, -8, 7, -4, 11)
clock = time.clock()
sensor.set_auto_exposure(True, exposure_us=1000)# 设置1000，运行得快

uart3 = UART(3,115200)   #定义串口3变量
uart3.init(115200, bits=8, parity=None, stop=1) # 初始化串口3

def find_max(blobs):    #定义寻找色块面积最大的函数
    max_size=0
    for blob in blobs:
        if blob.pixels() > max_size:
            max_blob=blob
            max_size = blob.pixels()
            return max_blob

while(True):
    clock.tick()
    img = sensor.snapshot().lens_corr(1.8)#鱼眼镜头畸变校正 # 拍照
    blobs = img.find_blobs([red_threshold_01])
    max_b = find_max(blobs)
    #cx=0;cy=0;
    img.draw_cross(81, 59, size = 1, color = (0, 255, 0)) # 板子中心画十字


    if blobs:
            # 如果找到了目标颜色
            cx=max_b[5] # 返回色块(int)的中心x位置
            cy=max_b[6] # 返回色块(int)的中心y位置。
            #img.draw_rectangle(max_b[0:4]) # 绘制矩形
            img.draw_cross(max_b[5], max_b[6],color=(255, 0, 0),size=2) # 画十字

            data = bytearray([0xa3, 0xb3, cx, cy, 0xc3])  # 打包：帧头1 + 帧头2 + cx + cy + 帧尾
            uart3.write(data)       # 通过串口3发送数据包给单片机

            print(cx,cy) # 打印坐标
            LED(1).on()
    else:
        LED(1).off()

    #lcd.write(img)
