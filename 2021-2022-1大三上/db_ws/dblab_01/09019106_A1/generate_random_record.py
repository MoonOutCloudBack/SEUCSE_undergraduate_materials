import random
import string

base, count = 105, 5000

with open('random_student.txt', 'w') as f:
    upper_letters = string.ascii_uppercase
    lower_letters = string.ascii_lowercase

    for i in range(count):
        # generate random student name
        rand_name = '"' + random.choice(upper_letters)
        for _ in range(random.randint(3, 12)):
            rand_name += random.choice(lower_letters)
        rand_name += ', ' + random.choice(upper_letters) + '."'

        # generate random record
        record = '%d' % (base + i) + '\t' \
                 + rand_name + '\t' \
                 + ('"m"' if random.randint(0, 2) == 0 else '"d"') + '\t' \
                 + '%d' % random.randint(15, 100) + '\t' \
                 + '%d' % random.randint(1, 6) + '\t' \
                 + '%.2f' % round(random.uniform(0, 4), 1) + '\n'
        # print(record)
        f.write(record)
