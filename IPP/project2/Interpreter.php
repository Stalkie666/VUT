<?php

namespace IPP\Student;

use DOMDocument;
use DOMXPath;
use Exception;
use IPP\Core\AbstractInterpreter;
use IPP\Core\Exception\NotImplementedException;
use IPP\Core\Exception\XMLException;
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
            return ReturnCode::INVALID_XML_ERROR;
        }
        
        
        if ($this->checkForExpectedStructure() == false){
            $this->stderr->writeString("Somethings is wrong\n");
            return ReturnCode::INVALID_SOURCE_STRUCTURE;
        }
        // load instructions into array
        $this->loadInstructions();
        // return ReturnCode::OK;



        // check for jumps and labels if they are valid
        $retVal = $this->checkForDuplicateLabelsAndNonexistentingOnes();
        if($retVal !== 0){
            return $retVal;
        }


        // interpret
        // $this->runInterpretInstructions();


        // $val = $this->input->readString();
        // $this->stdout->writeString("stdout");
        // $this->stderr->writeString("stderr");
        

        return ReturnCode::OK;

        // throw new NotImplementedException;
    }


    // nejaka kontrola na labels pred zacatkem
    // budu potrebovat pole pro promene, jedno pole pro GF, LF a TF
    // budu potrebovat stack(zasobnik), asi bude zajistovat pole
    // potom metodu pro chovani kazde instrukce
    // nejakou metodu kde se bude rozhodovat, ktera instrukce zrovna probiha




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
                        return ReturnCode::SEMANTIC_ERROR;
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
                        return ReturnCode::SEMANTIC_ERROR;
                    }
                }
            }
            
        }
        return 0;
    }
    // Nejaky funkce se switchem pro prochazeni instrukci
    private $currentInstructionIndex;

    public function runInterpretInstructions() : int{
        $this->currentInstructionIndex = 0;

        $this->globalFrame = new Frame;
        $this->localFramesArrays = [];
        $this->currentLocalFrame = NULL;
        $this->temporaryFrame = NULL;

        $this->dataStack = [];
        $this->callStack = [];

        while($this->currentInstructionIndex < count($this->Instructions)){
            $retVal = ReturnCode::OK;

            $currentInstruction = $this->Instructions[$this->currentInstructionIndex];
            $opcode = $currentInstruction['opcode'];

            switch($opcode){
                case 'MOVE':
                    break;
                case 'CREATEFRAME':
                    break;
                case 'PUSHFRAME':
                    break;
                case 'POPFRAME':
                    break;
                case 'DEFVAR':
                    break;
                case 'CALL':
                    break;
                case 'RETURN':
                    break;
                case 'PUSHS':
                    break;
                case 'POPS':
                    break;
                case 'ADD':
                    break;
                case 'SUB':
                    break;
                case 'MUL':
                    break;
                case 'IDIV':
                    break;
                case 'LT':
                    break;
                case 'GT':
                    break;
                case 'EQ':
                    break;
                case 'AND':
                    break;
                case 'OR':
                    break;
                case 'NOT':
                    break;
                case 'INT2CHAR':
                    break;
                case 'STRI2INT':
                    break;
                case 'READ':
                    break;
                case 'WRITE':
                    break;
                case 'CONCAT':
                    break;
                case 'STRLEN':
                    break;
                case 'GETCHAR':
                    break;
                case 'SETCHAR':
                    break;
                case 'TYPE':
                    break;
                case 'LABEL':
                    break;
                case 'JUMP':
                    break;
                case 'JUMPIFEQ':
                    break;
                case 'JUMPIFNEQ':
                    break;
                case 'EXIT':
                    break;
                case 'DPRINT':
                    break;
                case 'BREAK':
                    break;
                // should not happen
                default:
                    return ReturnCode::INTEGRATION_ERROR;
            }

            if( $retVal !== ReturnCode::OK )
                return $retVal;

            // next instruction
            $this->currentInstructionIndex++;

        }
        
        return ReturnCode::OK;
    }

    // MOVE
    public function Move() : int{
        // TODO:
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
            return ReturnCode::FRAME_ACCESS_ERROR;
        $this->currentLocalFrame = $this->temporaryFrame;
        $this->localFramesArrays[] = $this->currentLocalFrame;
        $this->temporaryFrame = NULL;
        return ReturnCode::OK;
    }
    // POPFRAME
    public function PopFrame():int{
        if(empty($this->localFramesArrays))
            return ReturnCode::FRAME_ACCESS_ERROR;
        $this->temporaryFrame = $this->currentLocalFrame;
        array_pop($this->localFramesArrays);
        if( empty($this->localFramesArrays) )
            $this->currentLocalFrame = NULL;
        else{
            end($this->localFramesArrays);
            $this->currentLocalFrame = current($this->localFramesArrays);
        }
        return ReturnCode::OK;
    }
    // DEFVAR
    public function DefVar():int{
        // TODO:
        return ReturnCode::OK;
    }
    // CALL
    public function Call():int{
        $this->callStack[] = $this->currentInstructionIndex + 1;
        for( $i = 0; $i < count($this->Instructions); $i++ ){
            if( $this->Instructions[$i]->opcode === 'LABEL' ){
                if( $this->Instructions[$i]->args[0]['value'] === $this->Instructions[$this->currentInstructionIndex]->args[0]['value'] ){
                    $this->currentInstructionIndex = $i;
                    break;
                }
            }
        }
        return ReturnCode::OK;
    }
    // RETURN
    public function Return():int{
        if( empty($this->callStack) )
            return ReturnCode::VALUE_ERROR;
        end($this->callStack);
        $this->currentInstructionIndex = current($this->callStack);
        array_pop($this->callStack);
        return ReturnCode::OK;
    }
    // PUSHS
    public function Pushs():int{
        return ReturnCode::OK;
    }
    // POPS
    public function Pops():int{
        return ReturnCode::OK;
    }
    // ADD
    public function Add():int{
        return ReturnCode::OK;
    }
    // SUB
    public function Sub():int{
        return ReturnCode::OK;
    }
    // MUL
    public function Mul():int{
        return ReturnCode::OK;
    }
    // IDIV
    public function Idiv():int{
        return ReturnCode::OK;
    }
    // LT
    public function Lt():int{
        return ReturnCode::OK;
    }
    // GT
    public function Gt():int{
        return ReturnCode::OK;
    }
    // EQ
    public function Eq():int{
        return ReturnCode::OK;
    }
    // AND
    public function And():int{
        return ReturnCode::OK;
    }
    // OR
    public function Or():int{
        return ReturnCode::OK;
    }
    // NOT
    public function Not():int{
        return ReturnCode::OK;
    }
    // INT2CHAR
    public function Int2Char():int{
        return ReturnCode::OK;
    }
    // STRI2INT
    public function Stri2Int():int{
        return ReturnCode::OK;
    }
    // READ
    public function Read():int{
        return ReturnCode::OK;
    }
    // WRITE
    public function Write():int{
        return ReturnCode::OK;
    }
    // CONCAT
    public function Concat():int{
        return ReturnCode::OK;
    }
    // STRLEN
    public function Strlen():int{
        return ReturnCode::OK;
    }
    // GETCHAR
    public function GetChar():int{
        return ReturnCode::OK;
    }
    // SETCHAR
    public function SetChar():int{
        return ReturnCode::OK;
    }
    // TYPE
    public function Type():int{
        return ReturnCode::OK;
    }
    // LABEL
    public function Label():int{
        return ReturnCode::OK;
    }
    // JUMP
    public function Jump():int{
        return ReturnCode::OK;
    }
    // JUMPIFEQ
    public function JumpIfEq():int{
        return ReturnCode::OK;
    }
    // JUMPIFNEQ
    public function JumpIfNeq():int{
        return ReturnCode::OK;
    }
    // EXIT
    public function Exit():int{
        return ReturnCode::OK;
    }
    // DPRINT
    public function DPrint():int{
        return ReturnCode::OK;
    }
    // BREAK
    public function Break():int{
        return ReturnCode::OK;
    }
}


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
}

class Frame{
    private $Frame;

    public function __construct()
    {
        $this->Frame = [];
    }

    public function getVar($name) : Variable | NULL{
        foreach( $this->Frame as $Variable ){
            if( $Variable->name == $name )
                return $Variable;
        }
        return NULL;
    }

    public function addVar($name, $type = NULL, $value = NULL ){
        $newVar = new Variable($name,$type,$value);
        $this->Frame[] = $newVar;
    }


}