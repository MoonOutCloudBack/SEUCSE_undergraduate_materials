#Area and perimeter program
valid = False

import math
area = ""
perimeter = ""
def output(area, perimeter):
    print("the area is ", area)
    print("the perimeter is ", perimeter)

while valid == False:
    Shape= input("""\n1. Triangle (equalateral)
2. Square
3. Pentagon
please choose an option by inputting it's number: """)

    if Shape == '1':
        print("Triangle, OK!")
        side = int(input("please enter side length: "))
        area = (math.sqrt(3)/4)*side**2
        perimeter = side*3
        output(area, perimeter)
        input("press any key to exit")
        valid = True
        
    elif Shape == '2':
        print("Square, OK!")
        side = int(input("please enter side length: "))
        area = side**2
        perimeter = side*4
        output(area, perimeter)
        input("press any key to exit")
        valid = True
        
    elif Shape == '3':
        print("Triangle, OK!")
        side = int(input("please enter side length: "))
        area = (1/4)*math.sqrt(5(5+(2*math.sqrt(5))))*side**2
        perimeter = side*5
        output(area, perimeter)
        input("press any key to exit")
        valid = True
    
    else:
        print("invalid input, please try again")
        
    
