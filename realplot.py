import serial
import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.pyplot import cm
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
X_HIST = []
Y_HIST = []

def animate(i):

    try:
        if ARDUINO:
            rawString = arduino.readline().strip()
            x = float(rawString.decode('utf-8').split(';')[0])
            y = float(rawString.decode('utf-8').split(';')[1])
        else:
            x = 50 + np.random.normal(scale=25)
            y = 50 + np.random.normal(scale=25)
        X_HIST.append(x)
        Y_HIST.append(y)
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

#heatmap
fig = plt.figure(figsize=(5,5))
resolution = np.linspace(0,100,20)
H, xedges, yedges  = np.histogram2d(X_HIST, Y_HIST, bins=(resolution,resolution), normed =True)
H = H.T
ax = fig.add_subplot(111, title='NonUniformImage: interpolated', aspect='equal', xlim=xedges[[0, -1]], ylim=yedges[[0, -1]])
im = mpl.image.NonUniformImage(ax, interpolation='bilinear',cmap=cm.Purples)
xcenters = (xedges[:-1] + xedges[1:]) / 2
ycenters = (yedges[:-1] + yedges[1:]) / 2
circle1 = ax.add_artist(plt.Circle((50, 50), radio, color='r', fill=False,alpha=0.5))
ax.scatter(points, cincuentas, s = 0.5, c='k', alpha=0.5)
ax.scatter(cincuentas, points, s = 0.5, c='k' , alpha=0.5)
im.set_data(xcenters, ycenters, H)
ax.images.append(im)
plt.show(im)