#!/usr/bin/python

from decimal import *
import sys

data1 = []
data2 = []
data3 = []
data4 = []
data5 = []

file1 = sys.argv[1]
file2 = sys.argv[2]
file3 = sys.argv[3]
out_file = sys.argv[4]


def find_t(time, data):
    for i in range(0, len(data)):
        if (time == data[i][0]):
            return data[i]
        elif (time < data[i][0]):
            a = (data[i][1] - data[i-1][1])/(data[i][0] - data[i - 1][0])
            b = data[i][1] - a * data[i][0]
            return (time, a * time + b)


with open(file1) as f:
    for line in f:
        time, value = [Decimal(x) for x in line.split()]
        data1.append((time, value))

with open(file2) as f:
    for line in f:
        time, value = [Decimal(x) for x in line.split()]
        data2.append((time, value))

with open(file3) as f:
    for line in f:
        time, value = [Decimal(x) for x in line.split()]
        data3.append((time, value))

for i in data1:
    if (i[0] >= data2[0][0] and i[0] <= data2[-1][0] and i[0] >= data3[0][0] and i[0] <= data3[-1][0]):
        data4.append(find_t(i[0], data2))
        data5.append(find_t(i[0], data3))
    elif(i[0] > data2[-1][0]):
        data4.append((i[0], Decimal('0.001')))
        data5.append(find_t(i[0], data3))
    elif(i[0] < data3[0][0]):
        data5.append((i[0], Decimal('0')))
        data4.append(find_t(i[0], data2))

with open('infected.csv', 'w') as f:
    for i in data4:
        f.write(str(i[0]) + " " + str(i[1]) + "\n")

with open('removed.csv', 'w') as f:
    for i in data5:
        f.write(str(i[0]) + " " + str(i[1]) + "\n")

with open(out_file, 'w') as f:
    for i in range(0, len(data5)):
        f.write(str(data5[i][0]) + " " + str(min(data5[i][1]/data4[i][1], 70)) + "\n")
