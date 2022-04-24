# sudo apt-get install python3.5-tk
# pip install matplotlib
# pip install pandas
# pip install python3-tk
 
import matplotlib.pyplot as plt
import csv

x = []

ax = []
ay = []
az = []
with open('./IMU_Arduino_ESP32/src/log326-CommaTrim.txt','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        x.append(int(row[0]))
        ax.append( float(row[1]) )
        ay.append( float(row[2]) )
        az.append( float(row[3]) )
# plt.plot(x, ax, label='raw:ax', color='r')
# plt.plot(x, ay, label='raw:ay', color='g')
# plt.plot(x, az, label='raw:az', color='b')

# ----------------------------------------------------------------------

# plt.plot(x, temp, label='temp', color='r')

y = []
with open('fileSquareRoot.txt','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        y.append(10 * float(row[0]))
plt.plot(x,y, label='Magnitude')

S_detectCandidate = []
with open('S_detectCandidate.txt','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        S_detectCandidate.append(100 + 30 * float(row[0]))
# plt.plot(x, S_detectCandidate, label='S_detectCandidate', color='y')

x1 = []
peak = []
with open('peak.txt','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        x1.append(int(row[0]))
        peak.append(10 * float(row[1]))
plt.plot(x1, peak, label='peak', color='red')

x2 = []
vally = []
with open('vally.txt','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        x2.append(int(row[0]))
        vally.append(10 * float(row[1]))
plt.plot(x2, vally, label='vally', color='green')



'''
# ----------------------------------------------------------------------

timeData = []
score = []
with open('scoring.csv','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        score.append( (int(row[1]) + 10000000) / 1000000000)
        timeData.append( int(row[0]))

score1 = []
counter = 0;
for abc in x:
    if (len(score) > counter):
        if abc == timeData[counter]:
            score1.append( score[counter] )
            counter = counter + 1
        else :
            score1.append( 0 )
    else :
        score1.append( 0 )
plt.plot(x, score1, label='score1', color='y')
# ----------------------------------------------------------------------

timeData = []
postproc = []
with open('postproc.csv','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        postproc.append( int(row[1])/200000000 ) 
        timeData.append( int(row[0]) )

postproc1 = []
counter = 0;
for abc in x:
    if (len(postproc) > counter):
        if abc == timeData[counter]:
            postproc1.append( postproc[counter] )
            counter = counter + 1
        else :
            postproc1.append( 0 )
    else :
        postproc1.append( 0 )
plt.plot(x, postproc1, label='postproc1', color='blue')
'''



mng = plt.get_current_fig_manager()
mng.full_screen_toggle()

plt.xlabel('x')
plt.ylabel('y')
plt.ticklabel_format(useOffset=False, style='plain')
plt.title('ZTAG: Step and Jump counter')
plt.legend()
plt.show()
