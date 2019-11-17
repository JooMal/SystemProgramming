mport sys

def main() :
        data = {}
        for index, line in enumerate(sys.stdin):
                group, number = line.split(',')
                number = float(number)
                if group in data.keys() :
                        data[group] = max(data[group], number)
                else :
                        data[group] = number

        for group, number in data.items() :
                print('{}\t{}'.format(group, number))

if __name__ == '__main__' :
        main()
