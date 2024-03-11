# Implementační dokumentace k 1. úloze do IPP 2023/2024
 Jméno a příjmení: Jakub Hamadej
 Login: xhamad03
## 1. ÚVOD:
 Zadání bylo implementovat v jazyce Python 3.10 skript parse.py, který bude provádět lexikální a syntaktickou analýzu jazyka IPPcode24.

## 2. Implementace:
 Skript je spuštěn příkazem v příkazové řádce `python3.10 parse.py`.
 Skript parse.py čte zdrojový kód v jazyce IPPcode24 ze standartního vstupu. Zkontroluje vstup po lexikální a syntaktické stránce a pokud je vše v pořádku, výsupem bude XML reprezentace programu na standartním výstupu. Pokud ne, je script ukončen s návratovým kódem různým od 0, který popisuje co na vstupu bylo špatně.
 V této implementaci jsou užity tyto knihovny. `xml.etree.ElementTree` pro generování XML stromu. `xml.dom.minidom` pro zlepšení čitelnosti XML stromu na výstupu. `sys` pro násilné ukončení scriptu a pro výpis XML reprezentace na standartní výstup. `re` pro regulární výrazy.
 Naopak v této implementaci není použito autorem navržené OOP z důvodu nepotřebnosti pro základní řešení zadání.

## 3. Argumenty scriptu
 --help : Vypíše nápovědu na standartní výstup a ukončí script.

 Žádný jiný argument není akceptován a skončí ukončením scriptu s kódem 10.

## 4. Funkce:

#### Help()
Funkce vypíše na standartní výstup nápovědu a ukončí script.

#### getArrOfStrings(line)
Funkce přijme string který rozdělí na dvě části, část před komentářem a po něm, komentář zahodí a část před komentářem rozdělí na jednotlivá slova a vrátí tato slova uložená v poli.

#### varXML/symbXML/labelXML/typeXML(instruction_node,argN,data)
Tyto funce zkontrolují správnost argumentu uloženého v proměnné `data` podle daného typu. Poté vytvoří `SubElement` pro `instruction_node` a přidá jí číslo uložené v proměnné `argN`.
Pokud není vše v pořádku, script je ukončen s návratovám kódem 23.

#### addLineIntoXMLTree(root,inputArr,order)
Tato funkce přijímá tyto argumenty: `root` je root XML tree z ElementTree, `inputArr` je pole slov rozdělených ve funkci `getArrOfStrings(line)`,  `order` je proměnná, která udává o kolikátou `instruction` se jedná.
Funkce zjistí jestli první prvek v `inputArr` odpovídá některé z instrukcí v jazyce IPPcode24, pokud ano, pak pro `root` vytvoří SubElement s názvem `instruction`, zkontroluje zprávný počet parametrů v instrukci a pro jednotlivé argumenty zavolá odpovídající funkce pro jejich kontrolu. Pokud instrukce není nalezena, script je ukončen kódem 22.
