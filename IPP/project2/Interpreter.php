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






        // $val = $this->input->readString();
        // $this->stdout->writeString("stdout");
        // $this->stderr->writeString("stderr");
        

        return ReturnCode::OK;

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

    
}

class Stack{
    // samotna promena na ukladani
    //push
    //pop
    // mozna - empty, init, ...
}