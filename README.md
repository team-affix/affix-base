# affix-base
 Fundamental Functions and Typenames for Team Affix

### Requirements for Successful Build
- Configuring build: cryptlib.lib
    1. Open cryptopp/cryptest.sln
    1. Right-click on project (cryptlib) in the upper-right hand corner
    1. Click properties
    1. Switch to Release mode
    1. Find option C/C++ -> Code Generation -> Runtime Library
    1. Set option to /MD
    1. Click Apply
    1. Switch to Debug mode
    1. Find same option as above
    1. Set option to /MDd
    1. Click Apply, then OK
- Building cryptlib.lib
    1. Open cryptopp/cryptest.sln
    1. Go to Build -> Batch Build
    1. Check the box for project (cryptlib) Configuration: Debug, Platform: Win32
    1. Check the box for project (cryptlib) Configuration: Release, Platform: Win32
    1. Check the box for project (cryptlib) Configuration: Debug, Platform: x64
    1. Check the box for project (cryptlib) Configuration: Release, Platform: x64
    1. Click Build