import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time
import numpy as np

ARDUINO = False

if ARDUINO:
    arduino = serial.Serial('COM8', 9600)
    arduino.setDTR(False)
    time.sleep(1)
    arduino.flushInput()
    arduino.setDTR(True)


fig = plt.figure(figsize=(5,5))
ax1 = fig.add_subplot(1,1,1)
ax1.set_xlim([0, 100])
ax1.set_ylim([0, 100])
points = np.linspace(0,100,200)
cincuentas = np.ones(200)*50
radio = 20

def animate(i):

    try:
        if ARDUINO:
            rawString = arduino.readline().strip()
            x = float(rawString.decode('utf-8').split(';')[0])
            y = float(rawString.decode('utf-8').split(';')[1])
        else:
            x = 50 + np.random.normal(scale=3)
            y = 50 + np.random.normal(scale=10)
        if (x-50)**2 + (y-50)**2<= radio**2:
            color = 'b'
        else:
            color = 'r'

        #print(x,y)
        ax1.clear()
        ax1.set_xlim([0, 100])
        ax1.set_ylim([0, 100])
        circle1 = ax1.add_artist(plt.Circle((50, 50), radio, color='r', fill=False))
        ax1.scatter(points, cincuentas, s = 0.5, c='k', alpha=0.8)
        ax1.scatter(cincuentas, points, s = 0.5, c='k' , alpha=0.8)
        ax1.scatter(x,y, color = color,s = 1000)



    except KeyboardInterrupt:
        print("Exiting")
        arduino.close()


ani = animation.FuncAnimation(fig, animate, interval=10)
plt.show()