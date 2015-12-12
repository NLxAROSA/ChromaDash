# README #

# Introducing ChromaDash for Project CARS #

This project connects Project CARS to your Razer Chroma keyboard to show various information. It's a windows native command line application (written in C++ and a bit of plain old C) that will connect to Project CARS via Shared Memory and to your Razer Chroma keyboard via the Chroma SDK. The following information is displayed on the keyboard:

- RPMs -> function key row
- Tyre wear -> Q,W,A,S
- Suspension damage -> R,T,F,G
- Brake damage -> U,I,J,K
- Aero damage -> HOME
- Engine damage -> INSERT
- Engine warning -> PAGE UP

* See https://www.youtube.com/watch?v=AReeik9pvEc for an example video.

# Building and running from source

1. Get Razer Synapse from here: http://www.razerzone.com/synapse/ (Synapse includes the Chroma SDK along with it)
2. Select your device in Synapse, go to the 'Chroma apps' tab and enable Chroma Apps.
3. Download the ChromaDash source code
4. Open it up in Visual Studio 2015 (get the free version from here: https://www.visualstudio.com/en-us/downloads/download-visual-studio-vs.aspx)
5. Hit build in Visual Studio.

# Running ChromaDash

1. Start the application by double-clicking it.
2. Windows will complain about security (coming from an unknown/unverified source) both when downloading and starting the application. Please allow/unblock the application in order to allow it to start.
3. Console will show a notification about the application being started and connected (or not) and will log various messages during operation.

# Connecting to Project CARS

1. Start Project CARS.
2. Enable Shared Memory in Options -> Visual -> Hardware
3. Start a racing session.

If all went well, you should now be up and running and RPMs and gears displaying on your Razer keyboard!

# Additional information

* See http://www.projectcarsgame.com for more info on Project CARS.
* See http://developer.razerzone.com/chroma/ for more info on Razer Chroma SDK.

# Troubleshooting:

Q: App throws an error about not being able to initialize the Chroma SDK.
A: Install Razer Synapse, or get the Chroma SDK.

Q: App throws an error about not being able to connect to Project CARS.
A: Make sure Project CARS is started and Shared Memory is enabled.

Q: App doesn't throw any errors, but my keyboard does nothing.
A: Enable Chroma Apps in Synapse by selecting your device and going into the 'Chroma Apps' tab.

Q: I found an issue or have a feature request.
A: Please register your issue or request here: https://github.com/NLxAROSA/ChromaDash/issues