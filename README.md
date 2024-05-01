# Byond CID randomizer
This is a tool that randomizes your CID in Byond, aka, your hardware ID, or what keeps you banned when you change your IP Address. It's meant to work in most Byond versions, thus I hooked the DeviceIoControl so there aren't any offsets that need to be updated in the future. If your OS version is too old, I don't think the DeviceIoControl is even called, but the GetVolumeInformationA function gets called. Regardless, your CID is a md5, so you can hook the md5 function instead if you want it to work 100%.

# Installing
Compile it with Visual Studio, both the DLL and the Launcher, they're separated by folders.

# Usage
If you don't want to inject the DLL yourself, you can use the launcher, which will inject it for you. To use the launcher, simply open up the command prompt in the compiled launcher executable folder and type
```
./byondLauncher.exe "[your byond.exe/dreamseeker.exe path here]" "[the compiled dll path]"
```
If the injection works, it will open a console window, if the hooked DeviceIoControl gets called, you will know because the console will tell you.
