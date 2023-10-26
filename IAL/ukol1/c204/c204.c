/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

bool solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
	// never trust anybody include yourself
	if( stack == NULL || Stack_IsEmpty(stack)
		|| postfixExpression == NULL || postfixExpressionLength == NULL ){
			return;
		}
	char tmp;
	while( !Stack_IsEmpty(stack) ){
		// get char from stack
		Stack_Top(stack,&tmp);
		// pop top of stack
		Stack_Pop(stack);
		if(tmp == '(') break;
		// add into posfix
		postfixExpression[(*postfixExpressionLength)++] = tmp;
	}
}

// my function for check priority of operators
int getPriority(char c){
	int priority = 0;
	switch(c){
		case '*': case '/':
			priority = 2;
			break;
		case '+': case '-':
			priority = 1;
			break;
		default:
			break;
	}
	return priority;
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
	
	// work as is written in presentation
	switch (c)
	{
		case '(':
			Stack_Push(stack,c);
			break;
		case ')':
			untilLeftPar(stack,postfixExpression,postfixExpressionLength);
			break;
		case '=':
			untilLeftPar(stack,postfixExpression,postfixExpressionLength);
			postfixExpression[(*postfixExpressionLength)++] = c;
			break;
		default:{
			char tmp = '\0';
			
			if(!Stack_IsEmpty(stack)) Stack_Top(stack,&tmp);
			if( Stack_IsEmpty(stack) || tmp == '(' ||  getPriority(tmp) < getPriority(c) ) Stack_Push(stack,c);
			else{
				// adding operators from top of the stack into posfix as long as priority on top of the stack is equal or higher than curent operator
				while(getPriority(tmp) >= getPriority(c) && !Stack_IsEmpty(stack)){
					postfixExpression[(*postfixExpressionLength)++] = tmp;
					Stack_Pop(stack);
					if(!Stack_IsEmpty(stack)) Stack_Top(stack,&tmp);
				}
				Stack_Push(stack,c);
			}
		}
			break;
	}
}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {
	// I don't trust to nobody
	if(!infixExpression) return NULL;
	// allocate array for postfix to return and check is suceeded
	char * postfix = (char*)calloc(MAX_LEN,sizeof(char));
	if(!postfix) return NULL;
	unsigned postfixLen = 0;
	Stack * stack = (Stack*)malloc(sizeof(Stack));
	Stack_Init(stack);

	//reading infix input string
	int infixPos = 0;
	for(char c = infixExpression[infixPos++]; c != '\0';c = infixExpression[infixPos++] ){
		// add to posfix
		if(    ( c >= '0' && c <= '9' )
			|| ( c >= 'a' && c <= 'z' )
			|| ( c >= 'A' && c <= 'Z' ) ){
				postfix[postfixLen++] = c;
			}
		// operators - call doOperation function
		else{
			doOperation(stack,c,postfix,&postfixLen);
		}
	}

	// cleaning
	Stack_Dispose(stack);
	free(stack);
	
	// i trust to author that he send me only correct inputs
	return postfix;
}


/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */
void expr_value_push( Stack *stack, int value ) {
	// just bit operations
	
	Stack_Push(stack, (char)(value >> 0) );
	Stack_Push(stack, (char)(value >> 8 ) );
	Stack_Push(stack, (char)(value >> 16 ) );
	Stack_Push(stack, (char)(value >> 24 ) );
}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */
void expr_value_pop( Stack *stack, int *value ) {
	
	*value = 0;
	unsigned int mask = 0xff;
	for(int i = 4; i > 0; --i){
		char tmp = 0;
		// just bit operations
		if(!Stack_IsEmpty(stack)) Stack_Top(stack,&tmp);
		*value |= ( (int)tmp << ( (i-1)*8)) & ( mask << ((i-1)*8) );
		Stack_Pop(stack);
	}
}


/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstpní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @return výsledek vyhodnocení daného výrazu na základě poskytnutých hodnot proměnných
 */
bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value ) {
	char * posfix = infix2postfix(infixExpression);
	if(posfix == NULL) return false;
	char * tmpFree = posfix;
	Stack * stack = (Stack*)malloc(sizeof(Stack));
	Stack_Init(stack);

	while( *posfix != '\0' ){
		switch (*posfix){
			// most of it same - diference only in what that two operands do
			case '+':{
				int tmp1 = 1,tmp2 = 1;
				expr_value_pop(stack,&tmp1);
				expr_value_pop(stack,&tmp2);
				tmp1 += tmp2;
				expr_value_push(stack,tmp1);
				}
				break;
			case '-':{
				int tmp1 = 1, tmp2 = 1;
				expr_value_pop(stack,&tmp1);
				expr_value_pop(stack,&tmp2);
				tmp1 = tmp2 - tmp1;
				expr_value_push(stack,tmp1);
				}
				break;
			case '*':{
				int tmp1 = 1, tmp2 = 1;
				expr_value_pop(stack,&tmp1);
				expr_value_pop(stack,&tmp2);
				tmp1 = tmp1*tmp2;
				expr_value_push(stack,tmp1);
				}
				break;
			case '/':{
				int tmp1 = 1, tmp2 = 1;
				expr_value_pop(stack,&tmp1);
				expr_value_pop(stack,&tmp2);
				if( tmp1 == 0 ){
					// clean mess and return false
					free(tmpFree);
					Stack_Dispose(stack);
					free(stack);
					return false;
				}
				tmp1 = tmp2 / tmp1;
				expr_value_push(stack,tmp1);
				}
				break;
			// get return value from top of the stacl
			case '=':
				expr_value_pop(stack,value);
				break;
			default:
				// get values from table and add it on top of the stack
				for(int i = 0; i < variableValueCount;++i){
					if( *posfix == variableValues[i].name ){
						expr_value_push(stack,variableValues[i].value);
						break;
					}
				}
				break;
		}
		posfix++;
	}
	// cleaning mess
	free(tmpFree);
	Stack_Dispose(stack);
	free(stack);
	return true;
}

/* Konec c204.c */
