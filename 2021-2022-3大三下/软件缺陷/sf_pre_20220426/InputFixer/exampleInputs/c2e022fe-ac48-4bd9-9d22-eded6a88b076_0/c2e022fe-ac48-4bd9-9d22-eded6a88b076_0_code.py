def main ():
    print("\nThe output will be a factorial.\n")
    print("Enter an integer:")
    while True:
        number = int( input ("") )
        if number > 0 :
            number = int ( number )
            factorial  = 1
            startValue = 0
            while True:
                if startValue == number:
                    print ("\nThe factorial of", number ,"is", factorial,".\n")
                    break
                else:
                    for i in range ( 1,number + 1 ): 
                        if startValue < number :
                            factorial = factorial * i
                            startValue = startValue + 1
                        else:
                            break
            break
        elif number < 0:  # check if number is negative
            print ("\nFactorials do not exist for negative numbers.\n")
            break
        elif number == 0:   # check if number is positive
            print ("\nThe  factorial of 0 is 1.\n")
            break
        else:
            print(
"""
Error----> use integers only ( whole numbers )
                
Enter an integer:              
"""
)
main ()