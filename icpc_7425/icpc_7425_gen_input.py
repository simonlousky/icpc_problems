import random

SPACE_RANGE = 10000

for i in range(1000):
    wells_no = random.randint(1, 5)
    pipes_no = random.randint(1, 10)
    print("{} {}".format(wells_no, pipes_no))
    for i in range(wells_no):
        print("{} {}".format(random.randint(-SPACE_RANGE,SPACE_RANGE), random.randint(-SPACE_RANGE,SPACE_RANGE)))
    for i in range(pipes_no):
        print("{} {} {}".format(random.randint(1, wells_no), \
            random.randint(-SPACE_RANGE, SPACE_RANGE),\
                random.randint(-SPACE_RANGE, SPACE_RANGE)))
    print()
