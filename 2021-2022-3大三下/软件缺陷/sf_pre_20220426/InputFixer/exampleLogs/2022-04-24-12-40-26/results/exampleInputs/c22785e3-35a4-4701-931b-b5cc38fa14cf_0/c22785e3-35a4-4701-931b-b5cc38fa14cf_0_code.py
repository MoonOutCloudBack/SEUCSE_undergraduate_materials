print('You have to enter the password to access the app: ')
myCode = int(input('what is the code? '))
password={'app':1111}
while myCode!=password['app']:
    print ('You entered wrong password. Try again ')
    myCode=int(input('what is the code? '))
print ('Correct! ')
