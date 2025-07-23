from maix import image, display, camera
import serial
from time import time
from maix import gpio

PH6 = gpio.gpio(6,'H',1,2)         #绿
PH7 = gpio.gpio(7,'H',1,2)         #黑
ser = serial.Serial("/dev/ttyS1", 115200)  # 连接串口
red_thresholds = [(95, 100, -20, 53, -17, 53)]
block_thresholds = [(0, 10, -8, 10, -7, 12)]
Usart_Data_block="BX100Y100DK"
Usart_Data_red="RX100Y100S10000DK"
t =  time()
while True:
    if time()-t>0.05:
        t = time()
        img = camera.capture()
        img.draw_rectangle(115, 115, 125, 125, color=(0, 0, 255), thickness=1)
        P6 = PH6.get_value()
        P7 = PH7.get_value()
        if P6!=0:
            blobs_red = img.find_blobs(red_thresholds, merge=True, pixels_threshold=20)
            MAX_S = 0
            MAX_I = 0
            NOW_I = 0
            if blobs_red:
                for i in blobs_red:
                    img.draw_rectangle(i["x"], i["y"], i["x"] + i["w"], i["y"] + i["h"], color=(255, 0, 0), thickness=1)
                    if i["pixels"] > MAX_S:
                        MAX_S = i["pixels"]
                        MAX_I = NOW_I
                    NOW_I = NOW_I + 1
                if NOW_I > 1:
                    data_x = '%03d' % blobs_red[MAX_I]["centroid_x"]
                    data_y = '%03d' % blobs_red[MAX_I]["centroid_y"]
                else:
                    data_x = '%03d' % i["centroid_x"]
                    data_y = '%03d' % i["centroid_y"]
                Usart_Data_red = 'RX'+data_x+'Y'+data_y+'DK'
                ser.write(Usart_Data_red.encode("UTF-8"))
        if P7!=0:
            blobs_block = img.find_blobs(block_thresholds, merge=True, pixels_threshold=60)
            MAX_S = 0
            MAX_I = 0
            NOW_I = 0
            if blobs_block:
                for i in blobs_block:
                    img.draw_rectangle(i["x"], i["y"], i["x"] + i["w"], i["y"] + i["h"], color=(255, 0, 0), thickness=1)
                    if i["pixels"] > MAX_S:
                        MAX_S = i["pixels"]
                        MAX_I = NOW_I
                    NOW_I = NOW_I + 1
                if NOW_I > 1:
                    data_x = '%03d' % blobs_block[MAX_I]["centroid_x"]
                    data_y = '%03d' % blobs_block[MAX_I]["centroid_y"]
                    data_s = '%05d' % blobs_block[MAX_I]["pixels"]
                else:
                    data_x = '%03d' % i["centroid_x"]
                    data_y = '%03d' % i["centroid_y"]
                    data_s = '%05d' % i["pixels"]
                Usart_Data_block = 'BX' + data_x + 'Y' + data_y + 'S' + data_s+ 'DK'
                ser.write(Usart_Data_block.encode("UTF-8"))
        img.draw_string(10, 10, 'R='+str(P6), color = (255, 0, 0))
        img.draw_string(10, 30, 'B='+str(P7), color = (255, 0, 0))
        display.show(img)

