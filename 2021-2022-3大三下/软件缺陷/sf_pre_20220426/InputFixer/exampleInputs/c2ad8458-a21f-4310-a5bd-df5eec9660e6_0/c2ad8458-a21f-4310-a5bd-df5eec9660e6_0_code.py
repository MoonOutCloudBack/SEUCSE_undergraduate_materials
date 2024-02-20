number, t = map(int, input().split())

start = list(range(1, number+1 ))
start.reverse()
work, goal = [], []
count = 0

def hanoi(n, start, goal, work):
    global count 
    if n<=0:
        return 
    
    hanoi(n-1, start, work, goal)
    
    goal.append(start.pop())
    count += 1
    if count == t:
        return 

    
    hanoi(n-1, work, goal, start)
    
if __name__ == "__main__":
    hanoi(number, start, goal, work)
    print(count)
    print(" ".join(list(map(str, start))) if start else "-")
    print(" ".join(list(map(str, work)))  if work else "-")
    print(" ".join(list(map(str, goal)))  if goal else "-")
 
