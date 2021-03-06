//%LICENSE////////////////////////////////////////////////////////////////
//
// Licensed to The Open Group (TOG) under one or more contributor license
// agreements.  Refer to the OpenPegasusNOTICE.txt file distributed with
// this work for additional information regarding copyright ownership.
// Each contributor licenses this file to you under the OpenPegasus Open
// Source License; you may not use this file except in compliance with the
// License.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//////////////////////////////////////////////////////////////////////////
//
//%/////////////////////////////////////////////////////////////////////////////
#include "FQLQueryStatement.h"
#include "FQLQueryStatementRep.h"
#include <iostream>
#include <Pegasus/Common/Stack.h>
#include <Pegasus/Common/Tracer.h>

PEGASUS_USING_STD;

PEGASUS_NAMESPACE_BEGIN

FQLQueryStatement::FQLQueryStatement(
    const CIMClass& queryClass,
    const String& query)
{

    PEG_METHOD_ENTER(TRC_FQL,"FQLQueryStatement::FQLQueryStatement");
    _rep = new FQLQueryStatementRep(queryClass, query);
    // Set the _rep into the base class also
    FQLQueryStatement::_rep = _rep;

    PEG_METHOD_EXIT();
}

FQLQueryStatement::FQLQueryStatement(
    const String& query)
{
    PEG_METHOD_ENTER(TRC_FQL,"FQLQueryStatement::FQLQueryStatement");
    _rep = new FQLQueryStatementRep(query);
    // Set the _rep into the base class also
    FQLQueryStatement::_rep = _rep;

    PEG_METHOD_EXIT();
}


FQLQueryStatement::FQLQueryStatement()
{
    _rep = new FQLQueryStatementRep();
    // Set the _rep into the base class also
    FQLQueryStatement::_rep = _rep;
}

FQLQueryStatement::FQLQueryStatement(const FQLQueryStatement& statement)
{
    _rep = new FQLQueryStatementRep(*statement._rep);

    // Set the _rep into the base class also
    FQLQueryStatement::_rep = _rep;
}

FQLQueryStatement& FQLQueryStatement::operator=(const FQLQueryStatement& rhs)
{
    if(&rhs != this)
    {
        if(_rep) delete _rep;
        _rep = new FQLQueryStatementRep(*rhs._rep);

        // Set the _rep into the base class also
        FQLQueryStatement::_rep = _rep;
    }

  return *this;
}


FQLQueryStatement::~FQLQueryStatement()
{
    delete _rep;
}

void FQLQueryStatement::clear()
{
    _rep->clear();
}


CIMPropertyList FQLQueryStatement::getQueryPropertyList(
    const CIMObjectPath& inClassName)
{
    return _rep->getQueryPropertyList(inClassName);
}

Boolean FQLQueryStatement::appendQueryPropertyName(const CIMName& x)
{
    return _rep->appendQueryPropertyName(x);
}

Boolean FQLQueryStatement::evaluateQuery(
    const FQLPropertySource* source) const
{
    return _rep->evaluateQuery(source);
}

void FQLQueryStatement::print() const
{
    _rep->print();
}

String FQLQueryStatement::toString() const
{
     return _rep->toString();
}

Boolean FQLQueryStatement::evaluate(const CIMInstance& inCI)
{
    return _rep->evaluate(inCI);
}


/** Returns the number of unique property names from the query

*/
Uint32 FQLQueryStatement::getQueryPropertyNameCount() const
{
    return _rep->getQueryPropertyNameCount();
}

/** Gets the i-th unique property appearing in the where clause.
*/
const CIMName& FQLQueryStatement::getQueryPropertyName(Uint32 i) const
{
    return _rep->getQueryPropertyName(i);
}

void FQLQueryStatement::appendOperation(FQLOperation x)
{
    DCOUT << "FQLQueryStatement::appendOperation = "
        << FQLOperationToString(x) << endl;
    _rep->appendOperation(x);
}

/** Appends an operand to the operand array. This method should
    only be called by the parser.
*/
void FQLQueryStatement::appendOperand(const FQLOperand& x)
{
    DCOUT << "FQLQueryStatement::appendOperand = " << x.toString() << endl;
    _rep->appendOperand(x);
}

/** Chains an operand to the operand array. This method should
    only be called by the parser.
*/
void FQLQueryStatement::chainOperand(const FQLOperand& x)
{
    DCOUT << "FQLQeryStatement::chainOperand = " << x.toString() << endl;
    _rep->chainOperand(x);
}

PEGASUS_NAMESPACE_END
