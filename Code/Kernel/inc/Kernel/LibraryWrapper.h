#ifndef KERNEL_LIBRARY_WRAPPER_H_
#define KERNEL_LIBRARY_WRAPPER_H_

#include <string>

#include "Kernel/DllExport.h"

namespace Kernel
{

/** @class LibraryWrapper LibraryWrapper.h Kernel/LibraryWrapperr.h

 Class for wrapping a shared library.
 
 */
class KERNEL_EXPORT LibraryWrapper
{
public:
	LibraryWrapper();
	virtual ~LibraryWrapper();

	//Returns true if DLL is opened or already open
	bool OpenLibrary(const std::string&);

	bool OpenLibrary(const std::string&, const std::string&);

private:
	/** An untyped pointer to the loaded library.
	 * This is created and deleted by this class.
	 **/
	void* module;
};

} // namespace Kernel

#endif //KERNEL_LIBRARY_WRAPPER_H_
