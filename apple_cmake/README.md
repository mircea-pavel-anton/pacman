## What's this for?

This `MacBundle.cmake` contains CMake instruction to build MacOS GUI `.app` bundle. It also embeds the icon to the app bundle.  It also copies metadata, and all static assets into the bundle at build time. 

### Extra notes
If you installed SFML as "Frameworks", instead of "dylibs", you will need to embed the SFML frameworks (using Xcode) into the App Bundle, if you want to share your app with others. Use CMake GUI to generate an **XCode** project. Open project in XCode. Click the root folder "pacman". In the top toolbar, click menu **Editor** > **Add Build Phase** > **Add Copy Files Build Phase**. Click the `[+]`, and select all SFML frameworks from `/Library/Frameworks` (audio, video, system etc.). Then **Product** > **Build** from top toolbar. Done.
