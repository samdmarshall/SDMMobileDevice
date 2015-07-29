# Opening Issues

## Bug Reports

**Required Materials:**

* Symbolicated Crash Report(s) 
* Example project that reproduces the issue

To properly address the bug I need as much contextual information about how to reproduce it as possible. This means crash report logs and example code that will reproduce the same issue are necessary for me to make progress with your report. 

NOTE: when creating the sample project that reproduces the issue it must be entirely self-contained. Please check-out any submodules or other dependencies and include them with the sample. To be able to take action on any particular problem I expect that when I download the sample I can open it in Xcode and hit "build". Thanks!

## Feature Requests/Enhancements

When filing enhancements to this library, please show that MobileDevice.framework also supports the desired feature. 

---

# Submitting Pull Requests
Pull requests are very welcome to this project, however I have a couple of guidelines that should be followed when submitting a pull request.

## Code Styling
Please use clang-format before performing any pull requests. Use the .clang-format file in this repo.

## Cosmetic Changes
Any pull requests that alter only cosmetic appearance of the code, such as:
* Replacing hardcoded hex values with the proper constant counterpart
* Memory management
* Almost anything that doesn't alter existing code logic

Will be reviewed and almost always be accepted. So long as there are no breaking changes that occur through a cosmetic change request, it will be accepted as quickly as possible.

## Functionality Changes
If you wish to submit pull requests that alter the functionality or logical process of the code, then I am going to require that references to where the changes can be found inside of MobileDevice.framework. Please provide a reference to which function and where in that function the requested change can be found so I can compare that with the existing code. The reason for this is that I want to keep this as cross-compatible with Apple's framework code as possible. Please also include a detailed description of what the change is doing and a working example of code that uses this change so it can be tested.