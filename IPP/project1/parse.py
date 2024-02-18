import xml.etree.ElementTree as ET
import xml.dom.minidom as minidom
import sys

#--help
def help():
    print("printuju help")
    sys.exit(0)

def getArrOfStrings(line):
    arr = line.split("#",1)
    arr = arr[0].split()
    return arr


#MAIN
    
#check for incomming arguments    
n =  len(sys.argv)   
if n > 1:
    if sys.argv[1] == "--help":
        help()
    else:
        print("Unknown argument")
        sys.exit(10)

firstLine = sys.stdin.readline()
if firstLine.lower() != ".ippcode24\n":
    print("Spatna hlavicka: " + firstLine.lower())
    sys.exit(0)

for line in sys.stdin:
    arr = getArrOfStrings(line)
    if arr:
        print(arr)
    






#vyplivnout vystup pokud je na vstupu vsechno v poradku - v parse_kopie.py
