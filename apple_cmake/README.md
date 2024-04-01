## What's this for

This contains cmake instructions to build MacOS GUI `.app` bundle. It embeds the icon to the app bundle.  It also copies all assets/Resources  into the bundle at build time. 

If you installed SFML as Frameworks, You will need to manually embed the SFML frameworks using Xcode into the bundle before building for Release, if you want to share your app. Click project folder in Xcode, click **Add Build Step** > choose **Copy Files** > select all listed SFML frameworks, then Add [+]. Also you will need to find `freetype.framework` file from `/Library/Frameworks` and add it too.
