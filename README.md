# trigBoardV8_BaseFirmware

Project fully documented here: 
https://trigboard-docs.readthedocs.io/en/latest/index.html

Release:
3/19/20 - First Release

6/04/20
All trigBoards after this date will be burned with this code
- Fixed a minor bug in the RTC init tab
- Fixed an issue where the battery threshold wasn't saving
- Changed the code directory a little bit for OTA/USB programming of the bin files
- Pretty big change was to use an event handler to detect when the WiFi connection is established - both on WiFi and UDP tabs

6/05/20
- Under development

6/22/20
- Minor performance improvements

8/05/20
- added battery offset to battery voltage measurement

10/10/20
- small change to allow the password to be entered in blank in the configurator in case of connecting to an unsecured network
