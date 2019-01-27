import serial
import serial.tools.list_ports
import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.pyplot import cm
import matplotlib.animation as animation
import numpy as np

ARDUINO = True
STATE = False
#PREV_STATE = False
#FILTRO_MEDIANA_X = [50,50]
#FILTRO_MEDIANA_Y = [50,50]

if ARDUINO:
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        if "Arduino" in p[1]:
            arduino = serial.Serial(p[0], 9600)
            arduino.setDTR(False)
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
PESO = [4, 4, 4, 4, 4, 4, 4, 4, 4 ,4, 4, 4, 4, 4, 4 ,4, 4]
X_MED = [50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50]
Y_MED = [50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50]

def animate(i):
    global STATE
    global PESO
    global X_MED
    global Y_MED

    if ARDUINO:
        rawString = arduino.readline().strip()
        x_raw = float(rawString.decode('utf-8').split(';')[0])
        y_raw = float(rawString.decode('utf-8').split(';')[1])
        peso = float(rawString.decode('utf-8').split(';')[2])
        #if (peso > 5000) & (PREV_STATE == False):
        #    PREV_STATE = True
        #if (peso < 5000) & (PREV_STATE == True):
        #    plt.close()
    else:
        x_raw = 50 + np.random.normal(scale=10)
        y_raw = 50 + np.random.normal(scale=10)
        peso = 16000

    PESO.pop(0)
    PESO.append(peso)
    X_MED.pop(0)
    X_MED.append(x_raw)
    Y_MED.pop(0)
    Y_MED.append(y_raw)

    PESO_SORTED = PESO.copy()
    X_SORTED = X_MED.copy()
    Y_SORTED = Y_MED.copy()

    PESO_SORTED.sort()
    X_SORTED.sort()
    Y_SORTED.sort()

    x = X_SORTED[9]
    y = Y_SORTED[9]

    if (STATE == False) & (PESO_SORTED[9] > 15000):
        STATE = True

    if STATE:
        #print('Saving')
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
    if STATE:
        ax1.scatter(x,y, color = color,s = 1000)
    else:
        ax1.scatter(50,50, color = color,s = 1000)


ani = animation.FuncAnimation(fig, animate, interval=15)
plt.show()

#heatmap
fig = plt.figure(figsize=(5,5))
resolution = np.linspace(0,100,20)
H, xedges, yedges  = np.histogram2d(X_HIST, Y_HIST, bins=(resolution,resolution), normed =True)
H = H.T
ax = fig.add_subplot(111, title='Mapa de Calor', aspect='equal', xlim=xedges[[0, -1]], ylim=yedges[[0, -1]])
im = mpl.image.NonUniformImage(ax, interpolation='bilinear',cmap=cm.Purples)
xcenters = (xedges[:-1] + xedges[1:]) / 2
ycenters = (yedges[:-1] + yedges[1:]) / 2
circle1 = ax.add_artist(plt.Circle((50, 50), radio, color='r', fill=False,alpha=0.5))
ax.scatter(points, cincuentas, s = 0.5, c='k', alpha=0.5)
ax.scatter(cincuentas, points, s = 0.5, c='k' , alpha=0.5)
im.set_data(xcenters, ycenters, H)
ax.images.append(im)
plt.show(im)