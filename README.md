# README #

# Introducing ChromaDash for Project CARS #

This project connects Project CARS to your Razer Chroma keyboard to show various information. It's a windows native command line application (written in C++ and a bit of plain old C) that will connect to Project CARS via Shared Memory and to your Razer Chroma keyboard via the Chroma SDK. At the moment this is limited to using the F-key row as an RPM meter and the numerical row for showing gears. Expect much more in the future!

# Building from source

1. Download the source code
2. Open it up in Visual Studio 2015 (get the free version from here: https://www.visualstudio.com/en-us/downloads/download-visual-studio-vs.aspx)

# Running ChromaDash

1. Hit build in Visual Studio.
2. Start the application by double-clicking it.
3. Windows will complain about security (coming from an unknown/unverified source) both when downloading and starting the application. Please allow/unblock the application in order to allow it to start. Windows Firewall will also ask for permission to access the local network. Please allow it access for proper functioning.
4. If Windows complains about missing DLLs (e.g. MSVCR120.DLL or similar), please download and install the Visual C++ Redistributable Packages for Visual Studio 2013 from http://www.microsoft.com/en-us/download/details.aspx?id=40784 (vcredist_x86.exe), then start again.
5. Console will show a notification about the application being started and connected (or not) and will log various messages during operation.

# Connecting to Project CARS

1. Start Project CARS.
2. Enable Shared Memory in Options -> Visual -> Hardware
3. Open up a browser (Chrome for best results) and go to http://localhost:8080/crest/v1/api
4. Profit! F5/Refresh until you drop!

# Configuring Synapse

1. Get Razer Synapse from here: http://www.razerzone.com/synapse/
2. Select your device in Synapse, go to the 'Chroma apps' tab and enable Chroma Apps.

If all went well, you should now be up and running!

# Additional information

* See http://www.projectcarsgame.com for more info on Project CARS.
* See http://developer.razerzone.com/chroma/ for more info on Razer Chroma SDK.