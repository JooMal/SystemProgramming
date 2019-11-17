mport sys

def read_input(file):
        group = {}
        for index in range(0,16) :
                group[index]=[]
        for line in file :
                key, value = line.split(',')
                group[int(key)].append(float(value))
        return group

def main(seperator=','):
        group = read_input(sys.stdin)
        for key in group.keys() :
                for value in group[key] :
                        print('%s%s%s'%(key,seperator,float(value)))

if __name__ == "__main__":
        main()
