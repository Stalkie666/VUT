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
    #tohle pry nepotrebuju, ET to udela za mne, neverim jim, takze zalohu sem
    #for i, a in enumerate(arr):
    #   arr[i] = a.replace('<','&lt;').replace('>','&gt;').replace('&','&amp;')
    return arr

#zpracovani radky do xml tree
def addLineIntoXMLTree(root,inputArr):
    instruction_node = ET.SubElement(root,inputArr[0])
    
    #asi to nacpat do pole poli jednotlive instrukce a potom s tim neco vymyslet

    #MOVE ⟨var⟩ ⟨symb⟩ 
    #INT2CHAR ⟨var⟩ ⟨symb⟩
    #STRLEN ⟨var⟩ ⟨symb⟩
    #TYPE ⟨var⟩ ⟨symb⟩

    #DEFVAR ⟨var⟩
    #POPS ⟨var⟩

    #CALL ⟨label⟩
    #LABEL ⟨label⟩ 
    #JUMP ⟨label⟩
    
    #PUSHS ⟨symb⟩
    #WRITE ⟨symb⟩
    #EXIT ⟨symb⟩
    #DPRINT ⟨symb⟩ 
    
    #ADD ⟨var⟩ ⟨symb1⟩ ⟨symb2⟩
    #SUB ⟨var⟩ ⟨symb1⟩ ⟨symb2⟩
    #MUL ⟨var⟩ ⟨symb1⟩ ⟨symb2⟩
    #IDIV ⟨var⟩ ⟨symb1⟩ ⟨symb2⟩
    #LT/GT/EQ ⟨var⟩ ⟨symb1⟩ ⟨symb2⟩ 
    #AND/OR/NOT ⟨var⟩ ⟨symb1⟩ ⟨symb2⟩
    #STRI2INT ⟨var⟩ ⟨symb1⟩ ⟨symb2⟩
    #CONCAT ⟨var⟩ ⟨symb1⟩ ⟨symb2⟩
    #GETCHAR ⟨var⟩ ⟨symb1⟩ ⟨symb2⟩ 
    #SETCHAR ⟨var⟩ ⟨symb1⟩ ⟨symb2⟩
    #JUMPIFEQ ⟨label⟩ ⟨symb1⟩ ⟨symb2⟩
    #JUMPIFNEQ ⟨label⟩ ⟨symb1⟩ ⟨symb2⟩ 
    
    
    
    
    
    
    
arrayOfCommands = [
    ['MOVE','var','symb'],
    ['CREATEFRAME'],
    ['PUSHFRAME'],
    ['POPFRAME'],
    ['DEFVAR','var'],
    ['CALL','label'],
    ['RETURN'],
    ['PUSHS','symb'],
    ['POPS','var'],
    ['ADD', 'var','symb','symb'],
    ['SUB', 'var','symb','symb'],
    ['MUL', 'var','symb','symb'],
    ['IDIV','var','symb','symb'],
    ['LT',  'var','symb','symb'],
    ['GT',  'var','symb','symb'],
    ['EQ',  'var','symb','symb'],
    ['AND', 'var','symb','symb'],
    ['OR',  'var','symb','symb'],
    ['NOT', 'var','symb','symb'],
    ['INT2CHAR','var','symb'],
    ['STRING2INT','var','symb','symb'],
    ['READ','var','type'], #type = {int,string,bool}
    ['WRITE','symb'],
    ['CONCAT','var','symb','symb'],
    ['STRLEN','var','symb'],
    ['GETCHAR','var','symb','symb'],
    ['SETCHAR','var','symb','symb'],
    ['TYPE','var','symb'],
    ['LABEL','label'],
    ['JUMP','label'],
    ['JUMPIFEQ', 'label','symb','symb'],
    ['JUMPIFNEQ','label','symb','symb'],
    ['EXIT','symb'],
    ['DPRINT','symb'],
    ['BREAK']
]


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
        #print(arr)
    

#vyplivnout vystup pokud je na vstupu vsechno v poradku - v parse_kopie.py
tree = ET.ElementTree(root)
tree_str = ET.tostring(root,encoding="utf-8",xml_declaration=True,method='xml').decode('utf-8')
tree_str = minidom.parseString(tree_str).toprettyxml(indent=f"{' ' * 4}",encoding='UTF-8').decode('utf-8')
sys.stdout.write(tree_str)