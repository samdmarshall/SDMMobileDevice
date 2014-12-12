# Submitting Pull Requests
Pull requests are very welcome to this project, however I have a couple of guidelines that should be followed when submitting a pull request.

## Cosmetic Changes
Any pull requests that alter only cosmetic appearance of the code, such as:
* Replacing hardcoded hex values with the proper constant counterpart
* Memory management
* Almost anything that doesn't alter existing code logic

Will be reviewed and almost always be accepted. So long as there are no breaking changes that occur through a cosmetic change request, it will be accepted as quickly as possible.

## Functionality Changes
If you wish to submit pull requests that alter the functionality or logical process of the code, then I am going to require that references to where the changes can be found inside of MobileDevice.framework. Please provide a reference to which function and where in that function the requested change can be found so I can compare that with the existing code. The reason for this is that I want to keep this as cross-compatible with Apple's framework code as possible. Please also include a detailed description of what the change is doing and a working example of code that uses this change so it can be tested.