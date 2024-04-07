# Author: Jakub Hamadej (xhamad03), FIT VUT

import xml.etree.ElementTree as ET
import xml.dom.minidom as minidom
import sys
import re

#help function, print if is called --help argument
def help():
    print("###################### Parser.py ######################")
    print("This script load input from standard input (stdin), convert input if is correct according to IPPcore24 language and write it's XML representations on standard output (stdout)")
    print("Arguments:")
    print("--help : print this info")
    sys.exit(0)

#function for get rid of the comments in line and spliting instructions
def getArrOfStrings(line):
    arr = line.split("#",1)
    return arr[0].split()

#function for checking if var type is correct
def varXML  (instruction_node,argN,data):
    if str(data).split('@',1)[0] in ['GF','TF','LF']:
        node = ET.SubElement(instruction_node,argN,type='var')
        patern = r'^[a-zA-Z0-9_\-&%*$!?]+$'
        if re.search(patern,str(data).split('@',1)[1]):
            node.text = data
        else:
            print(str(data).split('@',1)[1])
            sys.exit(23)
    else:
        sys.exit(23)

#function for checking if symb type is correct
def symbXML (instruction_node,argN,data):
    if not re.search(r'^[^@]*@[^@]*$',str(data)):
        sys.exit(23)
    if str(data).split('@',1)[0] in ['GF','TF','LF']:
        node = ET.SubElement(instruction_node,argN,type='var')
        patern = r'^[a-zA-Z0-9_\-&%*$!?]+$'
        if re.search(patern,str(data).split('@',1)[1]):
            node.text = data
        else:
            print(str(data).split('@',1)[1])
            sys.exit(23)
    elif str(data).split('@',1)[0] in ['int','string','bool','nil']:
        node = ET.SubElement(instruction_node,argN,type=str(data).split('@',1)[0])
        type = str(data).split('@',1)[0]
        constant = str(data).split('@',1)[1]
        if type == 'int' and ( re.search(r'^[-]?[0-9]+$',constant) or re.search(r'^-?0x[0-9A-Fa-f]+$',constant) or re.search(r'^-?0o[0-7]+$',constant)):
            node.text = constant
        elif (type == 'bool' and (constant == 'true' or constant == 'false')):
            node.text = constant
        elif type == 'nil' and constant == 'nil':
            node.text = 'nil'
        elif type == 'string' and re.search(r'[^\x00-\x1F\x23\x5C]|\\[0-9]{3}',constant):
            node.text = constant
        else:
            sys.exit(23)    
    else:
        sys.exit(23)

#function for checking if label type is correct
def labelXML(instruction_node,argN,data):
    patern = r'^[a-zA-Z0-9_\-&%*$!?]+$'
    if re.search(patern,data):
        node = ET.SubElement(instruction_node,argN,type='label')
        node.text = data
    else:
        sys.exit(23)

#function for checking if type type is correct
def typeXML (instruction_node,argN,data):
    if str(data) in ['int','string','bool']:
        node = ET.SubElement(instruction_node,argN,type='type')
        node.text = data
    else:
        sys.exit(23)

#function for processing a line into an XML tree
def addLineIntoXMLTree(root,inputArr,order):
    isfound = False
    
    for item in arrayOfCommands:
        if item[0] == (inputArr[0].upper()) :
            instruction_node = ET.SubElement(root,"instruction",order=str(order),opcode=item[0])
            isfound = True

            #checking the number of parameters
            if len(item) != len(inputArr):
                print('Bad number of parameters in given instruction')
                print(inputArr)
                sys.exit(23)
            #loop for creating and checking individual parameters
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
    #if given instruction does not exist => end script
    if isfound == False:
        print('Unknown instruction: ')
        print(inputArr)
        sys.exit(22)
    
#just array of instructions which can be accepted
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
    ['NOT', 'var','symb'],
    ['INT2CHAR','var','symb'],
    ['STRI2INT','var','symb','symb'],
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
    
#check for incomming arguments - if you try more argument - it will end script with error   
n =  len(sys.argv)   
if n > 1:
    if sys.argv[1] == "--help":
        help()
    else:
        print("Unknown argument")
        sys.exit(10)

# checking if given input has .ippcode24
hasHeader = False
for firstLine in sys.stdin:
    arr = getArrOfStrings(firstLine)
    if arr and len(arr) == 1 and arr[0].lower() == ".ippcode24":
        hasHeader = True    
        break

if hasHeader is False:
    print("Missing header '.ippcode24', practicaly input contains only comments or is completely empty.")
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
    

#output the result if everything is in order on the input.
tree = ET.ElementTree(root)
tree_str = ET.tostring(root,encoding="utf-8",xml_declaration=True,method='xml').decode('utf-8')
tree_str = minidom.parseString(tree_str).toprettyxml(indent=f"{' ' * 4}",encoding='UTF-8').decode('utf-8')
sys.stdout.write(tree_str)
sys.exit(0)