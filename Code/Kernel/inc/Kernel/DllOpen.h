#ifndef KERNEL_DLLOPEN_H_
#define KERNEL_DLLOPEN_H_

#include <string>

#include "Kernel/DllExport.h"

namespace Kernel
{
class Logger;

/** @class DllOpen DllOpen.h 

 Simple class for opening shared libraries at run-time. Works for Windows and Linux.

 */
class KERNEL_EXPORT DllOpen
{
public:
	/// Static method for opening the shared library
	static void* OpenDll(const std::string&);

	/// Static method for opening the shared library
	static void* OpenDll(const std::string&, const std::string&);

	/// Static method for retrieving a function pointer
	static void* GetFunction(void*, const std::string&);

	/// Static method for closing the shared library
	static void CloseDll(void*);

	/// Static method for converting a filename to a libName (without lib___.so or ___.dll)
	static const std::string ConvertToLibName(const std::string&);

    /// Adds a directiry to the dll search path.
    static void addSearchDirectory(const std::string&);

private:
	/// Constructor private as not needed
	DllOpen()
	{};
	/// Copy operator private as not needed
	DllOpen(const DllOpen &)
	{};
	///Destructor private as not needed	
	~DllOpen()
	{};

	//private functions specific to implementation
	/// Implementation specifc static method for opening a shared library
	static void* OpenDllImpl(const std::string&);

	/// Implementation specifc static method for retrieving a function pointer
	static void* GetFunctionImpl(void*, const std::string&);

	/// Implementation specifc static method for closing a shared library
	static void CloseDllImpl(void*);

    /// Implementation specifc static method for adding a directiry to the dll search path.
    static void addSearchDirectoryImpl(const std::string&);

	/// Static reference to the logger class
	static Kernel::Logger& log;

	///lib prefix
	static const std::string LIB_PREFIX;
	///lib postfix
	static const std::string LIB_POSTFIX;
	///path seperator 
	static const std::string PATH_SEPERATOR;
};

} // namespace Kernel

#endif /*KERNEL_DLLOPEN_H_*/
