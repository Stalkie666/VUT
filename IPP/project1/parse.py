import xml.etree.ElementTree as ET
import xml.dom.minidom as MD
import sys

#--help
def help():
    print("printuju help")
    sys.exit(0)




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


# header = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
header = ET.Element("xml",version="1.0",encoding="UTF-8")
print(ET.tostring(header))

header = ET.Element("Program")
a = ET.SubElement(header,"MOV")


#nacist vstup - nacitat po radcich (asi getline? - nebo jeji python ekvivalent - jedna se o 'sys.stdin.readline()' )

#zkontrolovat vstup
    #nacist prvni slovo na radku, podle toho jednat - dobra zprava, na nejake alokace pameti muzu srat
        #behem prochazeni kontrolovat jestli nenarazime predcasne (nebo prilis pozde, muze tam byt o operand navic) 
                                                                            #na komentar, muze byt pouze jednoradkovi
        #pokud by se jednolo o komentar od zacatku tak muzeme zahodit, neni potreba

#vyplivnout vystup pokud je na vstupu vsechno v poradku - nastudovat jak ma to XML vypadat
#ET.dump(header) #asi takto
#print(ET.tostring(header))