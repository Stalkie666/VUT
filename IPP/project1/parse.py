import xml.etree.ElementTree as ET

#--help
def help():
    print("printuju help")



#MAIN


header = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
a = ET.Element(header)





#nacist vstup - nacitat po radcich (asi getline? - nebo jeji python ekvivalent)

#zkontrolovat vstup
    #nacist prvni slovo na radku, podle toho jednat - dobra zprava, na nejake alokace pameti muzu srat
        #behem prochazeni kontrolovat jestli nenarazime predcasne (nebo prilis pozde, muze tam byt o operand navic) 
                                                                            #na komentar, muze byt pouze jednoradkovi
        #pokud by se jednolo o komentar od zacatku tak muzeme zahodit, neni potreba

#vyplivnout vystup pokud je na vstupu vsechno v poradku - nastudovat jak ma to XML vypadat
ET.dump(a) #asi takto
help()