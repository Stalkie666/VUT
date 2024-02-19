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
    retVal = []
    for i, a in enumerate(arr):
        arr[i] = a.replace('<','&lt;').replace('>','&gt;').replace('&','&amp;')
    return arr

def addLineIntoXMLTree(root,inputArr):
    instruction_node = ET.SubElement(root,inputArr[0])
    #MOVE ⟨var⟩ ⟨symb⟩ 
    #CREATEFRAME
    #PUSHFRAME 
    #POPFRAME
    #DEFVAR ⟨var⟩
    #CALL ⟨label⟩ 
    #RETURN 
    #PUSHS ⟨symb⟩
    #POPS ⟨var⟩
    #ADD ⟨var⟩ ⟨symb1⟩ ⟨symb2⟩
    #SUB ⟨var⟩ ⟨symb1⟩ ⟨symb2⟩
    #MUL ⟨var⟩ ⟨symb1⟩ ⟨symb2⟩
    #IDIV ⟨var⟩ ⟨symb1⟩ ⟨symb2⟩
    #LT/GT/EQ ⟨var⟩ ⟨symb1⟩ ⟨symb2⟩ 
    #AND/OR/NOT ⟨var⟩ ⟨symb1⟩ ⟨symb2⟩ 
    #INT2CHAR ⟨var⟩ ⟨symb⟩
    #STRI2INT ⟨var⟩ ⟨symb1⟩ ⟨symb2⟩ 
    #READ ⟨var⟩ ⟨type⟩
    #WRITE ⟨symb⟩
    #CONCAT ⟨var⟩ ⟨symb1⟩ ⟨symb2⟩ 
    #STRLEN ⟨var⟩ ⟨symb⟩
    #GETCHAR ⟨var⟩ ⟨symb1⟩ ⟨symb2⟩ 
    #SETCHAR ⟨var⟩ ⟨symb1⟩ ⟨symb2⟩ 
    #TYPE ⟨var⟩ ⟨symb⟩
    #LABEL ⟨label⟩ 
    #JUMP ⟨label⟩
    #JUMPIFEQ ⟨label⟩ ⟨symb1⟩ ⟨symb2⟩
    #JUMPIFNEQ ⟨label⟩ ⟨symb1⟩ ⟨symb2⟩ 
    #EXIT ⟨symb⟩
    #DPRINT ⟨symb⟩ 
    #BREAK 



#MAIN
    
#check for incomming arguments    
n =  len(sys.argv)   
if n > 1:
    if sys.argv[1] == "--help":
        help()
    else:
        print("Unknown argument")
        sys.exit(10)

firstLine = sys.stdin.readline().split("#",1)[0].split()
if  len(firstLine) != 1 and firstLine[0].lower() != ".ippcode24":
    print("Spatna hlavicka: " + firstLine.lower())
    sys.exit(0)

#decompose input in array of strings and get rid of comments in given code
root = ET.Element('program')
root.set("language","IPPcode24")
for line in sys.stdin:
    arr = getArrOfStrings(line)
    if arr:
        addLineIntoXMLTree(root,arr)
    

#vyplivnout vystup pokud je na vstupu vsechno v poradku - v parse_kopie.py
tree = ET.ElementTree(root)
tree_str = ET.tostring(root,encoding="utf-8",xml_declaration=True).decode()
sys.stdout.write(tree_str)