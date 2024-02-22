import xml.etree.ElementTree as ET
import xml.dom.minidom as minidom
import sys
import re

#--help
def help():
    print("printuju help")
    sys.exit(0)

def getArrOfStrings(line):
    arr = line.split("#",1)
    return arr[0].split()

def varXML  (instruction_node,argN,data):
    if str(data).split('@',1)[0] in ['GF','TF','LF']:
        node = ET.SubElement(instruction_node,argN,type='var')
        node.text = data
    else:
        sys.exit(23)

def symbXML (instruction_node,argN,data):
    if str(data).split('@',1)[0] in ['GF','TF','LF']:
        node = ET.SubElement(instruction_node,argN,type='var')
        patern = r'^[a-zA-Z0-9_\-&%*$!?]+$'
        if re.search(patern,str(data).split('@',1)[1]):
            node.text = data
        else:
            print(str(data).split('@',1)[1])
            sys.exit(23)
    elif str(data).split('@',1)[0] in ['int','string','bool','nil']:
        #zde jeste kontrolovat ze kdyz je to int tak tam fakt cpu int atd
        node = ET.SubElement(instruction_node,argN,type=str(data).split('@',1)[0])
        #node.text = str(data).split('@',1)[1]
        type = str(data).split('@',1)[0]
        constant = str(data).split('@',1)[1]
        if type == 'int' and re.search(r'^[-]?[0-9]+$',constant):
            node.text = constant
        elif (type == 'bool' and (constant == 'true' or constant == 'false')):
            node.text = constant
        elif type == 'nil' and constant == 'nil':
            node.text = 'nil'
        elif type == 'string':
            node.text = constant
        else:
            sys.exit(23)    
    else:
        sys.exit(23)

def labelXML(instruction_node,argN,data):
    patern = r'^[a-zA-Z0-9_\-&%*$!?]+$'
    if re.search(patern,data):
        node = ET.SubElement(instruction_node,argN,type='label')
        node.text = data
    else:
        sys.exit(23)

def typeXML (instruction_node,argN,data):
    if str(data).lower() in ['int','string','bool']:
        node = ET.SubElement(instruction_node,argN,type='type')
        node.text = data
    else:
        sys.exit(23)

#zpracovani radky do xml tree
def addLineIntoXMLTree(root,inputArr,order):
    isfound = False
    for item in arrayOfCommands:
        if item[0] == inputArr[0]:
            instruction_node = ET.SubElement(root,"instruction",order=str(order),opcode=inputArr[0])
            isfound = True

            #kontrola počtu parametrů
            if len(item) != len(inputArr):
                print('Fucking spatna delka prikazu')
                print(inputArr)
                sys.exit(23)
            #for-cyklus pro vytvareni a kontrolu jednotlivych parametru
            for i in range(1,len(inputArr)):
                argN = "arg" + str(i)
                if item[i] == 'var':
                    varXML(instruction_node,argN,inputArr[i])
                elif item[i] == 'symb':
                    symbXML(instruction_node,argN,inputArr[i])
                elif item[i] == 'label':
                    labelXML(instruction_node,argN,inputArr[i])
                else:
                    typeXML(instruction_node,argN,inputArr[i])
            break
    if isfound == False:
        print('Fucking chyba: ')
        print(inputArr)
        sys.exit(22)
    

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
    sys.exit(21)

#decompose input in array of strings and get rid of comments in given code
root = ET.Element('program')
root.set("language","IPPcode24")

order = 1
for line in sys.stdin:
    arr = getArrOfStrings(line)
    if arr:
        addLineIntoXMLTree(root,arr,order)
        order += 1
    

#vyplivnout vystup pokud je na vstupu vsechno v poradku
tree = ET.ElementTree(root)
tree_str = ET.tostring(root,encoding="utf-8",xml_declaration=True,method='xml').decode('utf-8')
tree_str = minidom.parseString(tree_str).toprettyxml(indent=f"{' ' * 4}",encoding='UTF-8').decode('utf-8')
sys.stdout.write(tree_str)