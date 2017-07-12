//
// NestedDiagnosticContext.h
//
// $Id: //poco/1.3/Foundation/include/Poco/NestedDiagnosticContext.h#1 $
//
// Library: Foundation
// Package: Core
// Module:  NestedDiagnosticContext
//
// Definition of the NestedDiagnosticContext class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Foundation_NestedDiagnosticContext_INCLUDED
#define Foundation_NestedDiagnosticContext_INCLUDED


#include "Poco/Foundation.h"
#include <vector>
#include <ostream>


namespace Poco {


class NDCScope;


class Foundation_API NestedDiagnosticContext
	/// This class implements a Nested Diagnostic Context (NDC),
	/// as described in Neil Harrison's article "Patterns for Logging 
	/// Diagnostic Messages" in "Pattern Languages of Program Design 3"
	/// (Addison-Wesley).
	///
	/// A NDC maintains a stack of context information, consisting of
	/// an informational string (e.g., a method name), as well as an
	/// optional source code line number and file name.
	/// NDCs are especially useful for tagging log messages with
	/// context information which is very helpful in a multithreaded
	/// server scenario.
	/// Every thread has its own private NDC, which is automatically
	/// created when needed and destroyed when the thread terminates.
	///
	/// The NDCScope (or NDC::Scope) class can be used to automatically
	/// push information at the beginning of a scope, and to pop it
	/// at the end.
	/// The poco_ndc(info) macro augments the information with a
	/// source code line number and file name.
{
public:
	typedef NDCScope Scope;

	NestedDiagnosticContext();
		/// Creates the NestedDiagnosticContext.

	NestedDiagnosticContext(const NestedDiagnosticContext& ctx);
		/// Copy constructor.

	~NestedDiagnosticContext();
		/// Destroys the NestedDiagnosticContext.
		
	NestedDiagnosticContext& operator = (const NestedDiagnosticContext& ctx);
		/// Assignment operator.
		
	void push(const std::string& info);
		/// Pushes a context (without line number and filename) onto the stack.
		
	void push(const std::string& info, int line, const char* filename);
		/// Pushes a context (including line number and filename) 
		/// onto the stack. Filename must be a static string, such as the
		/// one produced by the __FILE__ preprocessor macro.

	void pop();
		/// Pops the top-most context off the stack.
		
	int depth() const;
		/// Returns the depth (number of contexts) of the stack.
	
	std::string toString() const;
		/// Returns the stack as a string with entries
		/// delimited by colons. The string does not contain
		/// line numbers and filenames.
		
	void dump(std::ostream& ostr) const;
		/// Dumps the stack (including line number and filenames)
		/// to the given stream. The entries are delimited by
		/// a newline.

	void dump(std::ostream& ostr, const std::string& delimiter) const;
		/// Dumps the stack (including line number and filenames)
		/// to the given stream.
		
	void clear();
		/// Clears the NDC stack.
	
	static NestedDiagnosticContext& current();
		/// Returns the current thread's NDC.

private:
	struct Context
	{
		std::string info;
		const char* file;
		int         line;
	};
	typedef std::vector<Context> Stack;
	
	Stack _stack;
};


typedef NestedDiagnosticContext NDC;


class Foundation_API NDCScope
	/// This class can be used to automatically push a context onto
	/// the NDC stack at the beginning of a scope, and to pop
	/// the context at the end of the scope.
{
public:
	NDCScope(const std::string& info);
		/// Pushes a context on the stack.
		
	NDCScope(const std::string& info, int line, const char* filename);
		/// Pushes a context on the stack.

	~NDCScope();
		/// Pops the top-most context off the stack.
};


//
// inlines
//
inline NDCScope::NDCScope(const std::string& info)
{
	NestedDiagnosticContext::current().push(info);
}

	
inline NDCScope::NDCScope(const std::string& info, int line, const char* filename)
{
	NestedDiagnosticContext::current().push(info, line, filename);
}


inline NDCScope::~NDCScope()
{
	NestedDiagnosticContext::current().pop();
}


//
// helper macros
//
#define poco_ndc(func) \
	Poco::NDCScope _theNdcScope(#func, __LINE__, __FILE__)


#if defined(_DEBUG)
	#define poco_ndc_dbg(func) \
		Poco::NDCScope _theNdcScope(#func, __LINE__, __FILE__)
#else
	#define poco_ndc_dbg(func)
#endif


} // namespace Poco


#endif // Foundation_NestedDiagnosticContext_INCLUDED