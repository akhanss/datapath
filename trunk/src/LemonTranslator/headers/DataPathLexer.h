//
//  Copyright 2012 Alin Dobra and Christopher Jermaine
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
/** \file
 *  This C header file was generated by $ANTLR version 3.3 Nov 30, 2010 12:50:56
 *
 *     -  From the grammar source file : DataPath.g
 *     -                            On : 2012-06-14 15:15:13
 *     -                 for the lexer : DataPathLexerLexer *
 * Editing it, at least manually, is not wise. 
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The lexer DataPathLexer has the callable functions (rules) shown below,
 * which will invoke the code for the associated rule in the source grammar
 * assuming that the input stream is pointing to a token/text stream that could begin
 * this rule.
 * 
 * For instance if you call the first (topmost) rule in a parser grammar, you will
 * get the results of a full parse, but calling a rule half way through the grammar will
 * allow you to pass part of a full token stream to the parser, such as for syntax checking
 * in editors and so on.
 *
 * The parser entry points are called indirectly (by function pointer to function) via
 * a parser context typedef pDataPathLexer, which is returned from a call to DataPathLexerNew().
 *
 * As this is a generated lexer, it is unlikely you will call it 'manually'. However
 * the methods are provided anyway.
 * * The methods in pDataPathLexer are  as follows:
 *
 *  -  void      pDataPathLexer->T__110(pDataPathLexer)
 *  -  void      pDataPathLexer->T__111(pDataPathLexer)
 *  -  void      pDataPathLexer->T__112(pDataPathLexer)
 *  -  void      pDataPathLexer->QUERRY(pDataPathLexer)
 *  -  void      pDataPathLexer->BYPASS(pDataPathLexer)
 *  -  void      pDataPathLexer->SYNTHESIZE(pDataPathLexer)
 *  -  void      pDataPathLexer->NEW(pDataPathLexer)
 *  -  void      pDataPathLexer->DELETE(pDataPathLexer)
 *  -  void      pDataPathLexer->CREATE(pDataPathLexer)
 *  -  void      pDataPathLexer->DROP(pDataPathLexer)
 *  -  void      pDataPathLexer->RUN(pDataPathLexer)
 *  -  void      pDataPathLexer->DATATYPE(pDataPathLexer)
 *  -  void      pDataPathLexer->FROM(pDataPathLexer)
 *  -  void      pDataPathLexer->FUNCTION(pDataPathLexer)
 *  -  void      pDataPathLexer->RELATION(pDataPathLexer)
 *  -  void      pDataPathLexer->FLUSH(pDataPathLexer)
 *  -  void      pDataPathLexer->QUIT(pDataPathLexer)
 *  -  void      pDataPathLexer->OPKEYWORD(pDataPathLexer)
 *  -  void      pDataPathLexer->WAYPOINT(pDataPathLexer)
 *  -  void      pDataPathLexer->SCANNER(pDataPathLexer)
 *  -  void      pDataPathLexer->SELECT(pDataPathLexer)
 *  -  void      pDataPathLexer->CONNECTIONS(pDataPathLexer)
 *  -  void      pDataPathLexer->WRITER(pDataPathLexer)
 *  -  void      pDataPathLexer->Tokens(pDataPathLexer)
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
 */
// [The "BSD licence"]
// Copyright (c) 2005-2009 Jim Idle, Temporal Wave LLC
// http://www.temporal-wave.com
// http://www.linkedin.com/in/jimidle
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef	_DataPathLexer_H
#define _DataPathLexer_H
/* =============================================================================
 * Standard antlr3 C runtime definitions
 */
#include    <antlr3.h>

/* End of standard antlr 3 runtime definitions
 * =============================================================================
 */
 
#ifdef __cplusplus
extern "C" {
#endif

// Forward declare the context typedef so that we can use it before it is
// properly defined. Delegators and delegates (from import statements) are
// interdependent and their context structures contain pointers to each other
// C only allows such things to be declared if you pre-declare the typedef.
//
typedef struct DataPathLexer_Ctx_struct DataPathLexer, * pDataPathLexer;

// Include delegate definition header files
//
#include	<DataPath_BaseLexer.h>
         


#ifdef	ANTLR3_WINDOWS
// Disable: Unreferenced parameter,							- Rules with parameters that are not used
//          constant conditional,							- ANTLR realizes that a prediction is always true (synpred usually)
//          initialized but unused variable					- tree rewrite variables declared but not needed
//          Unreferenced local variable						- lexer rule declares but does not always use _type
//          potentially unitialized variable used			- retval always returned from a rule 
//			unreferenced local function has been removed	- susually getTokenNames or freeScope, they can go without warnigns
//
// These are only really displayed at warning level /W4 but that is the code ideal I am aiming at
// and the codegen must generate some of these warnings by necessity, apart from 4100, which is
// usually generated when a parser rule is given a parameter that it does not use. Mostly though
// this is a matter of orthogonality hence I disable that one.
//
#pragma warning( disable : 4100 )
#pragma warning( disable : 4101 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4189 )
#pragma warning( disable : 4505 )
#pragma warning( disable : 4701 )
#endif

/** Context tracking structure for DataPathLexer
 */
struct DataPathLexer_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_LEXER    pLexer;
	pDataPath_BaseLexer	gBaseLexer;

     void (*mT__110)	(struct DataPathLexer_Ctx_struct * ctx);
     void (*mT__111)	(struct DataPathLexer_Ctx_struct * ctx);
     void (*mT__112)	(struct DataPathLexer_Ctx_struct * ctx);
     void (*mQUERRY)	(struct DataPathLexer_Ctx_struct * ctx);
     void (*mBYPASS)	(struct DataPathLexer_Ctx_struct * ctx);
     void (*mSYNTHESIZE)	(struct DataPathLexer_Ctx_struct * ctx);
     void (*mNEW)	(struct DataPathLexer_Ctx_struct * ctx);
     void (*mDELETE)	(struct DataPathLexer_Ctx_struct * ctx);
     void (*mCREATE)	(struct DataPathLexer_Ctx_struct * ctx);
     void (*mDROP)	(struct DataPathLexer_Ctx_struct * ctx);
     void (*mRUN)	(struct DataPathLexer_Ctx_struct * ctx);
     void (*mDATATYPE)	(struct DataPathLexer_Ctx_struct * ctx);
     void (*mFROM)	(struct DataPathLexer_Ctx_struct * ctx);
     void (*mFUNCTION)	(struct DataPathLexer_Ctx_struct * ctx);
     void (*mRELATION)	(struct DataPathLexer_Ctx_struct * ctx);
     void (*mFLUSH)	(struct DataPathLexer_Ctx_struct * ctx);
     void (*mQUIT)	(struct DataPathLexer_Ctx_struct * ctx);
     void (*mOPKEYWORD)	(struct DataPathLexer_Ctx_struct * ctx);
     void (*mWAYPOINT)	(struct DataPathLexer_Ctx_struct * ctx);
     void (*mSCANNER)	(struct DataPathLexer_Ctx_struct * ctx);
     void (*mSELECT)	(struct DataPathLexer_Ctx_struct * ctx);
     void (*mCONNECTIONS)	(struct DataPathLexer_Ctx_struct * ctx);
     void (*mWRITER)	(struct DataPathLexer_Ctx_struct * ctx);
     void (*mTokens)	(struct DataPathLexer_Ctx_struct * ctx);
    const char * (*getGrammarFileName)();
    void            (*reset)  (struct DataPathLexer_Ctx_struct * ctx);
    void	    (*free)   (struct DataPathLexer_Ctx_struct * ctx);
        
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pDataPathLexer DataPathLexerNew         (pANTLR3_INPUT_STREAM instream);
ANTLR3_API pDataPathLexer DataPathLexerNewSSD      (pANTLR3_INPUT_STREAM instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the lexer will work with.
 * \{
 *
 * Antlr will define EOF, but we can't use that as it it is too common in
 * in C header files and that would be confusing. There is no way to filter this out at the moment
 * so we just undef it here for now. That isn't the value we get back from C recognizers
 * anyway. We are looking for ANTLR3_TOKEN_EOF.
 */
#ifdef	EOF
#undef	EOF
#endif
#ifdef	Tokens
#undef	Tokens
#endif 
#define EOF      -1
#define T__110      110
#define T__111      111
#define T__112      112
#define FILTER      4
#define JOIN      5
#define GLA      6
#define PRINT      7
#define AGGREGATE      8
#define EQUAL      9
#define SEPARATOR      10
#define ATTRIBUTES      11
#define TEXTLOADER      12
#define FILE      13
#define MATCH_DP      14
#define CASE_DP      15
#define BOOL_T      16
#define ID      17
#define INT      18
#define EXPONENT      19
#define FLOAT      20
#define COMMENT      21
#define WS      22
#define ESC_SEQ      23
#define STRING      24
#define SEMICOLON      25
#define COMMA      26
#define COLON      27
#define DOT      28
#define LPAREN      29
#define RPAREN      30
#define LSQ      31
#define RSQ      32
#define QMARK      33
#define LAND      34
#define LOR      35
#define BAND      36
#define BOR      37
#define XOR      38
#define LNOT      39
#define ISEQUAL      40
#define NEQUAL      41
#define LS      42
#define GT      43
#define LE      44
#define GE      45
#define NOT      46
#define SLEFT      47
#define SRIGHT      48
#define PLUS      49
#define MINUS      50
#define TIMES      51
#define DIVIDE      52
#define MOD      53
#define HEX_DIGIT      54
#define UNICODE_ESC      55
#define OCTAL_ESC      56
#define OP      57
#define NEWSTATEMENT      58
#define OLDSTATEMENT      59
#define ATTS      60
#define ATT      61
#define FUNCTION      62
#define OPDEF      63
#define OPERATOR      64
#define UOPERATOR      65
#define DELWAYPOINT      66
#define DELQUERY      67
#define CRDATATYPE      68
#define CRSYNONIM      69
#define FCT      70
#define TPATT      71
#define ATTFROM      72
#define ATTWT      73
#define ATTSWT      74
#define RUN__      75
#define QUERRY__      76
#define WAYPOINT__      77
#define SELECT__      78
#define TERMCONN      79
#define SCANNER__      80
#define WRITER__      81
#define GLATEMPLATE      82
#define LIST      83
#define CRGLA      84
#define CRRELATION      85
#define FLUSHTOKEN      86
#define QUITTOKEN      87
#define FILE__      88
#define TEXTLOADER__      89
#define ATTC      90
#define QUERRY      91
#define BYPASS      92
#define SYNTHESIZE      93
#define NEW      94
#define DELETE      95
#define CREATE      96
#define DROP      97
#define RUN      98
#define DATATYPE      99
#define FROM      100
#define RELATION      101
#define FLUSH      102
#define QUIT      103
#define OPKEYWORD      104
#define WAYPOINT      105
#define SCANNER      106
#define SELECT      107
#define CONNECTIONS      108
#define WRITER      109
#define Tokens      113
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for DataPathLexer
 * =============================================================================
 */
/** \} */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
