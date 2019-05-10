import numpy as np
import pandas as pd
import math
import tkinter as tk
def outStone(stone):
    root = tk.Tk()
    width = int(4.75*50)
    height = int(11.28*50)
    x = 2.375
    y = 4.88
    print(width,height)
    root.geometry("237x564")
    canvas = tk.Canvas(root, width=width, height=height)
    canvas.create_oval((2.375-1.83)*50,(4.88-1.83)*50,(2.375+1.83)*50,(4.88+1.83)*50,fill='blue')
    canvas.create_oval((2.375-1.22)*50,(4.88-1.22)*50,(2.375+1.22)*50,(4.88+1.22)*50,fill='white')
    canvas.create_oval((2.375-0.61)*50,(4.88-0.61)*50,(2.375+0.61)*50,(4.88+0.61)*50,fill='red')
    canvas.create_line(2.375*50,1.22*50,2.375*50,11.28*50)
    canvas.create_line(0,4.88*50,4.75*50,4.88*50)
    canvas.create_oval((stone[0]-0.145)*50,(stone[1]-0.145)*50,(stone[0]+0.145)*50,(stone[1]+0.145)*50,fill="yellow")

    canvas.place(x=0,y=0)
    root.mainloop()
def outBoard(board):
    root = tk.Tk()
    width = int(4.75*50)
    height = int(11.28*50)
    x = 2.375
    y = 4.88
    root.geometry("237x564")
    canvas = tk.Canvas(root, width=width, height=height)    
    canvas.create_oval((2.375-1.83)*50,(4.88-1.83)*50,(2.375+1.83)*50,(4.88+1.83)*50,fill='blue')
    canvas.create_oval((2.375-1.22)*50,(4.88-1.22)*50,(2.375+1.22)*50,(4.88+1.22)*50,fill='white')
    canvas.create_oval((2.375-0.61)*50,(4.88-0.61)*50,(2.375+0.61)*50,(4.88+0.61)*50,fill='red')
    canvas.create_line(2.375*50,1.22*50,2.375*50,11.28*50)
    canvas.create_line(0,4.88*50,4.75*50,4.88*50)
    for i in range(16):
        stone = [board[i*2], board[i*2+1]]
        if stone[0]+stone[1]!=0:
            if i%2==0:
                canvas.create_oval((stone[0]-0.145)*50,(stone[1]-0.145)*50,(stone[0]+0.145)*50,(stone[1]+0.145)*50,fill="yellow")
            else:
                canvas.create_oval((stone[0]-0.145)*50,(stone[1]-0.145)*50,(stone[0]+0.145)*50,(stone[1]+0.145)*50,fill="black")
                

    canvas.place(x=0,y=0)
    root.mainloop()
    
def getDist(stone):
    ans = math.sqrt((stone[0]-2.375)**2 + (stone[1]-4.88)**2)
    return ans

def getDegree(x,y):
    radian = math.atan2(x-2.375, y-4.88)
    return 180*radian/math.pi

def getVector(board, target, isMine):
    """
    0~2:rank
    3~10:x
    11~22:y
    23~28:dist
    29:isMine
    30~41:angle
    """
    ans = ""
    rank = getRank(board, target)
    x = board[target*2]
    y = board[target*2+1]
    degree = getDegree(x,y)
    dist = getDist([x,y])
    if rank==0:
        ans+="1,0,0,"
    elif rank==1:
        ans+="0,1,0,"
    elif rank==2:
        ans+="0,0,1,"
    else:
        ans+="0,0,0,"

    if x<2.375-1.83:
        ans += "1,0,0,0,0,0,0,0,"
    elif 2.375-1.83<=x<2.375-1.22:
        ans += "0,1,0,0,0,0,0,0,"
    elif 2.375-1.22<=x<2.375-0.61:
        ans += "0,0,1,0,0,0,0,0,"
    elif 2.375-0.61<=x<2.375:
        ans += "0,0,0,1,0,0,0,0,"
    elif 2.375<=x<2.375+0.61:
        ans += "0,0,0,0,1,0,0,0,"
    elif 2.375+0.61<=x<2.375+1.22:
        ans += "0,0,0,0,0,1,0,0,"
    elif 2.375+1.22<=x<2.375+1.83:
        ans += "0,0,0,0,0,0,1,0,"
    elif 2.375+1.83<=x:
        ans += "0,0,0,0,0,0,0,1,"
    else:
        ans += "0,0,0,0,0,0,0,0,"

    if y<4.88-1.83:
        ans += "1,0,0,0,0,0,0,0,0,0,0,0,"
    elif 4.88-1.83<=y<4.88-1.22:
        ans += "0,1,0,0,0,0,0,0,0,0,0,0,"
    elif 4.88-1.22<=y<4.88-0.61:
        ans += "0,0,1,0,0,0,0,0,0,0,0,0,"
    elif 4.88-0.61<=y<4.88:
        ans += "0,0,0,1,0,0,0,0,0,0,0,0,"
    elif 4.88<=y<4.88+0.61:
        ans += "0,0,0,0,1,0,0,0,0,0,0,0,"
    elif 4.88+0.61<=y<4.88+1.22:
        ans += "0,0,0,0,0,1,0,0,0,0,0,0,"
    elif 4.88+1.22<=y<4.88+1.83:
        ans += "0,0,0,0,0,0,1,0,0,0,0,0,"
    elif 4.88+1.83<=y<4.88+2.68:
        ans += "0,0,0,0,0,0,0,1,0,0,0,0,"
    elif 4.88+2.68<=y<4.88+3.53:
        ans += "0,0,0,0,0,0,0,0,1,0,0,0,"
    elif 4.88+3.53<=y<4.88+4.38:
        ans += "0,0,0,0,0,0,0,0,0,1,0,0,"
    elif 4.88+4.38<=y<4.88+5.23:
        ans += "0,0,0,0,0,0,0,0,0,0,1,0,"
    elif 4.88+5.23<=y:
        ans += "0,0,0,0,0,0,0,0,0,0,0,1,"
    else:
        ans += "0,0,0,0,0,0,0,0,0,0,0,0,"
    
    if dist<0.61:
        ans += "1,0,0,0,0,0,"
    elif 0.61<=dist<1.22:
        ans += "0,1,0,0,0,0,"
    elif 1.22<=dist<1.83:
        ans += "0,0,1,0,0,0,"
    elif 1.83<=dist<3.05:
        ans += "0,0,0,1,0,0,"
    elif 3.05<=dist<4.27:
        ans += "0,0,0,0,1,0,"
    elif 4.27<=dist<5.49:
        ans += "0,0,0,0,0,1,"
    else:
        ans += "0,0,0,0,0,0,"

    isGuarded = False
    if isGuarded:
        ans +="1,"
    else:
        ans+="0,"

    if target%2==isMine:
        ans += "1,0,"
    else:
        ans += "0,1,"

    if 0<=degree<30:
        ans += "1,0,0,0,0,0,0,0,0,0,0,0,"
    elif 30<=degree<60:
        ans += "0,1,0,0,0,0,0,0,0,0,0,0,"
    elif 60<=degree<90:
        ans += "0,0,1,0,0,0,0,0,0,0,0,0,"
    elif 90<=degree<120:
        ans += "0,0,0,1,0,0,0,0,0,0,0,0,"
    elif 120<=degree<150:
        ans += "0,0,0,0,1,0,0,0,0,0,0,0,"
    elif 150<=degree<180:
        ans += "0,0,0,0,0,1,0,0,0,0,0,0,"
    elif 180<=degree<210:
        ans += "0,0,0,0,0,0,1,0,0,0,0,0,"
    elif 210<=degree<240:
        ans += "0,0,0,0,0,0,0,1,0,0,0,0,"
    elif 240<=degree<270:
        ans += "0,0,0,0,0,0,0,0,1,0,0,0,"
    elif 270<=degree<300:
        ans += "0,0,0,0,0,0,0,0,0,1,0,0,"
    elif 300<=degree<330:
        ans += "0,0,0,0,0,0,0,0,0,0,1,0,"
    elif 330<=degree<360:
        ans += "0,0,0,0,0,0,0,0,0,0,0,1,"
    else:
        ans += "0,0,0,0,0,0,0,0,0,0,0,0,"
    return ans

def getRank(board, target):
    stones = []
    for i in range(16):
        stones.append([board[i*2], board[i*2+1]])
    dists = []
    for i in range(16):
        if stones[i][0]+stones[i][1]==0.00:
            dists.append(99999)
        else:
            dists.append(getDist(stones[i]))
    sort = []
    for i in range(16):
        sort.append(dists[i])
    for j in range(16):
        for i in range(15):
            if sort[i]>sort[i+1]:
                tmp = sort[i]
                sort[i]=sort[i+1]
                sort[i+1]=tmp
    for i in range(16):
        if dists[target]==sort[i]:
            return i

def getScore(board, turn):
    #turn==0: 0,2,4,6,8,10,12,14 is my stone
    ranks = []
    for i in range(16):
        ranks.append(getRank(board,i))
        #ranks[0]=0番目の石のランク
    no0Stone = 0
    for i in range(16):
        if ranks[i]==0:
            no0Stone = i
            break
    scoredPlayer = no0Stone%2
    score = 0
    for j in range(8):
        isGet = False
        for i in range(8):
            if board[(2*i+scoredPlayer)*2]+board[(2*i+scoredPlayer)*2+1]==0:
                break
            if ranks[2*i+scoredPlayer]==j:
                score+=1
                isGet=True
        if not(isGet):
            break
    if turn!=scoredPlayer:
        score *= -1
    return score



df = pd.read_csv("./allLogs.csv")
"""
preboard(32), w, a, p, nextBoard(32)
"""
for line in range(len(df)):
    preBoard = []
    nextBoard = []
    w = int(df.iloc[line,32])
    a = int(df.iloc[line,33])
    p = int(df.iloc[line,34])
    target = int(df.iloc[line,67])
    for i in range(32):
        preBoard.append(float(df.iloc[line, i]))
        nextBoard.append(float(df.iloc[line, i+35]))
    for i in range(2):
        isMine = i
        vec = getVector(preBoard, target, isMine)
        score=getScore(nextBoard, isMine)-getScore(preBoard, isMine)

        ans = str(vec)+str(w)+","+str(a)+","+str(p)+","+str(score)+"\n"
        with open("./logs.csv", 'a') as f:
            f.write(ans)