<?php

namespace IPP\Student;

use DOMDocument;
use DOMXPath;
use Exception;
use IPP\Core\AbstractInterpreter;
use IPP\Core\Exception\NotImplementedException;
use IPP\Core\Exception\XMLException;
use IPP\Core\Exception\IPPException;
use IPP\Core\ReturnCode;

class Interpreter extends AbstractInterpreter
{

    private DOMDocument $root;

    private $Instructions;

    private ?Frame $globalFrame;
    private $localFramesArrays;
    private ?Frame $currentLocalFrame;
    private ?Frame $temporaryFrame;

    private $dataStack;
    private $callStack;

    public function execute(): int
    {
        // TODO: Start your code here
        // Check \IPP\Core\AbstractInterpreter for predefined I/O objects:
        try{
            $this->root = $this->source->getDOMDocument();
        }catch(XMLException $e){
            throw new GeneralException("Invalid XML", ReturnCode::INVALID_XML_ERROR);
        }
        
        
        if ($this->checkForExpectedStructure() == false){
            $this->stderr->writeString("Somethings is wrong\n");
            throw new GeneralException("Wrong structure of input instructions", ReturnCode::INVALID_SOURCE_STRUCTURE);
        }
        // load instructions into array
        $this->loadInstructions();



        // check for jumps and labels if they are valid
        $retVal = $this->checkForDuplicateLabelsAndNonexistentingOnes();


        // interpret
        $retVal = $this->runInterpretInstructions();


        // $val = $this->input->readString();
        // $this->stdout->writeString("stdout");
        // $this->stderr->writeString("stderr");
        

        return $retVal;

        // throw new NotImplementedException;
    }




    public function checkForExpectedStructure(): bool
    {
        $previousOrder = 0;
        $xpath = new DOMXPath(($this->root));
        $query = "/program[count(@*) = 1 and @language='IPPcode24']";
        $programNodes = $xpath->query($query);

        if( $programNodes->length != 1 ){
            return false;
        }

        $query = "/program/*";
        $instructionNodes = $xpath->query($query);
        foreach($instructionNodes as $node){
            // check if all elements are instructions
            if( $node->nodeName !== 'instruction' )
                return false;
            // and has only two atributes - order,opcode
            if( !$node->hasAttribute('order') || !$node->hasAttribute('opcode') )
                return false;
            // check correct order
            if( $previousOrder >= $node->getAttribute('order') )
                return false;
            $previousOrder = $node->getAttribute('order');
            // check argNodes
            $argNodes = [];
            foreach ($node->childNodes as $child) {
                if ($child->nodeType === XML_ELEMENT_NODE) {
                    $argNodes[] = $child;
                }
            }
            // check if args are correctly named
            foreach( $argNodes as $argNode ){
                if( !in_array($argNode->nodeName, ['arg1','arg2','arg3']))
                    return false;
                // and has correct attribute called 'type'
                if( $argNode->attributes->length != 1 || !$argNode->hasAttribute('type'))
                    return false;
            }
        }

        return true;
    }

    public function loadInstructions() : bool{
        // load every instruction
        foreach( $this->root->getElementsByTagName('instruction') as $instructionNode ){
            $instruction = new Instruction($instructionNode->getAttribute('opcode'),$instructionNode->getAttribute('order'));
            for ($i = 1; $i <= 3; $i++) {
                $argName = 'arg' . $i;
                $argNode = $instructionNode->getElementsByTagName($argName)->item(0);
                if ($argNode) {
                    // save argument of instruction
                    $instruction->addArgument($argNode->getAttribute('type'),trim($argNode->nodeValue));
                }
            }
            $this->Instructions[] = $instruction;
        }

        return true;
    }

    public function checkForDuplicateLabelsAndNonexistentingOnes() : int{
        $encounteredLabels = [];

        foreach( $this->Instructions as $instruction ){
            $opcode = $instruction->opcode;
            if( $opcode === 'LABEL' ){
                $labelArgs = array_filter($instruction->args, function ($arg) {
                    return $arg['type'] === 'label';
                });
                if (!empty($labelArgs)) {
                    $labelValue = $labelArgs[0]['value'];
        
                    // Check if the label value has been encountered before
                    if (isset($encounteredLabels[$labelValue])) {
                        throw new GeneralException("Duplicite labels", ReturnCode::SEMANTIC_ERROR);
                    } else {
                        // Store the encountered label value
                        $encounteredLabels[$labelValue] = true;
                    }
                }
            }
        }
        // check for nonexisting labels in jumps
        foreach($this->Instructions as $instruction){
            $opcode = $instruction->opcode;
            if( $opcode === 'JUMP' || $opcode === 'JUMPIFEQ' || $opcode === 'JUMPIFNEQ' || $opcode === 'CALL' ){
                $labelArgs = array_filter($instruction->args, function ($arg) {
                    return $arg['type'] === 'label';
                });
                if (!empty($labelArgs)) {
                    $labelValue = $labelArgs[0]['value'];   
                    // Check if the label value has been encountered before
                    if (!isset($encounteredLabels[$labelValue])) {
                        throw new GeneralException("Jumps on nonexist labels", ReturnCode::SEMANTIC_ERROR);
                    }
                }
            }
            
        }
        return 0;
    }
    // Nejaky funkce se switchem pro prochazeni instrukci
    private $currentInstructionIndex;
    private int $instructionCounterForBreak;

    public function runInterpretInstructions() : int{
        $this->currentInstructionIndex = 0;
        $this->instructionCounterForBreak = 0;

        $this->globalFrame = new Frame;
        $this->localFramesArrays = [];
        $this->currentLocalFrame = NULL;
        $this->temporaryFrame = NULL;

        $this->dataStack = [];
        $this->callStack = [];
        

        while($this->currentInstructionIndex < count($this->Instructions)){
            $retVal = ReturnCode::OK;
            $currentInstruction = $this->Instructions[$this->currentInstructionIndex];

            $opcode = $currentInstruction->opcode;

            print("Test:\n");
            print($this->currentInstructionIndex . " ");
            print($opcode . "\n");

            switch($opcode){
                case 'MOVE':
                    $retVal = $this->Move();
                    break;
                case 'CREATEFRAME':
                    $this->CreateFrame();
                    break;
                case 'PUSHFRAME':
                    $retVal = $this->PushFrame();
                    break;
                case 'POPFRAME':
                    $retVal = $this->PopFrame();
                    break;
                case 'DEFVAR':
                    $retVal = $this->DefVar();
                    break;
                case 'CALL':
                    $retVal = $this->Call();
                    break;
                case 'RETURN':
                    $retVal = $this->Return();
                    break;
                case 'PUSHS':
                    $retVal = $this->Pushs();
                    break;
                case 'POPS':
                    $retVal = $this->Pops();
                    break;
                case 'ADD':
                    $retVal = $this->Add();
                    break;
                case 'SUB':
                    $retVal = $this->Sub();
                    break;
                case 'MUL':
                    $retVal = $this->Mul();
                    break;
                case 'IDIV':
                    $retVal = $this->Idiv();
                    break;
                case 'LT':
                    $retVal = $this->Lt();
                    break;
                case 'GT':
                    $retVal = $this->Gt();
                    break;
                case 'EQ':
                    $retVal = $this->Eq();
                    break;
                case 'AND':
                    $retVal = $this->And();
                    break;
                case 'OR':
                    $retVal = $this->Or();
                    break;
                case 'NOT':
                    $retVal = $this->Not();
                    break;
                case 'INT2CHAR':
                    $retVal = $this->Int2Char();
                    break;
                case 'STRI2INT':
                    $retVal = $this->Stri2Int();
                    break;
                case 'READ':
                    $retVal = $this->Read();
                    break;
                case 'WRITE':
                    $retVal = $this->Write();
                    break;
                case 'CONCAT':
                    $retVal = $this->Concat();
                    break;
                case 'STRLEN':
                    $retVal = $this->Strlen();
                    break;
                case 'GETCHAR':
                    $retVal = $this->GetChar();
                    break;
                case 'SETCHAR':
                    $retVal = $this->SetChar();
                    break;
                case 'TYPE':
                    $retVal = $this->Type();
                    break;
                case 'LABEL':
                    // do nothing
                    break;
                case 'JUMP':
                    $this->Jump();
                    break;
                case 'JUMPIFEQ':
                    $retVal = $this->JumpIfEq();
                    break;
                case 'JUMPIFNEQ':
                    $retVal = $this->JumpIfNeq();
                    break;
                case 'EXIT':
                    $retVal = $this->Exit();
                    return $retVal;
                    break;
                case 'DPRINT':
                    $retVal = $this->DPrint();
                    break;
                case 'BREAK':
                    $this->Break();
                    break;
                // should not happen
                default:
                    throw new GeneralException("General error", ReturnCode::INTEGRATION_ERROR);
            }

            if( $retVal !== ReturnCode::OK )
                return $retVal;

            // next instruction
            $this->currentInstructionIndex++;
            $this->instructionCounterForBreak++;

        }
        
        return ReturnCode::OK;
    }

    // MOVE
    public function Move() : int{
        $destination = $this->getVariableFromArgument($this->Instructions[$this->currentInstructionIndex]->args[0]);
        $source = $this->parseSymbFromArgument($this->Instructions[$this->currentInstructionIndex]->args[1]);
       
        $destination->setValue( $source['type'],$source['value'] );
    
        return ReturnCode::OK;
    }
    // CREATEFRAME
    public function CreateFrame(): int{
        $this->temporaryFrame = new Frame();
        return ReturnCode::OK;
    }
    // PUSHFRAME
    public function PushFrame(): int{
        if( $this->temporaryFrame === NULL )
            throw new GeneralException("Temporary frame was not inicialized", ReturnCode::FRAME_ACCESS_ERROR);
        $this->currentLocalFrame = $this->temporaryFrame;
        $this->localFramesArrays[] = $this->currentLocalFrame;
        $this->temporaryFrame = NULL;
        return ReturnCode::OK;
    }
    // POPFRAME
    public function PopFrame():int{
        if(empty($this->localFramesArrays))
            throw new GeneralException("Empty stack of local frames", ReturnCode::FRAME_ACCESS_ERROR);
        $this->temporaryFrame = $this->currentLocalFrame;
        array_pop($this->localFramesArrays);
        if( empty($this->localFramesArrays) ){
            $this->currentLocalFrame = NULL;
        }
        else{
            end($this->localFramesArrays);
            $this->currentLocalFrame = current($this->localFramesArrays);
        }
        return ReturnCode::OK;
    }
    // DEFVAR
    public function DefVar():int{
        $instruction = $this->Instructions[$this->currentInstructionIndex];
        $inputVal = $instruction->args[0]['value'];
        $parts = $this->splitArgumentValue($inputVal);

        if( $parts['type'] == "GF" )
            $retVal = $this->globalFrame->addVar($parts['value']);
        else if( $parts['type'] == "LF" && $this->currentLocalFrame !== NULL)
            $retVal = $this->currentLocalFrame->addVar($parts['value']);
        else if( $parts['type'] == "TF" && $this->temporaryFrame !== NULL)
            $retVal = $this->temporaryFrame->addVar($parts['value']);
        else
            throw new GeneralException("Wrong Frame or uninicialized one", ReturnCode::FRAME_ACCESS_ERROR);
        return $retVal;
    }
    // CALL
    public function Call():int{
        $this->callStack[] = $this->currentInstructionIndex + 1;
        $this->MakeJump();
        return ReturnCode::OK;
    }
    // RETURN
    public function Return():int{
        if( empty($this->callStack) )
            throw new GeneralException("Call stack is empty", ReturnCode::VALUE_ERROR);
        end($this->callStack);
        $this->currentInstructionIndex = current($this->callStack);
        array_pop($this->callStack);
        return ReturnCode::OK;
    }
    // PUSHS
    public function Pushs():int{
        $source = $this->parseSymbFromArgument( $this->Instructions[$this->currentInstructionIndex]->args[0] );

        $this->dataStack[] = $source;

        return ReturnCode::OK;
    }
    // POPS
    public function Pops():int{
        if( empty($this->dataStack) ) throw new GeneralException("Empty data stack", ReturnCode::VALUE_ERROR);

        $destination = $this->getVariableFromArgument($this->Instructions[$this->currentInstructionIndex]->args[0]);

        end($this->dataStack);
        $src = current($this->dataStack);
        array_pop($this->dataStack); 

        $destination->setValue($src['type'],$src['value']);

        return ReturnCode::OK;
    }
    // ADD
    public function Add():int{
        $instructionArguments = $this->Instructions[$this->currentInstructionIndex]->args;
        $destination = $this->getVariableFromArgument($instructionArguments[0]);
        $symb1 = $this->parseSymbFromArgument($instructionArguments[1]);
        $symb2 = $this->parseSymbFromArgument($instructionArguments[2]);

        if( $symb1['type'] !== "int" || $symb2['type'] !== "int" )
            throw new GeneralException("One of given symb was not type of int", ReturnCode::OPERAND_TYPE_ERROR);

        $destination->setValue($symb1['type'],(intval($symb1['value']) + intval($symb2['value'])) );
        
        return ReturnCode::OK;
    }
    // SUB
    public function Sub():int{
        $instructionArguments = $this->Instructions[$this->currentInstructionIndex]->args;
        $destination = $this->getVariableFromArgument($instructionArguments[0]);
        $symb1 = $this->parseSymbFromArgument($instructionArguments[1]);
        $symb2 = $this->parseSymbFromArgument($instructionArguments[2]);

        if( $symb1['type'] !== "int" || $symb2['type'] !== "int" )
            throw new GeneralException("One of given symb was not type of int", ReturnCode::OPERAND_TYPE_ERROR);

        $destination->setValue($symb1['type'],(intval($symb1['value']) - intval($symb2['value'])) );
        return ReturnCode::OK;
    }
    // MUL
    public function Mul():int{
        $instructionArguments = $this->Instructions[$this->currentInstructionIndex]->args;
        $destination = $this->getVariableFromArgument($instructionArguments[0]);
        $symb1 = $this->parseSymbFromArgument($instructionArguments[1]);
        $symb2 = $this->parseSymbFromArgument($instructionArguments[2]);

        if( $symb1['type'] !== "int" || $symb2['type'] !== "int" )
            throw new GeneralException("One of given symb was not type of int", ReturnCode::OPERAND_TYPE_ERROR);

        $destination->setValue($symb1['type'],(intval($symb1['value']) * intval($symb2['value'])) );

        return ReturnCode::OK;
    }
    // IDIV
    public function Idiv():int{
        $instructionArguments = $this->Instructions[$this->currentInstructionIndex]->args;
        $destination = $this->getVariableFromArgument($instructionArguments[0]);
        $symb1 = $this->parseSymbFromArgument($instructionArguments[1]);
        $symb2 = $this->parseSymbFromArgument($instructionArguments[2]);

        if( $symb1['type'] !== "int" || $symb2['type'] !== "int" )
            throw new GeneralException("One of given symb was not type of int", ReturnCode::OPERAND_TYPE_ERROR);
        if( $symb2['value'] === "0" )
            throw new GeneralException("Division by zero is forbiden",ReturnCode::OPERAND_VALUE_ERROR);

        $destination->setValue($symb1['type'],(intval($symb1['value']) / intval($symb2['value'])) );

        return ReturnCode::OK;
    }
    // LT
    public function Lt():int{
        $instructionArguments = $this->Instructions[$this->currentInstructionIndex]->args;
        $destination = $this->getVariableFromArgument($instructionArguments[0]);
        $symb1 = $this->parseSymbFromArgument($instructionArguments[1]);
        $symb2 = $this->parseSymbFromArgument($instructionArguments[2]);

        if( $symb1['type'] !== $symb2['type'] )
            throw new GeneralException("Given symbs are not same of type", ReturnCode::OPERAND_TYPE_ERROR);

        
        if( $symb1['type'] === "int" ){
            $symb1['value'] = intval($symb1['value']);
            $symb2['value'] = intval($symb2['value']);
        }

        if( $symb1['value'] < $symb2['value'] ){
            $destination->setValue("bool","true");
        }
        else{
            $destination->setValue("bool","false");
        }

        return ReturnCode::OK;
    }
    // GT
    public function Gt():int{
        $instructionArguments = $this->Instructions[$this->currentInstructionIndex]->args;
        $destination = $this->getVariableFromArgument($instructionArguments[0]);
        $symb1 = $this->parseSymbFromArgument($instructionArguments[1]);
        $symb2 = $this->parseSymbFromArgument($instructionArguments[2]);

        if( $symb1['type'] !== $symb2['type'] )
            throw new GeneralException("Given symbs are not same of type", ReturnCode::OPERAND_TYPE_ERROR);

        
        if( $symb1['type'] === "int" ){
            $symb1['value'] = intval($symb1['value']);
            $symb2['value'] = intval($symb2['value']);
        }

        if( $symb1['value'] > $symb2['value'] ){
            $destination->setValue("bool","true");
        }
        else{
            $destination->setValue("bool","false");
        }


        return ReturnCode::OK;
    }
    // EQ
    public function Eq():int{
        $instructionArguments = $this->Instructions[$this->currentInstructionIndex]->args;
        $destination = $this->getVariableFromArgument($instructionArguments[0]);
        $symb1 = $this->parseSymbFromArgument($instructionArguments[1]);
        $symb2 = $this->parseSymbFromArgument($instructionArguments[2]);

        if( $symb1['type'] !== $symb2['type'] )
            throw new GeneralException("Given symbs are not same of type", ReturnCode::OPERAND_TYPE_ERROR);

        
        if( $symb1['type'] === "int" ){
            $symb1['value'] = intval($symb1['value']);
            $symb2['value'] = intval($symb2['value']);
        }

        if( $symb1['value'] == $symb2['value'] ){
            $destination->setValue("bool","true");
        }
        else{
            $destination->setValue("bool","false");
        }


        return ReturnCode::OK;
    }
    // AND
    public function And():int{
        $instructionArguments = $this->Instructions[$this->currentInstructionIndex]->args;
        $destination = $this->getVariableFromArgument($instructionArguments[0]);
        $symb1 = $this->parseSymbFromArgument($instructionArguments[1]);
        $symb2 = $this->parseSymbFromArgument($instructionArguments[2]);

        if( $symb1['type'] !== "bool" || $symb2['type'] !== "bool" )
            throw new GeneralException("One of given symb was not type of bool", ReturnCode::OPERAND_TYPE_ERROR);

        if( $symb1['value'] == "true" && $symb2['value'] == "true" ){
            $destination->setValue("bool","true");
        }
        else{
            $destination->setValue("bool","false");
        }


        return ReturnCode::OK;
    }
    // OR
    public function Or():int{
        $instructionArguments = $this->Instructions[$this->currentInstructionIndex]->args;
        $destination = $this->getVariableFromArgument($instructionArguments[0]);
        $symb1 = $this->parseSymbFromArgument($instructionArguments[1]);
        $symb2 = $this->parseSymbFromArgument($instructionArguments[2]);

        if( $symb1['type'] !== "bool" || $symb2['type'] !== "bool" )
            throw new GeneralException("One of given symb was not type of bool", ReturnCode::OPERAND_TYPE_ERROR);

        if( $symb1['value'] == "true" || $symb2['value'] == "true" ){
            $destination->setValue("bool","true");
        }
        else{
            $destination->setValue("bool","false");
        }


        return ReturnCode::OK;
    }
    // NOT
    public function Not():int{
        $instructionArguments = $this->Instructions[$this->currentInstructionIndex]->args;
        $destination = $this->getVariableFromArgument($instructionArguments[0]);
        $symb1 = $this->parseSymbFromArgument($instructionArguments[1]);

        if( $symb1['type'] !== "bool" )
            throw new GeneralException("Given symb was not type of bool", ReturnCode::OPERAND_TYPE_ERROR);

        if( $symb1['value'] == "true"){
            $destination->setValue("bool","false");
        }
        else{
            $destination->setValue("bool","true");
        }
        return ReturnCode::OK;
    }
    // INT2CHAR
    public function Int2Char():int{
        $instructionArguments = $this->Instructions[$this->currentInstructionIndex]->args;
        $destination = $this->getVariableFromArgument($instructionArguments[0]);
        $symb = $this->parseSymbFromArgument($instructionArguments[1]);

        $tmp = mb_chr($symb['value'],'UTF-8');

        if( $tmp === false ) throw new GeneralException("It is not unicode compatible", ReturnCode::STRING_OPERATION_ERROR);

        $destination->setValue("string",$tmp);

        return ReturnCode::OK;
    }
    // STRI2INT
    public function Stri2Int():int{
        $instructionArguments = $this->Instructions[$this->currentInstructionIndex]->args;
        $destination = $this->getVariableFromArgument($instructionArguments[0]);
        $symb1 = $this->parseSymbFromArgument($instructionArguments[1]);
        $symb2 = $this->parseSymbFromArgument($instructionArguments[2]);
        
        if( $symb1['type'] !== "string" ) throw new GeneralException("Symb1 is not a string", ReturnCode::OPERAND_TYPE_ERROR);
        if( $symb2['type'] !== "int" ) throw new GeneralException("Symb2 is not a int", ReturnCode::OPERAND_TYPE_ERROR);
        
        $n = intval($symb2['value']);
        $len = mb_strlen($symb1['value'],  'UTF-8' );
        
        if(  $n >= $len  ) throw new GeneralException("Out of range",ReturnCode::STRING_OPERATION_ERROR);
   
        $character = mb_substr( $symb1['value'], $len, 1, 'UTF-8');
        $destination->setValue("string",$character);
        
        return ReturnCode::OK;
    }
    // READ
    public function Read():int{
        // TODO:
        return ReturnCode::OK;
    }
    // WRITE
    public function Write():int{
        // TODO:
        return ReturnCode::OK;
    }
    // CONCAT
    public function Concat():int{
        // TODO:
        return ReturnCode::OK;
    }
    // STRLEN
    public function Strlen():int{
        // TODO:
        return ReturnCode::OK;
    }
    // GETCHAR
    public function GetChar():int{
        // TODO:
        return ReturnCode::OK;
    }
    // SETCHAR
    public function SetChar():int{
        // TODO:
        return ReturnCode::OK;
    }
    // TYPE
    public function Type():int{
        // TODO:
        return ReturnCode::OK;
    }
    // JUMP
    public function Jump():int{
        $this->MakeJump();
        return ReturnCode::OK;
    }
    // JUMPIFEQ
    public function JumpIfEq():int{
        // TODO:
        return ReturnCode::OK;
    }
    // JUMPIFNEQ
    public function JumpIfNeq():int{
        // TODO:
        return ReturnCode::OK;
    }
    // EXIT
    public function Exit():int{
        // TODO:
        return ReturnCode::OK;
    }
    // DPRINT
    public function DPrint():int{
        // TODO:
        return ReturnCode::OK;
    }
    // BREAK
    public function Break():int{
        // $this->stderr->writeString("stderr");
        $this->stderr->writeString("BREAK:\n");
        $this->stderr->writeString("Number of executed instructions: ");
        $this->stderr->writeInt($this->instructionCounterForBreak);
        $this->stderr->writeString("\nGlobal frame:\n");
        $this->stderr->writeString($this->globalFrame->getPrintableFrame());
        $this->stderr->writeString("\nLocal frame:\n");
        if( $this->currentLocalFrame === NULL )
            $this->stderr->writeString("(Frame is not inicialized)");
        else
            $this->stderr->writeString($this->currentLocalFrame->getPrintableFrame());
        $this->stderr->writeString("\nTemporary frame:\n");
        if( $this->temporaryFrame === NULL )
            $this->stderr->writeString("(Frame is not inicialized)");
        else
            $this->stderr->writeString($this->temporaryFrame->getPrintableFrame());
        $this->stderr->writeString("\nData stack:\n");
        foreach( $this->dataStack as $Data ){
            $this->stderr->writeString("Type = " . $Data['type'] . "\t Value = " . $Data['value'] . "\n" );
        }
        if( count($this->dataStack) <= 0 )
            $this->stderr->writeString("<empty>");
        $this->stderr->writeString("\n");

        return ReturnCode::OK;
    }

    // help functions
    public function MakeJump(){
        for( $i = 0; $i < count($this->Instructions); $i++ ){
            if( $this->Instructions[$i]->opcode === 'LABEL' ){
                if( $this->Instructions[$i]->args[0]['value'] === $this->Instructions[$this->currentInstructionIndex]->args[0]['value'] ){
                    $this->currentInstructionIndex = $i;
                    break;
                }
            }
        }
    }
    // split given argument into to parts
    public function splitArgumentValue(string $input): ?array{
        $parts = explode("@",$input, 2);
        $type = $parts[0];
        $value = $parts[1];

        return array(
            'type' => $type,
            'value' => $value
        );
    }
    public function getVariableFromArgument($argument): Variable{
        $input = $argument['value'];
        $parts = $this->splitArgumentValue($input);
        // getVariable
        $retVal = $this->getVariableFromFrame($parts['type'],$parts['value']);
        if( $retVal === NULL ) throw new GeneralException( "Variable does not exist",ReturnCode::VARIABLE_ACCESS_ERROR);
        return $retVal;
    }
    // get Variable if exists or NULL if not
    public function getVariableFromFrame(string $frame, string $name): Variable | NULL {
        $retVal = NULL;
        if( $frame === "GF" )
            $retVal = $this->globalFrame->getVar($name);
        else if( $frame === "LF" && $this->currentLocalFrame !== NULL )
            $retVal = $this->currentLocalFrame->getVar($name);
        else if( $frame === "TF" && $this->temporaryFrame !== NULL )
            $retVal = $this->temporaryFrame->getVar($name);

        return $retVal;
    }
    // parse informations from instruction argument
    public function getConstantFromInstructionArgument($argument): ?array{
        return array(
            'type' => $argument['type'],
            'value' => $argument['value']
        );
    }

    public function parseSymbFromArgument($argument): array | NULL{
        $source = NULL;
        if( $argument['type'] === "var" ){
            $input = $argument['value'];
            $parts = $this->splitArgumentValue($input);
            $source = $this->getVariableFromFrame($parts['type'],$parts['value']);
            if( $source === NULL ) throw new GeneralException("Nonexisting variable",ReturnCode::VARIABLE_ACCESS_ERROR);
            $source = $source->getTypeAndValue();
            if( $source['value'] === NULL && $source['type'] === NULL )
                throw new GeneralException("Uninicialized value", ReturnCode::VALUE_ERROR);
        }
        else{
            $source = $this->getConstantFromInstructionArgument(
                $argument);
        }

        return $source;
    }
}

































// Instruction
class Instruction{
    public $opcode;
    public $order;
    public $args = [];

    public function __construct($opcode, $order, $args = []) {
        $this->opcode = $opcode;
        $this->order = $order;
        $this->args = $args;
    }

    public function addArgument($type, $value) {
        $this->args[] = ['type' => $type, 'value' => $value];
    }
}
// Variable for frame
class Variable{
    public $name;
    public $type;
    public $value;

    public function __construct($name, $type = NULL, $value = NULL){
        $this->name = $name;
        $this->type = $type;
        $this->value = $value;
    }
    
    public function setValue($type, $value){
        $this->type = $type;
        $this->value = $value;
    }
    // return type and value of given Variable
    public function getTypeAndValue(): array | NULL{
        return array(
            'type' => $this->type,
            'value' => $this->value
        );
    }
    
    public function getPrintableValue() : string{
        $retVal = $this->name ." (" 
                    . ($this->type === NULL ? "<undefined>" : $this->type) 
                    .") => " 
                    . ($this->value === NULL ? "<undefined>" : $this->value );
        return $retVal;
    }
}
// Frames for variables
class Frame{
    private $Frame;

    public function __construct()
    {
        $this->Frame = [];
    }
    // return Variable by given name
    public function getVar($name) : Variable | NULL{
        foreach( $this->Frame as $Variable ){
            if( $Variable->name == $name )
                return $Variable;
        }
        return NULL;
    }

    public function addVar($name, $type = NULL, $value = NULL ): int{
        foreach( $this->Frame as $Variable ){
            if( $Variable->name === $name )
                throw new GeneralException( "Variable already exists",ReturnCode::SEMANTIC_ERROR);
        }
        $newVar = new Variable($name,$type,$value);
        $this->Frame[] = $newVar;
        return ReturnCode::OK;
    }

    public function getPrintableFrame() : string{
        $retVal = "";
        foreach( $this->Frame as $variable  ){
            $retVal = $retVal  . $variable->getPrintableValue() . "\n";
        }
        if( count($this->Frame) <= 0 ) $retVal = "(empty frame)";
        return $retVal;
    }



}

class GeneralException extends IPPException{
    public function __construct(string $message, int $code){
        parent::__construct($message,$code);
    }
}